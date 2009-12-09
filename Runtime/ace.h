// Note these are in a .h due to the inlining

/////////////////////////////
// EVALUATE CONDITION

// Parameters evaluation
inline void condition::EvaluateParams()
{
	ExpressionIterator e = params.begin();
	ExpressionConstIterator params_end = params.end();
	ExpReturn* expPtr = cndParamList;

	for ( ; e != params_end; ++e)
		(*e)->Evaluate(expPtr++);

	// Null the last entry
	expPtr->eType = EXPTYPE_NULL;
}

// Parameters evaluation with selected object
inline void condition::EvaluateParams(class CRunObject* selObj, int solindex)
{
	ExpressionIterator e = params.begin();
	ExpressionConstIterator params_end = params.end();
	ExpReturn* expPtr = cndParamList;

	for ( ; e != params_end; ++e)
		(*e)->Evaluate(expPtr++, selObj, solindex);

	// Null the last entry
	expPtr->eType = EXPTYPE_NULL;
}

// Gateway function for condition routines
inline int condition::Do()
{
	switch (cndType) {

	// System condition
	case CONDITIONTYPE_SYSTEM:
		return DoSystem();

	// Single condition
	case CONDITIONTYPE_SINGLE:
		return DoSingle();

	// Multiple (containered) condition
	case CONDITIONTYPE_MULTIPLE:
		return DoMultiple();

	// Containered family (for family movements)
	case CONDITIONTYPE_MULTIPLEFAMILY:
		return DoMultipleFamily();

	// Will not reach here
	default:

		__assume(0);
	}
}

// Process a system condition: there is no system instance, so a simple true or false
// return is all that is required.
inline int condition::DoSystem()
{
	EvaluateParams();

	D_ENTER(DCS_CONDITION, &(pRuntime->system));

	// No selection; call system routine
	int ret = (pRuntime->system.*routine)(cndParamList);

	D_RUNTIME();

	// ret		isNegated		Result
	//	0			0			0
	//	0			1			1
	//	1			0			1
	//	1			1			0
	// Hence XOR can compute negation.
	return ret ^ isNegated;
}

// Run a static condition
inline int condition::DoStatic(ObjList& o)
{
	// None selected or existing: event fails
	if (o.empty())
		return false;

	CRunObject* pObjToCall = o.front();

	// For family conditions: make the object think it's a family type so it picks from the family SOL
	CRunObjType* pOriginalType = pObjToCall->pType;
	pObjToCall->pType = this->pType;		// Use condition type

	pRuntime->solIndex = 0;

	D_ENTER(DCS_CONDITION, pObjToCall);
	// Routine selects on its own
	int ret = (pObjToCall->*routine)(cndParamList);

	D_RUNTIME();

	// Restore the object's actual type
	pObjToCall->pType = pOriginalType;

	return ret;
}

