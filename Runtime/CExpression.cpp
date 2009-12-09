// CExpression
// Class to handle an expression
// By Tigerworks

#include "StdAfx.h"

///////////////////////////////////
// CExpression functions

void CExpression::NotifyError(const char* msg)
{
	throw runtime_error(msg);
}

// Hackhack - static pointer - will only work with variable name types as first param for now
static CRunObjType* pExpressionOwner = NULL;

void CExpression::ParseTokenStream()
{
	// Make the tree
	int step = 0;
	pExpressionOwner = NULL;
	ExpTree = DoTree(step);
	ExpTree = PostSortTree(ExpTree);

	// Determine if the overall expression is constant, collapsing constant parts along
	// the way.
	isConstant = ExpTree->Constant();

	// If constant, get the constant value.
	if (isConstant) {
		CollapseConstantTree(ExpTree);
		ExpTree->Evaluate(&constantValue);
	}
}


/*
enum ExpToken {
	T_NULL,
	T_ANY_BINARY_OPERATOR,
	T_ANY_FUNCTION,
	T_ANY_VALUE,

	T_INTEGER,		T_FLOAT,		T_STRINGLITERAL,	T_IDENTIFIER,
	T_LEFTBRACKET,	T_RIGHTBRACKET,	T_COMMA,			T_DOT,
	T_ADD,			T_SUBTRACT,		T_MULTIPLY,			T_DIVIDE,		T_MOD,
	T_POWER,
	T_SIN,			T_COS,			T_TAN,				T_SQRT,
};
  */

CExpression::CExpression()
{
	ExpTree = NULL;
	Depth = 0;
	SolID = 0;
	pSelectedObject = NULL;
	// Constant until proven variable
	isConstant = true;
	constantValue.eType = EXPTYPE_NULL;
	overrideCalledOID = false;
}

// Evaluate expression
/*
void CExpression::Evaluate(ExpReturn* eReturn, CRunObject* const selObj)
{

	pSelectedObject = selObj;

	// Evaluate variable expression
	if (ExpTree != NULL)
		ExpTree->Evaluate(eReturn);

}
*/

int OpPrecedence(ExpPart* Part)
{
	switch (Part->t) {
	case T_CONDITIONAL:
		return 0;
	case T_AND:
		return 1;
	case T_OR:
		return 2;
	case T_EQUAL: case T_LESS: case T_LESSEQUAL: case T_GREATER: case T_GREATEREQUAL:
	case T_NOTEQUAL:
		return 3;
	case T_ADD:
		return 4;
	case T_SUBTRACT:
		return 5;	// Subtraction before addition
	case T_MULTIPLY:
		return 6;
	case T_MOD:
		return 7;
	case T_DIVIDE: 
		return 8;
	case T_POWER:
		return 9;
	case T_SIN: case T_COS: case T_TAN: case T_SQRT:
	case T_FUNC_INT: case T_FUNC_FLOAT: case T_FUNC_STR:
		return 10;
	case T_AT:
		return 11;
	case T_DOT:
		return 12;
	default:
		return 13;
	}
}

// Operator precedences
bool HasPrecedence(ExpPart* l, ExpPart* r)
{
//	if (OpPrecedence(r) == 11 || OpPrecedence(l) == 11)
//		return false;
	// R is in greater bracket depth:  L has no precedence
	if (l->bracketdepth < r->bracketdepth)
		return false; // work this out later

	if (OpPrecedence(l) > OpPrecedence(r))
		return true;


	return false;
}

bool SamePrecedence(ExpPart* l, ExpPart* r)
{
//
	// Non-operators must not be switched
	if (OpPrecedence(r) == 13)
		return false;
	else
		return (OpPrecedence(l) == OpPrecedence(r)
		&& (l->bracketdepth == r->bracketdepth));
}

