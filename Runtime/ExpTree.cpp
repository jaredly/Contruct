
#include "StdAfx.h"

ExpPart::ExpPart()
{
	bracketdepth = 0;
}

ExpPart*& ExpPart::GetPLeftOperand()
{
	// Return a reference to a null pointer
	return (ExpPart*&)(pCExp->pRuntime->nullPtr);
}

ExpPart*& ExpPart::GetPRightOperand()
{
	// Return a reference to a null pointer
	return (ExpPart*&)(pCExp->pRuntime->nullPtr);
}

#ifdef CONSTRUCT_DEBUGGER
const char* GetTypeName(ExpType t) {
	switch(t) {
	case EXPTYPE_INTEGER: return "integer";
	case EXPTYPE_FLOAT: return "float";
	case EXPTYPE_STRING: return "string";
	case EXPTYPE_ARRAY: return "array";
	case EXPTYPE_BINARY: return "binary";
	case EXPTYPE_VARIABLENAME: return "variable name";
	case EXPTYPE_NULL: return "null";
	default: return "unknown";
	}
}
void ReportTypeMismatchBinary(CRuntime* pRuntime, ExpReturn* l, ExpReturn* r, const char* operation) {
	CString msg;
	msg.Format("Cannot %s with %s and %s.  Operator had no effect", operation, GetTypeName(l->Type()), GetTypeName(r->Type()));
	pRuntime->LogMsg(msg);
}
void ReportTypeMismatchUnary(CRuntime* pRuntime, ExpReturn* p, const char* operation) {
	CString msg;
	msg.Format("Cannot perform %s on %s.  Operator had no effect", operation, GetTypeName(p->Type()));
	pRuntime->LogMsg(msg);
}
#define TYPEMISMATCH2(l, r, oper) ReportTypeMismatchBinary(pCExp->pRuntime, l, r, oper)
#define TYPEMISMATCH1(p, oper) ReportTypeMismatchUnary(pCExp->pRuntime, p, oper)
#else
#define TYPEMISMATCH2(l, r, oper)
#define TYPEMISMATCH1(p, oper)
#endif

///////////
// Collapse a constant tree
// If an ExpPart* returns true for Constant(), evaluate it, and reallocate a new
// literal to represent the constant part of that expression.
// Ctor
CollapseConstantTreeObj::CollapseConstantTreeObj(ExpPart*& p)
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
		static_cast<ExpVariableName*>(p)->index = e.eData.vni.varIndex;
		static_cast<ExpVariableName*>(p)->pOwnerType = e.eData.vni.pOwnerType;
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
	temp = value;
	// Send the temp address as the cstring to modify
	er->eData.str = &temp;
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
			// Cast the ExpStore to ExpReturn
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
	index = 0;
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
	er->eData.vni.varIndex = index;
	er->eData.vni.pOwnerType = pOwnerType;
}

