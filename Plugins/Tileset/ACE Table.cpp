// Include StdAfx
#include "StdAfx.h"
#ifdef RUN_ONLY
#include "..\..\Common\CommonAceDef.hpp"
#endif

//////////////////////////////////////////////////////////////////////////////////
// ACE table
//////////////////////////////////////////////////////////////////////////////////

void DefineACES(MicroAceTime* at)
{
	// Define your ACE tables here.
	// Use ADDPARAM before each ADDACT/ADDCND/ADDEXP to add parameters.
	// See the documentation for full details.

	/////////////////////////////
	// Conditions
	//ADDCND(2000, &ExtObject::cMyCondition, "", "%o My condition", 0);

	/////////////////////////////
	// Actions
	//ADDPARAM(PARAM_VALUE, "Example parameter", "Here is an example parameter.");
	//ADDACT(2000, &ExtObject::aMyAction, "", "Example action (%0)", 0);

	/////////////////////////////
	// Expressions
	//ADDEXP(2000, &ExtObject::eMyExpression, "MyExpression", "", RETURN_INTEGER);


#include "..\..\Common\CommonAceTable.hpp"
}


