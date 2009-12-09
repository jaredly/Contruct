
#include "stdafx.h"

// For debugger action counter
#ifdef CONSTRUCT_DEBUGGER
int g_actionIndex;
#endif

/////////////////////////////
// CEVENT
// Single simple event
void CEvent::Run()
{
	// Set the current event
	pRuntime->pCurrentEvent = this;

	ConditionIterator c = cnds.begin();
	ConditionConstIterator cnds_end = cnds.end();
	pCurCnd = &c;

	// Run each condition
	for ( ; c != cnds_end; ++c) {

		// Condition false?  Stop.
		if (!c->Do()) {
			if (pRuntime->suppress_else_event)
				pRuntime->suppress_else_event = false;
			else
				RunElseEvent(this, elseEvent);
			return;
		}
	}

	// Run the actions
	RunActionList(acts);
}

// Run an event with a selected object (for generatevent)
void CEvent::Run(int oid, CRunObject* pCaller)
{
	// Set the current event
	pRuntime->pCurrentEvent = this;

	ConditionIterator c = cnds.begin();
	ConditionConstIterator cnds_end = cnds.end();
	pCurCnd = &c;

	// Select the caller
	if (pCaller != NULL)
		pRuntime->Select(pCaller, oid);

	// Run each condition
	for ( ; c != cnds_end; ++c) {

		// Condition false?  Stop.
		if (!c->Do()) {
			if (pRuntime->suppress_else_event)
				pRuntime->suppress_else_event = false;
			else
				RunElseEvent(this, elseEvent);
			return;
		}
	}

	// By now all conditions are known true and the selected object lists are set up.
	// Execute all actions.
	RunActionList(acts);
}

// Run fast event; skip the first condition
inline void CEvent::RunFast()
{
	// Set the current event
	pRuntime->pCurrentEvent = this;

	// Cache condition iterators
	ConditionIterator c = ++(cnds.begin());					// Skip first condition
	ConditionConstIterator conditions_end = cnds.end();
	pCurCnd = &c;

	// Run each condition; skip first condition
	for ( ; c != conditions_end; ++c) {

		// Condition false?  Stop.
		if (!c->Do()) {
			if (pRuntime->suppress_else_event)
				pRuntime->suppress_else_event = false;
			else
				RunElseEvent(this, elseEvent);
			return;
		}
	}

	// By now all conditions are known true and the selected object lists are set up.
	// Execute all actions.
	RunActionList(acts);
}

// Get event type (normal/parent/group)
EVENTTYPE CEvent::GetType()
{
	return EVENT_NORMAL;
}

// Get event info for calling event components from SDK
void CEvent::GetEventInfo(EventParametersInfo& ei)
{
	if (acts.empty())
		ei.pActs = NULL; // No actions worth pointing to
	else
		ei.pActs = &acts;

	ei.pCnds = &cnds;
	ei.pSubEvents = NULL;
}

// Get event trigger info for sorting to the trigger map
EventTriggerInfo CEvent::GetTriggerInfo()
{
	EventTriggerInfo eti;
	eti.cndID = cnds.front().cndID;
	eti.oid = cnds.front().oid;
	eti.pCnd = &(cnds.front());
	eti.fasttrigger = fasttrigger;
	eti.pvConditions = &cnds;
	return eti;
}

// Check for a matching top condition for sorting to the trigger map
bool CEvent::TriggerMatch(condition* pCnd)
{
	// Match if the condition parameter lists match
	ExpressionIterator here, there;

	ExpressionList *mine, *theirs;
	mine = &(cnds.front().params);
	theirs = &(pCnd->params);

	// Diff num params: no match
	if (mine->size() != theirs->size())
		return false;

	// 0 params each: match
	if (mine->size() == 0 && theirs->size() == 0)
		return true;

	// Test each parameter for equality
	// We now know they both have the same number of parameters
	here = mine->begin();
	there = theirs->begin();

	vector<Token>::iterator myToks;
	vector<Token>::iterator theirToks;

	for ( ; here != mine->end(); here++, there++) {

		// Test tokens for equality
		// Different token counts: differ
		if ((*here)->toks.size() != (*there)->toks.size())
			return false;

		// Loop each token and check for equality
		myToks = (*here)->toks.begin();
		theirToks = (*there)->toks.begin();

		for ( ; myToks != (*here)->toks.end(); myToks++, theirToks++) {
			
			if (myToks->t != theirToks->t || myToks->str != theirToks->str)
				return false;
		}
	}

	// All tests succeeded
	return true;
}

