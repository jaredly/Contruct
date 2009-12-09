
#include "stdafx.h"
#include "ExpTree.h"

ExpPart::ExpPart()
{
	bracketdepth = 0;
}

static const int g_zero = 0;

ExpPart*& ExpPart::GetPLeftOperand()
{
	// Return a reference to a null pointer
	return (ExpPart*&)(g_zero);
}

ExpPart*& ExpPart::GetPRightOperand()
{
	// Return a reference to a null pointer
	return (ExpPart*&)(g_zero);
}

///////////
// Collapse a constant tree
// If an ExpPart* returns true for Constant(), evaluate it, and reallocate a new
// literal to represent the constant part of that expression.
void CollapseConstantTree(ExpPart*& p)
{
	// Store existing details
	CExpression* pExpression = p->pCExp;
	int tnum = p->tokennum;

	// Create an ExpReturn, evaluate in to it, then delete the old ExpPart.
	ExpReturn e;
	p->Evaluate(&e);

	// For deleting later (the string might be used)
	ExpPart* oldPart = p;

	// Reallocate a literal to represent that part of the tree.
	switch(e.Type()) {
	case EXPTYPE_INTEGER:
		p = new ExpInteger;
		p->t = T_INTEGER;
		static_cast<ExpInteger*>(p)->value = e.GetInt64();
		break;
	case EXPTYPE_FLOAT:
		p = new ExpFloat;
		p->t = T_FLOAT;
		static_cast<ExpFloat*>(p)->value = e.GetDouble();
		break;
	case EXPTYPE_STRING:
		p = new ExpString;
		p->t = T_STRINGLITERAL;
		static_cast<ExpString*>(p)->value = *(e.GetStringPtr());
		break;
	case EXPTYPE_ARRAY:
		// Arrays cannot be folded; they collapse their elements but nothing can be
		// done with a const array
		p = oldPart;
		return;
	case EXPTYPE_VARIABLENAME:
		p = new ExpVariableName;
		static_cast<ExpVariableName*>(p)->name = *(e.GetStringPtr());
		break;
	default:
		throw runtime_error("Unexpected type in a constant expression");
	}

	// Set the pCExp and tokennum back.
	p->pCExp = pExpression;
	p->tokennum = tnum;

	// Delete the old tree part now
	delete oldPart;
}

// Do a standard binary constant collapse for a binary operator.
bool DoBinaryCollapse(ExpPart*& l, ExpPart*& r)
{
	bool leftConst = l->Constant();
	bool rightConst = r->Constant();

	// If the L part is constant, collapse it.
	if (leftConst) CollapseConstantTree(l);

	// If the R part is constant, collapse it.
	if (rightConst) CollapseConstantTree(r);

	// This part is constant if the left and right parts are both constant.
	return (leftConst && rightConst);
}

#define DEFINE_STD_GETOPERANDS(type) ExpPart*& type::GetPLeftOperand() {	return l; } \
									 ExpPart*& type::GetPRightOperand() { return r; }

#define DEFINE_STD_BINARY(type)		type::type() {	l = NULL;	r = NULL; } \
						type::~type(){	if (l != NULL)	delete l;	if (r != NULL)	delete r; }

/////////////
// ExpInteger
void ExpInteger::Evaluate(ExpReturn* er)
{
	er->eType = EXPTYPE_INTEGER;
	er->eData.iVal = value;
}

ExpInteger::ExpInteger()
{
	value = 0;
}

ExpInteger::~ExpInteger()
{
}

bool ExpInteger::Constant()
{
	return true;
}

/////////////
// ExpFloat
void ExpFloat::Evaluate(ExpReturn* er)
{
	er->eType = EXPTYPE_FLOAT;
	er->eData.fVal = value;
}

ExpFloat::ExpFloat()
{
	value = 0;
}

ExpFloat::~ExpFloat()
{
}

bool ExpFloat::Constant()
{
	return true;
}

/////////////
// ExpString
void ExpString::Evaluate(ExpReturn* er)
{
	er->eType = EXPTYPE_STRING;
}

ExpString::ExpString()
{
	value = "";
}

ExpString::~ExpString()
{
}

bool ExpString::Constant()
{
	return true;
}

/////////////
// ExpArray
void ExpArray::Evaluate(ExpReturn* er)
{
	// Constant?  Just return the saved array
	if (isConstant) {
		er->eType = EXPTYPE_ARRAY;
		er->eData.arr.size = arr.size();

		// Point to stored content
		er->eData.arr.pArray = (ExpStore*)&(arr[0]);
	}
	// Variable content: evaluate to the array and return
	else {
		ExpParamIterator i = expElements.begin();
		ExpParamConstIterator elements_end = expElements.end();

		vector<ExpReturn>::iterator arrIndex = arr.begin();

		for ( ; i != elements_end; ++i) {
			(*i)->Evaluate(&(*arrIndex));
			arrIndex++;
		}

		er->eType = EXPTYPE_ARRAY;
		er->eData.arr.size = arr.size();
		er->eData.arr.pArray = (ExpStore*)&(arr[0]);
	}
}

ExpArray::ExpArray()
{
	noKill = false;
}

ExpArray::~ExpArray()
{
	// Delete array items
	if (!noKill) {
		ExpParamIterator i = expElements.begin();
		ExpParamConstIterator elements_end = expElements.end();

		for ( ; i != elements_end; i++)
			delete (*i);
	}
}

