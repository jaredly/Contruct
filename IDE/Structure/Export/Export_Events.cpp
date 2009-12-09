// File includes
#include "StdAfx.h"
#include "Export.h"

void CExport::ExportParameter(CEditorParam& param, int runtimeOid, int edittimeOid)
{
	int oid;

	// Create the preliminary parameter string
	CString param_string = param.CreateParamString(this);

	// Is it an object parameter, if so, normalize it
	if (param.m_type == EDITORPARAM_OBJECT)
	{
		int val = atoi(param_string);
		CString newstring;
		newstring.Format("%d", val);

		oid = 0;

		if(param_string == newstring) {
			oid = ObjTypeIDCnvt(atoi(param_string));
			param_string.Format("%d", oid);
		}

		else // attribute..
		{
			
			// Export actual attribute name, including quotes.  No processing needed.
			/*
			param_string = param_string.Mid(1, param_string.GetLength()-2);

			int index = 0;
			vector<CString>::iterator i = application->m_Traits.begin();

			for( ; i != application->m_Traits.end(); i++) {

				if(param_string == *i)
					oid = m_ExportTypesNumbers.size() + application->m_Families.size()+index;

				index++;
			}*/
		}
	}

	if (param.m_type == EDITORPARAM_TRANSITION)
		if (param_string == "None") 
			param_string = "";

	// Private variable
	if (param.m_type == EDITORPARAM_PRIVATEVARIABLE)
	{
		int varid = atoi(param_string);

		CObjType* pType = application->FindObjTypeFromNumber(param.oid);

		for(vector<PrivateValue>::iterator i = pType->m_PrivateValues.begin(); i!= pType->m_PrivateValues.end(); i++) {
			if(i->identifier == varid) {
				param_string.Format("_varindex(%d, %d)", i - pType->m_PrivateValues.begin(), runtimeOid);
				break;
			}
		}
	}
	// Global variable
	if (param.m_type == EDITORPARAM_GLOBALVARIABLE)
	{
		int varid = atoi(param_string);
		int index = 0;

		for(list<CApplication::GlobalVariable>::iterator i = application->global_variables.begin(); i!= application->global_variables.end(); i++, index++) {
			if(i->identifier == varid) {
				param_string.Format("%d", index);
				break;
			}
		}
	}

	// file resources
	if (param.m_type == EDITORPARAM_BINARYRESOURCE)
	{
		CString temp = param_string;
		temp.Remove('\"');

		int Index = 1;

		for(list<BinaryResource>::iterator i = application->resources.file_resources.begin();
			i != application->resources.file_resources.end(); i++)
		{
			Index++;
			if (i->GetName() == temp) break;
		}

		Index--;

		param_string.Format("%d", Index);
	}

	/*
	CEditorParam TempParam;
	TempParam.oid = edittimeOid;
	TempParam.CreateFromString(param_string, application, 0, "", true);
	*/

	// Export expression tokens
	CTypeChecker tokeniser;
	tokeniser.pApp = application;
	tokeniser.objMap = &(application->object_types);
	tokeniser.oidCaller = edittimeOid;
	tokeniser.pCallerType = application->FindObjTypeFromNumber(edittimeOid);
	int unused;
	tokeniser.ParseUserString(param_string, &unused);
	ProcessEdittimeExpressionInformation(tokeniser.toks, application, edittimeOid);

	// Export number of tokens
	eventBlock << tokeniser.toks.size();

	vector<Token>::iterator tok = tokeniser.toks.begin();

	for ( ; tok != tokeniser.toks.end(); tok++) {

		// Export different data depending on the token.  Try not to export plaintext.
		switch (tok->t) {
			case T_INTEGER:
				eventBlock << (int)tok->t;
				eventBlock << _atoi64(tok->str);
				break;
			case T_FLOAT:
				eventBlock << (int)tok->t;
				eventBlock << atof(tok->str);
				break;
			// String literals, identifiers and variable names need to export their strings.
			case T_STRINGLITERAL:
			case T_IDENTIFIER:
				eventBlock << (int)tok->t;
				eventBlock << tok->str;
				break;
			case T_VARIABLENAME:
				{
				eventBlock << (int)tok->t;

				bool found = false;

				if (tok->oidOwner == -1) {
					int varid = tok->id;
					
					for(list<CApplication::GlobalVariable>::iterator i = application->global_variables.begin(); i!= application->global_variables.end(); i++) {
						if(i->identifier == varid) {
							eventBlock << i->name;
							found = true;
							break;
						}
					}
				}
				else {
					int varid = tok->id;
					CObjType* pType = application->FindObjTypeFromNumber(tok->oidOwner);

					for(vector<PrivateValue>::iterator i = pType->m_PrivateValues.begin(); i!= pType->m_PrivateValues.end(); i++) {
						if(i->identifier == varid) {
							eventBlock << i->name;
							found = true;
							break;
						}
					}
				}

				// Not broken yet: variable not found!
				if (!found)
					throw;

				break;
				}//case
			case T_COLOR:
				eventBlock << (int)T_INTEGER;
				eventBlock << (__int64)tok->id;
				break;
			default:
				eventBlock << (int)tok->t;
				break;

		}
	}
	
	//eventBlock<<CAP_BEGINPARAM;
	//eventBlock.append((const char*)param_string, param_string.GetLength());
	//eventBlock<<'\0';
	//eventBlock<<CAP_ENDPARAM;
}