// Get a list of object types whose SOLs are modified by running this event
void CEvent::GetSolModifiers(CRunLayout* frame)
{
	if (elseEvent)
		elseEvent->GetSolModifiers(frame);

	// If a condition or action uses Select() or QuerySelectXXX(), it must specify SOL_MODIFIER, because
	// running that condition or action is going to cause undefined changes to the frame SOL.  This would mess
	// up the SOLs considering the undefined changes are not referenced, so we have to clear up all the frame's SOLs.
	if (manualSolModifier) {
		modifiers = &(frame->typeReferences);
		modifiesSol = true;
		return;
	}

	modifiers = new ObjTypeList;

	ConditionIterator c = cnds.begin();
	ConditionConstIterator cnds_end = cnds.end();

	for ( ; c != cnds_end; c++)
		AddSolModifier(modifiers, c->pType);

	modifiesSol = !(modifiers->empty());
}

// Get parameters pointer for first condition (might be obsolete?)
ExpressionList* CEvent::GetParamsPtr()
{
	return &(cnds.front().params);
}

// Destroy all the conditions and actions
CEvent::~CEvent()
{
	ConditionIterator c;
	ActionIterator a;

	for (c = cnds.begin(); c != cnds.end(); c++)
		c->Destroy();

	for (a = acts.begin(); a != acts.end(); a++)
		a->Destroy();

	if (modifiers != &(pRuntime->objects))
		delete modifiers;
}

/////////////////////////////
// CEVENTPARENT
// Event with sub events or else clauses
void CEventParent::Run()
{
	// Set the current event
	pRuntime->pCurrentEvent = this;

	ConditionIterator c = cnds.begin();
	pCurCnd = &c;

	// Run each condition
	ConditionConstIterator cnds_end = cnds.end();

	for ( ; c != cnds_end; ++c) {

		// Condition false?  Stop.
		if (!c->Do()) {
			if (pRuntime->suppress_else_event)
				pRuntime->suppress_else_event = false;
			else
				RunElseEvent(this, elseEvent);
			return;
		}
	}

	// By now all conditions are known true and the selected object lists are set up.
	// Execute all actions.
	RunActionList(acts);

	// Then execute sub events
	RunSubEventList(subEvents);
}

void CEventParent::Run(int oid, CRunObject* pCaller)
{
	// Set the current event
	pRuntime->pCurrentEvent = this;

	ConditionIterator c = cnds.begin();
	pCurCnd = &c;
	ConditionConstIterator cnds_end = cnds.end();

	if (pCaller != NULL)
		pRuntime->Select(pCaller, oid);

	// Run each condition
	for ( ; c != cnds_end; ++c) {

		// Condition false?  Stop.
		if (!c->Do()) {
			if (pRuntime->suppress_else_event)
				pRuntime->suppress_else_event = false;
			else
				RunElseEvent(this, elseEvent);
			return;
		}
	}

	// By now all conditions are known true and the selected object lists are set up.
	// Execute all actions.
	RunActionList(acts);

	// Then execute sub events
	RunSubEventList(subEvents);
}