bool ExpArray::Constant()
{
	// Const collapse all items
	ExpParamIterator i = expElements.begin();
	ExpParamConstIterator elements_end = expElements.end();

	isConstant = true;

	for ( ; i != elements_end; i++) {
		if ((*i)->Constant())
			CollapseConstantTree(*i);
		else
			isConstant = false;
	}

	// If it is constant, get me to evaluate
	if (isConstant) {
		ExpReturn unused;

		// Evaluate checks for the isConstant flag so temporarily turn it off
		isConstant = false;
		this->Evaluate(&unused);
		isConstant = true;
	}

	return isConstant!=0;
}

/////////////
// ExpVariableName
ExpVariableName::ExpVariableName()
{
	//index = 0;
}

ExpVariableName::~ExpVariableName()
{
}

bool ExpVariableName::Constant()
{
	return true;
}

void ExpVariableName::Evaluate(ExpReturn *er)
{
	er->eType = EXPTYPE_VARIABLENAME;
	er->eData.str = &name;
}

/////////////
// ExpBinary
void ExpBinary::Evaluate(ExpReturn* er)
{
	er->eType = EXPTYPE_BINARY;
}

ExpBinary::ExpBinary()
{
	data.data = NULL;
	data.size = 0;
}

ExpBinary::~ExpBinary()
{
	if (data.data != NULL)
		delete [] data.data;
}

bool ExpBinary::Constant()
{
	return true;
}

/////////////
// ExpIdent
void ExpIdent::Evaluate(ExpReturn* er)
{
	// Set the expression return location
	//pRuntime->curExpStackDepth++;
	//pRuntime->ExpReturnStack[pRuntime->curExpStackDepth] = er;

	ExpReturn* expPtr = expParamList;

	// System call?
	if (this->pType == NULL) {
		if (!pCExp->CheckSysParamCount(this->ident, parameters.size())) {
			CString msg;
			msg.Format("'%s' does not take %d parameters.", ident, parameters.size());
			throw runtime_error((const char*)msg);
		}
	}
	else {
		if (!pCExp->CheckParamCount(this->ident, "<default>", parameters.size())) {
			CString msg;
			msg.Format("Expression '%s' does not take %d parameters.", ident, parameters.size());
			throw runtime_error((const char*)msg);
		}
	}

	// Parameters exist?  Evaluate them
	if (parameters.size() > 0) {
		
		ExpParamIterator i = parameters.begin();
		ExpParamConstIterator parameters_end = parameters.end();

		int index = 0;

		// Evaluate every parameter
		for ( ; i != parameters_end; i++, index++) {

			// Send to address of corresponding expParamList array entry
			(*i)->Evaluate(expPtr);

			if (pType == NULL) {
				// Special check for variable name param
				if (expPtr->Type() == EXPTYPE_VARIABLENAME) {
					if (!pCExp->PrivateVariableExists("<system>", ident, index, parameters.size(), *(expPtr->GetStringPtr()))) {
						CString msg;
						msg.Format("'%s' is not a global variable", *(expPtr->GetStringPtr()));
						throw runtime_error((const char*)msg);
					}
				}
				else if (!pCExp->SysParamValid(ident, index, parameters.size(), expPtr->Type())) {
					CString msg;
					msg.Format("Parameter %d of '%s' does not take '%s'", index+1, ident, GetTypeName(expPtr->Type()));
					throw runtime_error((const char*)msg);
				}
			}
			else {
				// Special check for variable name param
				if (expPtr->Type() == EXPTYPE_VARIABLENAME) {
					if (!pCExp->PrivateVariableExists(ident, "<default>", index, parameters.size(), *(expPtr->GetStringPtr()))) {
						CString msg;
						msg.Format("'%s' is not a private variable in '%s' (parameter %d)", *(expPtr->GetStringPtr()), ident, index + 1);
						throw runtime_error((const char*)msg);
					}
				}
				else if (!pCExp->ParamValid(ident, "<default>", index, parameters.size(), expPtr->Type())) {
					CString msg;
					msg.Format("Parameter %d of expression '%s' does not take '%s'", index+1, ident, GetTypeName(expPtr->Type()));
					throw runtime_error((const char*)msg);
				}	
			}

			expPtr++;
		}
	}

	// Now send up the return type of this expression...
	if (pType == NULL) {
		er->eType = pCExp->SysGetType(ident);
	}
	else {
		er->eType = pCExp->GetExpressionType(ident, "<default>");
	}

	// Roll back the stack depth
	//pRuntime->curExpStackDepth--;
}

ExpIdent::ExpIdent()
{
	oid = -1;
	expID = -1;
}

ExpIdent::~ExpIdent()
{
	// Delete parameters
	vector<ExpPart*>::iterator i;

	for (i = parameters.begin(); i != parameters.end(); i++)
		delete (*i);

	parameters.clear();
}

bool ExpIdent::Constant()
{
	// Const collapse all parameters
	vector<ExpPart*>::iterator i;

	for (i = parameters.begin(); i != parameters.end(); i++) {

		// This parameter is constant
		if ((*i)->Constant())
			CollapseConstantTree((*i));	// Collapse it
	}

	// Return false; no way of telling if the plugin will return a constant value
	// or not.
	return false;
}