/////////////
// ExpBinary
void ExpBinary::Evaluate(ExpReturn* er)
{
	er->eType = EXPTYPE_BINARY;
	er->eData.bin = data;
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

	// Parameters exist?  Evaluate them
	if (hasParameters) {

		ExpReturn* expPtr = expParamList;
		ExpPart** curParam = parameters;

		// Evaluate every parameter
		for ( ; curParam != paramsEnd; curParam++)

			// Send to address of corresponding expParamList array entry
			(*curParam)->Evaluate(expPtr++);
	}

	// System call?
	if (this->pType == NULL) {

		D_ENTER(DCS_EXPRESSION, pSystemObject);

		(pSystemObject->*routine)(expParamList, *er);

		D_RUNTIME();
	}
	else {

		CRunObject* objToCall;

		// OID of caller is the OID of this expression call?
		if (this->oid == pCExp->oidCaller || pCExp->overrideCalledOID) {

			// Call the owner's expression routine
			objToCall = pCExp->pSelectedObject;

			// Still NULL?
			if (objToCall == NULL) {
				if (pType->pSol->selectAll)
					objToCall = pType->instances.front();
				else if (!(pType->pSol->sol.empty()))
					objToCall = pType->pSol->sol.front();
			}

		}

		else {

			SelObjList& rSol = *(pType->pSol);

			// Call the routine for the paired instance
			if (rSol.selectAll) {

				if (pTypeInstances->size() > 0)

					// SOL index refers to instance list if all selected
					objToCall = pTypeInstances->operator []((pCExp->SolID) % pTypeInstances->size());

				// No objects to call!  Return zero!
				// TODO - best solution?
				else {
					er->eType = EXPTYPE_INTEGER;
					er->eData.iVal = 0;
					
					// Give a debug warning
#ifdef CONSTRUCT_DEBUGGER
					CString msg;
					msg.Format("Expression '%s': no '%s' objects exist: returning 0", ident, ident);
					pRuntime->LogMsg(msg);
#endif
					return;
				}

			}

			else

				// Use index to the SOL for pairing
				objToCall = rSol.sol[(pCExp->SolID) % rSol.sol.size()];
		
		}//else

		// Do the call
		D_ENTER(DCS_EXPRESSION, objToCall);

		objToCall->ReturnDefaultValue(expParamList, *er);

		D_RUNTIME();
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
	ExpPart** curParam = parameters;

	for ( ; curParam != paramsEnd; curParam++)
		delete *curParam;
}

bool ExpIdent::Constant()
{
	// Const collapse all parameters
	ExpPart** curParam = parameters;

	bool allParamsConstant = true;

	for ( ; curParam != paramsEnd; curParam++) {

		// This parameter is constant
		if ((*curParam)->Constant())
			CollapseConstantTree(*curParam);	// Collapse it
		else
			allParamsConstant = false;
	}

	// All parameters are constant: evaluate the parameters
	if (allParamsConstant) {

		ExpReturn* expPtr = expParamList;
		ExpPart** curParam = parameters;

		// Evaluate every parameter
		for ( ; curParam != paramsEnd; curParam++)

			// Send to address of corresponding expParamList array entry
			(*curParam)->Evaluate(expPtr++);

		// Switch off parameter evaluation since all parameters are constant and we've evaluated them already.
		hasParameters = false;
	}

	// Return false; no way of telling if the plugin will return a constant value
	// or not.

	// UNLESS it is a system expression, in which we can mark constant all expressions which return
	// a constant output for a constant input.
	// Note i havent bothered for expressions like max and min.  Nobody types max(3,4,5,6).
	if (oid == -1 && allParamsConstant) {
		switch (expID) {
		case 10:					// newline
		case 21:					// numtokens
		case 22:					// gettoken
		case SYSTEM_VARINDEX:		// _varindex
			return true;
		default:
			return false;
		}

	}

	return false;
}

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
			er->eData.iVal += param.eData.iVal;
			return;
		case EXPTYPE_FLOAT:
			// Adding float to integer: convert type to float
			er->eType = EXPTYPE_FLOAT;
			er->eData.fVal = (float)er->eData.iVal + param.eData.fVal;
			return;
		}//switch

		break;

	case EXPTYPE_FLOAT:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			er->eData.fVal += param.eData.iVal;
			return;
		case EXPTYPE_FLOAT:
			er->eData.fVal += param.eData.fVal;
			return;
		}//switch

		break;

	case EXPTYPE_STRING:

		switch (param.eType) {
		case EXPTYPE_STRING:
			*(er->eData.str) += *(param.eData.str);
			return;
		}//switch

		break;

	}//switch

	// Types invalid: return without modifying er (!)
	// eg. "Hello" + 5 gives "Hello"
	TYPEMISMATCH2(er, &param, "add");
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
			er->eData.iVal -= param.eData.iVal;
			return;
		case EXPTYPE_FLOAT:
			er->eType = EXPTYPE_FLOAT;
			er->eData.fVal = (float)er->eData.iVal - param.eData.fVal;
			return;
		}//switch

		break;

	case EXPTYPE_FLOAT:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			er->eData.fVal -= param.eData.iVal;
			return;
		case EXPTYPE_FLOAT:
			er->eData.fVal -= param.eData.fVal;
			return;
		}//switch

		break;

	}//switch

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH2(er, &param, "subtract");
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
			er->eData.iVal *= param.eData.iVal;
			return;
		case EXPTYPE_FLOAT:
			er->eType = EXPTYPE_FLOAT;
			er->eData.fVal = (float)er->eData.iVal * param.eData.fVal;
			return;
		}//switch

		break;

	case EXPTYPE_FLOAT:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			er->eData.fVal *= param.eData.iVal;
			return;
		case EXPTYPE_FLOAT:
			er->eData.fVal *= param.eData.fVal;
			return;
		}//switch

		break;

	}//switch

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH2(er, &param, "multiply");
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
			er->eData.fVal = er->eData.iVal;
			er->eData.fVal /= param.eData.iVal;			
			return;
		case EXPTYPE_FLOAT:
			er->eType = EXPTYPE_FLOAT;
			er->eData.fVal = (float)er->eData.iVal / param.eData.fVal;
			return;
		}//switch

		break;

	case EXPTYPE_FLOAT:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			er->eData.fVal /= param.eData.iVal;
			return;
		case EXPTYPE_FLOAT:
			er->eData.fVal /= param.eData.fVal;
			return;
		}//switch

		break;
	}//switch

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH2(er, &param, "divide");
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
			er->eData.iVal %= param.eData.iVal;
			return;
		case EXPTYPE_FLOAT:
			er->eData.iVal %= (__int64)param.eData.fVal;
			return;
		}//switch

		break;

	case EXPTYPE_FLOAT:
		
		switch (param.eType) {
		case EXPTYPE_INTEGER:
			er->eData.iVal = (__int64)er->eData.fVal % param.eData.iVal;
			er->eType = EXPTYPE_INTEGER;
			return;
		case EXPTYPE_FLOAT:
			er->eData.iVal = (__int64)er->eData.fVal % (__int64)param.eData.fVal;
			er->eType = EXPTYPE_INTEGER;
			return;
		}
		break;

	}//switch

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH2(er, &param, "modulus");
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
			er->eData.iVal = pow(double(er->eData.iVal), double(param.eData.iVal));
			return;
		case EXPTYPE_FLOAT:
			er->eType = EXPTYPE_FLOAT;
			er->eData.fVal = pow(double(er->eData.iVal), param.eData.fVal);
			return;
		}//switch

		break;

	case EXPTYPE_FLOAT:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			er->eData.fVal = pow(er->eData.fVal, double(param.eData.iVal));
			return;
		case EXPTYPE_FLOAT:
			er->eData.fVal = pow(er->eData.fVal, param.eData.fVal);
			return;
		}//switch

		break;

	}//switch

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH2(er, &param, "raise to power");
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

	// Clean up any value held data
	//value.Free();
}