void CEventParent::RunFast()
{
	// Set the current event
	pRuntime->pCurrentEvent = this;

	// Cache condition iterators
	ConditionIterator c = ++(cnds.begin());		// Skip first condition
	ConditionConstIterator conditions_end = cnds.end();
	pCurCnd = &c;

	// Run each condition; skip first condition
	for ( ; c != conditions_end; ++c) {

		// Condition false?  Stop.
		if (!c->Do()) {
			if (pRuntime->suppress_else_event)
				pRuntime->suppress_else_event = false;
			else
				RunElseEvent(this, elseEvent);
			return;
		}
	}

	// Run actions
	RunActionList(acts);

	// Then execute sub events
	RunSubEventList(subEvents);
}

EVENTTYPE CEventParent::GetType()
{
	return EVENT_PARENT;
}

void CEventParent::GetEventInfo(EventParametersInfo& ei)
{
	if (acts.empty())
		ei.pActs = NULL;		// No actions worth pointing to
	else
		ei.pActs = &acts;

	ei.pCnds = &cnds;
	ei.pSubEvents = &subEvents;
}

EventTriggerInfo CEventParent::GetTriggerInfo()
{
	EventTriggerInfo eti;
	eti.cndID = cnds.front().cndID;
	eti.oid = cnds.front().oid;
	eti.pCnd = &(cnds.front());
	eti.fasttrigger = fasttrigger;
	eti.pvConditions = &cnds;
	return eti;
}

bool CEventParent::TriggerMatch(condition* pCnd)
{
	// Match if the condition parameter lists match
	ExpressionIterator here, there;

	ExpressionList *mine, *theirs;
	mine = &(cnds.front().params);
	theirs = &(pCnd->params);

	// Diff num params
	if (mine->size() != theirs->size())
		return false;
	if (mine->size() == 0 && theirs->size() == 0)
		return true;

	// Test each parameter for equality

	here = mine->begin();
	there = theirs->begin();

	vector<Token>::iterator myToks;
	vector<Token>::iterator theirToks;

	for ( ; here != mine->end(); here++, there++) {

		// Test each token for equality
		if ((*here)->toks.size() != (*there)->toks.size())
			return false;

		myToks = (*here)->toks.begin();
		theirToks = (*there)->toks.begin();

		for ( ; myToks != (*here)->toks.end(); myToks++, theirToks++) {
			
			if (myToks->t != theirToks->t || myToks->str != theirToks->str)
				return false;
		}
	}

	// All tests succeeded
	return true;
}

// Get a list of object types whose SOLs are modified by running this event
void CEventParent::GetSolModifiers(CRunLayout* frame)
{
	if (elseEvent)
		elseEvent->GetSolModifiers(frame);

	if (manualSolModifier) {
		modifiers = &(frame->typeReferences);
		modifiesSol = true;
	}
	else {
		modifiers = new ObjTypeList;

		ConditionIterator c = cnds.begin();
		ConditionConstIterator cnds_end = cnds.end();

		for ( ; c != cnds_end; c++)
			AddSolModifier(modifiers, c->pType);

		modifiesSol = !(modifiers->empty());
	}

	// Loop subevents and find their SOL modifiers
	EventIterator i = subEvents.begin();
	EventConstIterator subEvents_end = subEvents.end();

	for ( ; i != subEvents_end; i++)
		(*i)->GetSolModifiers(frame);
}

ExpressionList* CEventParent::GetParamsPtr()
{
	return &(cnds.front().params);
}

// Clean up
CEventParent::~CEventParent()
{
	EventIterator i;

	ConditionIterator c;
	ActionIterator a;

	for (c = cnds.begin(); c != cnds.end(); c++)
		c->Destroy();

	for (a = acts.begin(); a != acts.end(); a++)
		a->Destroy();

	for (i = subEvents.begin(); i != subEvents.end(); i++)
		delete (*i);

	if (modifiers != &(pRuntime->objects))
		delete modifiers;
}

/////////////////////////////
// CEVENTGROUP
void CEventGroup::Run()
{
	// Group is activated?
	if (Activated) {

		if (isTopLevel) {

			// Execute events, no SOL deriving
			RunInlineEventList(events);

		}
		else {
	
			// Derive from parent SOLs
			RunSubEventList(events);
		}
	}
}