bool CExport::ReallocateEvent(CEditorEvent* pOldEvent, CEditorEvent* cutoff)
{
	CEditorEvent* newparent = &m_event_memory.back();

	CEditorEvent* oldparent = pOldEvent;

	// We no longer want actions from parent events appearing in the reallocated event, so i commented it out - Davo
	//newparent->m_Actions =		oldparent->m_Actions;
	newparent->m_Conditions =	oldparent->m_Conditions;
	newparent->m_type =			oldparent->m_type;
	newparent->m_sActive =		oldparent->m_sActive;
	newparent->m_sTitle =		oldparent->m_sTitle;
	newparent->event_sheet_include = oldparent->event_sheet_include;
	newparent->m_number		=   oldparent->m_number;

	for(int i = 0; i < oldparent->m_EventList.size(); i++)
	{
		if(cutoff == oldparent->m_EventList[i])
		{
			newparent->m_EventList.push_back(cutoff);
			oldparent->m_EventList.clear();
			return true;
		}
		else
		{
			m_event_memory.push_back(CEditorEvent(false));
			newparent->m_EventList.push_back(&m_event_memory.back());

			if(ReallocateEvent(oldparent->m_EventList[i], cutoff))
				return true;
			else
			{
				// We dont want to include this event list because its not part of the chain of events
				// relating to the trigger
				// We dont need to worry about freeing memory because its in a memory vector which is freed
				// later :)
				newparent->m_EventList.pop_back();
				
			}
		}
	}

	return false;
}

	map<int,int> m_EventSheetConvert;

int CExport::EventSheetIndex(int id)
{
	map<int, int>::iterator i = m_EventSheetConvert.find(id);

	if(i== m_EventSheetConvert.end())
		m_EventSheetConvert[id] = m_EventSheetConvert.size();

	return m_EventSheetConvert[id];
}

void CExport::ReallocateTriggerlessEvent(CEditorEvent* cutoff)
{
	if(cutoff == m_triggerless)
		return;

	m_event_memory.push_back(CEditorEvent(false));
	CEditorEvent* new_first_triggerless = &m_event_memory.back();

	ReallocateEvent(m_triggerless, cutoff);

	m_triggerless = new_first_triggerless;
}


