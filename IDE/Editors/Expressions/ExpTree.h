/////////////////////////////////////////////////////////////
//	The classes used in the expression tree for CExpression


// STDAFX

#ifndef EXPTREE_HPP
#define EXPTREE_HPP

#include "CExpression.h"

// ExpPart:  Any part of an expression
class __declspec(novtable) ExpPart
{
public:
	ExpPart();
	int tokennum;
	ExpToken t;
	class CExpression* pCExp;
	int bracketdepth;

	// Pure virtual evaluate function: evaluate this part
	virtual void Evaluate(ExpReturn* er) = 0;

	// Pure virtual constant function: determine if the expression is constant below here
	virtual bool Constant() = 0;

	// Virtual get left operand
	virtual ExpPart*& GetPLeftOperand();
	virtual ExpPart*& GetPRightOperand();
};

typedef vector<ExpPart*>									ExpParamList;
typedef vector<ExpPart*>::iterator							ExpParamIterator;
typedef const vector<ExpPart*>::const_iterator				ExpParamConstIterator;

/////////////////////////////////////////////////////////////
// Operands

// ExpInteger
// A 64-bit integer value
class ExpInteger : public ExpPart
{
public:
	ExpInteger();
	~ExpInteger();

	__int64 value;

	void Evaluate(ExpReturn* er);
	bool Constant();
};

// ExpFloat
// A 64-bit float value
class ExpFloat : public ExpPart
{
public:
	ExpFloat();
	~ExpFloat();

	double value;

	void Evaluate(ExpReturn* er);
	bool Constant();
};

// ExpString
// A string value
class ExpString : public ExpPart
{
public:
	ExpString();
	~ExpString();

	CString value;
	CString temp;

	void Evaluate(ExpReturn* er);
	bool Constant();
};

// ExpArray
// An array value, eg. {1, 5}
class ExpArray : public ExpPart
{
public:
	ExpArray();
	~ExpArray();

	// Expression elements eg. trees for {x + y, 5 * 3 + 2}
	ExpParamList expElements;

	// The evaluated array to return
	vector<ExpReturn> arr;

	int isConstant;		// nonzero if constant
	int noKill;			// nonzero to prevent destructor deallocating

	void Evaluate(ExpReturn* er);
	bool Constant();
};

// ExpBinary
// A binary value, any data, any size
class ExpBinary : public ExpPart
{
public:
	ExpBinary();
	~ExpBinary();

	ExpBinaryType data;

	void Evaluate(ExpReturn* er);
	bool Constant();
};

// ExpVariableName
// A variable name.  The type of 'Var' in Sprite('Var').  Translates name to integer index
class ExpVariableName : public ExpPart
{
public:
	ExpVariableName();
	~ExpVariableName();

	CString name;

	void Evaluate(ExpReturn* er);
	bool Constant();
};

// ExpIdent
// A string identifier, e.g. the "Obj" of "Obj.Func(a,b,c)"
// or a global variable, eg "XMouse"
class ExpIdent : public ExpPart
{
public:
	ExpIdent();
	~ExpIdent();

	CString ident;

	int oid;				// object the identifier referenced, -1 for system
	int isCallerOid;		// Nonzero if oid == pCExp->oidCaller
	int pType;				// zero if system object
	int expID;				// system expression ID if system, i.e. XMouse -> oid -1, expID 0

	int movementCall;		// Nonzero if movement SDK call; via the pLinkInstance member

	// Cache pointers
	//CRuntime* pRuntime;
	//SystemObject* pSystemObject;
	//ObjList* pTypeInstances;
	//ACROUTINE routine;

	// Parameters
	ExpParamList parameters;				// Parameter expression trees
	int hasParameters;
	ExpReturn expParamList[CRUNTIME_MAX_PARAMS];// Parameter evaluation results

	void Evaluate(ExpReturn* er);
	bool Constant();
};

//////////////
// Operators
#define DEFINE_BINARY_OPERATOR_CLASS(type)	\
											\
	class type : public ExpPart				\
	{										\
	public:									\
		type();								\
		~type();							\
		ExpPart*& GetPLeftOperand();		\
		ExpPart*& GetPRightOperand();		\
		ExpPart*	l;						\
		ExpPart*	r;						\
		void Evaluate(ExpReturn* er);		\
		bool Constant();					\
	};

#define DEFINE_UNARY_OPERATOR_CLASS(type)	\
											\
	class type : public ExpPart				\
	{										\
	public:									\
		type();								\
		~type();							\
		ExpPart*	p;						\
		void Evaluate(ExpReturn* er);		\
		bool Constant();					\
	};

// ExpAdd
DEFINE_BINARY_OPERATOR_CLASS(ExpAdd)

// ExpSubtract
DEFINE_BINARY_OPERATOR_CLASS(ExpSubtract)

// ExpMultiply
DEFINE_BINARY_OPERATOR_CLASS(ExpMultiply)

// ExpDivide
DEFINE_BINARY_OPERATOR_CLASS(ExpDivide)

// ExpMod
DEFINE_BINARY_OPERATOR_CLASS(ExpMod)

