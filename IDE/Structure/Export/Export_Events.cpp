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


map<int,int> m_EventSheetConvert;
int CExport::EventSheetIndex(int id)
{
	map<int, int>::iterator i = m_EventSheetConvert.find(id);

	if(i== m_EventSheetConvert.end())
		m_EventSheetConvert[id] = m_EventSheetConvert.size();

	return m_EventSheetConvert[id];
}

void CExport::AddEvent(CEditorEvent* curevent, ExportBlock* root, int current_sheet)
{
	const int event_type_group = 2;
	const int event_type_include = INCLUDE_TYPE;
	const int event_type_event = EVENT_TYPE;
	const int event_type_script = SCRIPT_TYPE;

	const int max_nesting = 100;


	switch (curevent->m_type)
	{
	case event_type_group:
		{
			ExportGroup* e_group = new ExportGroup(curevent->m_sTitle, curevent->m_sActive);
			root->addChildBack(e_group);
		}
		break;
	case event_type_include:
		{
			if (curevent->event_sheet_include == -1) 
				return;

			if(m_depth > max_nesting)
				return;

			EventSheet* evntsheet = application->FindEventSheetFromNumber(curevent->event_sheet_include);
			if(evntsheet)
			{
				for (EventVector::iterator i = evntsheet->m_EventList.begin(); i != evntsheet->m_EventList.end(); i++)
				{
					int scope = m_EventSheetNumber;
					m_EventSheetNumber = EventSheetIndex(evntsheet->identifier);
					AddEvent(*i, root, evntsheet->identifier);
					m_EventSheetNumber = scope;	
				}
			}
		}
		break;
	case event_type_script:
		{
			ExportScript* e_script = new ExportScript(curevent->m_cText);
			root->addChildBack(e_script);
		}
		break;
	case event_type_event:
		{
			ExportEvent* e_event = new ExportEvent(curevent->m_number, m_EventSheetNumber);
			root->addChildBack(e_event);

			ConditionVector::iterator c = curevent->m_Conditions.begin();
			for(; c!= curevent->m_Conditions.end(); c++)
			{
				CEditorCondition* cnd = *c;
				if( cnd->Valid(application) && cnd->m_bEnabled && curevent->m_bEnabled)
				{
					e_event->conditions.push_back(ExportCondition(cnd->oid, cnd->mid, cnd->cndID, cnd->m_Negate, cnd->IsTrigger(application), &cnd->params ));
				}
			}

			ActionVector::iterator a = curevent->m_Actions.begin();
			for(; a!= curevent->m_Actions.end(); a++)
			{
				CEditorAction* act = *a;
				if( act->Valid(application) && act->m_bEnabled && curevent->m_bEnabled)
				{
					e_event->actions.push_back(ExportAction(act->oid, act->mid, act->actID, &act->params ));
				}
			}
		}
		break;

	}

	for (EventVector::iterator i = curevent->m_EventList.begin(); i != curevent->m_EventList.end(); i++)
	{	
		AddEvent(*i, root->getLastChild(), current_sheet);
	}

}

// Generate events
void CExport::GenerateEvents()
{
	// Tell the runtime how many event sheets we have and their names
	eventBlock << application->event_sheets.size();

	for(list<EventSheet*>::iterator s = application->event_sheets.begin(); s!= application->event_sheets.end(); s++)
	{
		eventBlock << (*s)->GetName();
	}

	// Note: I dont think the names of the event sheets would line up with the event sheet indexes, revise code


	int numLayouts = application->layouts.GetSize();

	eventBlock << numLayouts;

	// Export an event sheet for each layout
	for (int layoutIndex = 0; layoutIndex < numLayouts; layoutIndex++)
	{
		// Todo: Loop with POSITION instead of index stuff
		CLayout* pFrame = application->layouts.GetAt(application->layouts.FindIndex(layoutIndex));
		EventSheet* sheet = pFrame->event_sheet;
		m_EventSheetNumber = EventSheetIndex(sheet->identifier);
		m_depth = 0;

		eventBlock << CAP_BEGINEVENTLIST;

		ExportEvent root(0,0); // the root should never export
		for (EventVector::iterator i = sheet->m_EventList.begin(); i != sheet->m_EventList.end(); i++)
		{	
			AddEvent(*i, &root, sheet->identifier);
		}

		ProcessEventsElse(&root);
		ProcessEventsTriggers(&root);
		ExportEvents(&root);

		eventBlock<<CAP_ENDEVENTLIST;
	}
}
void CExport::ExportEvents( ExportBlock* root )
{
	if(root->getFirstChild())
		root->getFirstChild()->Export(this);

}