// Perform a standard object condition: check all instances and modify
// the SOL appropriately
inline int condition::DoSingle()
{
	pRuntime->solIndex = 0;
	int solIndex = 0;

	if (constantParams) 
	{
		EvaluateParams();

		// SOL references: current selectAll bool, current SOL list
		ObjList& rSol = pType->pSol->sol;
		int& rSelectAll = pType->pSol->selectAll;

		// Call the condition SDK routine for each instance

		// First selecting condition: test all instances
		if (rSelectAll) 
		{
			// Reset SOL, keep the capacity
			rSol.resize(0);

			// Instances pointer
			ObjList& instances = pType->instances;

			// Static condition
			if (isStatic) return DoStatic(instances);

			// Cache instances end
			ObjReadIterator i = instances.begin();
			ObjConstIterator instances_end = instances.end();

			// Test every instance
			for ( ; i != instances_end; ++i) 
			{
				pRuntime->solIndex = solIndex;

				D_ENTER(DCS_CONDITION, *i);
				
				//(*i)->pType = pType;

				// Run the condition routine and add to SOL if true
				if(((*i)->*routine)(cndParamList) ^ isNegated)
					
					rSol.push_back(*i);

				D_RUNTIME();

				++solIndex;

			}//for

			rSelectAll = false;	// this object is now being referenced

			// If objects were selected, the condition returns true
			return !rSol.empty();
		}

		// Else another further selecting condition: only test those selected
		// thusfar
		else 
		{
			// Static condition
			if (isStatic) return DoStatic(rSol);

			ObjIterator i = rSol.begin();
			ObjReadIterator o = i;
			ObjConstIterator rSol_end = rSol.end();

			// Test all selected objects
			for ( ; o != rSol_end; ++o)
			{
				pRuntime->solIndex = solIndex;
				D_ENTER(DCS_CONDITION, *i);

				// Run the condition routine
				if(((*o)->*routine)(cndParamList) ^ isNegated) 
				{
					// True:  create a list at the beginning of the vector of matching objects
					*i = *o;
					++i;
				}
				
				D_RUNTIME();
				
				solIndex ++;
			}//for

			// 'i' will still be in the begin position when nothing matched.
			if (i == rSol.begin())
				return false;
			else
			{
				// Resize to new size, 'i' being the furthest new element
				rSol.resize(i - rSol.begin());
				return true;
			}

		}//selectall

	}//params are constant

	////////////////////////////
	else
	{
		// SOL references: current selectAll bool, current SOL list
		ObjList& rSol = pType->pSol->sol;
		int& rSelectAll = pType->pSol->selectAll;

		// Call the condition SDK routine for each instance

		// First selecting condition: test all instances
		if (rSelectAll) 
		{
			// Reset SOL, keep the capacity
			rSol.resize(0);

			// Instances pointer
			ObjList& instances = pType->instances;

			// Static condition
			if (isStatic) {
				if (instances.empty()) return false;

				EvaluateParams(instances.front(), 0);	// evaluate parameters for first instance
				return DoStatic(instances);
			}

			// Cache instances end
			ObjReadIterator i = instances.begin();
			ObjConstIterator instances_end = instances.end();

			// Test every instance
			for ( ; i != instances_end; ++i) 
			{
				// Evaluate parameters for every instance
				//(*i)->pType = pType;
				pRuntime->solIndex = solIndex;

				EvaluateParams(*i, solIndex++);

				D_ENTER(DCS_CONDITION, *i);

				// Run the condition routine and add to SOL
				if(((*i)->*routine)(cndParamList) ^ isNegated)
					
					rSol.push_back(*i);

				D_RUNTIME();

			}//for

			rSelectAll = false;	// this object has now been referenced

			// If objects were selected, the condition returns true
			return !rSol.empty();
		}

		// Else another further selecting condition: only test those selected
		// thusfar
		else 
		{
			// Static condition
			if (isStatic) {
				if (rSol.empty()) return false;

				// Evaluate parameters for first selected
				EvaluateParams(rSol.front(), 0);
				return DoStatic(rSol);
			}

			ObjIterator i = rSol.begin();
			ObjReadIterator o = i;
			ObjConstIterator rSol_end = rSol.end();

			// Test all selected objects
			for ( ; o != rSol_end; ++o)
			{
				pRuntime->solIndex = solIndex;
				// Instance parameter evaluation
				EvaluateParams(*o, solIndex++);

				D_ENTER(DCS_CONDITION, *i);

				// Run the condition routine
				if(((*o)->*routine)(cndParamList) ^ isNegated) 
				{
					// True:  create a list at the beginning of the vector of matching objects
					*i = *o;
					++i;
				}

				D_RUNTIME();
			}//for

			// 'i' will still be in the begin position when nothing matched.
			if (i == rSol.begin())
				return false;
			else 
			{
				// Resize to new size, 'i' being the furthest new element
				rSol.resize(i - rSol.begin());
				return true;
			}

		}//selectall
	}
}