ExpPart* SwapTree(ExpPart* parent)
{
	return parent;

	// DAVO: I'm too lazy to remove all the calls to this...so I made it have no effect
	ExpPart* newParent = parent->GetPRightOperand();

	// Child's left operand becomes parent's right operand
	parent->GetPRightOperand() = parent->GetPRightOperand()->GetPLeftOperand();

	// Child becomes parent (pAdd becomes child's left operand)
	newParent->GetPLeftOperand() = parent;

	return newParent;
}

ExpPart* DoTreeSwap(ExpPart* parent)
{
	// Identical to the above
	ExpPart* newParent = parent->GetPRightOperand();

	// Child's left operand becomes parent's right operand
	parent->GetPRightOperand() = parent->GetPRightOperand()->GetPLeftOperand();

	// Child becomes parent (pAdd becomes child's left operand)
	newParent->GetPLeftOperand() = parent;

	return newParent;
}


ExpPart* PostSortTree(ExpPart* parent)
{
	// If conditional statement don't postsort
	if (dynamic_cast<ExpConditional*>(parent) != NULL)
		return parent;

	ExpPart*& parentL = parent->GetPLeftOperand();
	ExpPart*& parentR = parent->GetPRightOperand();

	if(parentL && parentR) 
	{
		ExpPart* temp = parentR;

		if(SamePrecedence(parent, parentR)
			|| HasPrecedence(parent, parentR))
		{
			ExpPart* theParent = DoTreeSwap(parent);

			// Now we ask our new right child if it wants to swap childrens childrens if they want to swap.
			parentR = PostSortTree(parentR);
			
			// And now our new parent
			return PostSortTree(theParent);
		}
		// Okay nothing swapped...ask our left and right
		else
		{
			// left wont grow
			parentL = PostSortTree(parentL);

			// right may grow!
			ExpPart* oldright = parentR;
			parentR = PostSortTree(parentR);
			
			if(oldright != parentR)				
				return PostSortTree(parent);
			
			return parent;
		}
	}
	// After swapping  children,
	return parent;
}

/*
ExpPart* PostSortTree(ExpPart* parent)
{
for(int x = 0; x < 10; x ++)
PostSortTree2(parent);

return PostSortTree2(parent);
/*	if(parent->donePost)
		return parent;
	else
		parent->donePost = true;*/

//}


// For divide operator etc: convert right-to-left evaluation to left-to-right
void SwapRightToLeft(ExpPart* parent)
{
	return;
	//DAVO: Also too lazy :P
	ExpPart* parentLeft = parent->GetPLeftOperand();
	ExpPart* parentRight = parent->GetPRightOperand();

	ExpPart* rhsRight = parentRight->GetPRightOperand();

	// Now switch right part for left part and adjust numbers for operator order to work left-to-right
	parentRight->GetPRightOperand() = parentRight->GetPLeftOperand();
	parentRight->GetPLeftOperand() = parentLeft;
	parent->GetPLeftOperand() = parentRight;
	parent->GetPRightOperand() = rhsRight;
}

#define CSTR_BUF(a) ((const char*)(a))

