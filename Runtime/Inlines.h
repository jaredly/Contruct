
// Inline functions
//
// To avoid linker errors, some inline functions are defined here.
// Bit messy but hey, gives performance boosts.
inline void CExpression::Evaluate(ExpReturn* eReturn, CRunObject* const selObj, int solindex)
{
	pSelectedObject = selObj;
	SolID = solindex;

	// Evaluate variable expression
	ExpTree->Evaluate(eReturn);
}

// Non-selecting inline
inline void CExpression::Evaluate(ExpReturn* eReturn)
{
	pSelectedObject = NULL;

	// Evaluate variable expression
	ExpTree->Evaluate(eReturn);
}

// Clear given list of SOLs
inline void ClearSOL(ObjTypeList* otl)
{
	ObjTypeReadIterator i = otl->begin();
	ObjTypeConstIterator otl_end = otl->end();

	for ( ; i != otl_end; ++i)
		(*i)->pSol->selectAll = true;
}

// New SOLs for certain types only
inline void NewSOL(ObjTypeList* otl)
{
	// Update SOL pointers in object types
	ObjTypeReadIterator i = otl->begin();
	ObjTypeConstIterator objects_end = otl->end();

	for ( ; i != objects_end; ++i) {

		CRunObjType* pType = *i;

		//int curSolDepth = ++(pType->solDepth);
		pType->curSol++;

		// Push back new SOL if the vector does not extend this far
		if (pType->curSol == pType->sols.end()) {

			pType->sols.push_back(SelObjList());
			pType->curSol = pType->sols.end();
			--(pType->curSol);

		}
		else {

			// New SOL must be clean
			pType->curSol->selectAll = true;

		}


		// Update pSol
		pType->pSol = &(*(pType->curSol));
	}
}

// Inherit SOLs for only a certain set of object types.  Used in subevents.
inline void NewSOLCopy(ObjTypeList* otl)
{
	// Update SOL pointers in object types
	ObjTypeReadIterator i = otl->begin();
	ObjTypeConstIterator objects_end = otl->end();

	for ( ; i != objects_end; ++i) {

		CRunObjType* pType = *i;

		SOLIterator oldSol = pType->curSol++;

		// Push back new SOL if the list does not extend this far
		if (pType->curSol == pType->sols.end()) {

			pType->sols.push_back(SelObjList());
			pType->curSol = pType->sols.end();		// Make curSol point at the back element
			--(pType->curSol);

		}

		// Copy SOL
		if (!(pType->curSol->selectAll = oldSol->selectAll)) {
			// select all flag is off - copy sol content (slow, but no better solution)
			pType->curSol->sol = oldSol->sol;
		}

		// Update pSol
		pType->pSol = &(*(pType->curSol));

	}//for
}

// Pop back a recursion depth and point back to old SOL
inline void RestoreSOL(ObjTypeList* otl)
{
	// Update SOL pointers in object types
	ObjTypeReadIterator i = otl->begin();
	ObjTypeConstIterator objects_end = otl->end();

	for ( ; i != objects_end; ++i) {

		CRunObjType* pType = *i;

		// Decrement SOL iterator and use as new SOL
		pType->pSol = &(*(--(pType->curSol)));
	}
}

// VS doesn't seem to like usage of forward declared template functions.  Implementation here so it's before
// any usage.
template<class T>
void CRuntime::Translate(T& x, T& y)
{
	switch (translationMode) {
	case TRANSLATE_SCROLL:
		// Convert frame to screen coords
		x -= system.pLayout->scrollX;
		y -= system.pLayout->scrollY;
		return;
	case TRANSLATE_SCREEN:
		// Do nothing; use frame coords as screen coords (scrolling wont change pos)
		return;
	case TRANSLATE_ZERO:
		// Set to 0
		x = 0;
		y = 0;
		return;
	default:
		__assume(0);
	}
}

inline void CRuntime::Translate(point& p)
{
	Translate(p.x, p.y);
}