// ExpPower
DEFINE_BINARY_OPERATOR_CLASS(ExpPower)

// ExpAt
class ExpAt : public ExpPart
{
public:
	ExpAt();
	~ExpAt();
	ExpPart*& GetPLeftOperand();
	ExpPart*& GetPRightOperand();
	ExpPart*	l;
	ExpPart*	r;
	void Evaluate(ExpReturn* er);
	bool Constant();

	ExpReturn value;
};

// ExpEqual
DEFINE_BINARY_OPERATOR_CLASS(ExpEqual)

// ExpNotEqual
DEFINE_BINARY_OPERATOR_CLASS(ExpNotEqual)

// ExpLess
DEFINE_BINARY_OPERATOR_CLASS(ExpLess)

// ExpGreater
DEFINE_BINARY_OPERATOR_CLASS(ExpGreater)

// ExpLessEqual
DEFINE_BINARY_OPERATOR_CLASS(ExpLessEqual)

// ExpGreater
DEFINE_BINARY_OPERATOR_CLASS(ExpGreaterEqual)

// ExpAnd
class ExpAnd : public ExpPart
{
public:
	ExpAnd();
	~ExpAnd();
	ExpPart*& GetPLeftOperand();
	ExpPart*& GetPRightOperand();
	ExpPart*	l;
	ExpPart*	r;
	CString temp;
	void Evaluate(ExpReturn* er);
	bool Constant();
};

// ExpOr
DEFINE_BINARY_OPERATOR_CLASS(ExpOr)

// ExpConditional
class ExpConditional : public ExpPart
{
public:
	ExpConditional();
	~ExpConditional();

	ExpPart*& GetPLeftOperand();
	ExpPart*& GetPRightOperand();

	// Conditional: a ? b : c
	ExpPart*	a;
	ExpPart*	b;
	ExpPart*	c;

	void Evaluate(ExpReturn* er);
	bool Constant();
};

// ExpDot
class ExpDot : public ExpPart
{
public:
	ExpDot();
	~ExpDot();

	ExpPart*& GetPLeftOperand();
	ExpPart*& GetPRightOperand();

	// Dot: l.r (plugin call)
	ExpPart*	l;
	ExpPart*	r;

	//CRunObjType* pType;
	//ObjList* pTypeInstances;
	//CRuntime* pRuntime;
	CString fname;							// Expression name for unnamed expression calls
	CString objName;						// Object name
	int unnamedExp;							// Nonzero if unnamed expression mode

	int oid;								// Owner
	int isCallerOid;						// Nonzero if oid == pCExp->oidCaller
	int	movementCall;						// Nonzero if Movement SDK call via pLinkInstance

	// Parameters
	//ACROUTINE expRoutine;
	ExpParamList parameters;				// Parameter expression trees
	int hasParameters;
	ExpReturn expParamList[CRUNTIME_MAX_PARAMS];// Parameter evaluation results

	void Evaluate(ExpReturn* er);
	bool Constant();
};

//////////////////
// Unary operators

// ExpSin
DEFINE_UNARY_OPERATOR_CLASS(ExpSin)

// ExpCos
DEFINE_UNARY_OPERATOR_CLASS(ExpCos)

// ExpTan
DEFINE_UNARY_OPERATOR_CLASS(ExpTan)

// ExpSqrt
DEFINE_UNARY_OPERATOR_CLASS(ExpSqrt)

// ExpFuncInt
// For Int("56")
DEFINE_UNARY_OPERATOR_CLASS(ExpFuncInt)

// ExpFuncFloat
// For Float("7.7")
DEFINE_UNARY_OPERATOR_CLASS(ExpFuncFloat)

// ExpFuncStr
// For Str(5), Str(0.5)
class ExpFuncStr : public ExpPart
{
public:
	ExpFuncStr();
	~ExpFuncStr();
	ExpPart*	p;
	void Evaluate(ExpReturn* er);
	bool Constant();
	CString temp;
};

// ExpAsin
DEFINE_UNARY_OPERATOR_CLASS(ExpAsin)

// ExpAcos
DEFINE_UNARY_OPERATOR_CLASS(ExpAcos)

// ExpAtan
DEFINE_UNARY_OPERATOR_CLASS(ExpAtan)

// ExpAbs
DEFINE_UNARY_OPERATOR_CLASS(ExpAbs)

// ExpExp
DEFINE_UNARY_OPERATOR_CLASS(ExpExp)

// ExpLn
DEFINE_UNARY_OPERATOR_CLASS(ExpLn)

// ExpLog10
DEFINE_UNARY_OPERATOR_CLASS(ExpLog10)

// ExpCeil
DEFINE_UNARY_OPERATOR_CLASS(ExpCeil)

// ExpRound
DEFINE_UNARY_OPERATOR_CLASS(ExpRound)

// ExpRandom
DEFINE_UNARY_OPERATOR_CLASS(ExpRandom)

// ExpLen
DEFINE_UNARY_OPERATOR_CLASS(ExpLen)

#endif //EXPTREE_HPP