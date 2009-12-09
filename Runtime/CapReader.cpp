
#include "StdAfx.h"

////////////////////////////////
// Parse a CAP binary event list in to CEventLine tree form
// pData: pointer to binary data
// len: number of bytes of data
// pEvents: vector to send root events to
// pTriggers: vector to send trigger events to
#define READINTFROM(a)   *((int*)  &(pData[a])); a += sizeof(int)
#define READINT64FROM(a)   *((__int64*)  &(pData[a])); a += sizeof(__int64)
#define READFLOATFROM(a) *((float*)&(pData[a])); a += sizeof(float)
#define READDOUBLEFROM(a) *((double*)&(pData[a])); a += sizeof(double)
#define READBYTEFROM(a) pData[a]; a++

bool CCapReader::CheckForElseEvent(CEventLine* ev)
{
	// Was an Else event: make it subevent to the previous event
	if (IsElseEvent(ev)) {
		if (previousEvent == NULL)
			throw runtime_error("ELSE is not allowed to follow a trigger or be the first event in the event list.  It must follow an ordinary event.");

		// Is the previous event an event parent?
		CEventParent* evParent = dynamic_cast<CEventParent*>(*previousEvent);

		// Add else as a subevent else event
		if (evParent) {
			evParent->elseEvent = ev;
			previousEvent = &(evParent->elseEvent);
			return true;	// needs not be added to main list or trigger map
		}

		CEvent* lastEvent = dynamic_cast<CEvent*>(*previousEvent);

		// The last event was a normal event.
		if (lastEvent) {
			lastEvent->elseEvent = ev;
			previousEvent = &(lastEvent->elseEvent);
			return true;
		}

		throw runtime_error("Invalid ELSE event: ELSE must follow a standard event!  You may have put ELSE as the first event in a group of events.");
	}

	return false;	// was not an else event
}

// Send an event to a frame's event list or trigger map
void CCapReader::SendEvent(CEventLine* ev)
{
	if (CheckForElseEvent(ev))
		return;

	if (ev->trigger || ev->fasttrigger) {
		// Get modifiers
		ev->GetSolModifiers(pLayout);
		previousEvent = pRuntime->AddTriggerToMap(pLayout, ev);
	}
	else {
		pLayout->Events.push_back(ev);
		previousEvent = &(pLayout->Events.back());
	}
}

inline bool IsElseCondition(const condition& c)
{
	// Else = system id 17
	return c.oid == -1 && c.cndID == 17;
}

bool CCapReader::IsElseEvent(CEventLine* evl)
{
	CEvent* pEv;
	CEventParent* pEvParent;
	ConditionList* pCnds = NULL;
	
	// Cast to check what kind of event was parsed.
	if		(pEv = dynamic_cast<CEvent*>(evl))					pCnds = &(pEv->cnds);
	else if (pEvParent = dynamic_cast<CEventParent*>(evl))		pCnds = &(pEvParent->cnds);

	// Is an ELSE event?  Add to else vector instead of subevents.
	if (pCnds && !pCnds->empty())
		return IsElseCondition(pCnds->front());

	return false;
}

void CCapReader::ParseAndSendEvent()
{
	CEventLine* ev = ParseEventLine();

	// If OR spawned any extra events, send them first
	if (!auxEvents.empty()) {
		EventIterator i = auxEvents.begin();
		EventConstIterator aux_end = auxEvents.end();

		for ( ; i != aux_end; i++) {
			(*i)->number = ev->number;
			SendEvent(*i);
		}

		auxEvents.resize(0);
	}

	SendEvent(ev);
}

void CCapReader::ReadEventList(BYTE* _pData, int _len)
{
	// Save params to class
	pData = _pData;
	len = _len;

	cursor = 0;
	int i;

	// Read event sheet names
	int numEventSheets = READINTFROM(cursor);

	for (i = 0; i < numEventSheets; i++) {

		int nameLength = READINTFROM(cursor);
		pRuntime->eventSheetNames.push_back((const char*)&(pData[cursor]));
		cursor += nameLength;
	}

	// Attach one event list to one frame.  Event sheets are handled at compile time.
	int numEventLists = READINTFROM(cursor);

	for (i = 0; i < numEventLists; i++) {

		// Send these events to the given frame
		pLayout = pRuntime->Frames[i];

		// Check for header
		if (pData[cursor] != CAP_BEGINEVENTLIST)
			throw runtime_error("Read error: bad event list (1)");

		cursor++;

		// Read events
		while (pData[cursor] == CAP_BEGINEVENT || pData[cursor] == CAP_BEGINGROUP) {

			ParseAndSendEvent();

		}

		// Check for footers
		if (pData[cursor] != CAP_ENDEVENTLIST)
			throw runtime_error("Read error: bad event list (2)");

		cursor++;

		//if (cursor != len - 1)
		//	throw runtime_error("Read error: data beyond event list");

	}

	// TEMP
	//ShowInt("CAP root event count: ", pEvents.size());
}

//////////////////////////////////
// Recursively parse an event
CEventLine* CCapReader::ParseEventLine()
{
	switch (pData[cursor]) {
	case CAP_BEGINEVENT:
		return ParseEvent();
	case CAP_BEGINGROUP:
		return ParseEventGroup();
	default:
		throw runtime_error("Read error: bad event list (3)");
	}
}

inline bool IsOrCondition(const condition& c)
{
	// Or = system id 18
	return c.oid == -1 && c.cndID == 18;
}

inline CExpression* CreateIntegerSysExpression(__int64 value, CRuntime* pRuntime)
{
	CExpression* exp = new CExpression;

	exp->pRuntime = pRuntime;
	exp->oidCaller = -1;
	
	Token t;
	t.iValue = value;
	t.t = T_INTEGER;
	exp->toks.push_back(t);
	exp->ParseTokenStream();

	return exp;
}

// Functions to generate and poke in to the event tree the OR expansion events.
// See or_expand.txt for more info
namespace OR {

	static int flagNumber = 0;

	inline void MakeFlagCheckCondition(condition& c, CRuntime* pRuntime)
	{
		c.cndID = SYSTEM_OR_FLAGCHECK;
		c.isNegated = false;
		c.isStatic = false;
		c.number = -1;
		c.oid = -1;				// system
		c.pRuntime = pRuntime;
		c.constantParams = true;

		c.params.push_back(CreateIntegerSysExpression(flagNumber, pRuntime));

		c.pType = NULL;
		c.routine = (ACROUTINE)&SystemObject::cOR_FlagCheck;
		c.cndType = CONDITIONTYPE_SYSTEM;
	}

	inline void MakeFlagOnAction(action& a, CRuntime* pRuntime)
	{
		a.actID = -2;
		a.constantParams = true;
		a.oid = -1;
		
		a.params.push_back(CreateIntegerSysExpression(flagNumber, pRuntime));

		a.pRuntime = pRuntime;
		a.pType = NULL;
		a.routine = (ACROUTINE)&SystemObject::aOrFlagOn;
	}

	inline void MakeClauseEvent(const ConditionList& clause, CRuntime* pRuntime)
	{
		CEvent* pEvent = new CEvent;
		pEvent->cnds = clause;

		action a;
		MakeFlagOnAction(a, pRuntime);
		pEvent->acts.push_back(a);

		short eventFlags;
		if (clause.front().pType)
			eventFlags = clause.front().pType->pPlugin->pvCndRoutines->at(clause.front().cndID).flags;
		else
			eventFlags = SystemObject::GetCndFlags(clause.front().cndID);

		pEvent->fasttrigger = (eventFlags & CND_FASTTRIGGERED)!=0;
		pEvent->manualSolModifier = false;
		pEvent->number = -1;
		pEvent->pRuntime = pRuntime;
		pEvent->trigger = (eventFlags & CND_TRIGGERED)!=0;

		// Add this event as auxiliary event to be added by the parsing callee
		pRuntime->CapReader.auxEvents.push_back(pEvent);
	}
}

bool CCapReader::SliceAndDiceOrConditions(ConditionList& cnds, ActionList& acts)
{
	// Determine if ORs are used
	ConditionIterator c = cnds.begin();
	ConditionIterator lastOr = cnds.begin();
	ConditionConstIterator cnds_begin = cnds.begin();
	ConditionConstIterator cnds_end = cnds.end();

	bool hadOrConditions = false;

	// A list of each set of conditions that will be split in to separate events
	vector<ConditionList> clauses;

	// For each condition
	for ( ; c != cnds_end; c++) {

		if (IsOrCondition(*c)) {

			hadOrConditions = true;

			// First condition: invalid
			if (c == cnds_begin)
				throw runtime_error("Bad event using 'OR' conditions:  'OR' cannot be the first condition.");

			// Two OR conditions following each other
			if (c == lastOr && lastOr != cnds_begin)
				throw runtime_error("Bad event using 'OR' conditions: 'OR' cannot follow another 'OR' condition.");
			
			// Found OR condition: copy the conditions since the last OR to a new condition list and add it to clauses
			ConditionList clause;
			clause.insert(clause.begin(), lastOr, c);
			clauses.push_back(clause);

			// Set last OR iterator to next element, the first element to copy next time
			lastOr = c + 1;
		}
	}

	// Had OR conditions in this event
	if (hadOrConditions) {

		if (lastOr == cnds.end())
			throw runtime_error("Bad event using 'OR' conditions:  'OR' cannot be the last condition.");

		// Copy the final clause
		ConditionList clause;
		clause.insert(clause.begin(), lastOr, cnds.end());
		clauses.push_back(clause);

		// Now we need to split these conditions in to separate events: see or_expand.txt

		// Set given event to be the final evaluation event
		// Clear all conditions and replace with flag is on / merge SOLs conditions
		cnds.resize(0);

		condition c;
		action a;

		OR::MakeFlagCheckCondition(c, pRuntime);
		cnds.push_back(c);

		// Now add the clause events
		vector<ConditionList>::iterator i = clauses.begin();
		
		for ( ; i != clauses.end(); i++)
			OR::MakeClauseEvent(*i, pRuntime);

		OR::flagNumber++;
		pRuntime->system.orFlags.push_back(0);

	}//if hadOrConditions

	return hadOrConditions;
}