#define ELSENOTVALID(op) case EXPTYPE_NULL: \
							return;			\
						 default:			\
							throw runtime_error((const char*)(CString("'" #op "' does not work on '") + GetTypeName(er->Type()) + "' and '" + GetTypeName(param.Type()) + "'"))

#define DEFAULTNOTVALID(op)	default:			\
							throw runtime_error((const char*)(CString("'" #op "' does not work on '") + GetTypeName(er->Type()) + "' and '" + GetTypeName(param.Type()) + "'"))

/////////////
// ExpAdd
DEFINE_STD_GETOPERANDS(ExpAdd);
DEFINE_STD_BINARY(ExpAdd);

void ExpAdd::Evaluate(ExpReturn* er)
{
	ExpReturn param;

	l->Evaluate(er);
	r->Evaluate(&param);

	switch (er->eType) {

	case EXPTYPE_INTEGER:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			return;
		case EXPTYPE_FLOAT:
			// Adding float to integer: convert type to float
			er->eType = EXPTYPE_FLOAT;
			return;
		ELSENOTVALID(+);
		}//switch

		break;

	case EXPTYPE_FLOAT:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			return;
		case EXPTYPE_FLOAT:
			return;
		ELSENOTVALID(+);
		}//switch

		break;

	case EXPTYPE_STRING:

		switch (param.eType) {
		case EXPTYPE_STRING:
			return;
		ELSENOTVALID(+);
		}//switch

		break;

	// Types that can follow an unknown
	case EXPTYPE_NULL:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
		case EXPTYPE_FLOAT:
		case EXPTYPE_STRING:
			return;
		ELSENOTVALID(+);
		}

	DEFAULTNOTVALID(+);

	}//switch

}

bool ExpAdd::Constant()
{
	return DoBinaryCollapse(l, r);
}

/////////////
// ExpSubtract
DEFINE_STD_GETOPERANDS(ExpSubtract);
DEFINE_STD_BINARY(ExpSubtract);

void ExpSubtract::Evaluate(ExpReturn* er)
{
	ExpReturn param;

	l->Evaluate(er);
	r->Evaluate(&param);

	switch (er->eType) {

	case EXPTYPE_INTEGER:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			return;
		case EXPTYPE_FLOAT:
			er->eType = EXPTYPE_FLOAT;
			return;
		ELSENOTVALID(-);
		}//switch

		break;

	case EXPTYPE_FLOAT:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			return;
		case EXPTYPE_FLOAT:
			return;
		ELSENOTVALID(-);
		}//switch

		break;

	// Types that can follow an unknown
	case EXPTYPE_NULL:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
		case EXPTYPE_FLOAT:
			return;
		ELSENOTVALID(-);
		}

	DEFAULTNOTVALID(-);

	}//switch
}

bool ExpSubtract::Constant()
{
	return DoBinaryCollapse(l, r);
}

/////////////
// ExpMultiply
DEFINE_STD_GETOPERANDS(ExpMultiply);
DEFINE_STD_BINARY(ExpMultiply);

void ExpMultiply::Evaluate(ExpReturn* er)
{
	ExpReturn param;

	l->Evaluate(er);
	r->Evaluate(&param);

	switch (er->eType) {

	case EXPTYPE_INTEGER:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			return;
		case EXPTYPE_FLOAT:
			er->eType = EXPTYPE_FLOAT;
			return;
		ELSENOTVALID(*);
		}//switch

		break;

	case EXPTYPE_FLOAT:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			return;
		case EXPTYPE_FLOAT:
			return;
		ELSENOTVALID(*);
		}//switch

		break;

	// Types that can follow an unknown
	case EXPTYPE_NULL:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
		case EXPTYPE_FLOAT:
			return;
		ELSENOTVALID(*);
		}

	DEFAULTNOTVALID(*);

	}//switch
}

bool ExpMultiply::Constant()
{
	return DoBinaryCollapse(l, r);
}

/////////////
// ExpDivide
DEFINE_STD_GETOPERANDS(ExpDivide);
DEFINE_STD_BINARY(ExpDivide);

void ExpDivide::Evaluate(ExpReturn* er)
{
	ExpReturn param;

	l->Evaluate(er);
	r->Evaluate(&param);

	switch (er->eType) {

	case EXPTYPE_INTEGER:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			// Divide always returns float!  Experimental feature!
			er->eType = EXPTYPE_FLOAT;
			return;
		case EXPTYPE_FLOAT:
			er->eType = EXPTYPE_FLOAT;
			return;
		ELSENOTVALID(/);
		}//switch

		break;

	case EXPTYPE_FLOAT:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			return;
		case EXPTYPE_FLOAT:
			return;
		ELSENOTVALID(/);
		}//switch

	// Types that can follow an unknown
	case EXPTYPE_NULL:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
		case EXPTYPE_FLOAT:
			return;
		ELSENOTVALID(/);
		}

	DEFAULTNOTVALID(/);

	}
}

bool ExpDivide::Constant()
{
	return DoBinaryCollapse(l, r);
}

/////////////
// ExpMod
DEFINE_STD_GETOPERANDS(ExpMod);
DEFINE_STD_BINARY(ExpMod);

void ExpMod::Evaluate(ExpReturn* er)
{
	ExpReturn param;

	l->Evaluate(er);
	r->Evaluate(&param);

	switch (er->eType) {

	case EXPTYPE_INTEGER:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			return;
		ELSENOTVALID(%);
		}//switch

		break;

	// Types that can follow an unknown
	case EXPTYPE_NULL:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			return;
		ELSENOTVALID(%);
		}

	DEFAULTNOTVALID(%);

	}//switch
}

bool ExpMod::Constant()
{
	return DoBinaryCollapse(l, r);
}