void CExport::ProcessEventsElse( ExportBlock* root )
{

}

void CExport::ProcessEventsTriggers( ExportBlock* root )
{
	list<ExportBlock*> append;
	if(root->getFirstChild())
		root->getFirstChild()->ProcessEventsTriggers(root, append);	

	list<ExportBlock*>::iterator e = append.begin();
	for( ; e!= append.end(); e++)
	{
		root->addChildBack(*e);
	}
}

int CExport::ConvertMID(int mid, int oid)
{
	CObjType* obj = application->FindObjTypeFromNumber(oid);

	if (mid == -1)
		return mid;
	else if (mid >= 0)
		return obj->GetBehaviorIndexFromUniqueID(mid);
	else
	{
		int index = 1;

		for(list<CEffect>::iterator i = obj->effects.begin(); i!= obj->effects.end(); i++)
		{
			index++;
			if(i->m_effectID * -1  == mid)
			{
				return index * -1; // will start at -1 and go downwards as planned
			}
		}
		return -1;
	}

}

//--------------------------------------

ExportBlock::ExportBlock()
{
	next = NULL;
	prev = NULL;
	parent = NULL;
	first_child = NULL;
	last_child = NULL;
}

ExportBlock::ExportBlock( ExportBlock* other, bool children)
{
	next = NULL;
	prev = NULL;
	parent = NULL;
	first_child = NULL;
	last_child = NULL;

	// clone the blocks children
	if(other->first_child && children)
	{
		ExportBlock* loop = other->first_child;

		ExportBlock* prev_block = NULL;
		while(loop)
		{
			ExportBlock* block = loop->clone(children);
			if(!this->first_child)
				this->first_child = block; // first block is the first child
			this->last_child = block;

			block->parent = this;
			block->prev = prev_block;
			if(prev_block)
				prev_block->next = block;

			prev_block = block;
			loop = loop->next;
		}	
	}
}
void ExportBlock::ProcessEventsTriggers( ExportBlock* root, list<ExportBlock*>& append )
{
	if(first_child)
		first_child->ProcessEventsTriggers(root, append); // first process the child to ensure no sub events have triggers

	bool needsDeleting = ProcessThisEventForTrigger(root, append);

	if(next)
		next->ProcessEventsTriggers(root, append);

	if(needsDeleting)
	{
		detach();
		delete this;
	}
	// this abstract class (and others) wont have triggers
}

void ExportBlock::moveAfter( ExportBlock* block )
{
	detach();

	next = block->next;
	prev = block;
	parent = block->parent;

	block->next = this;


	// check the parents child to see if we've changed it (lol)
	if(parent)
	{
		if(parent->last_child == block)
		{
			parent->last_child = this;
		}
	}
	if(first_child)
		first_child->setParentForAllSiblings(parent);
}

void ExportBlock::moveBefore( ExportBlock* block )
{
	detach();

	next = block;
	prev = block->prev;
	parent = block->parent;

	block->prev = this;

	// check the parents child to see if we've changed it (lol)
	if(parent)
	{
		if(parent->first_child == block)
		{
			parent->first_child = this;
		}
	}
	if(first_child)
		first_child->setParentForAllSiblings(parent);
}

void ExportBlock::addChildBack( ExportBlock* newChild )
{
	newChild->detach();

	newChild->parent = this;
	if(newChild->first_child)
		newChild->first_child->setParentForAllSiblings(this);

	if(first_child && last_child)
	{
		last_child->next = newChild;
		newChild->prev = last_child;
		last_child = newChild;
	}
	else
	{
		first_child = newChild;
		last_child = newChild;
	}
}

void ExportBlock::addChildFront( ExportBlock* newChild )
{
	newChild->detach();

	newChild->parent = this;
	if(newChild->first_child)
		newChild->first_child->setParentForAllSiblings(this);

	if(first_child && last_child)
	{
		first_child->prev = newChild;
		newChild->next = first_child;
		first_child = newChild;
	}
	else
	{
		first_child = newChild;
		last_child = newChild;
	}
}

void ExportBlock::detach()
{
	// Connect our previous and next together ^_^
	if(prev)
	{
		prev->next = next;
	}
	if(next)
	{
		next->prev = prev;
	}
	if(parent)
	{
		if(parent->first_child == this)
		{
			parent->first_child = next;
		}
		if(parent->last_child == this)
		{
			parent->last_child = prev;
		}
	}

	// Now its not attached to anything
	next = NULL;
	prev = NULL;
	parent = NULL;
	if(first_child)
		first_child->setParentForAllSiblings(parent);
}