//////////////////////////////////
// Parse an event
CEventLine* CCapReader::ParseEvent()
{
	// We already know there's a CAP_BEGINEVENT
	cursor++;

	// Read event number; event sheet index
	int eventNumber = READINTFROM(cursor);
	int eventSheet = READINTFROM(cursor);

	//if (eventNumber < 0)
	//	throw runtime_error("Read error: bad event list (13)");

	if (eventSheet < 0 || eventSheet >= pRuntime->eventSheetNames.size())
		throw runtime_error("Read error: bad event list (14)");

	// Expected: begin conditions
	if (pData[cursor] != CAP_BEGINCONDITIONS)
		throw runtime_error("Read error: bad event list (5)");

	cursor++;

	vector<condition> cndList;
	vector<action> actList;

	// Get conditions thru to past CAP_ENDCONDITIONS
	short flags = GetConditions(cndList);

	// Expected: begin actions
	if (pData[cursor] != CAP_BEGINACTIONS)
		throw runtime_error("Read error: bad event list (6)");

	cursor++;

	// Get actions thru to past CAP_ENDACTIONS
	flags |= GetActions(actList);

	// Either CAP_ENDEVENT follows, in which case we make a CEvent,
	// or a CAP_BEGINEVENT for a sub event in which case we make a CEventParent.
	if (pData[cursor] == CAP_ENDEVENT) {
		CEvent* pEvent = new CEvent;
		pEvent->pRuntime = pRuntime;
		pEvent->acts = actList;
		pEvent->cnds = cndList;

		pEvent->trigger = flags & CND_TRIGGERED ? true : false;
		pEvent->fasttrigger = flags & CND_FASTTRIGGERED ? true : false;
		pEvent->manualSolModifier = flags & SOL_MODIFIER ? true : false;
		pEvent->number = eventNumber;
		pEvent->eventSheet = eventSheet;
		cursor++;

		// If this was an OR event, we need to make sure any trigger flags are set off.
		if (SliceAndDiceOrConditions(pEvent->cnds, pEvent->acts)) {
			pEvent->trigger = false;
			pEvent->fasttrigger = false;
		}

		return pEvent;
	}

	// ...or CAP_BEGINEVENT
	if (pData[cursor] == CAP_BEGINEVENT || pData[cursor] == CAP_BEGINGROUP) {
		CEventParent* pEventParent = new CEventParent;
		pEventParent->pRuntime = pRuntime;
		pEventParent->acts = actList;
		pEventParent->cnds = cndList;
		pEventParent->trigger = flags & CND_TRIGGERED ? true : false;
		pEventParent->fasttrigger = flags & CND_FASTTRIGGERED ? true : false;
		pEventParent->manualSolModifier = flags & SOL_MODIFIER ? true : false;
		pEventParent->number = eventNumber;
		pEventParent->eventSheet = eventSheet;

		// If this was an OR event, we need to make sure any trigger flags are set off.
		if (SliceAndDiceOrConditions(pEventParent->cnds, pEventParent->acts)) {
			pEventParent->trigger = false;
			pEventParent->fasttrigger = false;
		}
		
		// Get sub events
		while (pData[cursor] == CAP_BEGINEVENT || pData[cursor] == CAP_BEGINGROUP) {
			CEventLine* pEvLine = ParseEventLine();

			// Ordinary subevent: check for auxiliary events that may have been added by OR expansions
			if (!auxEvents.empty()) {
				EventIterator i = auxEvents.begin();
				EventConstIterator aux_end = auxEvents.end();

				for ( ; i != aux_end; i++) {
					(*i)->number = pEvLine->number;
					pEventParent->subEvents.push_back(*i);
				}
			}

			if (!CheckForElseEvent(pEvLine)) {
				pEventParent->subEvents.push_back(pEvLine);
				previousEvent = &(pEventParent->subEvents.back());
			}
		}

		pEventParent->HasSubEvents = !(pEventParent->subEvents.empty());

		if (pData[cursor] == CAP_ENDEVENT) {
			cursor++;
			return pEventParent;
		}
		else
			throw runtime_error("Read error: bad event list (7)");
	}

	// Expected end event, begin event or begin group
	throw runtime_error("Read error: bad event list (8)");
}

//////////////////////////////////
// Parse a group of events
CEventGroup* CCapReader::ParseEventGroup()
{
	// We already know there's a CAP_BEGINGROUP
	cursor++;

	// Get activated state
	CEventGroup* pGroup = new CEventGroup;
	pGroup->Activated = pData[cursor++] ? true : false;
	pGroup->trigger = false;
	pGroup->fasttrigger = false;
	pGroup->pRuntime = pRuntime;

	CString groupName;
	int groupNameLength = READINTFROM(cursor);
	groupName = &(pData[cursor]);
	cursor += groupNameLength;

	// Store a pointer to this group using the group name
	groupName.MakeLower();
	pRuntime->groupMap[groupName].push_back(pGroup);

	// Get events
	while (pData[cursor] == CAP_BEGINEVENT || pData[cursor] == CAP_BEGINGROUP) {

		CEventLine* curLine = ParseEventLine();

		if (!CheckForElseEvent(curLine)) {
			pGroup->events.push_back(curLine);
			previousEvent = &(pGroup->events.back());
		}
	}

	// Check for endgroup
	if (pData[cursor] != CAP_ENDGROUP)
		throw runtime_error("Read error: bad event list (4)");

	cursor++;

	return pGroup;
}

// Get conditions: returns flags of all conditions OR'd together
short CCapReader::GetConditions(vector<condition> &cndList)
{
	short flags = 0;

	int conditionNumber = 1;

	// Starting on first CAP_BEGINCONDITION
	while (pData[cursor] == CAP_BEGINCONDITION) {
		
		condition CurCnd;

		CurCnd.pRuntime = pRuntime;
		CurCnd.number = conditionNumber++;

		cursor++;

		// Get OID
		CurCnd.oid = READINTFROM(cursor);
		
		// Get CndID
		CurCnd.cndID = READINTFROM(cursor);

		// Get negation byte
		CurCnd.isNegated = READBYTEFROM(cursor);

		// Get movement
		int movement = READINTFROM(cursor);

		// Is a movement?  Switch the OID to the associated movement type for this object
		if (movement >= 0)
			CurCnd.oid = movementMap[MovementMapper(CurCnd.oid, movement)];

		// Find routine immediately for non system object
		if (CurCnd.oid > -1) {

			if (CurCnd.oid >= pRuntime->objects.size()) {
				CString msg;
				msg.Format("The event list is corrupt.\n\n(oid: %d, num objects: %d)", CurCnd.oid, pRuntime->objects.size());
				throw runtime_error((const char*)msg);
			}

			CurCnd.pType = pRuntime->pOid(CurCnd.oid);
			CurCnd.routine = CurCnd.pType->pPlugin->pvCndRoutines->at(CurCnd.cndID).routine;

			// Check for static flag
			short theseFlags = CurCnd.pType->pPlugin->pvCndRoutines->at(CurCnd.cndID).flags;
			flags |= theseFlags;
			CurCnd.isStatic = theseFlags & CND_STATIC;
		}
		else {
			// Get system infos from the System funcs
			CurCnd.routine = SystemObject::GetCndRoutine(CurCnd.cndID);
			CurCnd.pType = NULL;
			CurCnd.isStatic = false;

			flags |= SystemObject::GetCndFlags(CurCnd.cndID);
		}

		// Get parameters
		GetParams(CurCnd.params, CurCnd.oid);

		// Expected end condition
		if (pData[cursor] != CAP_ENDCONDITION)
			throw runtime_error("Read error: bad event list (9)");

		cursor++;

		// Check if all params are constant
		bool allParamsConstant = true;
		ExpressionIterator i;
		
		for (i = CurCnd.params.begin(); i != CurCnd.params.end(); i++) {

			// This one not const
			if (!((*i)->isConstant)) {
				allParamsConstant = false;
				break;
			}
		}

		CurCnd.constantParams = allParamsConstant;
		
		if (CurCnd.pType == NULL)
			CurCnd.cndType = CONDITIONTYPE_SYSTEM;
		else if (CurCnd.pType->contained)
			CurCnd.cndType = CONDITIONTYPE_MULTIPLE;
		else if (!CurCnd.pType->familyMovs.empty())
			CurCnd.cndType = CONDITIONTYPE_MULTIPLEFAMILY;
		else
			CurCnd.cndType = CONDITIONTYPE_SINGLE;

		cndList.push_back(CurCnd);
	}

	// Expected ENDCONDITIONS
	if (pData[cursor] != CAP_ENDCONDITIONS)
		throw runtime_error("Read error: bad event list (11)");

	cursor++;

	return flags;
}

// See fx spec.txt
#define FXACTID_ACTIVATE		0
#define FXACTID_DEACTIVATE		1
#define	FXACTID_FIRST_PARAM		2

void CCapReader::TranslateAction(action& a, int movementID)
{
	// Check for effect actions
	if (movementID < -1) {

		// Get effect index (-2 first, -3 second, etc.)
		int effectIndex = -2 - movementID;

		// Add the object ID parameter
		a.params.push_back(CreateIntegerSysExpression(a.oid, pRuntime));

		// Add the effect index parameter
		a.params.push_back(CreateIntegerSysExpression(effectIndex, pRuntime));

		// Translate action id
		if (a.actID == FXACTID_ACTIVATE)
			a.actID = SYSTEM_FX_ACTIVATE;
		else if (a.actID == FXACTID_DEACTIVATE)
			a.actID = SYSTEM_FX_DEACTIVATE;
		else if (a.actID >= FXACTID_FIRST_PARAM) {
			int paramIndex = a.actID - FXACTID_FIRST_PARAM;
			a.params.push_back(CreateIntegerSysExpression(paramIndex, pRuntime));

			a.actID = SYSTEM_FX_SETPARAM;
		}

		// Make a system action
		a.oid = -1;
	}
}