/////////////
// ExpPower
DEFINE_STD_GETOPERANDS(ExpPower);
DEFINE_STD_BINARY(ExpPower);

void ExpPower::Evaluate(ExpReturn* er)
{
	ExpReturn param;

	l->Evaluate(er);
	r->Evaluate(&param);

	switch (er->eType) {

	case EXPTYPE_INTEGER:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			return;
		case EXPTYPE_FLOAT:
			er->eType = EXPTYPE_FLOAT;
			return;
		ELSENOTVALID(^);
		}//switch

		break;

	case EXPTYPE_FLOAT:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			return;
		case EXPTYPE_FLOAT:
			return;
		ELSENOTVALID(^);
		}//switch

		break;

	// Types that can follow an unknown
	case EXPTYPE_NULL:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
		case EXPTYPE_FLOAT:
			return;
		ELSENOTVALID(%);
		}

	DEFAULTNOTVALID(%);

	}//switch
}

bool ExpPower::Constant()
{
	return DoBinaryCollapse(l, r);
}

/////////////
// ExpAt
DEFINE_STD_GETOPERANDS(ExpAt);

ExpAt::ExpAt()
{
	l = NULL;
	r = NULL;
	// Initialise value to 0.0 (so operator= works)
	value = 0.0;
}

ExpAt::~ExpAt()
{
	if (l) delete l;
	if (r) delete r;
}

void ExpAt::Evaluate(ExpReturn *er)
{
	ExpReturn param;

	// Evaluate both sides
	l->Evaluate(er);
	r->Evaluate(&param);

	// Left side not an array?  Have no effect
	if (er->Type() != EXPTYPE_ARRAY && er->Type() != EXPTYPE_NULL)
		throw runtime_error((const char*)(CString("'at' does not work on '") + GetTypeName(er->Type()) + "' and '" + GetTypeName(param.Type()) + "'"));

	// The actual thing returned by the array is unknown.
	// Consider {5, "hello"} at 2 - the type checker is not in a position to evaluate the actual values.
	// Hence 'at' returns an 'unknown'.
	er->eType = EXPTYPE_NULL;
}

bool ExpAt::Constant()
{
	return DoBinaryCollapse(l, r);
}


/////////////
// ExpDot
DEFINE_STD_GETOPERANDS(ExpDot);

//OPTIMISE: necessary to perform new stack every call?
//OPTIMISE: maintain expParamList when all parameters are constant?  partially maintain when some are constant?
void ExpDot::Evaluate(ExpReturn* er)
{
	ExpReturn* expPtr = expParamList;

	// Check parameter count OK
	if (!pCExp->CheckParamCount(objName, fname, parameters.size())) {
		CString msg;
		msg.Format("'%s.%s' does not take %d parameters", objName, fname, parameters.size());
		throw runtime_error((const char*)msg);
	}

	// Parameters exist?  Evaluate them
	if (hasParameters) {
		
		ExpParamIterator i = parameters.begin();
		ExpParamConstIterator parameters_end = parameters.end();

		int index = 0;

		// Evaluate every parameter
		for ( ; i != parameters_end; i++, index++) {

			// Send to address of corresponding expParamList array entry
			(*i)->Evaluate(expPtr);

			// Special check for variable name param
			if (expPtr->Type() == EXPTYPE_VARIABLENAME) {
				if (!pCExp->PrivateVariableExists(objName, fname, index, parameters.size(), *(expPtr->GetStringPtr()))) {
					CString msg;
					msg.Format("'%s' is not a private variable in '%s' (parameter %d)", *(expPtr->GetStringPtr()), fname, index + 1);
					throw runtime_error((const char*)msg);
				}
			}
			else if (!pCExp->ParamValid(objName, fname, index, parameters.size(), expPtr->Type())) {
				CString msg;
				msg.Format("Parameter %d of '%s.%s' does not take '%s'", index+1, objName, fname, GetTypeName(expPtr->Type()));
				throw runtime_error((const char*)msg);
			}

			expPtr++;

		}
	}

	// Undefined expression call?
	if (unnamedExp) {
		// TODO: check unnamed expression call is valid
	}
	
	// Obtain expression type from IDE
	er->eType = pCExp->GetExpressionType(objName, fname);
}

ExpDot::ExpDot()
{
	l = NULL;
	r = NULL;
}

ExpDot::~ExpDot()
{
	if (l != NULL)
		delete l;
	if (r != NULL)
		delete r;

	// Delete parameters
	vector<ExpPart*>::iterator i;

	for (i = parameters.begin(); i != parameters.end(); i++)
		delete (*i);

	parameters.clear();
}

bool ExpDot::Constant()
{
	// Const collapse all parameters
	vector<ExpPart*>::iterator i;

	for (i = parameters.begin(); i != parameters.end(); i++) {

		// This parameter is constant
		if ((*i)->Constant())
			CollapseConstantTree((*i));	// Collapse it
	}

	// Return false; no way of telling if the plugin will return a constant value
	// or not.
	return false;
}

////////////////////////////////////////////////////////
DEFINE_STD_GETOPERANDS(ExpEqual);
DEFINE_STD_BINARY(ExpEqual);