// Process multiple SOLs for container instances: call the condition owner's routines,
// and modify the SOL of all sibling instances.
inline int condition::DoMultiple()
{
	pRuntime->solIndex = 0;
	int solIndex = 0;

	if (constantParams) {

		EvaluateParams();

		// SOL references: current selectAll bool, current SOL list
		ObjList& rSol = pType->pSol->sol;
		int& rSelectAll = pType->pSol->selectAll;

		// Call the condition SDK routine for each instance
		ObjTypeList& rSisters = pType->sisterTypes;
		ObjTypeConstIterator sisters_end = rSisters.end();

		// First selecting condition: test all instances
		if (rSelectAll) {

			// Reset all container object SOLs
			rSol.resize(0);

			ObjTypeReadIterator t = rSisters.begin();

			for ( ; t != sisters_end; ++t)
				(*t)->pSol->sol.resize(0);

			// Loop the owner instances
			ObjList& instances = pType->instances;

			// Static condition
			if (isStatic) return DoStatic(instances);

			ObjConstIterator instances_end = instances.end();

			for (ObjReadIterator i = instances.begin(); i != instances_end; ++i) {

				pRuntime->solIndex = solIndex;
				++solIndex;
				D_ENTER(DCS_CONDITION, *i);
				//(*i)->pType = pType;

				// Run the condition routine
				if(((*i)->*routine)(cndParamList) ^ isNegated) {

					D_RUNTIME();

					// True, add instance to the SOL
					rSol.push_back(*i);

					// Add siblings to their SOLs
					ObjList& rSiblings = (*i)->siblings;
					ObjConstIterator siblings_end = rSiblings.end();

					for (ObjReadIterator s = rSiblings.begin(); s != siblings_end; ++s)

						(*s)->pType->pSol->sol.push_back(*s);

				}//if condition true

				D_RUNTIME();
				
			}//for instances

			// If objects were selected, the condition returns true
			if (!rSol.empty()) {
				rSelectAll = false;

				// Set siblings selectAll to false
				for (t = rSisters.begin(); t != sisters_end; ++t)
					(*t)->pSol->selectAll = false;			

				return true;
			}
			return false;
		}
		// Else another further selecting condition: only test those selected
		// thusfar
		else {

			// Static condition
			if (isStatic) {
				if (rSol.empty()) return false;

				// Evaluate parameters for first selected
				EvaluateParams(rSol.front(), 0);
				return DoStatic(rSol);
			}

			ObjIterator i = rSol.begin();
			ObjReadIterator o = i;
			ObjConstIterator rSol_end = rSol.end();
			int index = 0;

			// Test all selected objects
			for ( ; o != rSol_end; ++o) {

				pRuntime->solIndex = solIndex;
				++solIndex;

				D_ENTER(DCS_CONDITION, *i);

				// Run the condition routine
				if(((*o)->*routine)(cndParamList) ^ isNegated) {

					D_RUNTIME();

					// True:  create a list at the beginning of the vector of matching objects
					*i = *o;

					ObjList& rSiblings = (*i)->siblings;
					ObjConstIterator siblings_end = rSiblings.end();

					// Apply siblings to this position in SOL as well
					for (ObjReadIterator s = rSiblings.begin(); s != siblings_end; ++s)
						(*s)->pType->pSol->sol[index] = *s;

					++i;
					++index;
				}// if true

				D_RUNTIME();

			}//for

			// 'i' will still be in the begin position when nothing matched.
			if (i == rSol.begin())
				return false;
			else {
				// Resize to new size, 'i' being the furthest new element
				rSol.resize(index);

				// For sisters too
				for (ObjTypeReadIterator t = rSisters.begin(); t != sisters_end; ++t)
					(*t)->pSol->sol.resize(index);

				return true;
			}
		}//selectall
	}

	/////////////////////////

	// constantParams is false
	else {

		// SOL references: current selectAll bool, current SOL list
		int solIndex = 0;
		ObjList& rSol = pType->pSol->sol;
		int& rSelectAll = pType->pSol->selectAll;

		// Call the condition SDK routine for each instance
		ObjTypeList& rSisters = pType->sisterTypes;
		ObjTypeConstIterator sisters_end = rSisters.end();

		// First selecting condition: test all instances
		if (rSelectAll) {

			// Reset all container object SOLs
			rSol.resize(0);

			ObjTypeReadIterator t = rSisters.begin();

			for ( ; t != sisters_end; ++t)
				(*t)->pSol->sol.resize(0);

			// Loop the owner instances
			ObjList& instances = pType->instances;

			// Static condition
			if (isStatic) {
				if (instances.empty()) return false;

				EvaluateParams(instances.front(), 0);	// evaluate parameters for first instance
				return DoStatic(instances);
			}

			ObjConstIterator instances_end = instances.end();

			for (ObjReadIterator i = instances.begin(); i != instances_end; ++i) {

				pRuntime->solIndex = solIndex;
				++solIndex;
				//(*i)->pType = pType;
				EvaluateParams(*i, solIndex++);
				

				D_ENTER(DCS_CONDITION, *i);

				// Run the condition routine
				if(((*i)->*routine)(cndParamList) ^ isNegated) {

					D_RUNTIME();

					// True, add instance to the SOL
					rSol.push_back(*i);

					// Add siblings to their SOLs
					ObjList& rSiblings = (*i)->siblings;
					ObjConstIterator siblings_end = rSiblings.end();

					for (ObjReadIterator s = rSiblings.begin(); s != siblings_end; ++s)

						(*s)->pType->pSol->sol.push_back(*s);

				}//if condition true

				D_RUNTIME();
				
			}//for instances

			// If objects were selected, the condition returns true
			if (!rSol.empty()) {
				rSelectAll = false;

				// Set siblings selectAll to false
				for (t = rSisters.begin(); t != sisters_end; ++t)
					(*t)->pSol->selectAll = false;			

				return true;
			}
			return false;
		}
		// Else another further selecting condition: only test those selected
		// thusfar
		else {

			// Static condition
			if (isStatic) {
				if (rSol.empty()) return false;

				// Evaluate parameters for first selected
				EvaluateParams(rSol.front(), 0);
				return DoStatic(rSol);
			}

			ObjIterator i = rSol.begin();
			ObjReadIterator o = i;
			ObjConstIterator rSol_end = rSol.end();
			int index = 0;

			// Test all selected objects
			for ( ; o != rSol_end; ++o) {

				pRuntime->solIndex = solIndex;

				EvaluateParams(*o, solIndex++);

				D_ENTER(DCS_CONDITION, *o);

				// Run the condition routine
				if(((*o)->*routine)(cndParamList) ^ isNegated) {

					D_RUNTIME();

					// True:  create a list at the beginning of the vector of matching objects
					*i = *o;

					ObjList& rSiblings = (*i)->siblings;
					ObjConstIterator siblings_end = rSiblings.end();

					// Apply siblings to this position in SOL as well
					for (ObjReadIterator s = rSiblings.begin(); s != siblings_end; ++s)
						(*s)->pType->pSol->sol[index] = *s;

					++i;
					++index;
				}// if true

				D_RUNTIME();

			}//for

			// 'i' will still be in the begin position when nothing matched.
			if (i == rSol.begin())
				return false;
			else {
				// Resize to new size, 'i' being the furthest new element
				rSol.resize(index);

				// For sisters too
				for (ObjTypeReadIterator t = rSisters.begin(); t != sisters_end; ++t)
					(*t)->pSol->sol.resize(index);

				return true;
			}
		}//selectall

	}

}

