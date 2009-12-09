
#ifndef CEXPRESSION_H
#define CEXPRESSION_H

////////////////////////////////////////
// Structs used by CExpression

// Return value enumeration
// ACE flags
enum RETURNS 
{
	RETURN_INTEGER = 0x0001,		// Expression returning integer
	RETURN_STRING = 0x0002,			// Expression returning string
	CND_TRIGGERED = 0x0004,			// Trigger, by GenerateEvent
	CND_FASTTRIGGERED = 0x0008,		// Fast trigger, by GenerateEventFast
	RETURN_ARRAY = 0x0010,			// Expression returning an array
	SOL_MODIFIER = 0x0020,			// Condition or action calling Select, QuerySelect, on other objects
	RETURN_FLOAT = 0x0040,			// Expression returning a float
	CND_STATIC = 0x0080,			// Condition not called for all instances and must call Select() on self
};

// Token types
enum ExpToken {
	T_NULL,
	T_ANY_BINARY_OPERATOR,
	T_ANY_FUNCTION,
	T_ANY_VALUE,

	// Operand types
	T_INTEGER,		T_FLOAT,		T_STRINGLITERAL,	T_IDENTIFIER,	T_ARRAY,	T_VARIABLENAME,

	// Special operators: ( ) , . { } 
	T_LEFTBRACKET,	T_RIGHTBRACKET,	T_COMMA,			T_DOT,
	T_LEFTCURLY,	T_RIGHTCURLY,	T_AT,

	// Binary operators: + - * / % ^
	T_ADD,			T_SUBTRACT,		T_MULTIPLY,			T_DIVIDE,		T_MOD,
	T_POWER,
	// Unary function operators: sin cos tan sqrt str int float
	T_SIN,			T_COS,			T_TAN,				T_SQRT,
	T_FUNC_STR,		T_FUNC_INT,		T_FUNC_FLOAT,

	// Comparison operators = < > <= >= <> ? : and or
	T_EQUAL,		T_LESS,			T_GREATER,
	T_LESSEQUAL,	T_GREATEREQUAL,	T_NOTEQUAL,
	T_CONDITIONAL,	T_COLON,
	T_AND,			T_OR,

	// Mathematical operators
	T_ASIN,			T_ACOS,			T_ATAN,				T_ABS,
	T_EXP,			T_LN,			T_LOG10,			T_FLOOR,
	T_CEIL,			T_ROUND,		T_RANDOM,			T_LEN,
};

// A token
struct Token {
	ExpToken t;
	CString str;
	unsigned short length;

	__int64 iValue;
	double fValue;
};

//ExpReturn
#define EXPRETURN_NO_STORE
#include "..\Common\ExpReturn.hpp"

// Fairly obfuscated workaround to accessing private constructor of ExpReturn as a friend.  Meh.
class CollapseConstantTreeObj {
public:
	CollapseConstantTreeObj(ExpPart*& p);
};

#define CollapseConstantTree CollapseConstantTreeObj temp

////////////////////////////////////////
// CExpression class

class CExpression
{
public:

	CExpression();

	// Input a token stream
	void		ParseTokenStream();

	// Evaluate the expression
	void		Evaluate(ExpReturn* eReturn, class CRunObject* const selObj, int solindex);
	void		Evaluate(ExpReturn* eReturn);
	
	// Error handling
	void		NotifyError(const char* msg);

	friend class CEvent;
	friend class CEventParent;
	// For accessing CRuntime data
	friend class CRuntime;
	class CRuntime* pRuntime;

	// This expression is a parameter of an action or condition.
	// This is the OID of the object that action/condition belongs to (for selection)
	int oidCaller;

	// SOL index for pairing
	int SolID;

	// Selected object
	class CRunObject* pSelectedObject;

	// Bracket depth
	int			Depth;

	// To Tree
public:
	class ExpPart*	DoTree(int& step, bool unaryMode = false);
	class ExpPart*	DoBrackets(int& step);
private:
	void			LookupIdentifier(CString ident, class ExpIdent* part);
	void			LookupPluginExpression(int oid, CString fname, class ExpDot* pDot);

	// Top node of the expression tree
	class ExpPart*	ExpTree;

public:
	bool isConstant;
	ExpReturn constantValue;

	// The tokens making up the expression
	vector<Token> toks;

	int overrideCalledOID;
};

typedef vector<CExpression*>							ExpressionList;
typedef vector<CExpression*>::iterator					ExpressionIterator;
typedef vector<CExpression*>::const_iterator			ExpressionReadIterator;
typedef const vector<CExpression*>::const_iterator		ExpressionConstIterator;

void SwapLeftRight(ExpPart* parent);
ExpPart* PostSortTree(ExpPart* parent);

#endif