inline void CRuntime::Translate(point3d& p)
{
	Translate(p.x, p.y);
}

inline rect CRuntime::TranslateRect(const rect& r)
{
	cr_float w = r.width();
	cr_float h = r.height();
	point tl = r.topleft();
	Translate(tl);
	return cr::rect_xywh(tl.x, tl.y, w, h);
}

template<class T>
void CRuntime::TranslateBox(T& x1, T& y1, T& x2, T& y2)
{
	// Move a box's position according to Translate()
	T w = x2 - x1;
	T h = y2 - y1;
	Translate(x1, y1);
	x2 = x1 + w;
	y2 = y1 + h;
}

#ifdef CONSTRUCT_DEBUGGER
extern int g_actionIndex;	// 1-based at all times
#endif

// Inline for running a bunch of actions
__forceinline void RunActionList(ActionList& acts)
{
	// Iterators
	ActionIterator a = acts.begin();
	ActionConstIterator acts_end = acts.end();

	// The debugger will set g_actionIndex to 1
	D_START_ACTIONS();

	// Loop actions
	for ( ; a != acts_end; ++a) {

		// Do this action
		a->Do();

		// The debugger increments g_actionIndex
		D_NEXT_ACTION();
	}

	// Debugger sets g_actionIndex to -1 to indicate no longer in action space
	D_END_ACTIONS();
}

// Run sub-events with derivative SOLs
__forceinline void RunSubEventList(EventList& events)
{
	// Then execute sub events
	EventIterator e = events.begin();
	EventConstIterator events_end = events.end();

	for ( ; e != events_end; ++e) {

		// Inherit the SOL of needed types
		CEventLine* pEvent = *e;

		// If this event makes SOL changes
		if (pEvent->modifiesSol) {

			// Duplicate the SOLs this running event will change
			NewSOLCopy(pEvent->modifiers);

			pEvent->Run();

			// Undo the SOL changes that were made
			RestoreSOL(pEvent->modifiers);

		}
		// Event doesn't make SOL changes: safe to run on current SOLs
		else
			pEvent->Run();
	}//for
}

inline void RunElseEvent(CEventLine* parentEvent, CEventLine* elseEvent)
{
	if (elseEvent != NULL) {

		// Undo previous event SOL changes
		//if (parentEvent->modifiesSol)
		//	RestoreSOL(parentEvent->modifiers);

		ObjTypeList combined_modifiers(*(parentEvent->modifiers));
		combined_modifiers.insert(combined_modifiers.end(), elseEvent->modifiers->begin(), elseEvent->modifiers->end());

		if(!combined_modifiers.empty())
			NewSOLCopy(&combined_modifiers);

		// We need to invert the SOL here.  There is no good way to do this with the engine.
		// The else event is only run if the parent event totally failed.  This is only true when
		// it picks zero objects.  The event should also run for objects which weren't picked, but it never gets called.

		// As a temporary hack, enable 'select all' for the modifiers.  Best we can do.
		ObjTypeIterator t = combined_modifiers.begin();
		ObjTypeIterator end = combined_modifiers.end();

		for ( ; t != end; ++t)
			(*t)->pSol->selectAll = true;

		elseEvent->Run();

		// Undo else event SOL changes
		if (!combined_modifiers.empty())
			RestoreSOL(&combined_modifiers);

		// Restore to how it was
		//NewSOLCopy(parentEvent->modifiers);
	}
}

// Run a plain event list with no derivation eg. an event group's content
__forceinline void RunInlineEventList(EventList& events)
{
	// Then execute sub events
	EventIterator e = events.begin();
	EventConstIterator events_end = events.end();

	for ( ; e != events_end; ++e) {

		// Inherit the SOL of needed types
		CEventLine* pEvent = *e;

		pEvent->Run();

		if (pEvent->modifiesSol)
			ClearSOL(pEvent->modifiers);

	}//for
}


#include "ace.h"