void ExpAt::Evaluate(ExpReturn *er)
{
	ExpReturn param;

	// Evaluate both sides
	l->Evaluate(er);
	r->Evaluate(&param);

	// Left side not an array?  Have no effect
	if (er->Type() != EXPTYPE_ARRAY) {
#ifdef CONSTRUCT_DEBUGGER
		pCExp->pRuntime->LogMsg("Expression 'at': Expression on left is not an array.  Operator had no effect.");
#endif
		return;
	}

	// List of array indices to access, eg. 5 then 6 then 3 for at {5,6,3}
	int derefs[32];
	int	derefCount = 0;

	// Check right side type and get index
	switch(param.Type()) {
	case EXPTYPE_INTEGER:
		derefs[0] = param.eData.iVal;
		derefs[0]--;	// Make 0-based
		derefCount = 1;
		break;
	case EXPTYPE_FLOAT:
		derefs[0] = param.eData.fVal;
		derefs[0]--;	// Make 0-based
		derefCount = 1;
		break;
	case EXPTYPE_ARRAY:
		const ExpStore* i = param.GetArray();
		const ExpStore* const arr_end = i + param.GetArraySize();

		for ( ; i != arr_end; i++) {
			derefs[derefCount] = i->GetInt() - 1;
			derefCount++;
		}
	}

	const ExpReturn* elementPtr = er;

	// Dereference the array as needed
	for (int i = 0; i < derefCount; i++) {
		int index = derefs[i];
		
		// Return 0 for out-of-bounds
		if (index < 0 || index >= elementPtr->GetArraySize()) {
			er->eType = EXPTYPE_INTEGER;
			er->eData.iVal = 0;

#ifdef CONSTRUCT_DEBUGGER
			pCExp->pRuntime->LogMsg("Expression 'at': Array access out-of-bounds: returning 0");
#endif
			return;
		}

		elementPtr = (ExpReturn*)&(elementPtr->GetArray()[index]);
	}

	// Array element is another array?  Just pass on the reference
	if (elementPtr->Type() == EXPTYPE_ARRAY) {
		er->eType = EXPTYPE_ARRAY;
		er->eData.arr = elementPtr->eData.arr;
		return;
	}
	// Array element not another array: copy to temp value (arrays are reference semantics)
	else {
		// operator= copy to ExpStore, then operator= reference to ExpReturn
		value = *elementPtr;
		*er = (ExpReturn&)value;	// force reference copy
		return;
	}
}