// Get actions
short CCapReader::GetActions(vector<action> &actList)
{
	short flags = 0;

	// Starting on first CAP_BEGINACTION
	while (pData[cursor] == CAP_BEGINACTION) {
		
		action CurAct;

		CurAct.pRuntime = pRuntime;

		cursor++;

		// Get OID
		CurAct.oid = READINTFROM(cursor);
		
		// Get ActID
		CurAct.actID = READINTFROM(cursor);

		// Get movement
		int movement = READINTFROM(cursor);

		// Is a movement?  Switch the OID to the associated movement type for this object
		if (movement >= 0)
			CurAct.oid = movementMap[MovementMapper(CurAct.oid, movement)];

		// Get parameters
		GetParams(CurAct.params, CurAct.oid);

		// Check for effect actions
		TranslateAction(CurAct, movement);

		// Find routine immediately for non system object
		if (CurAct.oid > -1) {

			if (CurAct.oid >= pRuntime->objects.size()) {
				CString msg;
				msg.Format("The event list is corrupt.\n\n(oid: %d, num objects: %d)", CurAct.oid, pRuntime->objects.size());
				throw runtime_error((const char*)msg);
			}

			CurAct.pType = pRuntime->pOid(CurAct.oid);
			CurAct.routine = CurAct.pType->pPlugin->pvActRoutines->at(CurAct.actID).routine;
			flags |= CurAct.pType->pPlugin->pvActRoutines->at(CurAct.actID).flags;
		}
		// For system object
		else {
			CurAct.routine = SystemObject::GetActRoutine(CurAct.actID);
			flags |= SystemObject::GetActFlags(CurAct.actID);
			CurAct.pType = NULL;
		}

		// Expected end action
		if (pData[cursor] != CAP_ENDACTION)
			throw runtime_error("Read error: bad event list (10)");

		cursor++;

		// Check if all params are constant
		bool allParamsConstant = true;
		ExpressionIterator i;
		
		for (i = CurAct.params.begin(); i != CurAct.params.end(); i++) {

			// This one not const
			if (!((*i)->isConstant)) {
				allParamsConstant = false;
				break;
			}
		}

		CurAct.constantParams = allParamsConstant;

		actList.push_back(CurAct);
	}

	// Expected ENDACTIONS
	if (pData[cursor] != CAP_ENDACTIONS)
		throw runtime_error("Read error: bad event list (12)");

	cursor++;

	return flags;
}

// Get params
void CCapReader::GetParams(ExpressionList& expList, int oid)
{
	// Get number of params
	int paramCount = READINTFROM(cursor);

	for (int i = 0; i < paramCount; i++) {

		CExpression* exp = new CExpression;
		expList.push_back(exp);

		exp->pRuntime = pRuntime;
		exp->oidCaller = oid;

		int tokenCount = READINTFROM(cursor);

		for (int j = 0; j < tokenCount; j++) {

			int iToken = READINTFROM(cursor);

			exp->toks.push_back(Token());
			Token& t = exp->toks.back();
			t.t = (ExpToken)iToken;

			switch (t.t) {
			case T_INTEGER:
				t.iValue = READINT64FROM(cursor);
				break;
			case T_FLOAT:
				t.fValue = READDOUBLEFROM(cursor);
				break;
			case T_STRINGLITERAL:
			case T_IDENTIFIER:
			case T_VARIABLENAME:
				{
					int strLength = READINTFROM(cursor);
					t.str = (const char*)&(pData[cursor]);
					cursor += strLength;
				}
				break;
			}
		}//for

		exp->ParseTokenStream();
	}
}