// Convert an identifier to either its OID or system code
void CExpression::LookupIdentifier(CString ident, ExpIdent* part)
{
	ident.MakeLower();
	CString movementName;

	int f = ident.Find('[');

	// Search for and parse movement sub-names, ie. Sprite[Racecar]
	if (f != -1) {
		movementName = ident;
		ident = ident.Left(f);
		movementName.Delete(0, f+1);
		movementName.Delete(movementName.GetLength() - 1);
		movementName.MakeLower();

		//MessageBox(NULL, CString("Parsed movement exp ") + ident + "[" + movement + "]", NULL, NULL);
	}

	// Look through all object types to determine the OID
	vector<CRunObjType*>::iterator i;

	for (i = pRuntime->objects.begin(); i != pRuntime->objects.end(); i++) {

		CString objName = (*i)->Name;
		objName.MakeLower();

		// Identifier match?
		if (ident == objName) {
			// Get OID and quit
			part->oid = (*i)->oid;

			// OID needs to be movement ID
			if (f != -1) {
				CRunObjType* pType = pRuntime->objects[(*i)->oid];

				// Find sister type which is a movement and matches the movement name
				ObjTypeIterator t = pType->sisterTypes.begin();
				ObjTypeIterator sis_end = pType->sisterTypes.end();

				// Match named movement sister
				for ( ; t != sis_end; t++) {

					CString AuxName = (*t)->AuxName;
					AuxName.MakeLower();

					if ((*t)->movement && AuxName == movementName) {
						part->oid = (*t)->oid;
						return;
					}
				}

				// Can't find - try familyMovs instead
				t = pType->familyMovs.begin();
				sis_end = pType->familyMovs.end();

				for ( ; t != sis_end; t++) {

					CString AuxName = (*t)->AuxName;
					AuxName.MakeLower();

					if ((*t)->movement && AuxName == movementName) {
						part->oid = (*t)->oid;
						return;
					}
				}

				// Not found
				throw runtime_error((const char*)(CString("Behavior name '") + movementName + "' not found!"));

			}//if movement

			return;
		}//name match
	}// for each obj type

	// No match?  Look up system expressions
	part->oid = -1;

	SystemObject::GetExpRoutine(ident, part);
}

// Lookup a function name
void CExpression::LookupPluginExpression(int oid, CString fname, ExpDot* pDot)
{
	CRunObjType* pType = pRuntime->pOid(oid);

	pDot->fname = fname;

	fname.MakeLower();

	vector<CString>* pvNames = pType->pPlugin->pvExpNames;

	// Check against all expression names
	for (int i = 0; i < pvNames->size(); i++) {

		// Name matches
		CString n = pvNames->at(i);
		if (n == fname) {

			// Get corresponding expression routine
			pDot->expRoutine = pType->pPlugin->pvExpRoutines->at(i);
			return;
		}
	}

	// Not found: calling unnamed expression routine
	pDot->unnamedExp = true;

	/*
	// Not found; error
	CString msg = "Error:  expression \"" + fname + "\" not found in object \"";
	msg += pType->Name;
	msg += "\"";
	throw runtime_error((const char*)msg);
	*/
}

// standard routine shortcuts
void doUnaryExpPart(CExpression* pExp, int& step, ExpPart*& LeftPart, ExpPart* ptr)
{
	ptr->t = pExp->toks[step].t;
	ptr->pCExp = pExp;
	ptr->bracketdepth = pExp->Depth;
	LeftPart = ptr;
	step++;
}

void doBinaryExpPart(CExpression* pExp, int& step, ExpPart*& LeftPart, ExpPart* ptr)
{
	ptr->t = pExp->toks[step].t;
	ptr->pCExp = pExp;
	ptr->bracketdepth = pExp->Depth;
	ptr->GetPLeftOperand() = LeftPart;
	step++;

//	if (pExp->toks[step].t == T_LEFTBRACKET)
//		ptr->GetPRightOperand() = pExp->DoBrackets(step);
///	else
		ptr->GetPRightOperand() = pExp->DoTree(step);
}

// NEW_UNARY_EXP(Type): returns a new Type and handles common stuff like incrementing step
#define NEW_UNARY_EXP(a, b) new a; doUnaryExpPart(this, step, LeftPart, b)
#define NEW_UNARY_OPERATOR(a, b) new a; doUnaryExpPart(this, step, LeftPart, b); b->p = DoTree(step, true); b->p = PostSortTree(b->p);

// NEW_BINARY_EXP(Type): returns a new Type and handles common stuff for binary operators
#define NEW_BINARY_EXP(a, b) new a; doBinaryExpPart(this, step, LeftPart, b);

bool ShouldTerminate(int step, const vector<Token>& toks)
{
	if (step < toks.size()) {

		ExpToken t = toks[step].t;

		switch(t) {
			case T_RIGHTBRACKET:
			case T_COMMA:
			case T_COLON:
			case T_RIGHTCURLY:
				return true;
			default:
				return false;
		}
	}
	// No more tokens left: definitely terminate
	else
		return true;
}