bool ExpAt::Constant()
{
	return DoBinaryCollapse(l, r);
}


/////////////
// ExpDot
DEFINE_STD_GETOPERANDS(ExpDot);

void ExpDot::Evaluate(ExpReturn* er)
{
	// Next exp stack depth
	//pRuntime->curExpStackDepth++;
	//pRuntime->ExpReturnStack[pRuntime->curExpStackDepth] = er;

	// Parameters exist?  Evaluate them
	if (hasParameters) {

		ExpReturn* expPtr = expParamList;
		ExpPart** curParam = parameters;

		// Evaluate every parameter
		for ( ; curParam != paramsEnd; curParam++)

			// Send to address of corresponding expParamList array entry
			(*curParam)->Evaluate(expPtr++);
	}

	CRunObject* objToCall;

	// Undefined expression call?
	if (unnamedExp) {

		if (this->oid == pCExp->oidCaller)
			objToCall = pCExp->pSelectedObject;
		else {
			SelObjList& rSol = *(pType->pSol);
			if (rSol.selectAll)
				objToCall = pTypeInstances->operator []((pCExp->SolID) % pTypeInstances->size());
			else
				objToCall = rSol.sol[(pCExp->SolID) % rSol.sol.size()];
		}

		objToCall->ReturnUndefinedExpression(fname, expParamList, *er);

		// Decrement the stack depth (call complete)
		//pRuntime->curExpStackDepth--;
		return;
	}

	if (isCallerOid || pCExp->overrideCalledOID) {

		// Call the owner's expression routine
		objToCall = pCExp->pSelectedObject;

		// Still NULL?
		if (objToCall == NULL) {
			if (pType->pSol->selectAll) {

				// No instances
				if (pType->instances.empty()) {
					er->eType = EXPTYPE_INTEGER;
					er->eData.iVal = 0;
					
					// Give a debug warning
#ifdef CONSTRUCT_DEBUGGER
					CString msg;
					msg.Format("Expression '%s': no '%s' objects exist: returning 0", fname, pRuntime->objects[oid]->Name);
					pRuntime->LogMsg(msg);
#endif
					return;
				}

				objToCall = pType->instances.front();
			}
			else if (!(pType->pSol->sol.empty()))
				objToCall = pType->pSol->sol.front();
		}

	}
	else {

		SelObjList& rSol = *(pType->pSol);

		// Call the routine for the paired instance
		if (rSol.selectAll) {

			if (pTypeInstances->size() > 0)

				// SOL index refers to instance list if all selected
				objToCall = pTypeInstances->operator []((pCExp->SolID) % pTypeInstances->size());

			// Zero objects and asking for an expression!  Ouch!  Return 0 for now
			else {
				er->eType = EXPTYPE_INTEGER;
				er->eData.iVal = 0;
				
					// Give a debug warning
#ifdef CONSTRUCT_DEBUGGER
				CString msg;
				msg.Format("Expression '%s': no '%s' objects exist: returning 0", fname, pRuntime->objects[oid]->Name);
				pRuntime->LogMsg(msg);
#endif
				return;
			}
		}

		else

			// Use index to the SOL for pairing
			objToCall = rSol.sol[(pCExp->SolID) % rSol.sol.size()];
	
	}//else

	D_ENTER(DCS_EXPRESSION, objToCall);

	// Override the object type in case a family expression
	// (causes bugs... best leave out)
	//CRunObjType* oldType = objToCall->pType;
	//objToCall->pType = this->pType;

	// Do call
	(objToCall->*expRoutine)(expParamList, *er);

	// Restore type
	//objToCall->pType = oldType;

	D_RUNTIME();

	// Decrement the stack depth (call complete)
	//pRuntime->curExpStackDepth--;
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
	ExpPart** curParam = parameters;

	for ( ; curParam != paramsEnd; curParam++)
		delete *curParam;
}