////////////////////////////////
// Parse a CAP binary frame data and load it in to CRuntime
// pData: pointer to binary data
// len: number of bytes of data
void CCapReader::ReadFrameData(BYTE* _pData, int _len)
{
	pData = _pData;
	len = _len;
	cursor = 0;
	int i;

	bin ar(g_allocator);
	ar.attach(pData, len);

#ifdef CONSTRUCT_DIRECTX9
	UINT ps_major = D3DSHADER_VERSION_MAJOR(pRuntime->renderer.GetCaps().PixelShaderVersion);
	UINT ps_minor = D3DSHADER_VERSION_MINOR(pRuntime->renderer.GetCaps().PixelShaderVersion);
	float ps_version = ps_major;
	ps_version += (float)ps_minor / 10.0f;

#ifdef CONSTRUCT_PREVIEW
	switch (pRuntime->simShader) {
	case SS_PS14:
		ps_version = 1.4f;
		break;
	case SS_PS11:
		ps_version = 1.1f;
		break;
	case SS_PS00:
		ps_version = 0.0f;
		break;
	}
#endif
#endif

	//////////////////
	// OBJECT TYPES //
	//////////////////
	int numObjectTypes = ar.read<int>();

	// Loop and allocate these objects
	for (i = 0; i < numObjectTypes; i++) {

		CRunObjType* pType = new CRunObjType;
		
		int typeID = ar.read<int>();

		CString typeName;
		ar >> typeName;

		int typeIdentifier = ar.read<int>();

		pType->global = ar.read<char>() != 0;
		pType->destroyWhen = (DisableShaderWhen)ar.read<int>();
		pType->disabled = false;

#ifdef CONSTRUCT_DIRECTX9
		switch (pType->destroyWhen) {
		case SVA_PS20UNAVAILABLE:
			if (ps_version < 2.0f)
				pType->disabled = true;
			break;
		case SVA_PS20AVAILABLE:
			if (ps_version >= 2.0f)
				pType->disabled = true;
			break;
		case SVA_PS14UNAVAILABLE:
			if (ps_version < 1.4f)
				pType->disabled = true;
			break;
		case SVA_PS14AVAILABLE:
			if (ps_version >= 1.4f)
				pType->disabled = true;
			break;
		case SVA_PS11UNAVAILABLE:
			if (ps_version < 1.1f)
				pType->disabled = true;
			break;
		case SVA_PS11AVAILABLE:
			if (ps_version >= 1.1f)
				pType->disabled = true;
			break;
		}
#endif

		// Read private variable names
		int numPrivateVars = ar.read<int>();
		for (int j = 0; j < numPrivateVars; j++) {

			CString varName;
			ar >> varName;

			int varType = ar.read<int>();

			pType->privateVars.push_back(PrivateVar(varName, varType));
		}

		ar.advance(16);

		// Get data
		pType->pPlugin = &(pRuntime->plugins[typeIdentifier]);
		pType->pluginID = typeIdentifier;
		pType->Name = typeName;
		pType->contained = false;		// updated later in containers segment
		pType->oid = i;
		pType->pRuntime = pRuntime;
		pType->teamID = -1;
		pType->movement = false;
		pType->sols.push_back(SelObjList());
		pType->pSol = &(pType->sols.front());
		pType->curSol = pType->sols.begin();
		pType->createParams = NULL;
		pType->pLinkType = NULL;
		pType->noSerialize = false;

		if(ar.read<BYTE>() == 1)
		{
#ifdef PYTHONIMPORT
			int actionCount = ar.read<int>();
			for(int a = 0; a < actionCount; a ++){
				pType->pPlugin->py_actions.push_back(PyFunct());
				PyFunct& fct = pType->pPlugin->py_actions.back();
				ar >> fct.name; 
				fct.paramCount = ar.read<int>();		
			}

			int conditionCount = ar.read<int>();
			for(int c = 0; c < conditionCount; c ++){
				pType->pPlugin->py_conditions.push_back(PyFunct());
				PyFunct& fct = pType->pPlugin->py_conditions.back();
				ar >> fct.name; 
				fct.paramCount = ar.read<int>();		
			}

			int expressionCount = ar.read<int>();
			for(int e = 0; e < expressionCount; e ++){
				pType->pPlugin->py_expressions.push_back(PyFunct());
				PyFunct& fct = pType->pPlugin->py_expressions.back();
				ar >> fct.name; 
				fct.paramCount = ar.read<int>();		
			}
#endif
		}


		pRuntime->objects.push_back(pType);
	}

	/////////////////
	// Movement types
	int numMovements = ar.read<int>();

	// Get movement definitions
	for (i = 0; i < numMovements; i++) {

		// Get this movement declaration
		int objoid = ar.read<int>();
				
		int assocDLL = ar.read<int>();
		int movindex = ar.read<int>();

		CRunObjType* pObjType = pRuntime->objects[objoid];

		// Now generate a new object type for this movement
		CRunObjType* pMovType = new CRunObjType;
		ar >> pMovType->AuxName;
		//pMovType->AuxName.MakeLower();
		pMovType->contained = true;
		pMovType->pPlugin = &(pRuntime->plugins[assocDLL]);
		pMovType->movement = true;
		pMovType->Name.Format("Movement_%d_%d", objoid, assocDLL);	// Name generated: eg. Movement_12_2
		int movoid = pRuntime->objects.size();
		pMovType->oid = movoid;
		pMovType->pRuntime = pRuntime;
		pMovType->sols.push_back(SelObjList());
		pMovType->pSol = &(pMovType->sols.front());
		pMovType->teamID = -1;
		pMovType->curSol = pMovType->sols.begin();
		pMovType->disabled = false;
		pMovType->pLinkType = pObjType;
		pMovType->global = pObjType->global;
		pMovType->noSerialize = false;

		// Get the movement params
		int movDataSize = ar.read<int>();

		// Create a new buffer and copy the movement data to it
		BYTE* movData;
		if (movDataSize > 0) {
			movData = new BYTE[movDataSize];
			memcpy(movData, ar.curptr(), movDataSize);
		}
		else
			movData = NULL;

		ar.advance(movDataSize);

		if(ar.read<BYTE>() == 1)
		{
#ifdef PYTHONIMPORT
			int actionCount = ar.read<int>();
			for(int a = 0; a < actionCount; a ++){
				pMovType->pPlugin->py_actions.push_back(PyFunct());
				PyFunct& fct = pMovType->pPlugin->py_actions.back();
				ar >> fct.name; 
				fct.paramCount = ar.read<int>();		
			}

			int conditionCount = ar.read<int>();
			for(int c = 0; c < conditionCount; c ++){
				pMovType->pPlugin->py_conditions.push_back(PyFunct());
				PyFunct& fct = pMovType->pPlugin->py_conditions.back();
				ar >> fct.name; 
				fct.paramCount = ar.read<int>();		
			}

			int expressionCount = ar.read<int>();
			for(int e = 0; e < expressionCount; e ++){
				pMovType->pPlugin->py_expressions.push_back(PyFunct());
				PyFunct& fct = pMovType->pPlugin->py_expressions.back();
				ar >> fct.name; 
				fct.paramCount = ar.read<int>();		
			}
#endif
		}

		// Create an InitialObject for the movement
		initialObject* io = new initialObject;
		memset(io, 0, sizeof(initialObject));

		// Setup an initialObject for the movement.  Only a few valid members for movements.
		io->eAngle = 0.0f;
		io->eData = movData;
		io->eSize = movDataSize;
		io->pluginID = assocDLL;
		io->objectID = movoid;

		// Use as createParams for the movement type.
		pMovType->createParams = io;

		// Make linked type a sister
		pMovType->sisterTypes.push_back(pObjType);
		pObjType->contained = true;					// Also set contained flag in linked type

		// Copy linked type sisters to me
		ObjTypeIterator t = pObjType->sisterTypes.begin();
		ObjTypeConstIterator sisters_end = pObjType->sisterTypes.end();

		for ( ; t != sisters_end; t++)
			pMovType->sisterTypes.push_back(*t);

		// Add me to linked type sisters
		pObjType->sisterTypes.push_back(pMovType);

		// Add type
		pRuntime->objects.push_back(pMovType);

		// Map this object ID and movement DLL to the newly created object type
		movementMap[MovementMapper(objoid, movindex)] = movoid;
	}

	// Read traits
	int numTraits = ar.read<int>();

	for (i = 0; i < numTraits; i++) {

		//int len = ar.read<int>();

		CString traitName;
		ar >> traitName;

		CRunObjType* pTrait = new CRunObjType;
		pTrait->contained = false;
		pTrait->createParams = NULL;
		pTrait->pPlugin = NULL;
		pTrait->pluginID = -1;
		pTrait->movement = false;
		pTrait->Name = traitName;
		pTrait->oid = pRuntime->objects.size();
		pTrait->pRuntime = pRuntime;
		pTrait->teamID = i;
		pTrait->sols.push_back(SelObjList());
		pTrait->pSol = &(pTrait->sols.front());
		pTrait->curSol = pTrait->sols.begin();
		pTrait->disabled = false;
		pTrait->pLinkType = NULL;
		pTrait->noSerialize = false;
		pRuntime->objects.push_back(pTrait);

		int numObjsTrait = ar.read<int>();

		for (int j = 0; j < numObjsTrait; j++) {
			int traitOid = ar.read<int>();
			CRunObjType* pType = pRuntime->objects[traitOid];

			pType->teams.push_back(pTrait);
			pTrait->teams.push_back(pType);
		}
	}

	// READ FAMILIES
	// We need all the family types contiguous to be in keeping with the IDE
	vector<DelayedFamilyMovementAddition> delayedFMAdds;

	int numFamilies = ar.read<int>();

	int familyOidStart = pRuntime->objects.size();

	for (i = 0; i < numFamilies; i++) {
		CString familyName;
		ar >> familyName;

		int numChildren = ar.read<int>();

		// Create a vector of types in this family
		vector<CRunObjType*> familyTypes;
		familyTypes.reserve(numChildren);

		int j;
		for (j = 0; j < numChildren; j++) {
			int oid = ar.read<int>();
			familyTypes.push_back(pRuntime->objects[oid]);
		}

		if (numChildren == 0)
			continue;

		// Create the family type
		CRunObjType* pFamily = new CRunObjType;
		pFamily->contained = false;
		pFamily->createParams = NULL;
		pFamily->pPlugin = familyTypes.front()->pPlugin;	// All family members share plugin type
		pFamily->pluginID = familyTypes.front()->pluginID;
		pFamily->movement = false;
		pFamily->Name = familyName;
		pFamily->oid = pRuntime->objects.size();
		pFamily->pRuntime = pRuntime;
		pFamily->teamID = numTraits + i;
		pFamily->sols.push_back(SelObjList());
		pFamily->pSol = &(pFamily->sols.front());
		pFamily->curSol = pFamily->sols.begin();
		pFamily->disabled = false;
		pFamily->pLinkType = NULL;
		pFamily->noSerialize = false;

		// TODO:
		// sort out private variables and families

		// Add each family type to the team
		for (j = 0; j < numChildren; j++) {
			CRunObjType* curType = familyTypes[j];

			// First?  Use PVs from this object
			/*if (j == 0)
				pFamily->privateVars = curType->privateVars;
			else {

				// Else remove any PVs that aren't common to all
				vector<PrivateVar>::iterator p = pFamily->privateVars.begin();

				for ( ; p != pFamily->privateVars.end(); ) {
					// PV does not exist in this family?  Remove it
					if (exists_element(curType->privateVars.begin(), curType->privateVars.end(), *p))
						p++;
					else
						p = pFamily->privateVars.erase(p);
				}
			}*/

			pFamily->teams.push_back(curType);

			// Add this family to the teams of each object type
			curType->teams.push_back(pFamily);
		}


		// Read private variable names
		int numPrivateVars = ar.read<int>();
		for (j = 0; j < numPrivateVars; j++) {

			CString varName;
			ar >> varName;

			int varType = ar.read<int>();

			pFamily->privateVars.push_back(PrivateVar(varName, varType));
		}

		// We have our list of family private variables.  For each member type, generate a map of
		// what index family private variable goes to which type specific variable.
		for (j = 0; j < numChildren; j++) {
			CRunObjType* curType = familyTypes[j];

			// Insert empty map under this type
			vector<int>& curMap = pFamily->teamPvMap[curType];

			// For this type, iterate all family private vars...
			vector<PrivateVar>::iterator p = pFamily->privateVars.begin();
			vector<PrivateVar>::iterator end = pFamily->privateVars.end();

			// And add the real index of this family private var
			for ( ; p != end; p++)
				curMap.push_back(find_index(curType->privateVars.begin(), curType->privateVars.end(), p->name));
		}

		// Add this family
		pRuntime->objects.push_back(pFamily);

		// Check if the movements in this family are consistent across all types
		// If so, make a family for the movements.
		ObjTypeList familyMovs;
		vector<ObjTypeList> familyMovList;	// Full list of each movement type that goes with each movement index.
		vector<ObjTypeList>::iterator w;	// For iterating familyMovList

		// Generate a list of movements that should be in the family from the first type.
		ObjTypeIterator t = pFamily->teams.front()->sisterTypes.begin();
		ObjTypeIterator end = pFamily->teams.front()->sisterTypes.end();

		for ( ; t != end; t++) {
			if ((*t)->movement) {
				familyMovs.push_back(*t);
				familyMovList.push_back(ObjTypeList());
				familyMovList.back().push_back(*t);
			}
		}

		if (!familyMovs.empty()) {

			// Check if all types are consistent with this.
			t = pFamily->teams.begin() + 1;
			end = pFamily->teams.end();

			bool consistent = true;

			for ( ; t != end; t++) {

				if ((*t)->sisterTypes.size() != familyMovs.size()) {
					consistent = false;
					break;
				}

				ObjTypeIterator u = (*t)->sisterTypes.begin();
				ObjTypeIterator u_end = (*t)->sisterTypes.end();
				ObjTypeIterator v = familyMovs.begin();
				ObjTypeIterator v_end = familyMovs.end();
				w = familyMovList.begin();

				for ( ; u != u_end && v != v_end; u++, w++) {
					if ((*u)->movement) {
						if ((*u)->pPlugin != (*v)->pPlugin) {		// movements differ
							consistent = false;
							break;
						}
						w->push_back(*u);
						v++;
					}
				}

				if (!consistent)
					break;
			}

			// Consistent movements across family: make a new family for each family movement.
			if (consistent) {
				
				w = familyMovList.begin();
				t = familyMovs.begin();
				end = familyMovs.end();

				for ( ; t != end; t++, w++) {
					
					if (!(*t)->movement)
						continue;

					CRunObjType* pFamilyMov = new CRunObjType;
					pFamilyMov->AuxName = (*t)->AuxName;
					pFamilyMov->contained = false;
					pFamilyMov->createParams = NULL;
					pFamilyMov->destroyWhen = SVA_NOSETTING;
					pFamilyMov->disabled = false;
					pFamilyMov->global = false;
					pFamilyMov->noSerialize = false;
					pFamilyMov->movement = true;
					pFamilyMov->Name.Format("FMovement_%d_%d", pFamily->oid, t - familyMovs.begin());
					//pFamilyMov->oid = pRuntime->objects.size();	// assigned later
					pFamilyMov->pLinkType = pFamily;
					pFamilyMov->pluginID = (*t)->pluginID;
					pFamilyMov->pPlugin = (*t)->pPlugin;
					pFamilyMov->pRuntime = pRuntime;
					pFamilyMov->sols.push_back(SelObjList());
					pFamilyMov->pSol = &(pFamilyMov->sols.front());
					pFamilyMov->curSol = pFamilyMov->sols.begin();
					//pFamilyMov->teamID = pFamilyMov->oid;
					pFamilyMov->teams.push_back(pFamily);
					//pFamilyMov->familyMovs.push_back(pFamily);	// (other family movements in pFamily will reside here)
					pFamily->familyMovs.push_back(pFamilyMov);		// Add the type to the family

					// Update all movement types that are a part of this family
					ObjTypeIterator x = w->begin();
					ObjTypeIterator w_end = w->end();

					for ( ; x != w_end; x++) {
						(*x)->teams.push_back(pFamilyMov);
						(*x)->familyMovs.push_back(pFamilyMov);
					}

					/*
					pRuntime->objects.push_back(pFamilyMov);		// Add the object type
					movementMap[MovementMapper(pFamily->oid, t - familyMovs.begin())] = pFamilyMov->oid;
					*/
					delayedFMAdds.push_back(DelayedFamilyMovementAddition(pFamily->oid, t - familyMovs.begin(), pFamilyMov));
				}

			}// if consistent
		}
	}

	// Now add the family movement types in one block
	vector<DelayedFamilyMovementAddition>::iterator da = delayedFMAdds.begin();

	for ( ; da != delayedFMAdds.end(); da++) {
		int myOid = pRuntime->objects.size();
		da->pFMType->oid = myOid;
		da->pFMType->teamID = myOid;	// IsFamily() must return true
		movementMap[da->key] = myOid;
		pRuntime->objects.push_back(da->pFMType);

		// Add every family movement in the family to the family movement's familyMovs vector.
		// This is a bit of a hack, but DoMultipleFamily() expects this.
		da->pFMType->familyMovs.push_back(da->pFMType->pLinkType);

		ObjTypeIterator t = da->pFMType->pLinkType->familyMovs.begin();
		ObjTypeIterator end = da->pFMType->pLinkType->familyMovs.end();

		for ( ; t != end; t++) {
			if ((*t) != da->pFMType)
				da->pFMType->familyMovs.push_back(*t);
		}
	}

	int numContainers = ar.read<int>();

	for (i = 0; i < numContainers; i++) {

		int j;

		int numContainerObjects = ar.read<int>();

		ObjTypeList container;
		container.reserve(numContainerObjects * 2);	// movement types may be added

		// Read all types and combine them in to the complete container
		for (j = 0; j < numContainerObjects; j++) {

			CRunObjType* pCurType = pRuntime->objects[ar.read<int>()];
			container.push_back(pCurType);

			// Existing sister types (eg. movements) must all be collated in to the same container
			ObjTypeIterator t = pCurType->sisterTypes.begin();

			for ( ; t != pCurType->sisterTypes.end(); t++)
				push_back_unique(container, *t);
		}

		// Loop these container object types and make them connected in a container
		ObjTypeIterator t = container.begin();

		for ( ; t != container.end(); t++) {

			(*t)->contained = true;
			(*t)->sisterTypes.reserve(container.size() - 1);

			// Loop the container again adding all sister types
			ObjTypeIterator u = container.begin();

			for ( ; u != container.end(); u++) {
				if (*t == *u) continue;		// Type does not add self to sisterTypes

				push_back_unique((*t)->sisterTypes, *u);
			}
		}
	}

	pRuntime->objects.resize(pRuntime->objects.size());

	//////////////////
	// PARSE FRAMES //
	//////////////////
	int numFrames = ar.read<int>();

	for (int h = 0; h < numFrames; h++) {

		pLayout = new CRunLayout(pRuntime);
		pRuntime->Frames.push_back(pLayout);

		pLayout->typeReferences = pRuntime->objects;

		// Frame details
		pLayout->width = ar.read<int>();
		pLayout->height = ar.read<int>();
		ar >> pLayout->name;
		pLayout->bg_colorref = ar.read<int>();
		pLayout->unboundedScrolling = ar.read<bool>();
		bool appBackground = ar.read<bool>();

		if (appBackground)
			pLayout->bg_colorref = GetSysColor(COLOR_3DFACE);

		pLayout->bg_color = cr::color(pLayout->bg_colorref);

		pLayout->winWidth = pRuntime->winWidth;
		pLayout->winHeight = pRuntime->winHeight;

		/*
		// Minimum layout size is window size
		if (pLayout->width < pRuntime->winWidth)
			pLayout->width = pRuntime->winWidth;
		if (pLayout->height < pRuntime->winHeight)
			pLayout->height = pRuntime->winHeight;
		*/

		pLayout->scrollX = 0;
		pLayout->scrollY = 0;
		pLayout->mouseX = 0;
		pLayout->mouseY = 0;
		pLayout->frameXoffset = 0;
		pLayout->frameYoffset = 0;
		pLayout->number = h;

		// Read layout keys
		int keyCount = ar.read<int>();

		for (int keys = 0; keys < keyCount; keys++) 
		{
			CString s;
			int data = 0;
			int type = 0;
			ar >> s;
			type = ar.read<int>();

			if (type == 0)
			{
				data = ar.read<int>();
				pLayout->frameKeys[s] = (void*)data;
			}

			else
			{
				CString str;
				ar >> str;
				pLayout->frameKeysStrings[s] = str;
			}
		}

		//pLayout->initObjects.resize(pRuntime->objects.size());

		////////////
		// LAYERS //
		////////////

		int numlayers = ar.read<int>();

		for (i = 0; i < numlayers; i++) {

			int layerID = ar.read<int>();			// Layer ID
		
			CRunLayer* pLayer = new CRunLayer(pRuntime);
			pLayout->layers.push_back(pLayer);

			ar >> pLayer->name;
			pLayer->name.MakeLower();

			pLayer->number = i;

			pLayer->type = ar.read<char>();
			COLORREF layerColour = ar.read<int>();
			float a = ar.read<float>();

			pLayer->opacity = a * 100.0;
			pLayer->colourfilter = cr::color(layerColour);
			pLayer->UpdateFilter();

			pLayer->angle = ar.read<float>();
			pLayer->scrollXf = ar.read<float>();
			pLayer->scrollYf = ar.read<float>();
			pLayer->scrollXoffset = ar.read<float>();
			pLayer->scrollYoffset = ar.read<float>();
			pLayer->zoomXf = ar.read<float>();
			pLayer->zoomYf = ar.read<float>();
			pLayer->zoomXoffset = ar.read<float>();
			pLayer->zoomYoffset = ar.read<float>();

			char clearBack = ar.read<char>();
			COLORREF backColor = ar.read<int>();
			pLayer->forceOwnTexture = ar.read<bool>();
			pLayer->sampler = (CRunLayer::LayerSampler)ar.read<int>();
			pLayer->enable3d = ar.read<bool>();

			// If clearing the background convert the colorref
			if (clearBack)
				pLayer->backColor = cr::color(backColor);	// convert colorref
			else
				pLayer->backColor = cr::transparent_black;

			////////////////////////////////////
			// OBJECT INSTANCES ON THIS LAYER //
			////////////////////////////////////

			int layerInstanceCount = ar.read<int>();

	#ifdef CONSTRUCT_DIRECTX9
			// Layer has effects - a temp layer draw surface is needed
			if (pLayer->HasEffects() && pRuntime->tempLayerTarget == unallocated_texture)
				pRuntime->tempLayerTarget = pRuntime->CreateDisplayTargetTexture();
	#endif

			for (int j = 0; j < layerInstanceCount; j++) {

				pLayout->initObjects.push_back(initialObject());
				initialObject& et = pLayout->initObjects.back();

				// Get properties
				ar.read<int>();		// ignore identifier
				et.eX = ar.read<int>();
				et.eY = ar.read<int>();
				et.eWidth = ar.read<int>();
				et.eHeight = ar.read<int>();
				et.eAngle = ar.read<float>();
				et.eColor = cr::color_from_D3DCOLOR<cr_float>(ar.read<int>());	// don't premultiply here - this is for plugins which operate on non-premultiplied filters

				// 3 object IDs
				int instanceIdentifier = ar.read<int>();
				int instanceID = ar.read<int>();
				int instanceGlobalID = ar.read<int>();

				et.pluginID = instanceIdentifier;	// plugin/DLL identifier
				et.objectID = instanceGlobalID;	// Object type identifier
				et.instanceID = instanceID;		// Instance identifier
				et.layer = i;

				CRunObjType* pMyType = pRuntime->objects[et.objectID];

				// Initial private variables for this instance
				int numVars = ar.read<int>();
				for (int k = 0; k < numVars; k++) {

					CString varValue;
					ar >> varValue;
					et.initVars.push_back(varValue);
				}

				int eSize = ar.read<int>();
				if (eSize > 0) {
					et.eData = new BYTE[eSize];
					memcpy(et.eData, ar.curptr(), eSize);
				}
				else
					et.eData = NULL;

				et.eSize = eSize;
				ar.advance(eSize);

				// If my type has no createParams, use me as the default object to create
				if (!pMyType->createParams)
					pMyType->createParams = &et;

			}//for objects

		}//for layers

		int imagehandles_count = ar.read<int>();

		for (int t = 0; t < imagehandles_count; t++) {
			int imagehandle = ar.read<int>();

#ifndef APPRUNTIME
			ImageMap::iterator i = pRuntime->imagehandle_to_address.find(imagehandle);

			if (i == pRuntime->imagehandle_to_address.end())
				throw runtime_error("Invalid image handle referenced in layout (Davo's fault)");

			// Add the image handle to the layout references
			pLayout->used_image_handles.push_back(i->second);
#endif
		}

		pLayout->texture_loading = (CRunLayout::TextureLoading)ar.read<int>();

		/*
		// Generate quick report
		std::stringstream ss;
		ss << pLayout->used_image_handles.size() << " used textures for layout '" << pLayout->name << "':\n\n";
		
		ImageHandleIterator im = pLayout->used_image_handles.begin();
		for ( ; im != pLayout->used_image_handles.end(); ++im) {
			ss << (*im)->th->image_width << " x " << (*im)->th->image_height << "\n";
		}

		MessageBox(NULL, ss.str().c_str(), "Used images", MB_OK);
		*/

	}// for layouts

#ifndef APPRUNTIME
	// Read animations (not in app runtime)
	ParseAnimations(pRuntime->animationBank, ar);

	// Iterate the animation bank and set up the animation map
	Animations::iterator an = pRuntime->animationBank.begin();
	Animations::const_iterator animationBank_end = pRuntime->animationBank.end();

//	for (i = 0; an != animationBank_end; an++, i++)
	//	pRuntime->animationMap[an->handle] = i;
#endif

	ar.detach();

	//return pLayout;
}