// CHECK_TERMINATE(): if a right bracket or comma encountered, quit this recursion branch
#define CHECK_TERMINATE() if (unaryMode || ShouldTerminate(step, toks)) return LeftPart;

// Do Tree:  convert token stream to expression tree
ExpPart* CExpression::DoTree(int& step, bool unaryMode)
{
	ExpPart* LeftPart = NULL;
	ExpPart* Brackets = NULL;
	bool Bracketing; // Brackets override the operator precedentation

	// Too lazy to scope each case label lol
	ExpInteger* pInt;
	ExpFloat* pFloat;
	ExpString* pStr;
	ExpIdent* pIdent;
	ExpAdd* pAdd;
	ExpSubtract* pSub;
	ExpMultiply* pMult;
	ExpDivide* pDiv;
	ExpMod* pMod;
	ExpPower* pPow;
	ExpDot* pDot;
	ExpSin* pSin;
	ExpCos* pCos;
	ExpTan* pTan;
	ExpSqrt* pSqrt;
	ExpFuncInt* pFuncInt;
	ExpFuncFloat* pFuncFloat;
	ExpFuncStr* pFuncStr;
	ExpEqual* pEqual;
	ExpNotEqual* pNotEqual;
	ExpLess* pLess;
	ExpLessEqual* pLessEqual;
	ExpGreater* pGreater;
	ExpGreaterEqual* pGreaterEqual;
	ExpAnd* pAnd;
	ExpOr* pOr;
	ExpConditional* pConditional;
	ExpAsin* pAsin;
	ExpAcos* pAcos;
	ExpAtan* pAtan;
	ExpAbs* pAbs;
	ExpExp* pExp;
	ExpLn* pLn;
	ExpLog10* pLog10;
	ExpCeil* pCeil;
	ExpRound* pRound;
	ExpRandom* pRandom;
	ExpLen* pLen;
	ExpArray* pArray;
	ExpAt* pAt;
	ExpVariableName* pVariableName;
	bool dotIdentifier;

	while (step < toks.size()) {

		const Token& curTok = toks[step];

		if (curTok.t == T_NULL)
			break;

		Bracketing = false;

		switch (curTok.t) {

			//////////////////////
			// Operands
			//
			// Operands are simply kept to one side until we find an
			// operator to do something with.

		case T_INTEGER:
			
			pInt = NEW_UNARY_EXP(ExpInteger, pInt);
			pInt->value = curTok.iValue;

			// Return on close bracket or comma
			CHECK_TERMINATE();

			break;

		case T_FLOAT:

			pFloat = NEW_UNARY_EXP(ExpFloat, pFloat);
			pFloat->value = curTok.fValue;

			// Return on close bracket or comma
			CHECK_TERMINATE();

			break;

		case T_STRINGLITERAL:

			pStr = NEW_UNARY_EXP(ExpString, pStr);
			pStr->value = curTok.str;

			// Return on close bracket or comma
			CHECK_TERMINATE();

			break;

		case T_VARIABLENAME:
			{

			pVariableName = NEW_UNARY_EXP(ExpVariableName, pVariableName);

			CString name = curTok.str;
			name.MakeLower();

			// Lookup the private variable index
			if (pExpressionOwner != NULL)
				pVariableName->index = find_index(pExpressionOwner->privateVars.begin(), pExpressionOwner->privateVars.end(), name);
			else {
				vector<CString>::iterator it = pRuntime->globalNames.begin();
				vector<CString>::iterator end = pRuntime->globalNames.end();

				for ( ; it != end; it++) {
					CString curName = *it;
					curName.MakeLower();

					if (curName == name) {
						pVariableName->index = it - pRuntime->globalNames.begin();
						break;
					}
				}
			}

			pVariableName->pOwnerType = pExpressionOwner;
			
			// Return on close bracket or comma
			CHECK_TERMINATE();

			break;
			}

		case T_LEFTCURLY:

			pArray = NEW_UNARY_EXP(ExpArray, pArray);

			// NEW_UNARY_EXP increments step, but we want to put it back, because the loop
			// below expects to start on {
			step--;

			// Loop until all elements collected
			while (toks[step].t != T_RIGHTCURLY && step < toks.size()) {

				step++;		// Step over { or ,

				// Obtain element
				ExpPart* element = DoTree(step);

				// Add to elements
				pArray->expElements.push_back(element);

			}

			// Step over }
			step++;

			// Resize the evaluation buffer
			pArray->arr.resize(pArray->expElements.size());

			CHECK_TERMINATE();

			break;


			//////////////////////
			// Operators

		case T_ADD:
			
			pAdd = NEW_BINARY_EXP(ExpAdd, pAdd);

			// If child operator is lower precedence, this node must swap places with the child
			if (HasPrecedence(pAdd, pAdd->r))
				return SwapTree(pAdd);

			return pAdd;

		case T_SUBTRACT:

			pSub = NEW_BINARY_EXP(ExpSubtract, pSub);

			if (HasPrecedence(pSub, pSub->r))
				return SwapTree(pSub);

			return pSub;

		case T_MULTIPLY:

			pMult = NEW_BINARY_EXP(ExpMultiply, pMult);

			if (HasPrecedence(pMult, pMult->r))
				return SwapTree(pMult);

			return pMult;

		case T_DIVIDE:

			pDiv = NEW_BINARY_EXP(ExpDivide, pDiv);

			if (HasPrecedence(pDiv, pDiv->r) || SamePrecedence(pDiv, pDiv->r))
				return SwapTree(pDiv);
			
			// Force left-to-right evaluation
			/*
			if (SamePrecedence(pDiv, pDiv->r))
				SwapRightToLeft(pDiv);
			*/

			return pDiv;

		case T_MOD:

			pMod = NEW_BINARY_EXP(ExpMod, pMod);

			if (HasPrecedence(pMod, pMod->r))
				return SwapTree(pMod);

			// Force left-to-right evaluation
			if (SamePrecedence(pMod, pMod->r))
				SwapRightToLeft(pMod);

			return pMod;

		case T_POWER:

			pPow = NEW_BINARY_EXP(ExpPower, pPow);

			if (HasPrecedence(pPow, pPow->r))
				return SwapTree(pPow);

			// Force left-to-right evaluation
			if (SamePrecedence(pPow, pPow->r))
				SwapRightToLeft(pPow);

			return pPow;

		case T_AT:

			pAt = NEW_BINARY_EXP(ExpAt, pAt);

			if (HasPrecedence(pAt, pAt->r))
				return SwapTree(pAt);

			// Force left-to-right evaluation
			if (SamePrecedence(pAt, pAt->r))
				SwapRightToLeft(pAt);

			return pAt;

		case T_EQUAL:

			pEqual = NEW_BINARY_EXP(ExpEqual, pEqual);

			if (HasPrecedence(pEqual, pEqual->r))
				return SwapTree(pEqual);

			return pEqual;

		case T_NOTEQUAL:

			pNotEqual = NEW_BINARY_EXP(ExpNotEqual, pNotEqual);

			if (HasPrecedence(pNotEqual, pNotEqual->r))
				return SwapTree(pNotEqual);

			return pNotEqual;

		case T_LESS:

			pLess = NEW_BINARY_EXP(ExpLess, pLess);

			if (HasPrecedence(pLess, pLess->r))
				return SwapTree(pLess);

			return pLess;

		case T_LESSEQUAL:

			pLessEqual = NEW_BINARY_EXP(ExpLessEqual, pLessEqual);

			if (HasPrecedence(pLessEqual, pLessEqual->r))
				return SwapTree(pLessEqual);

			return pLessEqual;

		case T_GREATER:

			pGreater = NEW_BINARY_EXP(ExpGreater, pGreater);

			if (HasPrecedence(pGreater, pGreater->r))
				return SwapTree(pGreater);

			return pGreater;

		case T_GREATEREQUAL:

			pGreaterEqual = NEW_BINARY_EXP(ExpGreaterEqual, pGreaterEqual);

			if (HasPrecedence(pGreaterEqual, pGreaterEqual->r))
				return SwapTree(pGreaterEqual);

			return pGreaterEqual;

		case T_AND:

			pAnd = NEW_BINARY_EXP(ExpAnd, pAnd);

			if (HasPrecedence(pAnd, pAnd->r))
				return SwapTree(pAnd);

			return pAnd;

		case T_OR:

			pOr = NEW_BINARY_EXP(ExpOr, pOr);

			if (HasPrecedence(pOr, pOr->r))
				return SwapTree(pOr);

			return pOr;

		case T_CONDITIONAL:

			// a ? b : c
			pConditional = new ExpConditional;
			pConditional->t = T_CONDITIONAL;
			pConditional->pCExp = this;
			pConditional->bracketdepth = Depth;
			pConditional->a = LeftPart;	// Get condition part (a)
			step++;

			// Get TRUE part (b)
			/*if (toks[step].t == T_LEFTBRACKET)
				pConditional->b = DoBrackets(step);
			else*/
				pConditional->b = DoTree(step);
				pConditional->b = PostSortTree(pConditional->b);

			// Step over colon
			step++;

			// Get FALSE part (c)
			/*if (toks[step].t == T_LEFTBRACKET)
				pConditional->c = DoBrackets(step);
			else*/
				pConditional->c = DoTree(step);
				pConditional->c = PostSortTree(pConditional->c);

			// (never has precedence)

			return pConditional;

			// Identifier, eg. call without a dot: Object Object(5) MouseX
		case T_IDENTIFIER:
			{

			// Idents cause variability
			isConstant = false;

			pIdent = NEW_UNARY_EXP(ExpIdent, pIdent);
			pIdent->ident = curTok.str;
			pIdent->pRuntime = pRuntime;
			pIdent->pSystemObject = &(pRuntime->system);
			pIdent->numParams = 0;
			pIdent->paramsEnd = pIdent->parameters;

			for (int x = 0; x < CRUNTIME_MAX_PARAMS; x++)
				pIdent->expParamList[x].eType = EXPTYPE_NULL;

			// This will store the correct OID for this name, else -1 and load an expression routine
			LookupIdentifier(curTok.str, pIdent);

			// Check for OID caller match
			pIdent->isCallerOid = (pIdent->oid == this->oidCaller);

			// True if dot precedes this identifier (bit of a hack, but hey)
			// We can't check for unary mode, because unary operators use it and it would break "sin Array(1)"
			dotIdentifier = false;
			if (step > 1) if (toks[step-2].t == T_DOT) dotIdentifier = true;

			// Save expression owner for variable name translation.  Don't change the owner type if
			// this identifier is an expression name.
			if (!dotIdentifier) {
				if (pIdent->oid == -1)
					pExpressionOwner = NULL;
				else
					pExpressionOwner = pRuntime->objects[pIdent->oid];
			}

			// Cursor is currently on token after identifier (NEW_UNARY_EXP increments).
			// Check for left bracket & parameters if not part of a dot identifier (ie the x of object.x)
			if (step < toks.size() && !dotIdentifier) if (toks[step].t == T_LEFTBRACKET) {

				// While we're not on the closing bracket
				while (toks[step].t != T_RIGHTBRACKET && step < toks.size()) {

					// Step over left bracket/comma
					step++;

					// Obtain this parameter's tree, closing on the comma
					ExpPart* param = DoTree(step);
					param = PostSortTree(param);

					pIdent->parameters[pIdent->numParams++] = param;
					pIdent->paramsEnd = pIdent->parameters + pIdent->numParams;
				}

				// Step over right bracket
				step++;
			}

			// Now we have params parsed, but if this isn't a system expression, there isn't yet a routine stored for it.
			// This means it is the default return format, eg. Array(5).  Use the Default Return Value address.
			if (pIdent->oid > -1) {
				pIdent->pType = pRuntime->pOid(pIdent->oid);
				pIdent->pTypeInstances = &(pIdent->pType->instances);
				pIdent->routine = &CRunObject::ReturnDefaultValue;
			}
			else
				pIdent->pType = NULL;

			pIdent->hasParameters = pIdent->numParams > 0;

			// Return on close bracket or comma
			CHECK_TERMINATE();

			// Return if function call (followed by lbracket)
			if (toks[step].t == T_LEFTBRACKET)
				return LeftPart;

			break;
			}

		case T_DOT:

			// Plugin returns cause variability
			isConstant = false;

			pDot = new ExpDot;
			pDot->t = T_DOT;
			// Object name part
			pDot->l = (ExpIdent*)LeftPart;
			pDot->pCExp = this;
			pDot->pRuntime = pRuntime;
			pDot->bracketdepth = Depth;
			pDot->oid = ((ExpIdent*)(pDot->l))->oid;
			pDot->pType = pRuntime->objects[pDot->oid];
			pDot->pTypeInstances = &(pDot->pType->instances);
			pDot->unnamedExp = false;
			pDot->numParams = 0;
			pDot->paramsEnd = pDot->parameters;

			// Save owner type for variable name translation
			pExpressionOwner = pDot->pType;

			for (int x = 0; x < CRUNTIME_MAX_PARAMS; x++)
				pDot->expParamList[x].eType = EXPTYPE_NULL;

			step++;

			// Check for OID match
			pDot->isCallerOid = (pDot->oid == this->oidCaller);
				
			// Brackets not allowed after dot.  Obtain expression name part.
			pDot->r = (ExpIdent*)DoTree(step, true);

			// Now obtain a list of the parameters.  We are pointing after the expression
			// name part, i.e. after:   obj.exp [->] ( params ...
			// So if parameters exist, this is a left bracket.
			if (step < toks.size()) if (toks[step].t == T_LEFTBRACKET) {

				// While we're not on the closing bracket
				while (toks[step].t != T_RIGHTBRACKET && step < toks.size()) {

					// Step over left bracket/comma
					step++;

					// Obtain this parameter's tree, closing on the comma
					ExpPart* param = DoTree(step);
					param = PostSortTree(param);
					pDot->parameters[pDot->numParams++] = param;
					pDot->paramsEnd = pDot->parameters + pDot->numParams;
				}

				// Step over right bracket
				step++;
			}

			// Has parameters?
			pDot->hasParameters = pDot->numParams > 0;

			// Dot will have precedence over most operators
			if (HasPrecedence(pDot, pDot->r)) {

				ExpPart* newParent = SwapTree(pDot);

				// Look up the plugin expression routine
				LookupPluginExpression(((ExpIdent*)(pDot->l))->oid, ((ExpIdent*)(pDot->r))->ident, pDot);

				return newParent;
			}

			// Look up the plugin expression routine
			LookupPluginExpression(((ExpIdent*)(pDot->l))->oid, ((ExpIdent*)(pDot->r))->ident, pDot);

			//return pDot;
			LeftPart = pDot;

			CHECK_TERMINATE();

			break;

			//////////////////////
			// Unary operators

		case T_SIN:

			pSin = NEW_UNARY_OPERATOR(ExpSin, pSin);

			// Return on close bracket
			CHECK_TERMINATE();

			break;

		case T_COS:

			pCos = NEW_UNARY_OPERATOR(ExpCos, pCos);

			// Return on close bracket
			CHECK_TERMINATE();

			break;

		case T_TAN:

			pTan = NEW_UNARY_OPERATOR(ExpTan, pTan);

			// Return on close bracket
			CHECK_TERMINATE();

			break;

		case T_SQRT:

			pSqrt = NEW_UNARY_OPERATOR(ExpSqrt, pSqrt);

			// Return on close bracket
			/*
			if (step >= toks.size())
				return LeftPart;
			if (toks[step].t == T_RIGHTBRACKET)
				return LeftPart;
				*/
			CHECK_TERMINATE();

			break;

		case T_FUNC_INT:

			pFuncInt = NEW_UNARY_OPERATOR(ExpFuncInt, pFuncInt);

			// Return on close bracket
			CHECK_TERMINATE();

			break;

		case T_FUNC_FLOAT:

			pFuncFloat = NEW_UNARY_OPERATOR(ExpFuncFloat, pFuncFloat);

			// Return on close bracket
			CHECK_TERMINATE();

			break;

		case T_FUNC_STR:

			pFuncStr = NEW_UNARY_OPERATOR(ExpFuncStr, pFuncStr);

			// Return on close bracket
			CHECK_TERMINATE();

			break;

		case T_ASIN:

			pAsin = NEW_UNARY_OPERATOR(ExpAsin, pAsin);

			// Return on close bracket
			CHECK_TERMINATE();

			break;

		case T_ACOS:

			pAcos = NEW_UNARY_OPERATOR(ExpAcos, pAcos);

			// Return on close bracket
			CHECK_TERMINATE();

			break;

		case T_ATAN:

			pAtan = NEW_UNARY_OPERATOR(ExpAtan, pAtan);

			// Return on close bracket
			CHECK_TERMINATE();

			break;

		case T_ABS:

			pAbs = NEW_UNARY_OPERATOR(ExpAbs, pAbs);

			// Return on close bracket
			CHECK_TERMINATE();

			break;

		case T_EXP:

			pExp = NEW_UNARY_OPERATOR(ExpExp, pExp);

			// Return on close bracket
			CHECK_TERMINATE();

			break;

		case T_LN:

			pLn = NEW_UNARY_OPERATOR(ExpLn, pLn);

			// Return on close bracket
			CHECK_TERMINATE();

			break;

		case T_LOG10:

			pLog10 = NEW_UNARY_OPERATOR(ExpLog10, pLog10);

			// Return on close bracket
			CHECK_TERMINATE();

			break;

		case T_CEIL:

			pCeil = NEW_UNARY_OPERATOR(ExpCeil, pCeil);

			// Return on close bracket
			CHECK_TERMINATE();

			break;

		case T_ROUND:

			pRound = NEW_UNARY_OPERATOR(ExpRound, pRound);

			// Return on close bracket
			CHECK_TERMINATE();

			break;

		case T_RANDOM:

			pRandom = NEW_UNARY_OPERATOR(ExpRandom, pRandom);

			// Return on close bracket
			CHECK_TERMINATE();

			break;

		case T_LEN:

			pLen = NEW_UNARY_OPERATOR(ExpLen, pLen);

			// Return on close bracket
			CHECK_TERMINATE();

			break;

		case T_LEFTBRACKET:

			// THIS WILL BE CALLED ALWAYS NOW!!!
			//step++;
			//TOTIGS: Everywhere else you call DoBracket you dont add 1 to the step...
			// resulting in like 2 brackets omfg
			LeftPart = DoBrackets(step);

			if (unaryMode) return LeftPart;

			CHECK_TERMINATE()
			break;
			

		case T_NULL:
			step++;
			break;

		default:
				throw runtime_error("Syntax error in an expression.  Please report this bug.");

		}//switch
	}//while

	return LeftPart;
}

ExpPart* CExpression::DoBrackets(int& step)
{
	ExpPart* brackets;

	Depth++;

	// Step over left bracket
	step++;
	brackets = DoTree(step);
//	brackets = PostSortTree(brackets);

	Depth--;

	// Step over right bracket
	step++;


	return brackets;

}