void ExpEqual::Evaluate(ExpReturn* er)
{
	ExpReturn param;

	l->Evaluate(er);
	r->Evaluate(&param);

	switch (er->eType) {

	case EXPTYPE_INTEGER:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			return;
		case EXPTYPE_FLOAT:
			return;
		ELSENOTVALID(=);
		}//switch

		break;

	case EXPTYPE_FLOAT:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			er->eType = EXPTYPE_INTEGER;
			return;
		case EXPTYPE_FLOAT:
			er->eType = EXPTYPE_INTEGER;
			return;
		ELSENOTVALID(=);
		}//switch

		break;

	case EXPTYPE_STRING:

		switch (param.eType) {
		case EXPTYPE_STRING:
			er->eType = EXPTYPE_INTEGER;
			return;
		ELSENOTVALID(=);
		}

		break;

	// Types that can follow an unknown
	case EXPTYPE_NULL:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
		case EXPTYPE_FLOAT:
		case EXPTYPE_STRING:
			return;
		ELSENOTVALID(=);
		}

	DEFAULTNOTVALID(=);

	}//switch

}

bool ExpEqual::Constant()
{
	return DoBinaryCollapse(l, r);
}

////////////////////////////////////////////////////////
DEFINE_STD_GETOPERANDS(ExpNotEqual);
DEFINE_STD_BINARY(ExpNotEqual);

void ExpNotEqual::Evaluate(ExpReturn* er)
{
	ExpReturn param;

	l->Evaluate(er);
	r->Evaluate(&param);

	switch (er->eType) {

	case EXPTYPE_INTEGER:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			return;
		case EXPTYPE_FLOAT:
			return;
		ELSENOTVALID(not equal);
		}//switch

		break;

	case EXPTYPE_FLOAT:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			er->eType = EXPTYPE_INTEGER;
			return;
		case EXPTYPE_FLOAT:
			er->eType = EXPTYPE_INTEGER;
			return;
		ELSENOTVALID(not equal);
		}//switch

		break;

	case EXPTYPE_STRING:

		switch (param.eType) {
		case EXPTYPE_STRING:
			er->eType = EXPTYPE_INTEGER;
			return;
		ELSENOTVALID(not equal);
		}

		break;

	// Types that can follow an unknown
	case EXPTYPE_NULL:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
		case EXPTYPE_FLOAT:
		case EXPTYPE_STRING:
			return;
		ELSENOTVALID(not equal);
		}

	DEFAULTNOTVALID(not equal);

	}//switch
}

bool ExpNotEqual::Constant()
{
	return DoBinaryCollapse(l, r);
}

////////////////////////////////////////////////////////
DEFINE_STD_GETOPERANDS(ExpLess);
DEFINE_STD_BINARY(ExpLess);

void ExpLess::Evaluate(ExpReturn* er)
{
	ExpReturn param;

	l->Evaluate(er);
	r->Evaluate(&param);

	switch (er->eType) {

	case EXPTYPE_INTEGER:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			return;
		case EXPTYPE_FLOAT:
			return;
		ELSENOTVALID(<);
		}//switch

		break;

	case EXPTYPE_FLOAT:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			er->eType = EXPTYPE_INTEGER;
			return;
		case EXPTYPE_FLOAT:
			er->eType = EXPTYPE_INTEGER;
			return;
		ELSENOTVALID(<);
		}//switch

		break;

	// Types that can follow an unknown
	case EXPTYPE_NULL:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
		case EXPTYPE_FLOAT:
			return;
		ELSENOTVALID(<);
		}

	DEFAULTNOTVALID(<);

	}//switch
}

bool ExpLess::Constant()
{
	return DoBinaryCollapse(l, r);
}

////////////////////////////////////////////////////////
DEFINE_STD_GETOPERANDS(ExpLessEqual);
DEFINE_STD_BINARY(ExpLessEqual);

void ExpLessEqual::Evaluate(ExpReturn* er)
{
	ExpReturn param;

	l->Evaluate(er);
	r->Evaluate(&param);

	switch (er->eType) {

	case EXPTYPE_INTEGER:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			return;
		case EXPTYPE_FLOAT:
			return;
		ELSENOTVALID(<=);
		}//switch

		break;

	case EXPTYPE_FLOAT:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			er->eType = EXPTYPE_INTEGER;
			return;
		case EXPTYPE_FLOAT:
			er->eType = EXPTYPE_INTEGER;
			return;
		ELSENOTVALID(<=);
		}//switch

		break;

	// Types that can follow an unknown
	case EXPTYPE_NULL:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
		case EXPTYPE_FLOAT:
			return;
		ELSENOTVALID(<=);
		}

	DEFAULTNOTVALID(<=);

	}//switch

}

bool ExpLessEqual::Constant()
{
	return DoBinaryCollapse(l, r);
}

////////////////////////////////////////////////////////
DEFINE_STD_GETOPERANDS(ExpGreater);
DEFINE_STD_BINARY(ExpGreater);

void ExpGreater::Evaluate(ExpReturn* er)
{
	ExpReturn param;

	l->Evaluate(er);
	r->Evaluate(&param);

	switch (er->eType) {

	case EXPTYPE_INTEGER:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			return;
		case EXPTYPE_FLOAT:
			return;
		ELSENOTVALID(>);
		}//switch

		break;

	case EXPTYPE_FLOAT:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			er->eType = EXPTYPE_INTEGER;
			return;
		case EXPTYPE_FLOAT:
			er->eType = EXPTYPE_INTEGER;
			return;
		ELSENOTVALID(>);
		}//switch

		break;

	// Types that can follow an unknown
	case EXPTYPE_NULL:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
		case EXPTYPE_FLOAT:
			return;
		ELSENOTVALID(>);
		}

	DEFAULTNOTVALID(>);

	}//switch

}

