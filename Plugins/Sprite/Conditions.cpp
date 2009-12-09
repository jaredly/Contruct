// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Conditions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::cTrigger(LPVAL params)
{
	return true;
}

////////////////////
// STATIC CONDITION - for proper picking when multiple instances involved
long ExtObject::cOverlap(LPVAL params)
{
	CRunObjType* pOtherType = params[0].GetObjectParam(pRuntime);

	if (pOtherType == NULL)
		return FALSE;

	// Get a list of other type's objects
	CRunObject** other_objects;
	int other_count;
	pRuntime->GetTypeInstances(pOtherType, other_objects, other_count);

	if (other_count == 0 || other_objects == NULL)
		return FALSE;

	CRunObject** other_end = other_objects + other_count;

	// Retrieve a list of the selected objects
	CRunObject** selected_objs;
	int count;
	pRuntime->GetTypeSelectedInstances(pType, selected_objs, count);

	if (count == 0 || selected_objs == NULL)
		return FALSE;

	bool isNegated = pRuntime->ConditionIsNegated();

	// Copy the selected instances since SOL will be operated on
	vector<CRunObject*> selected;
	selected.reserve(count);
	CRunObject** vi = selected_objs;
	CRunObject** vend = selected_objs + count;

	for ( ; vi != vend; ++vi)
		selected.push_back(*vi);
	
	// Get event details
	EventParametersInfo2 epi;
	pRuntime->GetCurrentEventPtrs(epi);

	pRuntime->UnselectAll(pType);

	if (!isNegated)
		pRuntime->UnselectAll(pOtherType);

	// Iterate these instances
	vector<CRunObject*>::iterator i = selected.begin();
	vector<CRunObject*>::iterator end = selected.end();

	bool event_was_true = false;

	for ( ; i != end; ++i) {

		// Iterate other instances
		CRunObject** o = other_objects;
		bool collision_found = false;

		for ( ; o != other_end; ++o) {

			bool isOverlapping = pRuntime->QueryCollision(*i, *o);

			// Negated event: make sure we're not overlapping ANY instance before picking & running evemt
			if (isNegated) {
				if (isOverlapping) {
					collision_found = true;
					break;		// We're overlapping something, so "Is not overlapping X" cannot be true; don't run
				}
			}
			// Event not negated: pick objects as we find they're overlapping
			else {

				if (isOverlapping) {

					// Pick this object if not picked yet
					if (!collision_found) {
						collision_found = true;
						pRuntime->NewSOLCopy();
						pRuntime->SelectF(*i, pType);
					}

					// Select other object
					pRuntime->SelectF(*o, pOtherType);
				}
			}
		}

		if (isNegated) {
			// Skip if the object overlaps anything in a negated event
			if (collision_found)
				continue;
			else {
				// Pick this object - it doesn't overlap anything
				pRuntime->NewSOLCopy();
				pRuntime->SelectF(*i, pType);
			}
		}
		else {
			// Skip if the object overlaps nothing in a normal event
			if (!collision_found)
				continue;
		}

		if (epi.pCnds && !pRuntime->RunConditions(epi.pCnds)) {
			pRuntime->RestoreSOL();
			continue;
		}

		event_was_true = true;

		if (epi.pActs)
			pRuntime->RunActions(epi.pActs);

		if (epi.pSubEvents)
			pRuntime->RunSubEvents(epi.pSubEvents);

		pRuntime->RestoreSOL();
	}

	// Condition returns false, so suppress the else event if it ran
	if (event_was_true)
		pRuntime->SuppressElseEvent();

	// Event was fully executed above
	return FALSE;

}

long ExtObject::cOverlapAdvanced(LPVAL params)
{
	CRunObjType* pType = params[0].GetObjectParam(pRuntime);

	if (pType == NULL)
		return FALSE;

	float oldx = info.x;
	float oldy = info.y;
	info.x += params[1].GetFloat();
	info.y += params[2].GetFloat();
	UpdateBoundingBox();
	bool collision = pRuntime->QuerySelectCollision(this, pType);
	info.x = oldx;
	info.y = oldy;
	UpdateBoundingBox();	
	return collision;
}