#ifndef APPRUNTIME

// Read a block of animations
void CCapReader::ParseAnimations(Animations& animList, bin& ar)
{
	// Read animations
	int numAnims = ar.read<int>();
	vector<int> handles;
	int i;

	for (i = 0; i < numAnims; i++) {

		animList.push_back(CRunAnimation());
		CRunAnimation& anim = animList.back();
		anim.handle = ar.read<int>();
		handles.push_back(anim.handle);

		ar >> anim.name;
		anim.name.MakeLower();

		ar >> anim.tagID;

		anim.speed = ar.read<float>();
		anim.is_angle = ar.read<BYTE>() == 1 ? true: false;
		anim.angle = ar.read<float>();

		anim.repeat = ar.read<int>();
		anim.repeat_to = ar.read<int>();
		anim.pingpong = ar.read<int>() != 0;
		

		int numFrames = ar.read<int>();

		for (int j = 0; j < numFrames; j++) {
			anim.frames.push_back(CRunAnimationFrame());
			CRunAnimationFrame& animFrame = anim.frames.back();

			animFrame.timeFactor = ar.read<float>();
			int imgHandle = ar.read<int>();
			animFrame.imgHandle = pRuntime->imagehandle_to_address[imgHandle];		// convert handle to address
			ImageHandleInfo& info = *(animFrame.imgHandle);
			animFrame.th = unallocated_texture;
			animFrame.pivotX = info.hotspot.x;
			animFrame.pivotY = info.hotspot.y;
			animFrame.pivotAngle = atan2(animFrame.pivotY, animFrame.pivotX);
			animFrame.pivotDist = sqrt(animFrame.pivotX * animFrame.pivotX + animFrame.pivotY * animFrame.pivotY);

			// Unknown till animation calls Load()
			//animFrame.width = info.th->image_widthf;
			//animFrame.height = info.th->image_heightf;
			animFrame.width = 0;
			animFrame.height = 0;
		}

		// Get sub animations
		ParseAnimations(anim.subAnims, ar);

		int a;
		for (Animations::iterator i = anim.subAnims.begin(); i!= anim.subAnims.end(); )
		{
			if(i->is_angle)
			{
				anim.Angles.push_back(*i);
				anim.subAnims.erase(i);
				i = anim.subAnims.begin();
			}
			else
			{
				i++;
			}	
		}
		a = 0;
		for (Animations::iterator i = anim.Angles.begin(); i!= anim.Angles.end(); i++)
		{
			i->parent = &anim;
			pRuntime->animationPtrs[i->handle] = &*i;
			a++;
		}
		a = 0;
		for (Animations::iterator i = anim.subAnims.begin(); i!= anim.subAnims.end(); i++ )
		{
			i->parent = &anim;
			pRuntime->animationPtrs[i->handle] = &*i;
			a++;		
		}
		anim.parent = 0; // the parent who called this function will change this value so dont worry

		anim.GetReferencedImages(anim.referenced_images);

	}

	Animations::iterator a = animList.begin();
	for (int i = 0; i < numAnims; i++)
	{
		pRuntime->animationPtrs[a->handle] = &*a;
		a++;
	}
}