ExportBlock* ExportBlock::getPrev()
{
	return prev;
}

ExportBlock* ExportBlock::getNext()
{
	return next;
}

ExportBlock* ExportBlock::getParent()
{
	return parent;
}

ExportBlock* ExportBlock::getFirstChild()
{
	return first_child;
}

ExportBlock* ExportBlock::getLastChild()
{
	return last_child;
}



void ExportBlock::setParentForAllSiblings( ExportBlock* newParent )
{
	parent = newParent;

	if(next)
		next->setParentForAllSiblings( newParent );
}

ExportBlock::~ExportBlock()
{
	detach(); 

	ExportBlock* loop = first_child;
	ExportBlock* nextloop = NULL;
	while(loop)
	{
		nextloop = loop->first_child;
		delete loop; // free this child :D

		loop = nextloop; // move on to the next :P
	}
}
//-------------------------------------------------

ExportGroup::ExportGroup( CString Name, BYTE Active )
{
	name = Name;
	active = Active;
}

ExportGroup::ExportGroup( ExportGroup* other, bool children ) : ExportBlock(other, children)
{
	name = other->name;
	active = other->active;
}
void ExportGroup::Export( CExport* exporter )
{
	bin& eventBlock = exporter->eventBlock;

	eventBlock << CAP_BEGINGROUP;

	eventBlock << (BYTE)(active ? 1 : 0);
	eventBlock << name;

	// sub events
	if(getFirstChild())
		getFirstChild()->Export(exporter);

	eventBlock << CAP_ENDGROUP;

	if(getNext())
		getNext()->Export(exporter);
}

ExportBlock* ExportGroup::clone(bool children)
{	
	return new ExportGroup(this, children);
}
//-------------------------------------------------

ExportScript::ExportScript( CString Script )
{
	script = Script;
}

ExportScript::ExportScript( ExportScript* other, bool children ) : ExportBlock(other, children)
{
	script = other->script;
}
void ExportScript::Export( CExport* exporter )
{
	bin& eventBlock = exporter->eventBlock;

	eventBlock<<CAP_BEGINEVENT<<(int)0<<(int)0; // runtime expects to see event number/event sheet after beginevent
	eventBlock<<CAP_BEGINCONDITIONS;
	eventBlock<<CAP_ENDCONDITIONS;
	eventBlock<<CAP_BEGINACTIONS;
	eventBlock<<CAP_BEGINACTION;

	eventBlock<<(int)(-1); // Action OID (SYSTEM)
	eventBlock<<(int)(45); // Action ID (Run Script)
	eventBlock<<(int)(-1); // Action Mov ID
	eventBlock<<(int)(1);  // Param Count

	CEditorParam temp;
	CString script = script;
	script.Replace("\"", "\"\"");
	script = "\"" + script + "\"";
	temp.CreateFromString(script, exporter->application, 2, "");
	exporter->ExportParameter(temp, -1,-1);
	eventBlock<<CAP_ENDACTION;
	eventBlock<<CAP_ENDACTIONS;
	eventBlock<<CAP_ENDEVENT;

	if(getNext())
		getNext()->Export(exporter);
}

ExportBlock* ExportScript::clone(bool children)
{
	return new ExportScript(this, children);
}
//-------------------------------------------------

ExportEvent::ExportEvent( int LineNumber, int SheetNumber )
{
	linenumber = LineNumber;
	sheetnumber = SheetNumber;
}

ExportEvent::ExportEvent( ExportEvent* other, bool children ) : ExportBlock(other, children)
{
	actions = other->actions;
	conditions = other->conditions;
	linenumber = other->linenumber;
	sheetnumber = other->sheetnumber;
}

void ExportEvent::Export( CExport* exporter )
{
	bin& eventBlock = exporter->eventBlock;

	eventBlock << CAP_BEGINEVENT;
	eventBlock << linenumber;
	eventBlock << sheetnumber;
	eventBlock << CAP_BEGINCONDITIONS;
	list<ExportCondition>::iterator c = conditions.begin();
	for(; c!= conditions.end(); c++)
	{
		c->Export(exporter);
	}
	eventBlock << CAP_ENDCONDITIONS;
	eventBlock << CAP_BEGINACTIONS;
	list<ExportAction>::iterator a = actions.begin();
	for(; a!= actions.end(); a++)
	{
		a->Export(exporter);
	}
	eventBlock << CAP_ENDACTIONS;

	// Sub events
	if(getFirstChild())
		getFirstChild()->Export(exporter);

	eventBlock << CAP_ENDEVENT;

	// Next
	if(getNext())
		getNext()->Export(exporter);

}