void CEventGroup::Run(int oid, CRunObject* pCaller)
{
	// Cannot select caller for a group
	CEventGroup::Run();
}

// This should never be called, because a group cannot be triggered, but run
// the group normally anyway
void CEventGroup::RunFast()
{
	Run();
}

EVENTTYPE CEventGroup::GetType()
{
	return EVENT_GROUP;
}

void CEventGroup::GetEventInfo(EventParametersInfo& ei)
{
	ei.pActs = NULL;
	ei.pCnds = NULL;
	ei.pSubEvents = &events;
}

EventTriggerInfo CEventGroup::GetTriggerInfo()
{
	EventTriggerInfo eti;
	eti.cndID = -2;
	eti.oid = -2;
	eti.pCnd = NULL;
	eti.fasttrigger = fasttrigger;
	eti.pvConditions = NULL;
	return eti;
}

// Groups cannot be triggered so cannot be sorted in to the trigger map
bool CEventGroup::TriggerMatch(condition* pCnd)
{
	return false;
}

// Get a list of object types whose SOLs are modified by running this event
void CEventGroup::GetSolModifiers(CRunLayout* frame)
{
	// No sols modified directly by the group
	modifiers = NULL;
	modifiesSol = false;

	// Loop subevents and get their SOL modifiers
	EventIterator i = events.begin();
	EventConstIterator events_end = events.end();

	for ( ; i != events_end; i++)
		(*i)->GetSolModifiers(frame);
}

ExpressionList* CEventGroup::GetParamsPtr()
{
	return NULL;
}

// Clean up
CEventGroup::~CEventGroup()
{
	EventIterator i;

	for (i = events.begin(); i != events.end(); i++)
		delete (*i);
}

// Virtual runtime event execution functions
void CRuntime::RunActions(ACTS_PTR pActions)
{
	RunActionList(*(ActionList*)pActions);
}

void CRuntime::RunSubEvents(SUBEVENTS_PTR pSubEvents)
{
	// Save the current event pointer; this is changed by running subevents
	CEventLine* pThisEvent = pCurrentEvent;

	::RunSubEventList(*(EventList*)pSubEvents);

	// Restore pointer
	pCurrentEvent = pThisEvent;
}

// When a condition deallocates
void condition::Destroy()
{
	// Delete parameters
	for (ExpressionIterator i = params.begin(); i != params.end(); i++)
		delete *i;
}

// When an action deallocates
void action::Destroy()
{
	// Delete parameters
	for (ExpressionIterator i = params.begin(); i != params.end(); i++)
		delete *i;
}

// Helper function for GetSolModifiers: adds an entry if it doesnt already exist
void AddSolModifier(ObjTypeList* otl, CRunObjType* type)
{
	// System object ignored
	if (type == NULL)
		return;

	// If not found, add
	push_back_unique(*otl, type);
	
	// Add sister types
	ObjTypeIterator t = type->sisterTypes.begin();
	ObjTypeIterator end = type->sisterTypes.end();

	for ( ; t != end; t++)
		push_back_unique(*otl, *t);

	// Add family movement types
	t = type->familyMovs.begin();
	end = type->familyMovs.end();

	for ( ; t != end; t++)
		push_back_unique(*otl, *t);

	// Add all the families this object is in (for triggers)
	t = type->teams.begin();
	end = type->teams.end();

	for ( ; t != end; t++) {
		push_back_unique(*otl, *t);

		// If a family movement, add the main family types which can also be affected.
		if (type->IsFamily() && type->movement) {
			ObjTypeIterator f = (*t)->teams.begin();
			ObjTypeIterator teams_end = (*t)->teams.end();

			for ( ; f != teams_end; f++)
				AddSolModifier(otl, *f);
		}

		// Add the family's movements too
		ObjTypeIterator u = (*t)->familyMovs.begin();
		ObjTypeIterator familyMovs_end = (*t)->familyMovs.end();

		for ( ; u != familyMovs_end; u++)
			push_back_unique(*otl, *u);
	}
}