#endif

// Constructor for movement mapper
MovementMapper::MovementMapper(int _oid, int _movDll)
{ oid = _oid; movDll = _movDll; }

// Operator< for movement mapper
bool MovementMapper::operator <(const MovementMapper &mm) const
{
	__int64 left = ((__int64)oid << 32) + movDll;
	__int64 right = ((__int64)mm.oid << 32) + mm.movDll;

	return left < right;
}

// Recurse and add a submenu
void CCapReader::BuildMenu(HMENU Menu, bin& ar)
{
	// Load the name, ID, etc
	CString Name;
	int ID	= 0;

	ar >> Name >> ID;

	// Read sub item count
	int Count = 0;
	ar >> Count;

	// Rinse and repeat
	if (Count != 0)
	{
		HMENU SubMenu = CreatePopupMenu();

		for (int i = 0; i < Count; i++)
			BuildMenu(SubMenu, ar);

		AppendMenu(Menu, MF_POPUP, (int)SubMenu, Name);
	}
	
	else
		AppendMenu(Menu, MF_STRING, -1, Name);
}	

// Read menu resources to HMENUs
void CCapReader::ReadMenuResources(BYTE* _pData, int _len, vector<HMENU>& menus)
{
	// Still crashes, last checked 15 sept -AG
	/*
	// Set pointers
	pData = _pData;
	len = _len;
	cursor = 0;

	// Attach to bin
	bin ar(g_allocator);
	ar.attach(pData, len);

	int Count = 0;
	ar >> Count;

	for (int i = 0; i < Count; i++)
	{
		HMENU Menu;

		// Create menu and initialize it
		Menu = CreateMenu();

		// Do main starting items
		int Starting = 0;
		ar >> Starting;
		
		for (int i = 0; i < Starting; i++)
		{
			// Load text and ID
			CString Text;

			ar >> Text;

			AppendMenu(Menu, MF_STRING, -1, Text);

			HMENU SubMenu = GetSubMenu(Menu, i);

			int SubItems = 0;
			ar >> SubItems;

			// Rinse and repeat
			if (SubItems != 0)
				BuildMenu(SubMenu, ar);
		}

		menus.push_back(Menu);
	}

	ar.detach();
	*/
}