bool ExpGreater::Constant()
{
	return DoBinaryCollapse(l, r);
}

////////////////////////////////////////////////////////
DEFINE_STD_GETOPERANDS(ExpGreaterEqual);
DEFINE_STD_BINARY(ExpGreaterEqual);

void ExpGreaterEqual::Evaluate(ExpReturn* er)
{
	ExpReturn param;

	l->Evaluate(er);
	r->Evaluate(&param);

	switch (er->eType) {

	case EXPTYPE_INTEGER:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			return;
		case EXPTYPE_FLOAT:
			return;
		ELSENOTVALID(>=);
		}//switch

		break;

	case EXPTYPE_FLOAT:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			er->eType = EXPTYPE_INTEGER;
			return;
		case EXPTYPE_FLOAT:
			er->eType = EXPTYPE_INTEGER;
			return;
		ELSENOTVALID(>=);
		}//switch

		break;

	// Types that can follow an unknown
	case EXPTYPE_NULL:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
		case EXPTYPE_FLOAT:
			return;
		ELSENOTVALID(>=);
		}

	DEFAULTNOTVALID(>=);

	}//switch

}

bool ExpGreaterEqual::Constant()
{
	return DoBinaryCollapse(l, r);
}

////////////////////////////////////////////////////////
DEFINE_STD_GETOPERANDS(ExpAnd);
DEFINE_STD_BINARY(ExpAnd);

void ExpAnd::Evaluate(ExpReturn* er)
{
	ExpReturn param;

	l->Evaluate(er);
	r->Evaluate(&param);

	// Also does string concats
	switch (er->eType) {

	case EXPTYPE_INTEGER:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			return;
		case EXPTYPE_STRING: {
			er->eType = EXPTYPE_STRING;
			return;
			}
		 ELSENOTVALID(and);
		}//switch

	case EXPTYPE_FLOAT:
		switch(param.eType) {
		case EXPTYPE_STRING:
			er->eType = EXPTYPE_STRING;
			return;
		ELSENOTVALID(and);
		}
	case EXPTYPE_STRING:
		switch(param.eType) {
		case EXPTYPE_INTEGER:
			return;
		case EXPTYPE_FLOAT:
			return;
		case EXPTYPE_STRING:
			return;
		ELSENOTVALID(and);
		}

	// Types that can follow an unknown
	case EXPTYPE_NULL:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
		case EXPTYPE_FLOAT:
		case EXPTYPE_STRING:
			return;
		ELSENOTVALID(and);
		}

	DEFAULTNOTVALID(and);
	}//switch

	throw runtime_error((const char*)(CString("'and' does not work on '") + GetTypeName(er->Type()) + "' and '" + GetTypeName(param.Type()) + "'"));
}

bool ExpAnd::Constant()
{
	return DoBinaryCollapse(l, r);
}

////////////////////////////////////////////////////////
DEFINE_STD_GETOPERANDS(ExpOr);
DEFINE_STD_BINARY(ExpOr);

void ExpOr::Evaluate(ExpReturn* er)
{
	ExpReturn param;

	l->Evaluate(er);
	r->Evaluate(&param);

	switch (er->eType) {

	case EXPTYPE_INTEGER:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			return;
		ELSENOTVALID(or);
		}//switch

		break;

	// Types that can follow an unknown
	case EXPTYPE_NULL:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			return;
		ELSENOTVALID(%);
		}

	DEFAULTNOTVALID(%);
	}//switch

	throw runtime_error((const char*)(CString("'or' does not work on '") + GetTypeName(er->Type()) + "' and '" + GetTypeName(param.Type()) + "'"));
}

bool ExpOr::Constant()
{
	return DoBinaryCollapse(l, r);
}

////////////////////////////////////////////////////////
ExpConditional::ExpConditional()
{
	a = NULL;
	b = NULL;
	c = NULL;
}

ExpConditional::~ExpConditional()
{
	if (a) delete a;
	if (b) delete b;
	if (c) delete c;
}

ExpPart*& ExpConditional::GetPLeftOperand()
{
	return a;
}

// Never called but must be defined.  Ignore the "returning local" warning
#pragma warning(disable:4172)
ExpPart*& ExpConditional::GetPRightOperand()
{
	ExpPart* dummy = NULL;
	return dummy;
}
#pragma warning(default:4172)

void ExpConditional::Evaluate(ExpReturn* er)
{
	// Evaluate a ? b : c

	// Evaluate the condition
	a->Evaluate(er);

	if (er->eType != EXPTYPE_INTEGER && er->eType != EXPTYPE_NULL) {
	
		// Not an integer!  Cannot test condition.
		throw runtime_error((const char*)(CString("Operator ?: cannot test a '") + GetTypeName(er->Type()) + "'"));

	}

	// Evaluate b and c to ensure also valid
	b->Evaluate(er);
	c->Evaluate(er);
}

bool ExpConditional::Constant()
{
	if (a->Constant()) CollapseConstantTree(a);
	if (b->Constant()) CollapseConstantTree(b);
	if (c->Constant()) CollapseConstantTree(c);

	return (a->Constant() && b->Constant() && c->Constant());
}

#define DEG_TO_RAD(a) ((a) / (180 / 3.1415926535897932384626433832795))
#define RAD_TO_DEG(a) ((a) * (180 / 3.1415926535897932384626433832795))