inline int condition::DoMultipleFamily()
{
	pRuntime->solIndex = 0;
	int solIndex = 0;
	// Assume constantparams = false

	// SOL references: current selectAll bool, current SOL list
	ObjList& rSol = pType->pSol->sol;
	int& rSelectAll = pType->pSol->selectAll;
	ObjTypeList& familyMovs = pType->familyMovs;
	ObjTypeIterator familyMovs_end = familyMovs.end();

	// Call the condition SDK routine for each instance
	// Propagate across the family container (the familyMovs member)
	if (rSelectAll)
	{
		// Reset the SOL
		rSol.resize(0);

		// Reset family container SOLs
		ObjTypeIterator t = familyMovs.begin();

		for ( ; t != familyMovs_end; t++)
			(*t)->pSol->sol.resize(0);

		ObjList& instances = pType->instances;

		// Static condition
		if (isStatic) {
			if (instances.empty()) return false;

			EvaluateParams(instances.front(), 0);
			return DoStatic(instances);
		}

		ObjIterator i = instances.begin();
		ObjConstIterator instances_end = instances.end();

		// Test every instance
		for ( ; i != instances_end; i++) {

			pRuntime->solIndex = solIndex;

			// Evaluate parameters for every instance
			//(*i)->pType = pType;
			EvaluateParams(*i, solIndex);

			D_ENTER(DCS_CONDITION, *i);

			// Run the condition routine and add to SOL, including family container
			if(((*i)->*routine)(cndParamList) ^ isNegated) {

				D_RUNTIME();
				
				rSol.push_back(*i);

				// Add the equivalent instance to the family movement SOLs at solIndex
				t = familyMovs.begin();

				// Add the corresponding instance in the family to its SOL
				for ( ; t != familyMovs_end; t++)

					(*t)->pSol->sol.push_back((*t)->instances[solIndex]);

			}

			D_RUNTIME();

			solIndex++;

		}//for

		// If SOL not empty - switch off selectAll flags
		if (!rSol.empty()) {
			
			rSelectAll = false;

			for (t = familyMovs.begin(); t != familyMovs_end; t++)
				(*t)->pSol->selectAll = false;

			return true;
		}
		return false;
	}
	// rSelectAll = false
	else {

		ObjIterator rSol_begin = rSol.begin();
		ObjIterator i = rSol_begin;
		ObjReadIterator o = i;
		ObjIterator rSol_end = rSol.end();
		int index = 0;

		// Static condition
		if (isStatic) {
			if (rSol.empty()) return false;

			// Evaluate parameters for first selected
			EvaluateParams(rSol.front(), 0);
			return DoStatic(rSol);
		}

		for ( ; o != rSol_end; o++) {

			pRuntime->solIndex = solIndex;
			// Evaluate parameters for every instance
			EvaluateParams(*o, solIndex);

			D_ENTER(DCS_CONDITION, *o);

			// Run the condition routine and add to SOL, including family container
			if(((*o)->*routine)(cndParamList) ^ isNegated) {

				D_RUNTIME();

				// True:  create a list at the beginning of the vector of matching objects
				*i = *o;
				++i;

				// Repeat for movements
				for (ObjTypeIterator t = familyMovs.begin(); t != familyMovs_end; t++)

					(*t)->pSol->sol[index] = (*t)->pSol->sol[solIndex];

				++index;
			}

			solIndex++;

			D_RUNTIME();
		}

		// 'i' will still be in the begin position when nothing matched.
		if (i == rSol.begin())
			return false;
		else {
			// Resize to new size, 'i' being the furthest new element
			rSol.resize(index);

			// Repeat for movements
			for (ObjTypeIterator t = familyMovs.begin(); t != familyMovs_end; t++)
				(*t)->pSol->sol.resize(index);

			return true;
		}
	}
}