void CExport::AddEvent(CEditorEvent* curevent, int current_sheet)
{
	m_depth++;
			
	if(m_TriggerDepth >= 0)
	{
		CEditorEvent* triggerless = m_curTriggerlessEvent;

		triggerless->m_type = curevent->m_type;
		if(triggerless->m_type == INCLUDE_TYPE)
			triggerless->m_type = EVENT_TYPE;
		triggerless->m_Actions = curevent->m_Actions;
		triggerless->m_sActive = curevent->m_sActive;
		triggerless->m_sTitle = curevent->m_sTitle;
		triggerless->event_sheet_include = curevent->event_sheet_include;
		triggerless->m_number = curevent->m_number;
		
		//python
		triggerless->m_cText = curevent->m_cText;
	}
	
	// Group
	if (curevent->m_type == 2)
	{
		if(m_TriggerDepth <= 0)
		{
			eventBlock << CAP_BEGINGROUP;
			// m_sActive - is this on when the group is enabled, or disabled?!
			eventBlock << (BYTE)(curevent->m_sActive);
			eventBlock << curevent->m_sTitle;
		}

		for (int i = 0; i < curevent->m_EventList.size(); i++)
		{
			CEditorEvent* temp = m_curTriggerlessEvent;
			if(m_TriggerDepth >= 0)
			{
				m_event_memory.push_back(CEditorEvent(false));
				m_curTriggerlessEvent->m_EventList.push_back(&m_event_memory.back());
				m_curTriggerlessEvent = m_curTriggerlessEvent->m_EventList.back();
			}
			
				
			AddEvent(curevent->m_EventList.at(i), current_sheet);
			m_curTriggerlessEvent = temp;
		}

		if(m_TriggerDepth <= 0)
			eventBlock << CAP_ENDGROUP;
	}

	else if (curevent->m_type == INCLUDE_TYPE)
	{
		if (curevent->event_sheet_include == -1) return;
		if (curevent->event_sheet_include == current_sheet) return;

		if(m_TriggerDepth <= 0)
		{
		//	eventBlock<<CAP_BEGINEVENT<<0<<0;	// runtime expects to see event number/sheet after beginevent
		//	eventBlock<<CAP_BEGINCONDITIONS;
		//	eventBlock<<CAP_ENDCONDITIONS;
		//	eventBlock<<CAP_BEGINACTIONS;
		//	eventBlock<<CAP_ENDACTIONS;
		}
		if(m_depth < 100)
		{
			// we need to obtain the event list in the include
			EventSheet* evntsheet = this->application->FindEventSheetFromNumber(curevent->event_sheet_include);
			if(evntsheet)
			{
				for (int i = 0; i < evntsheet->m_EventList.size(); i++)
				{
					CEditorEvent* temp = m_curTriggerlessEvent;
					if(m_TriggerDepth >= 0)
					{
						m_event_memory.push_back(CEditorEvent(false));
						m_curTriggerlessEvent->m_EventList.push_back(&m_event_memory.back());
						m_curTriggerlessEvent = m_curTriggerlessEvent->m_EventList.back();
					}
					
					int scope = m_EventSheetNumber;
					m_EventSheetNumber = EventSheetIndex(evntsheet->identifier);

					AddEvent(evntsheet->m_EventList.at(i), evntsheet->identifier);
					m_curTriggerlessEvent = temp;	
				}
			}
		}
	}

	else if (curevent->m_type == SCRIPT_TYPE)
	{
		if(m_TriggerDepth <= 0)
		{
			eventBlock<<CAP_BEGINEVENT<<0<<0; // runtime expects to see event number/event sheet after beginevent
			eventBlock<<CAP_BEGINCONDITIONS;
			eventBlock<<CAP_ENDCONDITIONS;
			eventBlock<<CAP_BEGINACTIONS;
			eventBlock<<CAP_BEGINACTION;
			
			eventBlock<<(int)(-1); // Action OID (SYSTEM)
			eventBlock<<(int)(45); // Action ID (Run Script)
			eventBlock<<(int)(-1); // Action Mov ID
			eventBlock<<(int)(1);  // Param Count

			CEditorParam temp;
			CString script = curevent->m_cText;
			script.Replace("\"", "\"\"");
			script = "\"" + script + "\"";
			temp.CreateFromString(script, application, 2, "");
			ExportParameter(temp, -1,-1);
			eventBlock<<CAP_ENDACTION;
			eventBlock<<CAP_ENDACTIONS;
			eventBlock<<CAP_ENDEVENT;
		}
	}

	else if (curevent->m_type == EVENT_TYPE)
	{
		// something special we have to do : we need to loop all the conditions in this event and create tempory events for them, and then
		// export those. 
		bool hasATrigger = false;

		if(m_TriggerDepth >= 0)
		{
			for (int y = 0; y < curevent->m_Conditions.size(); y++)
			{
				CEditorCondition* curcnd = curevent->m_Conditions.at(y);
				if(curcnd->Valid(application) && curcnd->m_bEnabled && curevent->m_bEnabled)
				{
					if(application->FindObjTypeFromNumber(curcnd->oid))
					{
						if(curcnd->IsTrigger(application))
						{
							hasATrigger = true;

							m_export_later.push_back(CEditorEvent(false));
								
							CEditorEvent* item = &m_export_later.back();

							//item->m_Actions = triggerless.m_Actions;
							CEditorEvent* temp = m_triggerless;
							ReallocateTriggerlessEvent(m_curTriggerlessEvent);
							item->m_EventList.push_back(m_triggerless); // we give it the topmost triggerless event
							item->m_type = 0;	// normal event type
							item->m_Conditions.push_back(curcnd);	// the only condition we are going to have is the trigger. The sub event will contain the entire triggerless event
							m_triggerless = temp;
						}
						else
						{
						
							m_curTriggerlessEvent->m_Conditions.push_back(curcnd);	// copy pointer to condition
						}
					}
				}

			}
		}

		if(hasATrigger)
			m_TriggerDepth ++;

		if(m_TriggerDepth <= 0)
		{

			eventBlock << CAP_BEGINEVENT;

			eventBlock << curevent->m_number;
			eventBlock << m_EventSheetNumber;

			eventBlock<<CAP_BEGINCONDITIONS;

			// Conditions
			for (int y = 0; y < curevent->m_Conditions.size(); y++)
			{
				// Get condition
				CEditorCondition* curcondition = curevent->m_Conditions.at(y);
				if(!curcondition->ValidToDisplay(application)
					|| !curcondition->m_bEnabled 
					|| !curevent->m_bEnabled )
					continue;
				CObjType* obj = application->FindObjTypeFromNumber(curcondition->oid);

				eventBlock<<CAP_BEGINCONDITION;

				// Get condition OID
				int curOid = ObjTypeIDCnvt((*curcondition).oid);
				eventBlock<<curOid;

				// Cnd ID
				eventBlock<<(*curcondition).cndID;

				// Is negated
				if ((*curcondition).m_Negate)
					eventBlock<<(BYTE)1;
				else
					eventBlock<<(BYTE)0;
				bool bFound = false;

				if(curcondition->mid >= 0)
					eventBlock << obj->GetBehaviorIndexFromUniqueID(curcondition->mid);
				else
					eventBlock << curcondition->mid;

				eventBlock << curcondition->params.size();

				// Parameters
				for (int t = 0; t < curcondition->params.size(); t++)
				{
					ExportParameter(*(curcondition->params[t]), curOid, curcondition->oid);
				}

				eventBlock<<CAP_ENDCONDITION;
			}

			eventBlock<<CAP_ENDCONDITIONS;
			eventBlock<<CAP_BEGINACTIONS;

			// Actions
			for (int z = 0; z < curevent->m_Actions.size(); z++)
			{
			

				// Get action

				CEditorAction* curaction = curevent->m_Actions.at(z);
				if(!curaction->ValidToDisplay(application)
					|| !curaction->m_bEnabled
					|| !curevent->m_bEnabled)
					continue;

				CObjType* obj = application->FindObjTypeFromNumber(curaction->oid);

				eventBlock<<CAP_BEGINACTION;

				// Action OID
				int curOid = ObjTypeIDCnvt(curaction->oid);
		
				eventBlock<<curOid;

				// Action ID
				eventBlock<<curaction->actID;

				bool bFound = false;

				if(curaction->mid == -1)
					eventBlock << curaction->mid;
				else if(curaction->mid >= 0)
					eventBlock << obj->GetBehaviorIndexFromUniqueID(curaction->mid);
				else
				{
					int mid = -1;
					int index = 1;
					
					for(list<CEffect>::iterator i = obj->effects.begin(); i!= obj->effects.end(); i++)
					{
						index++;
						if(i->m_effectID * -1  == curaction->mid)
						{
							mid = index * -1; // will start at -1 and go downwards as planned
						}
					}
					eventBlock << mid;
				}

				eventBlock << curaction->params.size();

				// Parameters
				for (int t = 0; t < curaction->params.size(); t++)
				{
					ExportParameter(*(curaction->params[t]), curOid, curaction->oid);
				}

				eventBlock<<CAP_ENDACTION;
			}
			
			eventBlock<<CAP_ENDACTIONS;

		}

		for (int i = 0; i < curevent->m_EventList.size(); i++)
		{
			//bool previous_trigger;
			CEditorEvent* temp = m_curTriggerlessEvent;
			if(m_TriggerDepth >= 0)
			{
				m_event_memory.push_back(CEditorEvent(false));
				m_curTriggerlessEvent->m_EventList.push_back(&m_event_memory.back());
				m_curTriggerlessEvent = m_curTriggerlessEvent->m_EventList.back();
			}
			
			if (curevent->m_EventList.at(i)->m_bEnabled)
				AddEvent(curevent->m_EventList.at(i), current_sheet);

			m_curTriggerlessEvent = temp;

		}
		if(m_TriggerDepth <= 0)
			eventBlock<<CAP_ENDEVENT;
		
	// end iterator loop of all trigger events
		if(hasATrigger)
			m_TriggerDepth--;
	}
	m_depth--;
}