ExportBlock* ExportEvent::clone(bool children)
{
	return new ExportEvent(this, children);
}

bool ExportEvent::ProcessThisEventForTrigger( ExportBlock* root, list<ExportBlock*>& append )
{
	bool hasTrigger = false;

	list<ExportCondition>::iterator c = conditions.begin();

	for(; c!= conditions.end(); c++)
	{
		if(c->isTrigger())
		{
			hasTrigger = true;

			ExportEvent* newEvent = (ExportEvent*)this->clone(true);
			newEvent->RemoveAllTriggers();

			if(getParent() && getParent() != root)
			{
				ExportBlock* trigger_root = createConditionOnlyEvents(root);
				trigger_root->addChildBack(newEvent);

				ExportBlock* parent = trigger_root;
				while(parent->getParent() != NULL) // find the top parent
					parent = parent->getParent();

				// here we would check if parent is an ExportEvent, and if its not
				// do this:
				ExportEvent* blank_event = new ExportEvent(linenumber, sheetnumber);
				blank_event->addChildBack(parent);

				newEvent = blank_event;
			}

			newEvent->conditions.push_front(*c);

			append.push_back(newEvent);

			// now we need to include all the conditions above

		}
	}

	// There was a trigger in this event so destroy it!
	if(hasTrigger)
	{
		return true;
	}
	else
		return false;
}

ExportBlock* ExportBlock::createConditionOnlyEvents(ExportBlock* root)
{
	ExportBlock* block = clone(false);

	if(parent && parent != root )
	{
		block->parent = parent->createConditionOnlyEvents(root);
		block->parent->addChildBack(block);
	}

	return block;
}

bool ExportBlock::ProcessThisEventForTrigger( ExportBlock* root, list<ExportBlock*>& append )
{
	// Do nothing because it doesn't have a trigger
	return false;
}

//-------------------------------------------------

ExportBlock* ExportEvent::createConditionOnlyEvents(ExportBlock* root)
{
	ExportEvent* block = (ExportEvent*) ExportBlock::createConditionOnlyEvents(root);

	block->RemoveAllTriggers();
	block->actions.clear();

	return block;
}

void ExportEvent::RemoveAllTriggers()
{
	list<ExportCondition>::iterator c = conditions.begin();
	for(; c!= conditions.end(); c++)
	{
		if(c->isTrigger())
		{
			c = conditions.erase(c);
		}
	}
};

//-------------------------------------------------

ExportAction::ExportAction( int oid, int mid, int actid, ParamVector* params )
{
	this->oid = oid;
	this->mid = mid;
	this->actid = actid;
	this->params = params;
}

void ExportAction::Export( CExport* exporter )
{
	bin& eventBlock = exporter->eventBlock;

	eventBlock << CAP_BEGINACTION;

	// Action OID
	int curOid = exporter->ObjTypeIDCnvt(oid);

	eventBlock << curOid;

	// Action ID
	eventBlock << actid;

	int run_mid = exporter->ConvertMID(mid, oid);

	eventBlock << run_mid;

	eventBlock << params->size();

	// Parameters
	ParamVector::iterator p = params->begin();
	for (; p!= params->end(); p++)
	{
		exporter->ExportParameter(**p, curOid, oid);
	}

	eventBlock << CAP_ENDACTION;
}

//-------------------------------------------------

ExportCondition::ExportCondition( int oid, int mid, int cndid, bool negated, bool trigger, ParamVector* params )
{
	this->oid = oid;
	this->mid = mid;
	this->cndid = cndid;
	this->negated = negated;
	this->params = params;
	this->trigger = trigger;
}

void ExportCondition::Export( CExport* exporter )
{
	bin& eventBlock = exporter->eventBlock;

	eventBlock << CAP_BEGINCONDITION;

	// Condition OID
	int curOid = exporter->ObjTypeIDCnvt(oid);

	eventBlock << curOid;

	// Condition ID
	eventBlock << cndid;

	eventBlock << (BYTE) (negated ? 1:0);

	int run_mid = exporter->ConvertMID(mid, oid);

	eventBlock << run_mid;

	eventBlock << params->size();

	// Parameters
	ParamVector::iterator p = params->begin();
	for (; p!= params->end(); p++)
	{
		exporter->ExportParameter(**p, curOid, oid);
	}

	eventBlock << CAP_ENDCONDITION;
}

bool ExportCondition::isTrigger()
{
	return trigger;
}
//-------------------------------------------------