bool ExpDot::Constant()
{
	// Const collapse all parameters
	ExpPart** curParam = parameters;

	if(hasParameters)
	{
		bool allParamsConstant = true;


		for ( ; curParam != paramsEnd; curParam++) {

			// This parameter is constant
			if ((*curParam)->Constant())
				CollapseConstantTree(*curParam);	// Collapse it
			else
				allParamsConstant = false;
		}
	

		// All parameters are constant: evaluate the parameters
		if (allParamsConstant) {

			ExpReturn* expPtr = expParamList;
			ExpPart** curParam = parameters;

			// Evaluate every parameter
			for ( ; curParam != paramsEnd; curParam++)

				// Send to address of corresponding expParamList array entry
				(*curParam)->Evaluate(expPtr++);

			// Switch off parameter evaluation since all parameters are constant and we've evaluated them already.
			hasParameters = false;
		}
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
			er->eData.iVal = (er->eData.iVal == param.eData.iVal);
			return;
		case EXPTYPE_FLOAT:
			er->eData.iVal = (er->eData.iVal == param.eData.fVal);
			return;
		}//switch

		break;

	case EXPTYPE_FLOAT:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			er->eType = EXPTYPE_INTEGER;
			er->eData.iVal = (er->eData.fVal == param.eData.iVal);
			return;
		case EXPTYPE_FLOAT:
			er->eType = EXPTYPE_INTEGER;
			er->eData.iVal = (er->eData.fVal == param.eData.fVal);
			return;
		}//switch

		break;

	case EXPTYPE_STRING:

		switch (param.eType) {
		case EXPTYPE_STRING:
			er->eType = EXPTYPE_INTEGER;
			er->eData.iVal = (*(er->eData.str) == *(param.eData.str));
			return;
		}

		break;

	}//switch

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH2(er, &param, "test equality");
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
			er->eData.iVal = (er->eData.iVal != param.eData.iVal);
			return;
		case EXPTYPE_FLOAT:
			er->eData.iVal = (er->eData.iVal != param.eData.fVal);
			return;
		}//switch

		break;

	case EXPTYPE_FLOAT:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			er->eType = EXPTYPE_INTEGER;
			er->eData.iVal = (er->eData.fVal != param.eData.iVal);
			return;
		case EXPTYPE_FLOAT:
			er->eType = EXPTYPE_INTEGER;
			er->eData.iVal = (er->eData.fVal != param.eData.fVal);
			return;
		}//switch

		break;

	case EXPTYPE_STRING:

		switch (param.eType) {
		case EXPTYPE_STRING:
			er->eType = EXPTYPE_INTEGER;
			er->eData.iVal = (*(er->eData.str) != *(param.eData.str));
			return;
		}

		break;

	}//switch

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH2(er, &param, "test inequality");
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
			er->eData.iVal = (er->eData.iVal < param.eData.iVal);
			return;
		case EXPTYPE_FLOAT:
			er->eData.iVal = (er->eData.iVal < param.eData.fVal);
			return;
		}//switch

		break;

	case EXPTYPE_FLOAT:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			er->eType = EXPTYPE_INTEGER;
			er->eData.iVal = (er->eData.fVal < param.eData.iVal);
			return;
		case EXPTYPE_FLOAT:
			er->eType = EXPTYPE_INTEGER;
			er->eData.iVal = (er->eData.fVal < param.eData.fVal);
			return;
		}//switch

		break;

	}//switch

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH2(er, &param, "compare");
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
			er->eData.iVal = (er->eData.iVal <= param.eData.iVal);
			return;
		case EXPTYPE_FLOAT:
			er->eData.iVal = (er->eData.iVal <= param.eData.fVal);
			return;
		}//switch

		break;

	case EXPTYPE_FLOAT:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			er->eType = EXPTYPE_INTEGER;
			er->eData.iVal = (er->eData.fVal <= param.eData.iVal);
			return;
		case EXPTYPE_FLOAT:
			er->eType = EXPTYPE_INTEGER;
			er->eData.iVal = (er->eData.fVal <= param.eData.fVal);
			return;
		}//switch

		break;

	}//switch

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH2(er, &param, "compare");
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
			er->eData.iVal = (er->eData.iVal > param.eData.iVal);
			return;
		case EXPTYPE_FLOAT:
			er->eData.iVal = (er->eData.iVal > param.eData.fVal);
			return;
		}//switch

		break;

	case EXPTYPE_FLOAT:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			er->eType = EXPTYPE_INTEGER;
			er->eData.iVal = (er->eData.fVal > param.eData.iVal);
			return;
		case EXPTYPE_FLOAT:
			er->eType = EXPTYPE_INTEGER;
			er->eData.iVal = (er->eData.fVal > param.eData.fVal);
			return;
		}//switch

		break;

	}//switch

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH2(er, &param, "compare");
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
			er->eData.iVal = (er->eData.iVal >= param.eData.iVal);
			return;
		case EXPTYPE_FLOAT:
			er->eData.iVal = (er->eData.iVal >= param.eData.fVal);
			return;
		}//switch

		break;

	case EXPTYPE_FLOAT:

		switch (param.eType) {
		case EXPTYPE_INTEGER:
			er->eType = EXPTYPE_INTEGER;
			er->eData.iVal = (er->eData.fVal >= param.eData.iVal);
			return;
		case EXPTYPE_FLOAT:
			er->eType = EXPTYPE_INTEGER;
			er->eData.iVal = (er->eData.fVal >= param.eData.fVal);
			return;
		}//switch

		break;

	}//switch

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH2(er, &param, "compare");
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
			er->eData.iVal = (er->eData.iVal && param.eData.iVal);
			return;
		case EXPTYPE_STRING: {
			er->eType = EXPTYPE_STRING;
			char strbuf[32];
			_i64toa(er->eData.iVal, strbuf, 10);
			temp = strbuf;
			temp += *(param.eData.str);
			er->eData.str = &temp;
			return;
			}
		}//switch

	case EXPTYPE_FLOAT:
		switch(param.eType) {
		case EXPTYPE_STRING:
			{
				er->eType = EXPTYPE_STRING;
				char strbuf[32];
				_gcvt(er->eData.fVal, DBL_DIG, strbuf);
				temp = strbuf;
				if (temp.Right(1) == ".")
					temp = temp.Left(temp.GetLength() - 1);
				temp += *(param.eData.str);
				er->eData.str = &temp;
				return;
			}
		}
	case EXPTYPE_STRING:
		switch(param.eType) {
		case EXPTYPE_INTEGER:
			{
				char strbuf[32];
				_i64toa(param.eData.iVal, strbuf, 10);
				*(er->eData.str) += strbuf;
				return;
			}
		case EXPTYPE_FLOAT:
			{
				char strbuf[32];
				_gcvt(param.eData.fVal, DBL_DIG, strbuf);
				temp = strbuf;
				if (temp.Right(1) == ".")
					temp = temp.Left(temp.GetLength() - 1);
				*(er->eData.str) += temp;
				return;
			}
		case EXPTYPE_STRING:
			{
				*(er->eData.str) += *(param.eData.str);
				return;
			}
		}
	}//switch

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH2(er, &param, "perform 'and'");
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
			er->eData.iVal = (er->eData.iVal || param.eData.iVal);
			return;
		}//switch

		break;

	}//switch

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH2(er, &param, "perform 'Or'");
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

	// If the condition was true...
	if (er->eType == EXPTYPE_INTEGER) {

		// Condition was true
		if (er->eData.iVal)
			// Evaluate as true part (b)
			b->Evaluate(er);

		// Condition was false
		else
			// Evaluate as false part (c)
			c->Evaluate(er);

	}
	// Not an integer! Can't test it
	// Types invalid: return without modifying er (!)