/////////////////////////////
// EXECUTE ACTION

// Evaluate the parameters for this action
inline void action::EvaluateParams(class CRunObject* selObj, int solindex)
{
	// Iterators
	ExpressionIterator e = params.begin();
	ExpressionConstIterator params_end = params.end();
	ExpReturn* expPtr = actParamList;

	// Loop parameters and evaluate them to the actParamList
	for ( ; e != params_end; ++e)
		(*e)->Evaluate(expPtr++, selObj, solindex);

	// Null the last entry
	expPtr->eType = EXPTYPE_NULL;
}

// As above without a selected object
inline void action::EvaluateParams()
{
	ExpressionIterator e = params.begin();
	ExpressionConstIterator params_end = params.end();
	ExpReturn* expPtr = actParamList;

	for ( ; e != params_end; ++e)
		(*e)->Evaluate(expPtr++);

	// Null the last entry
	expPtr->eType = EXPTYPE_NULL;
}

// Execute action
inline void action::Do()
{
	pRuntime->solIndex = 0;
	int solIndex = 0;

	// For each selected object, call the instance routine.
	
	// System object
	if (pType == NULL) {
		EvaluateParams();								// Evaluate

		D_ENTER(DCS_ACTION, &(pRuntime->system));

		((pRuntime->system).*routine)(actParamList);	// Routine

		D_RUNTIME();

		return;											// Quit
	}

	// For parameters requiring one time evaluation
	if (constantParams) {

		// Evaluate parameters
		EvaluateParams();

		// All selected?  Call every instance
		if (pType->pSol->selectAll) {

			// end() can be cached - no deletes occur here
			ObjConstIterator instances_end = pType->instances.end();

			for (ObjReadIterator i = pType->instances.begin(); i != instances_end; ++i) {

				pRuntime->solIndex = solIndex;
				
				//(*i)->pType = pType;
				D_ENTER(DCS_ACTION, *i);

				// Run routine
				((*i)->*routine)(actParamList);

				D_RUNTIME();

				++solIndex;

			}

		}//if

		// Some selected?  Call the SOL entries
		else {
		
			// Cache sol reference
			ObjList& rSol = pType->pSol->sol;
			ObjConstIterator sol_end = rSol.end();

			for (ObjReadIterator i = rSol.begin(); i != sol_end; ++i) {

				pRuntime->solIndex = solIndex;
				
				//(*i)->pType = pType;
				D_ENTER(DCS_ACTION, *i);

				// Run routine
				((*i)->*routine)(actParamList);

				D_RUNTIME();

				++solIndex;

			}

		} //else

	}
	// For parameters requiring per-instance evaluation
	else {

		// All selected?  Call every instance
		if (pType->pSol->selectAll) {

			// end() can be cached - no deletes occur here
			ObjIterator instances_end = pType->instances.end();

			for (ObjReadIterator i = pType->instances.begin(); i != instances_end; ++i) {

				pRuntime->solIndex = solIndex;
				//(*i)->pType = pType;
				// Evaluate parameters
				
				EvaluateParams(*i, solIndex++);

				D_ENTER(DCS_ACTION, *i);

				// Run routine
				((*i)->*routine)(actParamList);

				D_RUNTIME();

			}//for
		}
		// Some selected?  Call the SOL entries
		else {

			// Cache sol reference
			ObjList& rSol = pType->pSol->sol;
			ObjConstIterator sol_end = rSol.end();

			for (ObjReadIterator i = rSol.begin(); i != sol_end; ++i) {

				pRuntime->solIndex = solIndex;
				//(*i)->pType = pType;
				EvaluateParams(*i, solIndex++);

				D_ENTER(DCS_ACTION, *i);

				// Run routine
				((*i)->*routine)(actParamList);

				D_RUNTIME();

			}//for
		} //else

	}//else (constparams)
}