// Generate events
void CExport::GenerateEvents()
{
	eventBlock << application->event_sheets.size();

	for(list<EventSheet*>::iterator s = application->event_sheets.begin(); s!= application->event_sheets.end(); s++)
	{
		eventBlock << (*s)->GetName();
	}
	

	int numLayouts = application->layouts.GetSize();
	
	eventBlock << numLayouts;

	//	m_depth = 0;
	//	m_TriggerDepth = 0;

	for (int layoutIndex = 0; layoutIndex < numLayouts; layoutIndex++)
	{
		// UNTESTED BEGIN
		m_depth = 0;
		m_TriggerDepth = 0;
		m_triggerless = 0;
		m_curTriggerlessEvent = 0;
		m_event_memory.clear();
		m_export_later.clear();
		// UNTESTED END

		//TODO: use proper POSITION iteration rather than this finding from index
		// - eh? it exports them in a specific order so they match with the level. Later we should
		// make it so it simply exports the event lists vector and the levels have the number of the 
		// event list they use...since we have event lists as sheets now... but thats a later thing :P

		CLayout* pFrame = application->layouts.GetAt(application->layouts.FindIndex(layoutIndex));

		eventBlock<<CAP_BEGINEVENTLIST;

		EventSheet* sheet = pFrame->event_sheet;

		m_EventSheetNumber = EventSheetIndex(sheet->identifier);

		for (int i = 0; i < sheet->m_EventList.size(); i++)
		{	
			m_event_memory.push_back(CEditorEvent(false));
			m_triggerless = &m_event_memory.back();
			m_curTriggerlessEvent = m_triggerless;		
			
			AddEvent(sheet->m_EventList.at(i), sheet->identifier);
		}

		m_TriggerDepth = -1;
		// Trigger time
		for(list<CEditorEvent>::iterator i = m_export_later.begin(); i != m_export_later.end(); i++)
		{
			// The topmost event (that is the ones listed in m_export_later) are always (28/8/07) just an empty
			// event with one condition (the trigger). The runtime apparently is unoptimised for this, so we need to 
			// put the trigger into the child.
			CEditorEvent& triggerevent = *i;
			if(triggerevent.m_Actions.size() == 0 && triggerevent.m_Conditions.size() == 1 && triggerevent.m_EventList.size() == 1)
			{
				//The above check is so if in the future the export later events are used differently this code wont skrew things up
				CEditorEvent* cur = triggerevent.m_EventList[0];
				if(cur->m_type == EVENT_TYPE)
				{
					cur->m_Conditions.insert(cur->m_Conditions.begin(), triggerevent.m_Conditions[0]);
					
					// The runtime filters out where the trigger is...
					AddEvent(cur, sheet->identifier);
					// We pop back our newly added condition because if we had:
					// button 1 clicked + button 2 clicked
					//    actions...
					// Then during the export process both the 2 separae events get the same pointer to the subevents and actions
					cur->m_Conditions.erase(cur->m_Conditions.begin());
				}
				else
					AddEvent(&(*i), sheet->identifier);
			}
			else
				AddEvent(&(*i), sheet->identifier);
		}

		// we list all events that we wish to export later (triggers at the moment) here

		eventBlock<<CAP_ENDEVENTLIST;
	}
}