#define DEFINE_STD_UNARY(type)	\
	type::type()				\
	{							\
		p = NULL;				\
	}							\
								\
	type::~type()				\
	{							\
		if (p != NULL)			\
			delete p;			\
	}							\
								\
	bool type::Constant()		\
	{							\
		if (p->Constant()) {	\
			CollapseConstantTree(p);	\
			return true;		\
		}						\
		return false;			\
	}

/////////////
// ExpSin
void ExpSin::Evaluate(ExpReturn* er)
{
	if (p == NULL)
		throw runtime_error("Missing parameter for 'sin'");

	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		er->eType = EXPTYPE_FLOAT;
		return;
	case EXPTYPE_FLOAT:
		return;
	case EXPTYPE_NULL: return;
	}

	// Types invalid
	throw runtime_error((const char*)(CString("'sin' does not work on '") + GetTypeName(er->Type()) + "'"));
}

DEFINE_STD_UNARY(ExpSin)

/////////////
// ExpCos
void ExpCos::Evaluate(ExpReturn* er)
{
	if (p == NULL)
		throw runtime_error("Missing parameter for 'cos'");

	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		er->eType = EXPTYPE_FLOAT;
		return;
	case EXPTYPE_FLOAT:
		return;
	case EXPTYPE_NULL: return;
	}

	// Types invalid
	throw runtime_error((const char*)(CString("'cos' does not work on '") + GetTypeName(er->Type()) + "'"));
}

DEFINE_STD_UNARY(ExpCos)

/////////////
// ExpTan
void ExpTan::Evaluate(ExpReturn* er)
{
	if (p == NULL)
		throw runtime_error("Missing parameter for 'tan'");

	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		er->eType = EXPTYPE_FLOAT;
		return;
	case EXPTYPE_FLOAT:
		return;
	case EXPTYPE_NULL: return;
	}

	// Types invalid
	throw runtime_error((const char*)(CString("'tan' does not work on '") + GetTypeName(er->Type()) + "'"));
}

DEFINE_STD_UNARY(ExpTan)

/////////////
// ExpSqrt
void ExpSqrt::Evaluate(ExpReturn* er)
{
	if (p == NULL)
		throw runtime_error("Missing parameter for 'sqrt'");

	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		er->eType = EXPTYPE_FLOAT;
		return;
	case EXPTYPE_FLOAT:
		return;
	case EXPTYPE_NULL: return;
	}

	// Types invalid
	throw runtime_error((const char*)(CString("'sqrt' does not work on '") + GetTypeName(er->Type()) + "'"));
}

DEFINE_STD_UNARY(ExpSqrt)

#define CSTR_BUF(a) ((const char*)(a))

/////////////
// ExpFuncInt
void ExpFuncInt::Evaluate(ExpReturn* er)
{
	if (p == NULL)
		throw runtime_error("Missing parameter for 'int' cast");

	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		return;	// Int() does not affect integers
	case EXPTYPE_FLOAT:
		er->eType = EXPTYPE_INTEGER;
		return;
	case EXPTYPE_STRING:
		er->eType = EXPTYPE_INTEGER;
		return;
	case EXPTYPE_NULL: return;
	}

	// Types invalid
	throw runtime_error((const char*)(CString("'int' cast does not work on '") + GetTypeName(er->Type()) + "'"));
}

DEFINE_STD_UNARY(ExpFuncInt)

/////////////
// ExpFuncFloat
void ExpFuncFloat::Evaluate(ExpReturn* er)
{
	if (p == NULL)
		throw runtime_error("Missing parameter for 'float' cast");

	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		er->eType = EXPTYPE_FLOAT;
		return;
	case EXPTYPE_FLOAT:
		return;
	case EXPTYPE_STRING:
		er->eType = EXPTYPE_FLOAT;
		return;
	case EXPTYPE_NULL: return;
	}

	// Types invalid
	throw runtime_error((const char*)(CString("'float' cast does not work on '") + GetTypeName(er->Type()) + "'"));
}

DEFINE_STD_UNARY(ExpFuncFloat)

/////////////
// ExpFuncStr
void ExpFuncStr::Evaluate(ExpReturn* er)
{
	if (p == NULL)
		throw runtime_error("Missing parameter for 'str' cast");

	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		er->eType = EXPTYPE_STRING;
		return;
	case EXPTYPE_FLOAT:
		er->eType = EXPTYPE_STRING;
		return;
	case EXPTYPE_STRING:
		return;
	case EXPTYPE_NULL: return;
	}

	// Types invalid
	throw runtime_error((const char*)(CString("'string' cast does not work on '") + GetTypeName(er->Type()) + "'"));
}

DEFINE_STD_UNARY(ExpFuncStr)

/////////////
// ExpAsin
void ExpAsin::Evaluate(ExpReturn* er)
{
	if (p == NULL)
		throw runtime_error("Missing parameter for 'asin'");

	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		er->eType = EXPTYPE_FLOAT;
		return;
	case EXPTYPE_FLOAT:
		return;
	case EXPTYPE_NULL: return;
	}

	// Types invalid
	throw runtime_error((const char*)(CString("'asin' does not work on '") + GetTypeName(er->Type()) + "'"));
}

DEFINE_STD_UNARY(ExpAsin)