long ExtObject::cOnCollide(LPVAL params)
{
	
	CRunObjType* pOtherType = params[0].GetObjectParam(pRuntime);

	if (pOtherType == NULL)
		return FALSE;

	void* pCurrentCnd = pRuntime->GetCurrentConditionPtr();

	// No collision record for this condition yet: create one
	if (collisionMemory.find(pCurrentCnd) == collisionMemory.end())
		collisionMemory[pCurrentCnd] = set<CRunObject*>();

	map<void*, set<CRunObject*> >::iterator c = collisionMemory.find(pCurrentCnd);
	set<CRunObject*>& cur_set = c->second;

	// Iterate all object instances and check if a first instance of collision has happened
	CRunObject** objects;
	int count;
	pRuntime->GetTypeSelectedInstances(pOtherType, objects, count);

	if (count == 0 || objects == NULL)
		return FALSE;

	// For each othertype object
	CRunObject** end = objects + count;
	CRunObject** i = objects;

	// Take local copy since SOL modified
	vector<CRunObject*> vec;
	vec.reserve(count);

	for ( ; i != end; ++i)
		vec.push_back(*i);

	bool any_collisions_happened = false;

	vector<CRunObject*>::iterator v = vec.begin();
	vector<CRunObject*>::iterator v_end = vec.end();

	for ( ; v != v_end; ++v) {

		bool isOverlapping = pRuntime->QueryCollision(this, *v);

		// Is overlapping
		if (isOverlapping) {

			// See if this object is in the set yet
			set<CRunObject*>::iterator f = cur_set.find(*v);

			// If not, add it and fire condition
			if (f == cur_set.end()) {
				cur_set.insert(*v);

				if (!pRuntime->IsSelectedF(*v, pOtherType))
					pRuntime->SelectF(*v, pOtherType);

				any_collisions_happened = true;
			}
		}
		// Is not overlapping: remove any record if there is one of this object
		else {

			if (!cur_set.empty()) {
				set<CRunObject*>::iterator f = cur_set.find(*v);

				if (f != cur_set.end())
					cur_set.erase(f);
			}
			
		}

	}

	if (any_collisions_happened)
		return true;
	else
		return pRuntime->SelectRegisteredCollisions(this, pOtherType);
}

long ExtObject::cValueCmp(LPVAL params)
{
	const ExpStore& l = privateVars[params[0].GetVariableIndex(pRuntime, pType)];
	const ExpReturn& r = params[2];

	return DoComparison(params[1].GetInt(), (const ExpBase&)l, (const ExpBase&)r);
}

// Static condition
long ExtObject::cPickLowestVar(LPVAL params)
{
	// Param 0: Private Variable (Variable name)
	int count;
	CRunObject** instances;
	pRuntime->GetTypeSelectedInstances(pType, instances, count);

	int varIndex = params[0].GetVariableIndex(pRuntime, pType);

	// We know these are sprites
	ExtObject** sprites = (ExtObject**)instances;

	bool anyResults = false;
	double bestVal;
	ExtObject* bestObj;

	// Iterate all looking for the lowest numeric value
	ExtObject** i = sprites;
	ExtObject** end = sprites + count;

	for ( ; i != end; i++) {
		
		// This private var is numerical
		const ExpStore& curVal = (*i)->privateVars[varIndex];

		if (curVal.Type() == EXPTYPE_INTEGER || curVal.Type() == EXPTYPE_FLOAT) {

			// No value yet: use as first
			if (!anyResults) {
				anyResults = true;
				bestVal = curVal.GetDouble();
				bestObj = *i;
				continue;				
			}

			// Else check if this value is better than the best
			if (curVal.GetDouble() < bestVal) {
				bestVal = curVal.GetDouble();
				bestObj = *i;
			}
			
		}
	}

	// No object had a numerical value - fail the event.
	if (!anyResults)
		return false;

	// Otherwise, select just the best object we found.
	pRuntime->SelectAll(pType);
	pRuntime->SelectF(bestObj, pType);
	return true;
}

// Static condition
long ExtObject::cPickHighestVar(LPVAL params)
{
	// Param 0: Private Variable (Variable name)
	int count;
	CRunObject** instances;
	pRuntime->GetTypeSelectedInstances(pType, instances, count);

	int varIndex = params[0].GetVariableIndex(pRuntime, pType);

	// We know these are sprites
	ExtObject** sprites = (ExtObject**)instances;

	bool anyResults = false;
	double bestVal;
	ExtObject* bestObj;

	// Iterate all looking for the lowest numeric value
	ExtObject** i = sprites;
	ExtObject** end = sprites + count;

	for ( ; i != end; i++) {
		
		// This private var is numerical
		const ExpStore& curVal = (*i)->privateVars[varIndex];

		if (curVal.Type() == EXPTYPE_INTEGER || curVal.Type() == EXPTYPE_FLOAT) {

			// No value yet: use as first
			if (!anyResults) {
				anyResults = true;
				bestVal = curVal.GetDouble();
				bestObj = *i;
				continue;				
			}

			// Else check if this value is better than the best
			if (curVal.GetDouble() > bestVal) {
				bestVal = curVal.GetDouble();
				bestObj = *i;
			}
			
		}
	}

	// No object had a numerical value - fail the event.
	if (!anyResults)
		return false;

	// Otherwise, select just the best object we found.
	pRuntime->SelectAll(pType);
	pRuntime->SelectF(bestObj, pType);
	return true;
}