void CCapReader::ReadPythonResources(BYTE* _pData, int _len)
{
	pData = _pData;
	len = _len;
	cursor = 0;

	HANDLE	handle;
	DWORD	written;

	handle = CreateFile(pRuntime->pythonZipPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	WriteFile(handle, pData, len, &written, 0);

	CloseHandle(handle);
}

// Read the general app property flags to an AppProperties struct
void CCapReader::ReadAppProperties(BYTE* _pData, int _len, AppProperties& props)
{
	// Set pointers & cursor
	pData = _pData;
	len = _len;
	cursor = 0;

	bin ar(g_allocator);
	ar.attach(pData, len);

	ar >> props.appTitle;

	ar >> props.winWidth >> props.winHeight;
	ar >> props.UseMenu; 

	ar >> props.screensaver;

	props.fpsMode = (cr::framerate_mode)ar.read<BYTE>();

	props.fps = ar.read<int>();

	props.fullscreen = ar.read<BYTE>()!=0;

	props.sampler = ar.read<int>();

	int numGlobals = ar.read<int>();

	for (int i = 0; i < numGlobals; i++) {
		CString name;
		int type;
		CString value;

		ar >> name >> type >> value;

		pRuntime->globalNames.push_back(name);

		if (type == 0)	// numeric
			pRuntime->globalVars.push_back(ExpStore(atof(value)));
		else if (type == 1)	//text
			pRuntime->globalVars.push_back(ExpStore(value));
	}

	// Read controls
	int numControls = ar.read<int>();

	for (int i = 0; i < numControls; i++) {
		CString control;
		int vk;
		int player;

		ar >> control >> vk >> player;

		control.MakeLower();
		pRuntime->keyControls[make_pair(control, player)] = RunControlInfo(vk);

		// Make alt buttons work
		if (vk == VK_MENU || vk == VK_LMENU || vk == VK_RMENU)
			pRuntime->disableAlt = true;
	}

	pRuntime->disableWindowsKey = ar.read<bool>();

	int numGlobalKeys = ar.read<int>();

	for (int i = 0; i < numGlobalKeys; i++)
	{
		CString s;
		int data;
		int type;
		ar >> s >> type;

		// Handle type and new system
		if (type == 0)
		{
			ar >> data;
			pRuntime->pluginKeys[s] = (void*)data;
		}
		else
		{
			CString str;
			ar >> str;
			pRuntime->pluginKeysStrings[s] = str;
		}
	}

#if defined(CONSTRUCT_DIRECTX9) && defined(CONSTRUCT_PREVIEW)
	pRuntime->simShader = (SimulatedShader)ar.read<int>();
#else
	pRuntime->simShader = SS_NOSIM;
	ar.read<int>();		// Discard value
#endif

	ar >> pRuntime->capFile;

	// Set the current directory to the .cap's path
	CString AppPath(pRuntime->capFile);
	int backslash = AppPath.ReverseFind('\\');
	AppPath.Delete(backslash + 1, AppPath.GetLength() - backslash - 1);
	SetCurrentDirectory(AppPath);

	ar >> pRuntime->fpsInCaption;

#ifdef CONSTRUCT_DIRECTX9
	ar >> pRuntime->motionBlur;
	ar >> pRuntime->blurNumSteps;
#else
	bool dummy1;
	int dummy2;
	ar >> dummy1 >> dummy2;

#endif

	int textRenderingMode;
	ar >> textRenderingMode;

	DWORD textModes[] = {
		NONANTIALIASED_QUALITY,
		ANTIALIASED_QUALITY,
		CLEARTYPE_QUALITY
	};

#ifdef CONSTRUCT_DIRECTX9
	pRuntime->textQuality = textModes[textRenderingMode];
#endif

	ar >> pRuntime->overrideTimeDelta;
	ar >> pRuntime->overrideTimeDeltaFPS;

	ar >> pRuntime->caption;
	ar >> pRuntime->minimize_box;
	pRuntime->resizable = (CRuntime::ResizeSetting)ar.read<int>();
	ar >> pRuntime->maximize_box;

	float min_fps_as_float;
	ar >> min_fps_as_float;
	pRuntime->minimum_fps = min_fps_as_float;	// minimum_fps is a double

	ar >> pRuntime->previewLayout;

	ar >> pRuntime->multisamples;

	pRuntime->texture_loading = (CRuntime::TextureLoading)ar.read<int>();

	ar.detach();
}

// Read the image data in to CD3D textures
#ifndef APPRUNTIME	// DirectX runtime

void DbgCollisionMaskToFile(CollisionMask& cm, int index, int id);

// Load the images data segment for a DirectX game.  All textures are loaded and added, and
// the map is generated to convert an image handle to a texture ID.

#ifndef CONSTRUCT_SDL
void CCapReader::ReadImageData(BYTE* _pData, int _len, ImageMap& imageMap, bool texturesOnly)
{
	pData = _pData;
	len = _len;
	cursor = 0;

	Renderer& renderer = pRuntime->renderer;

	// Number of images header
	int numImages = READINTFROM(cursor);
	int imageCount = 0;

	// Read
	for ( ; imageCount < numImages; imageCount++) {

		int imageHandle = READINTFROM(cursor);

		ImageHandleInfo* ih = new ImageHandleInfo;
		imageMap[imageHandle] = ih; 

		ih->hotspot.x = READINTFROM(cursor);
		ih->hotspot.y = READINTFROM(cursor);
		
		int actioncount = READINTFROM(cursor);
		for(int a = 0; a < actioncount; a ++)
		{
			if (texturesOnly) {
				cursor += sizeof(int);
				cursor += sizeof(int);

				int nameLength = READINTFROM(cursor);
				cursor += nameLength;
			}
			else {
				ih->points.push_back(ImagePoint());
				ImagePoint& ap = ih->points.back();

				ap.x = READINTFROM(cursor);
				ap.y = READINTFROM(cursor);

				int nameLength = READINTFROM(cursor);
				ap.name = (const char*)&(pData[cursor]);
				cursor += nameLength;
			}
		}

		int imageDataSize = READINTFROM(cursor);

		// If requesting to load all textures on startup, application will reference all textures
		/*
		if (pRuntime->texture_loading == CRuntime::tl_load_on_app_start) {
			ih->TransferToVRAM(renderer);
			ih->th->IncrementReferences();
		}
		*/

		// Store its PNG file
		ih->pngFileSize = imageDataSize;
		ih->pngFile = new BYTE[imageDataSize];
		
		// Make a copy of the PNG file in the image map
		memcpy(ih->pngFile, &(pData[cursor]), imageDataSize);

		// Advance cursor
		cursor += imageDataSize;

		// Read collision bitmask data
		CollisionMask& cm = ih->cm;
		int mask_width = READINTFROM(cursor);
		int mask_height = READINTFROM(cursor);
		int mask_pitch = READINTFROM(cursor);
		cm.width = mask_width;			// Keep fixed point logic for memory offsets
		cm.height = mask_height;
		cm.pitch = mask_pitch;

		if (texturesOnly) {
			cursor += mask_pitch * mask_height;
		}
		else {
			cm.bits[0] = cm.bits[1] = cm.bits[2] = cm.bits[3] = cm.bits[4] = cm.bits[5] = cm.bits[6] = cm.bits[7] = NULL;
			cm.mem_size = 0;

			// If collision mask data - read it in
			if (mask_pitch * mask_height > 0) { 

				cm.mem_size = mask_pitch * mask_height + 16;

				// Aligned allocate a new mask (in case of using SSE)
				cm.bits[0] = (BYTE*)_aligned_malloc(cm.mem_size, 16);

				// Copy the collision mask at the cursor to aligned memory
				memcpy(cm.bits[0], &(pData[cursor]), mask_pitch * mask_height);

				// Set the trailing memory clear
				memset(cm.bits[0] + (mask_pitch * mask_height), 0, 16);

				// Now generate the shifted masks
				pRuntime->GenerateShiftedMasks(cm);

				//static int id = 0;
				//DbgCollisionMaskToFile(cm, 0, id++);

				cursor += mask_pitch * mask_height;
			}
			// No collision data: use bounding box mode
			else
				cm.boundingbox = true;
		}
	}
}
#else
// SDL loading images
void CCapReader::ReadImageData(BYTE* _pData, int _len, CDisplay& pDisplay, ImageMap& imageMap, bool texturesOnly)
{
	pData = _pData;
	len = _len;
	cursor = 0;

	// Number of images header
	int numImages = READINTFROM(cursor);
	int imageCount = 0;

	// Read
	for ( ; imageCount < numImages; imageCount++) {

		int imageHandle = READINTFROM(cursor);
		imageMap[imageHandle].hotspot.x = READINTFROM(cursor);
		imageMap[imageHandle].hotspot.y = READINTFROM(cursor);
		
		int actioncount = READINTFROM(cursor);
		for(int a = 0; a < actioncount; a ++)
		{
			if (texturesOnly) {
				cursor += sizeof(int);
				cursor += sizeof(int);

				int nameLength = READINTFROM(cursor);
				cursor += nameLength;
			}
			else {
				imageMap[imageHandle].points.push_back(ImagePoint());
				ImagePoint& ap = imageMap[imageHandle].points.back();

				ap.x = READINTFROM(cursor);
				ap.y = READINTFROM(cursor);

				int nameLength = READINTFROM(cursor);
				ap.name = (const char*)&(pData[cursor]);
				cursor += nameLength;
			}
		}

		// Dump to disk
		int imageDataSize = READINTFROM(cursor);

		CString curFile;
		curFile.Format("%stemp.png", pRuntime->tempDir);

		// Create the file for writing
		FILE* f = fopen(CSTR(curFile), "wb");
		if (f == NULL) throw runtime_error("Error unpacking images");

		// Write the resource data to disk
		fwrite(&(pData[cursor]), 1, imageDataSize, f);
		fclose(f);

		SDL_Surface* temp = IMG_Load(curFile);

		// Create alpha surface
		SDL_Surface* pNew = SDL_DisplayFormatAlpha(temp);
		SDL_FreeSurface(temp);

		// Alpha
		SDL_SetAlpha(pNew, SDL_SRCALPHA, 255);

		CSDLTexture Texture;
		Texture.pImage = pNew;
		Texture.Attach();

		// Add the texture immediately
		int textureHandle = pDisplay.textures.size();

		pDisplay.textures[textureHandle] = Texture;

		// Add the map entry (which texture ID this handle now maps to)
		imageMap[imageHandle].iTexture = textureHandle;

		// Advance cursor
		cursor += imageDataSize;

		// Read collision bitmask data
		CollisionMask& cm = imageMap[imageHandle].cm;
		cm.width = READINTFROM(cursor);
		cm.height = READINTFROM(cursor);
		cm.pitch = READINTFROM(cursor);

		if (texturesOnly) {
			cursor += cm.pitch * cm.height;
		}
		else {
			cm.bits[0] = cm.bits[1] = cm.bits[2] = cm.bits[3] = cm.bits[4] = cm.bits[5] = cm.bits[6] = cm.bits[7] = NULL;

			// If collision mask data - read it in
			if (cm.pitch * cm.height > 0) { 

				// Aligned allocate a new mask (in case of using SSE)
				cm.bits[0] = (BYTE*)_aligned_malloc(cm.pitch * cm.height, 16);

				// Copy the collision mask at the cursor to aligned memory
				memcpy(cm.bits[0], &(pData[cursor]), cm.pitch * cm.height);

				// Now generate the shifted masks
				pRuntime->GenerateShiftedMasks(cm);

				static int id = 0;
				//DbgCollisionMaskToFile(cm, 0, id++);

				cursor += cm.pitch * cm.height;
			}
			// No collision data: use bounding box mode
			else
				cm.boundingbox = true;
		}
	}
}
#endif

#ifdef CONSTRUCT_DIRECTX9

bool CCapReader::ParseEffects(EffectList& effects, CRuntime* pRuntime, int numFX)
{
	// DirectX only

	bool ret = true;

	// 1 effect draws to temp surface 0 then to display
	// 2+ effects draw to and from both temp surfaces
	if (numFX == 1 && pRuntime->tempRenderTarget[0] == unallocated_texture)
		pRuntime->tempRenderTarget[0] = pRuntime->CreateDisplayTargetTexture();
	if (numFX >= 2) {
		if (pRuntime->tempRenderTarget[0] == unallocated_texture)
			pRuntime->tempRenderTarget[0] = pRuntime->CreateDisplayTargetTexture();
		if (pRuntime->tempRenderTarget[1] == unallocated_texture)
			pRuntime->tempRenderTarget[1] = pRuntime->CreateDisplayTargetTexture();
	}

	// Update the object effects list.
	for (int j = 0; j < numFX; j++) {
		
		int effectIndex = READINTFROM(cursor);

		CString name;
		int length = READINTFROM(cursor);
		name = (const char*)&(pData[cursor]);
		cursor += length;

		int temp = READINTFROM(cursor);
		DisableShaderWhen disableWhen = (DisableShaderWhen)temp;

		bool shaderEnabled = true;

		float ps_version = ps_major;
		ps_version += (float)ps_minor / 10.0f;

		// Check the effect was allowed to be added
		switch (disableWhen) {
		case SVA_PS20UNAVAILABLE:
			if (ps_version < 2.0f)
				shaderEnabled = false;
			break;
		case SVA_PS20AVAILABLE:
			if (ps_version >= 2.0f)
				shaderEnabled = false;
			break;
		case SVA_PS14UNAVAILABLE:
			if (ps_version < 1.4f)
				shaderEnabled = false;
			break;
		case SVA_PS14AVAILABLE:
			if (ps_version >= 1.4f)
				shaderEnabled = false;
			break;
		case SVA_PS11UNAVAILABLE:
			if (ps_version < 1.1f)
				shaderEnabled = false;
			break;
		case SVA_PS11AVAILABLE:
			if (ps_version >= 1.1f)
				shaderEnabled = false;
			break;
		}

		// Check effect index exists
		vector<CRunEffect*>::iterator ef = pRuntime->effects.begin();
		for ( ; ef != pRuntime->effects.end(); ef++) {
			if ((*ef)->id == effectIndex)
				break;
		}

		// Only add effect if it is supported.
		if (ef != pRuntime->effects.end() && shaderEnabled) {

			CRunEffect* pCurEffect = *ef;

			pCurEffect->name = name;

			effects.push_back(InstanceEffect());
			InstanceEffect& ie = effects.back();
			ie.fx = pCurEffect;
			ie.enabled = true;

			CRunEffect* pFirstEffect = effects.front().fx;

			// If any effect sets borderMode, set first, hence all effects to use border mode.
			if (pCurEffect->borderMode)
				pFirstEffect->borderMode = true;

			// Cross sampling uses another texture
			if ((numFX == 1 || pCurEffect->crossSampling) && (pRuntime->tempRenderTarget[1] == unallocated_texture))
				pRuntime->tempRenderTarget[1] = pRuntime->CreateDisplayTargetTexture();

			// SourceTexture must exist when both temp targets are used in effects chain and SourceTexture referenced
			if (pCurEffect->vSourceTexture && numFX >= 2 && pRuntime->tempSourceTarget == unallocated_texture) {
				pFirstEffect->usesSourceTexture = true;
				pRuntime->tempSourceTarget = pRuntime->CreateDisplayTargetTexture();
			}

			// For each parameter
			int numParams = READINTFROM(cursor);

			// Prevents the master effect in CRuntime effects list getting duplicated parameters
			bool addMasterParams = pCurEffect->params.empty();

			for (int k = 0; k < numParams; k++) {

				CRunEffectParameter p;
				p.type = READINTFROM(cursor);
				int length = READINTFROM(cursor);
				p.name = (const char*)&(pData[cursor]);
				cursor += length;
				length = READINTFROM(cursor);
				p.varname = (const char*)&(pData[cursor]);
				cursor += length;
				length = READINTFROM(cursor);
				p.value = (const char*)&(pData[cursor]);
				p.fValue = atof(p.value);
				cursor += length;

				if (addMasterParams)
					pCurEffect->params.push_back(p);

				ie.params.push_back(p);
			}

			if (addMasterParams)
				pCurEffect->SetupCustomParameters();

			// Loop each parameter and copy the handle
			for (int k = 0; k < numParams; k++)
				ie.params[k].handle = pCurEffect->params[k].handle;
		}
		// Effect is not supported.  Skip over parameter data.
		else {

			// An enabled shader is in use, but we are not allowed to run without that pixel shader
			// version.  The user has specified NOT to just skip over the effect, but prevent execution.
			if (shaderEnabled && !runWithoutPs)
				ret = false;

			// For each parameter
			int numParams = READINTFROM(cursor);

			for (int k = 0; k < numParams; k++) {

				int length = READINTFROM(cursor);
				length = READINTFROM(cursor);
				cursor += length;
				length = READINTFROM(cursor);
				cursor += length;
				length = READINTFROM(cursor);
				cursor += length;
			}
		}
		
	}

	return ret;
}
#endif 

void CCapReader::ReadHLSLData(unsigned char *_pData, int _len)
{
#ifdef CONSTRUCT_DIRECTX9
	pData = _pData;
	len = _len;
	cursor = 0;
	int i;

	// Run without shaders: allow effects not supported to be disabled
	runWithoutPs = READINTFROM(cursor);

	int numHLSL = READINTFROM(cursor);

	// For version checking pixel shader versions
	ps_major = D3DSHADER_VERSION_MAJOR(pRuntime->renderer.GetCaps().PixelShaderVersion);
	ps_minor = D3DSHADER_VERSION_MINOR(pRuntime->renderer.GetCaps().PixelShaderVersion);

#ifdef CONSTRUCT_PREVIEW
	switch (pRuntime->simShader) {
	case SS_PS14:
		ps_major = 1;
		ps_minor = 4;
		break;
	case SS_PS11:
		ps_major = 1;
		ps_minor = 1;
		break;
	case SS_PS00:
		ps_major = 0;
		ps_minor = 0;
		break;
	}
#endif

	for (i = 0; i < numHLSL; i++) {

		int hlslNameLength = READINTFROM(cursor);
		CString hlslName = (const char*)&(pData[cursor]);
		cursor += hlslNameLength;

		int majorVer = READINTFROM(cursor);
		int minorVer = READINTFROM(cursor);

		char hlslCrossSampling = READBYTEFROM(cursor);
		char hlslBorderMode = READBYTEFROM(cursor);
		char hlslFullscreenMode = READBYTEFROM(cursor);

		// Get HLSL data size
		int hlslSize = READINTFROM(cursor);

		// This pixel shader version not supported
		if (ps_major < majorVer || (ps_major == majorVer && ps_minor < minorVer)) {

			max_ps_major = majorVer;
			max_ps_minor = minorVer;

			// Skip shader data
			cursor += hlslSize;
			continue;
		
		}
		
		// Create an effect from the data
		CRunEffect* e = new CRunEffect;
		e->id = i;
		ID3DXBuffer* errors;
		D3DXCreateEffect(pRuntime->renderer.GetDevice(), &(pData[cursor]), hlslSize, NULL, NULL, 0, NULL, &(e->pEffect), &errors);

		if (errors != NULL)
			throw (const char*)(CString("Errors loading effect ") + hlslName + ":\n\n" + (const char*)errors->GetBufferPointer());
		
		// knowing the hlsl name is no longer useful, instead it uses the name given to the effect
		//e->name = hlslName;
		e->crossSampling = hlslCrossSampling != 0;
		e->borderMode = hlslBorderMode != 0;
		e->fullScreenMode = hlslFullscreenMode != 0;
		e->usesSourceTexture = false;
		e->majorVer = majorVer;
		e->minorVer = minorVer;
		e->renderWithRenderStates = false;

		if(e->majorVer == 0) {
			e->renderWithRenderStates = true;

			// Retrieve renderstate
			UINT passes;
			pRuntime->renderer.BeginScene();
			e->pEffect->Begin(&passes, NULL);
			e->pEffect->BeginPass(0);
			e->pEffect->CommitChanges();
			pRuntime->renderer.GetDevice()->GetRenderState(D3DRS_SRCBLEND, &e->SrcBlend);
			pRuntime->renderer.GetDevice()->GetRenderState(D3DRS_DESTBLEND, &e->DestBlend);
			e->pEffect->EndPass();
			e->pEffect->End();
			pRuntime->renderer.EndScene();
			
		}

		e->GetParameters();

		pRuntime->effects.push_back(e);

		cursor += hlslSize;
	}

	// Now get the list of objects and which HLSL they use
	int numObjectsHLSL = READINTFROM(cursor);

	for (i = 0; i < numObjectsHLSL; i++) {

		int oid = READINTFROM(cursor);
		int numFX = READINTFROM(cursor);

		CRunObjType* pType = pRuntime->objects[oid];

		if (!ParseEffects(pType->effects, pRuntime, numFX)) {

			// Only fail if object wasn't going to be created anyway
			if (!pType->disabled) {
				CString msg;
				msg.Format("This application uses Pixel Shader %d.%d for effects, but your graphics hardware only supports Pixel Shader %d.%d effects.  Please upgrade your graphics hardware or try again on another computer.",
						max_ps_major, max_ps_minor, ps_major, ps_minor);
				throw runtime_error((const char*)msg);
			}
		}
	}

	int numLayersHLSL = READINTFROM(cursor);

	for (i = 0; i < numLayersHLSL; i++) {

		int frameNum = READINTFROM(cursor);
		int layerIndex = READINTFROM(cursor);
		int numFX = READINTFROM(cursor);

		CRunLayout* pLayout = pRuntime->Frames[frameNum];

		if (!ParseEffects(pLayout->layers[layerIndex]->effects, pRuntime, numFX)) {
			CString msg;
			msg.Format("This application uses Pixel Shader %d.%d for effects, but your graphics hardware only supports Pixel Shader %d.%d effects.  Please upgrade your graphics hardware or try again on another computer.",
					max_ps_major, max_ps_minor, ps_major, ps_minor);
			throw runtime_error((const char*)msg);
		}

		// Layer has effects - a temp layer draw surface is needed
		if (pLayout->layers[layerIndex]->HasEffects() && pRuntime->tempLayerTarget == unallocated_texture)
			pRuntime->tempLayerTarget = pRuntime->CreateDisplayTargetTexture();

		if (!pLayout->layers[layerIndex]->effects.empty())
			pLayout->layers[layerIndex]->effects.back().fx->SetupCustomParameters();
	}

	int numTransitions = READINTFROM(cursor);

	for (i = 0; i < numTransitions; i++) {

		int nameLength = READINTFROM(cursor);
		CString transitionName = &(pData[cursor]);
		cursor += nameLength;

		transitionName.MakeLower();

		EffectList thisTransition;

		if (!ParseEffects(thisTransition, pRuntime, 1)) {
			CString msg;
			msg.Format("This application uses Pixel Shader %d.%d for effects, but your graphics hardware only supports Pixel Shader %d.%d effects.  Please upgrade your graphics hardware or try again on another computer.",
					max_ps_major, max_ps_minor, ps_major, ps_minor);
			throw runtime_error((const char*)msg);
		}

		if (thisTransition.empty())
			continue;

		// Frames use the layer target when transitioning
		if (pRuntime->tempLayerTarget == unallocated_texture)
			pRuntime->tempLayerTarget = pRuntime->CreateDisplayTargetTexture();

		pRuntime->Transitions.push_back(CRunTransition());
		CRunTransition& tr = pRuntime->Transitions.back();

		tr.progressHandle = thisTransition.front().fx->pEffect->GetParameterByName(NULL, "prog");
		tr.pRuntime = pRuntime;
		tr.transition = thisTransition.front().fx;
		tr.transition->GetParameters();
		tr.transition->SetupCustomParameters();
		tr.name = transitionName;
	}

	// If any effects are in use, allocate the display target texture - effects may need to read from
	// the display buffer, and can't read from the backbuffer.
	if ((numObjectsHLSL > 0 || numLayersHLSL > 0 || numTransitions > 0) && pRuntime->tempDisplayTarget == unallocated_texture)
		pRuntime->tempDisplayTarget = pRuntime->CreateDisplayTargetTexture();

	if (numTransitions > 0 && pRuntime->tempTransitionTarget == unallocated_texture)
		pRuntime->tempTransitionTarget = pRuntime->CreateDisplayTargetTexture();
#endif
}

#else				// App runtime

void CCapReader::ReadImageData(BYTE* _pData, int _len, map<int, HBITMAP>& imageBmps)
{
	pData = _pData;
	len = _len;
	cursor = 0;

	// Number of images header
	int numImages = READINTFROM(cursor);
	int imageCount = 0;

	// Read
	for ( ; imageCount < numImages; imageCount++) {
		int imageDataSize = READINTFROM(cursor);
		int imageHandle = READINTFROM(cursor);

		BITMAPFILEHEADER* pBmpHeader = (BITMAPFILEHEADER*)&(pData[cursor]);
		BITMAPINFOHEADER* pBmpInfo = (BITMAPINFOHEADER*)(pBmpHeader + 1);

		// Add the texture immediately
		HBITMAP hBmp = CreateBitmap(pBmpInfo->biWidth, pBmpInfo->biHeight, pBmpInfo->biPlanes, pBmpInfo->biBitCount, pBmpInfo + 1);

		// Add the map entry (which texture ID this handle now maps to)
		imageBmps[imageHandle] = hBmp;

		// Advance cursor
		cursor += imageDataSize;

		int width = READINTFROM(cursor);
		int height = READINTFROM(cursor);
		int pitch = READINTFROM(cursor);

		// If collision mask data - not valid - app runtime should use collmasks
		if (pitch * height > 0) 
			throw runtime_error("Invalid image bank format");
	}
}

#endif //APPRUNTIME