#ifdef CONSTRUCT_DEBUGGER
	pCExp->pRuntime->LogMsg("Cannot test conditional expression on a non-integer type.  Operator had no effect");
#endif
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
	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		er->eData.fVal = sin((double)DEG_TO_RAD(er->eData.iVal));
		er->eType = EXPTYPE_FLOAT;
		return;
	case EXPTYPE_FLOAT:
		er->eData.fVal = sin((double)DEG_TO_RAD(er->eData.fVal));
		return;
	}

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH1(er, "sin");
}

DEFINE_STD_UNARY(ExpSin)

/////////////
// ExpCos
void ExpCos::Evaluate(ExpReturn* er)
{
	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		er->eData.fVal = cos((double)DEG_TO_RAD(er->eData.iVal));
		er->eType = EXPTYPE_FLOAT;
		return;
	case EXPTYPE_FLOAT:
		er->eData.fVal = cos((double)DEG_TO_RAD(er->eData.fVal));
		return;
	}

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH1(er, "cos");
}

DEFINE_STD_UNARY(ExpCos)

/////////////
// ExpTan
void ExpTan::Evaluate(ExpReturn* er)
{
	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		er->eData.fVal = tan(DEG_TO_RAD(er->eData.iVal));
		er->eType = EXPTYPE_FLOAT;
		return;
	case EXPTYPE_FLOAT:
		er->eData.fVal = tan(DEG_TO_RAD(er->eData.fVal));
		return;
	}

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH1(er, "tan");
}