// Static condition
long ExtObject::cPickRandom(LPVAL params)
{
	int count;
	CRunObject** objs;
	pRuntime->GetTypeSelectedInstances(pType, objs, count);

	// Select all first, we're only going to pick one object.
	pRuntime->SelectAll(pType);

	// Generate a random number 0-count
	int index = pRuntime->Random() * count;

	// Select this object
	pRuntime->SelectF(objs[index], pType);

	return true;	// We selected something.
}

long ExtObject::cOnAnimationFinish(LPVAL params)
{
	CString l = params[0].GetString();

	if (m_Animation.pAnim == NULL)
		return false;

	CString r;

	if(m_Animation.pAnim->is_angle && m_Animation.pAnim->parent)
		r = m_Animation.pAnim->parent->name;
	else
		r = m_Animation.pAnim->name;

	l.MakeLower();
	r.MakeLower();

	return l == r;
}

long  ExtObject::cOnFrameChange(LPVAL params)
{
	return true;
}

long ExtObject::cAnimationPlaying(LPVAL params)
{
	CString l = params[0].GetString();

	if (m_Animation.pAnim == NULL)
		return false;
	//if (!m_Animation.playing)
	//	return false;

	CString r;

	if(m_Animation.pAnim->is_angle && m_Animation.pAnim->parent)
		r = m_Animation.pAnim->parent->name;

	else 
		r = m_Animation.pAnim->name;

	l.MakeLower();
	r.MakeLower();

	return l == r;
}

long ExtObject::cCompareAnimFrame(LPVAL params)
{
	return DoComparison(params[0].GetInt(), m_Animation.curFrame, params[1].GetInt() - 1);
}

long ExtObject::cPickClosest(LPVAL params)
{
	int count;
	CRunObject** instances;
	pRuntime->GetTypeSelectedInstances(pType, instances, count);

	float find_x = params[0].GetFloat();
	float find_y = params[1].GetFloat();

	// We know these are sprites
	ExtObject** sprites = (ExtObject**)instances;

	bool anyResults = false;
	double bestVal;
	ExtObject* bestObj;

	ExtObject** i = sprites;
	ExtObject** end = sprites + count;

	for ( ; i != end; i++) 
	{
		float dx = (*i)->info.x - find_x;
		float dy = (*i)->info.y - find_y;
		float curVal = dx*dx + dy*dy;

		if (!anyResults)
		{
			anyResults = true;
			bestVal = curVal;
			bestObj = *i;
			continue;				
		}

		if (curVal < bestVal)
		{
			bestVal = curVal;
			bestObj = *i;
		}
	}

	if (!anyResults)
		return false;

	pRuntime->SelectAll(pType);
	pRuntime->SelectF(bestObj, pType);

	return true;
}

long ExtObject::cPickFarthest(LPVAL params)
{
	int count;
	CRunObject** instances;
	pRuntime->GetTypeSelectedInstances(pType, instances, count);

	float find_x = params[0].GetFloat();
	float find_y = params[1].GetFloat();

	// We know these are sprites
	ExtObject** sprites = (ExtObject**)instances;

	bool anyResults = false;
	double bestVal;
	ExtObject* bestObj;

	ExtObject** i = sprites;
	ExtObject** end = sprites + count;

	for ( ; i != end; i++) 
	{
		float dx = (*i)->info.x - find_x;
		float dy = (*i)->info.y - find_y;
		float curVal = dx*dx + dy*dy;

		if (!anyResults)
		{
			anyResults = true;
			bestVal = curVal;
			bestObj = *i;
			continue;				
		}

		if (curVal > bestVal)
		{
			bestVal = curVal;
			bestObj = *i;
		}
	}

	if (!anyResults)
		return false;

	pRuntime->SelectAll(pType);
	pRuntime->SelectF(bestObj, pType);

	return true;
}