/////////////
// ExpAcos
void ExpAcos::Evaluate(ExpReturn* er)
{
	if (p == NULL)
		throw runtime_error("Missing parameter for 'acos'");

	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		er->eType = EXPTYPE_FLOAT;
		return;
	case EXPTYPE_FLOAT:
		return;
	case EXPTYPE_NULL: return;
	}

	// Types invalid
	throw runtime_error((const char*)(CString("'acos' does not work on '") + GetTypeName(er->Type()) + "'"));
}

DEFINE_STD_UNARY(ExpAcos)

/////////////
// ExpAtan
void ExpAtan::Evaluate(ExpReturn* er)
{
	if (p == NULL)
		throw runtime_error("Missing parameter for 'atan'");

	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		er->eType = EXPTYPE_FLOAT;
		return;
	case EXPTYPE_FLOAT:
		return;
	case EXPTYPE_NULL: return;
	}

	// Types invalid
	throw runtime_error((const char*)(CString("'atan' does not work on '") + GetTypeName(er->Type()) + "'"));
}

DEFINE_STD_UNARY(ExpAtan)

/////////////
// ExpAbs
void ExpAbs::Evaluate(ExpReturn* er)
{
	if (p == NULL)
		throw runtime_error("Missing parameter for 'abs'");

	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		return;
	case EXPTYPE_FLOAT:
		return;
	case EXPTYPE_NULL: return;
	}

	// Types invalid
	throw runtime_error((const char*)(CString("'abs' does not work on '") + GetTypeName(er->Type()) + "'"));
}

DEFINE_STD_UNARY(ExpAbs)

/////////////
// ExpExp (exponential, e^p)
void ExpExp::Evaluate(ExpReturn* er)
{
	if (p == NULL)
		throw runtime_error("Missing parameter for 'exp'");

	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		er->eType = EXPTYPE_FLOAT;
		return;
	case EXPTYPE_FLOAT:
		return;
	case EXPTYPE_NULL: return;
	}

	// Types invalid
	throw runtime_error((const char*)(CString("'exp' does not work on '") + GetTypeName(er->Type()) + "'"));
}

DEFINE_STD_UNARY(ExpExp)

/////////////
// ExpLn
void ExpLn::Evaluate(ExpReturn* er)
{
	if (p == NULL)
		throw runtime_error("Missing parameter for 'ln'");

	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		er->eType = EXPTYPE_FLOAT;
		return;
	case EXPTYPE_FLOAT:
		return;
	case EXPTYPE_NULL: return;
	}

	// Types invalid
	throw runtime_error((const char*)(CString("'ln' does not work on '") + GetTypeName(er->Type()) + "'"));
}

DEFINE_STD_UNARY(ExpLn)

/////////////
// ExpLog10
void ExpLog10::Evaluate(ExpReturn* er)
{
	if (p == NULL)
		throw runtime_error("Missing parameter for 'log10'");

	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		er->eType = EXPTYPE_FLOAT;
		return;
	case EXPTYPE_FLOAT:
		return;
	case EXPTYPE_NULL: return;
	}

	// Types invalid
	throw runtime_error((const char*)(CString("'log10' does not work on '") + GetTypeName(er->Type()) + "'"));
}

DEFINE_STD_UNARY(ExpLog10)

/////////////
// ExpCeil
void ExpCeil::Evaluate(ExpReturn* er)
{
	if (p == NULL)
		throw runtime_error("Missing parameter for 'ceil'");

	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		return; // no effect on integer
	case EXPTYPE_FLOAT:
		er->eType = EXPTYPE_INTEGER;
		return;
	case EXPTYPE_NULL: return;
	}

	// Types invalid
	throw runtime_error((const char*)(CString("'ceil' does not work on '") + GetTypeName(er->Type()) + "'"));
}

DEFINE_STD_UNARY(ExpCeil)

/////////////
// ExpRound
void ExpRound::Evaluate(ExpReturn* er)
{
	if (p == NULL)
		throw runtime_error("Missing parameter for 'round'");

	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		return; // no effect on integer
	case EXPTYPE_FLOAT:
		er->eType = EXPTYPE_INTEGER;
		return;
	case EXPTYPE_NULL: return;
	}

	// Types invalid
	throw runtime_error((const char*)(CString("'round' does not work on '") + GetTypeName(er->Type()) + "'"));
}

DEFINE_STD_UNARY(ExpRound)

/////////////
// ExpRandom
void ExpRandom::Evaluate(ExpReturn* er)
{
	if (p == NULL)
		throw runtime_error("Missing parameter for 'random'");

	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		return;
	case EXPTYPE_FLOAT:
		return;
	case EXPTYPE_NULL: return;
	}

	// Types invalid
	throw runtime_error((const char*)(CString("'random' does not work on '") + GetTypeName(er->Type()) + "'"));
}

ExpRandom::ExpRandom()
{
	p = NULL;
}

ExpRandom::~ExpRandom()
{
	if (p)
		delete p;
}

bool ExpRandom::Constant()
{
	// Random values are constantly changing!
	return false;
}

/////////////
// ExpLen
void ExpLen::Evaluate(ExpReturn* er)
{
	if (p == NULL)
		throw runtime_error("Missing parameter for 'len'");

	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_STRING:
		er->eType = EXPTYPE_INTEGER;
		return;
	case EXPTYPE_ARRAY:
		er->eType = EXPTYPE_INTEGER;
		return;
	case EXPTYPE_NULL: return;
	}

	// Types invalid
	throw runtime_error((const char*)(CString("'len' does not work on '") + GetTypeName(er->Type()) + "'"));
}

DEFINE_STD_UNARY(ExpLen)