DEFINE_STD_UNARY(ExpTan)

/////////////
// ExpSqrt
void ExpSqrt::Evaluate(ExpReturn* er)
{
	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		er->eType = EXPTYPE_FLOAT;
		er->eData.fVal = sqrt((double)er->eData.iVal);
		return;
	case EXPTYPE_FLOAT:

		er->eData.fVal = sqrt(er->eData.fVal);
		return;

	}

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH1(er, "sqrt");
}

DEFINE_STD_UNARY(ExpSqrt)

#define CSTR_BUF(a) ((const char*)(a))

/////////////
// ExpFuncInt
void ExpFuncInt::Evaluate(ExpReturn* er)
{
	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		return;	// Int() does not affect integers
	case EXPTYPE_FLOAT:
		er->eData.iVal = (__int64)er->eData.fVal;
		er->eType = EXPTYPE_INTEGER;
		return;
	case EXPTYPE_STRING:
		er->eData.iVal = _atoi64(*(er->eData.str));
		er->eType = EXPTYPE_INTEGER;
		return;
	}

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH1(er, "'int' cast");
}

DEFINE_STD_UNARY(ExpFuncInt)

/////////////
// ExpFuncFloat
void ExpFuncFloat::Evaluate(ExpReturn* er)
{
	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		er->eData.fVal = (double)er->eData.iVal;
		er->eType = EXPTYPE_FLOAT;
		return;
	case EXPTYPE_FLOAT:
		return;
	case EXPTYPE_STRING:
		er->eData.fVal = atof(*(er->eData.str));
		er->eType = EXPTYPE_FLOAT;
		return;
	}

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH1(er, "'float' cast");
}

DEFINE_STD_UNARY(ExpFuncFloat)

/////////////
// ExpFuncStr
void ExpFuncStr::Evaluate(ExpReturn* er)
{
	p->Evaluate(er);

	char strbuf[32];

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		_i64toa(er->eData.iVal, strbuf, 10);
		temp = strbuf;
		er->eData.str = &temp;
		er->eType = EXPTYPE_STRING;
		return;
	case EXPTYPE_FLOAT:
		_gcvt(er->eData.fVal, DBL_DIG, strbuf);
		temp = strbuf;
		if (temp.Right(1) == ".")
			temp.Delete(temp.GetLength() - 1);
		er->eData.str = &temp;
		er->eType = EXPTYPE_STRING;
		return;
	case EXPTYPE_STRING:
		return;
	}

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH1(er, "'string' cast");
}

DEFINE_STD_UNARY(ExpFuncStr)

/////////////
// ExpAsin
void ExpAsin::Evaluate(ExpReturn* er)
{
	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		er->eData.fVal = RAD_TO_DEG(asin((double)er->eData.iVal));
		er->eType = EXPTYPE_FLOAT;
		return;
	case EXPTYPE_FLOAT:
		er->eData.fVal = RAD_TO_DEG(asin((double)er->eData.fVal)) ;
		return;
	}

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH1(er, "asin");
}

DEFINE_STD_UNARY(ExpAsin)

/////////////
// ExpAcos
void ExpAcos::Evaluate(ExpReturn* er)
{
	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		er->eData.fVal = RAD_TO_DEG(acos((double)er->eData.iVal));
		er->eType = EXPTYPE_FLOAT;
		return;
	case EXPTYPE_FLOAT:
		er->eData.fVal = RAD_TO_DEG(acos((double)er->eData.fVal)) ;
		return;
	}

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH1(er, "acos");
}

DEFINE_STD_UNARY(ExpAcos)

/////////////
// ExpAtan
void ExpAtan::Evaluate(ExpReturn* er)
{
	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		er->eData.fVal = RAD_TO_DEG(atan((double)er->eData.iVal));
		er->eType = EXPTYPE_FLOAT;
		return;
	case EXPTYPE_FLOAT:
		er->eData.fVal = RAD_TO_DEG(atan((double)er->eData.fVal)) ;
		return;
	}

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH1(er, "atan");
}

DEFINE_STD_UNARY(ExpAtan)

/////////////
// ExpAbs
void ExpAbs::Evaluate(ExpReturn* er)
{
	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		er->eData.iVal = er->eData.iVal >= 0 ? er->eData.iVal : 0 - er->eData.iVal;
		return;
	case EXPTYPE_FLOAT:
		er->eData.fVal = er->eData.fVal >= 0 ? er->eData.fVal : 0 - er->eData.fVal;
		return;
	}

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH1(er, "abs");
}

DEFINE_STD_UNARY(ExpAbs)

/////////////
// ExpExp (exponential, e^p)
void ExpExp::Evaluate(ExpReturn* er)
{
	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		er->eData.fVal = exp((double)er->eData.iVal);
		er->eType = EXPTYPE_FLOAT;
		return;
	case EXPTYPE_FLOAT:
		er->eData.fVal = exp((double)er->eData.fVal);
		return;
	}

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH1(er, "exp");
}

DEFINE_STD_UNARY(ExpExp)

/////////////
// ExpLn
void ExpLn::Evaluate(ExpReturn* er)
{
	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		er->eData.fVal = log((double)er->eData.iVal);
		er->eType = EXPTYPE_FLOAT;
		return;
	case EXPTYPE_FLOAT:
		er->eData.fVal = log((double)er->eData.fVal);
		return;
	}

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH1(er, "ln");
}

DEFINE_STD_UNARY(ExpLn)

/////////////
// ExpLog10
void ExpLog10::Evaluate(ExpReturn* er)
{
	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		er->eData.fVal = log10((double)er->eData.iVal);
		er->eType = EXPTYPE_FLOAT;
		return;
	case EXPTYPE_FLOAT:
		er->eData.fVal = log10((double)er->eData.fVal);
		return;
	}

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH1(er, "log10");
}

DEFINE_STD_UNARY(ExpLog10)

/////////////
// ExpCeil
void ExpCeil::Evaluate(ExpReturn* er)
{
	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		return; // no effect on integer
	case EXPTYPE_FLOAT:
		er->eData.iVal = er->eData.fVal + 1.0;
		er->eType = EXPTYPE_INTEGER;
		return;
	}

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH1(er, "ceil");
}

DEFINE_STD_UNARY(ExpCeil)

/////////////
// ExpRound
void ExpRound::Evaluate(ExpReturn* er)
{
	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		return; // no effect on integer
	case EXPTYPE_FLOAT:
		if (er->eData.fVal >= 0.5)
			er->eData.iVal = er->eData.fVal + 0.5;
		else if (er->eData.fVal <= -0.5)
			er->eData.iVal = er->eData.fVal - 0.5;
		else
			er->eData.iVal = 0;
		er->eType = EXPTYPE_INTEGER;
		return;
	}

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH1(er, "round");
}

DEFINE_STD_UNARY(ExpRound)

/////////////
// ExpRandom
void ExpRandom::Evaluate(ExpReturn* er)
{
	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_INTEGER:
		er->eData.iVal = pCExp->pRuntime->mtRand53() * (double)er->eData.iVal;
		return;
	case EXPTYPE_FLOAT:
		er->eData.fVal = pCExp->pRuntime->mtRand53() * er->eData.fVal;
		return;
	}

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH1(er, "randomisation");
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
	p->Evaluate(er);

	switch (er->eType) {
	case EXPTYPE_STRING:
		er->eData.iVal = er->eData.str->GetLength();
		er->eType = EXPTYPE_INTEGER;
		return;
	case EXPTYPE_ARRAY:
		er->eData.iVal = er->eData.arr.size;
		er->eType = EXPTYPE_INTEGER;
		return;
	}

	// Types invalid: return without modifying er (!)
	TYPEMISMATCH1(er, "len");
}

DEFINE_STD_UNARY(ExpLen)