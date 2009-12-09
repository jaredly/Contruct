
#ifndef CEXPRESSION_H
#define CEXPRESSION_H

////////////////////////////////////////
// Structs used by CExpression

// Token types
enum ExpToken {
	T_NULL,
	T_ANY_BINARY_OPERATOR,
	T_ANY_FUNCTION,
	T_ANY_VALUE,

	// Operand types
	T_INTEGER,		T_FLOAT,		T_STRINGLITERAL,	T_IDENTIFIER,	T_ARRAY, T_VARIABLENAME,

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

	T_WHITESPACE,	T_COLOR,
};

// For edittime processing
enum TokenSubType {
	TT_NORMAL,			// Nothing special
	TT_OBJECTNAME,		// An object name
	TT_FAMILYNAME,		// A family name
	TT_MOVEMENTNAME,	// Identifier in Sprite[Movement] form; 'id' is movement id
};

// A token
struct Token {
	ExpToken t;
	CString str;
	unsigned short length;

	// For edittime processing
	long oidOwner;		// for variable names
	TokenSubType tsub;
	long id;

	// I removed the constructor because vectors call it when they reallocate, making the data change :(
	Token() : length(0), tsub(TT_NORMAL), oidOwner(-1), id(0), str("") {}
};

//ExpType
//#include "..\Common\ExpReturn.hpp"
//CHANGE
#include "..\..\..\Common\ExpReturn.hpp"

inline CString GetTypeName(ExpType t)
{
	switch (t) {
	case EXPTYPE_INTEGER:
		return "integer";
	case EXPTYPE_FLOAT:
		return "float";
	case EXPTYPE_STRING:
		return "string";
	case EXPTYPE_ARRAY:
		return "array";
	case EXPTYPE_BINARY:
		return "binary";
	case EXPTYPE_VARIABLENAME:
		return "variable name";
	default:
		return "unknown";
	}
}

void CollapseConstantTree(class ExpPart*& p);
void ParseBehaviorExpression(CString ident, CString& objName, CString& movementName);

void ProcessEdittimeExpressionInformation(vector<Token>& ttokens, class CApplication* pApp, long oid);

////////////////////////////////////////
// CExpression class

class CExpression
{
public:

	CExpression();

	// Input a user typed expression
	void		ParseUserString(const char* exp, int* pchpos = NULL, bool editorMode = false);
	void		PreProcessTokStream();

	// Return the user typed expression
	void		GetUserString(CString& str);

	// Top node of expression tree
	class ExpPart* ExpTree;

	// Evaluate the expression
	void		Evaluate(ExpReturn* eReturn, class CRunObject* const selObj, int solindex);
	void		Evaluate(ExpReturn* eReturn);

	// Validate the expression syntax
#ifndef RUNTIME
	void		Validate(int& chpos);
	ExpType		TypeCheck();
	virtual ~CExpression();

#endif
	
	// Error handling
	void		NotifyError(const char* msg);
	void		WarnTypeMismatch(const char* op, ExpReturn* l, ExpReturn* r);
	int			TokenToChar(int t);

	friend class CEvent;
	friend class CEventParent;
	// For accessing CRuntime data
	friend class CRuntime;
	class CRuntime* pRuntime;

	// This expression is a parameter of an action or condition.
	// This is the OID of the object that action/condition belongs to (for selection)
	int oidCaller;
	class CObjType* pCallerType;

	// SOL index for pairing
	int SolID;

	// Selected object
	class CRunObject* pSelectedObject;

	// Bracket depth
	int			Depth;

private:

	// Parsing functions
	bool		IsWhitespace(char ch);
	bool		IsChDigit(char ch);
	bool		IsChAlpha(char ch);
	bool		IsChAlphaNumeric(char ch);
	void		AppendToken(ExpToken t, CString str);
	int			AppendString(const char* str, bool editorMode, bool singleQuotes = false);
	int			ConsumeDigits(char* buf, const char* str);
	int			ConsumeAppendIdentifier(char* buf, const char* str);
	bool		StrContains(const char* str, char ch);
	void		CheckForKeywords(Token& t);

	bool		DetermineIfASpaceNeedsToBeInsertedHereForClarityForTheUserWhenConstructingAUserStringAgain(int i);

	// To Tree
public:
	class ExpPart*	DoTree(int& step, bool unaryMode = false);
	class ExpPart*	DoBrackets(int& step);
private:
	//bool			HasPrecedence(ExpPart* l, ExpPart* r);
	//int				OpPrecedence(ExpPart* Part);
	//class ExpPart*	SwapTree(ExpPart* parent);
	void			LookupIdentifier(CString ident, class ExpIdent* part);
	void			LookupExtExpression(int oid, CString fname, class ExpDot* pDot);

	// Validation functions
	int			GetRuleForToken(ExpToken t);
	bool		TokenFitsRule(ExpToken t, ExpToken rule);

	int whitespaceCount;

public:
	bool isConstant;
	ExpReturn constantValue;

	// The tokens making up the expression
	vector<Token> toks;

#ifndef RUNTIME
	// Virtual overrides for edittime checking

	// Return 'true' if 'objName' is a valid name of an object
	// Dont be case sensitive!
	virtual bool ObjectNameExists(CString objName)
	{
		return false;
	}

	virtual bool ObjectBehaviorExists(CString objName, CString movementName)
	{
		return false;
	}

	// Return 'true' if 'objName' can be used as a default value, eg. without a dot following it.
	// Return 'false' if objName is not 
	// Eg. Edit as opposed to Edit.Text.  We need to make an SDK option to enable the default value first.
	virtual bool ObjectHasDefaultValue(CString objName)
	{
		// Temporary solution: allow any object to use a default value
		return ObjectNameExists(objName);
	}

	// Return 'true' if 'expName' exists under 'objName', eg. Object.X
	virtual bool ObjectExpressionExists(CString objName, CString expName)
	{
		return false;
	}

	// Get the type of a plugin expression.
	// expName is "<default>" if its asking for the default overload, eg. Sprite(a,b,c)
	virtual ExpType GetExpressionType(CString objName, CString expName)
	{
		// Return type is 'any'
		return EXPTYPE_NULL;
	}

	// Return 'true' if objName.expName accepts numParams parameters.
	// expName is "<default>" if its asking for the default overload, eg. Sprite(a,b,c)
	virtual bool CheckParamCount(CString objName, CString expName, int numParams)
	{
		return false;
	}

	// Parameters valid: return 'true' if the given parameter is acceptable for the given expression
	// This is only called if num params > 0
	virtual bool ParamValid(CString objName, CString expName, int paramNum, int numParams, ExpType type)
	{
		return false;
	}

	virtual bool PrivateVariableExists(CString objName, CString expName, int paramNum, int numParams, CString varName)
	{
		return false;
	}

	bool IsValueCastable(ExpType type)
	{
		return type == EXPTYPE_INTEGER || type == EXPTYPE_FLOAT || type == EXPTYPE_STRING || type == EXPTYPE_NULL;
	}

	// For ACEs using GetStringPtr - must give a valid string
	bool IsStrictString(ExpType type)
	{
		// No way of telling an unknown is string
		return type == EXPTYPE_STRING || type == EXPTYPE_NULL;
	}

	bool IsStrictArray(ExpType type)
	{
		// No way of telling an unknown is array
		return type == EXPTYPE_ARRAY || type == EXPTYPE_NULL;
	}

	ExpType SysGetType(CString expName) {
		expName.MakeLower();

		if (expName == "mousex")				return EXPTYPE_INTEGER;
		else if (expName == "mousey")			return EXPTYPE_INTEGER;
		else if (expName == "loopindex")		return EXPTYPE_INTEGER;
		else if (expName == "looplength")		return EXPTYPE_INTEGER;
		else if (expName == "max")				return EXPTYPE_FLOAT;
		else if (expName == "min")				return EXPTYPE_FLOAT;
		else if (expName == "totalobjects")		return EXPTYPE_INTEGER;
		else if (expName == "global")			return EXPTYPE_NULL;
		else if (expName == "formatdecimal")	return EXPTYPE_STRING;
		else if (expName == "zeropad")			return EXPTYPE_STRING;
		else if (expName == "newline")			return EXPTYPE_STRING;
		else if (expName == "left")				return EXPTYPE_STRING;
		else if (expName == "right")			return EXPTYPE_STRING;
		else if (expName == "mid")				return EXPTYPE_STRING;
		else if (expName == "uppercase")		return EXPTYPE_STRING;
		else if (expName == "lowercase")		return EXPTYPE_STRING;
		else if (expName == "find")				return EXPTYPE_INTEGER;
		else if (expName == "remove")			return EXPTYPE_STRING;
		else if (expName == "replace")			return EXPTYPE_STRING;
		else if (expName == "trim")				return EXPTYPE_STRING;
		else if (expName == "reverse")			return EXPTYPE_STRING;
		else if (expName == "numtokens")		return EXPTYPE_INTEGER;
		else if (expName == "gettoken")			return EXPTYPE_STRING;
		else if (expName == "distance")			return EXPTYPE_FLOAT;
		else if (expName == "angle")			return EXPTYPE_FLOAT;
		else if (expName == "countmatching")	return EXPTYPE_INTEGER;
		else if (expName == "scrollx")			return EXPTYPE_FLOAT;
		else if (expName == "scrolly")			return EXPTYPE_FLOAT;
		else if (expName == "displaywidth")		return EXPTYPE_INTEGER;
		else if (expName == "displayheight")	return EXPTYPE_INTEGER;
		else if (expName == "ps_version")		return EXPTYPE_FLOAT;
		else if (expName == "rgb")				return EXPTYPE_INTEGER;
		else if (expName == "getred")			return EXPTYPE_INTEGER;
		else if (expName == "getgreen")			return EXPTYPE_INTEGER;
		else if (expName == "getblue")			return EXPTYPE_INTEGER;
		else if (expName == "tickcount")		return EXPTYPE_INTEGER;
		else if (expName == "timer")			return EXPTYPE_INTEGER;
		else if (expName == "layerzoomx")		return EXPTYPE_FLOAT;
		else if (expName == "layerzoomy")		return EXPTYPE_FLOAT;
		else if (expName == "layerscrollratex")	return EXPTYPE_FLOAT;
		else if (expName == "layerscrollratey")	return EXPTYPE_FLOAT;
		else if (expName == "layeropacity")		return EXPTYPE_FLOAT;
		else if (expName == "layername")		return EXPTYPE_STRING;
		else if (expName == "layercount")		return EXPTYPE_INTEGER;
		else if (expName == "zoomx")			return EXPTYPE_FLOAT;
		else if (expName == "zoomy")			return EXPTYPE_FLOAT;
		else if (expName == "timedelta")		return EXPTYPE_FLOAT;
		else if (expName == "scrollxleft")		return EXPTYPE_FLOAT;
		else if (expName == "scrollxright")		return EXPTYPE_FLOAT;
		else if (expName == "scrollytop")		return EXPTYPE_FLOAT;
		else if (expName == "scrollybottom")	return EXPTYPE_FLOAT;
		else if (expName == "displayangle")		return EXPTYPE_FLOAT;
		else if (expName == "clamp")			return EXPTYPE_FLOAT;
		else if (expName == "apppath")			return EXPTYPE_STRING;
		else if (expName == "layoutwidth")		return EXPTYPE_INTEGER;
		else if (expName == "layoutheight")		return EXPTYPE_INTEGER;
		else if (expName == "lerp")				return EXPTYPE_FLOAT;
		else if (expName == "vram")				return EXPTYPE_INTEGER;
		else if (expName == "displaydevice")	return EXPTYPE_STRING;
		else if (expName == "timescale")		return EXPTYPE_FLOAT;
		else if (expName == "layerangle")		return EXPTYPE_FLOAT;
		else if (expName == "linearaim")		return EXPTYPE_FLOAT;
		else if (expName == "layoutnumber")		return EXPTYPE_INTEGER;
		else if (expName == "layoutname")		return EXPTYPE_STRING;
		else if (expName == "sign")				return EXPTYPE_FLOAT;
		else if (expName == "qarp")				return EXPTYPE_FLOAT;
		else if (expName == "cosp")				return EXPTYPE_FLOAT;
		else if (expName == "anglediff")		return EXPTYPE_FLOAT;
		else if (expName == "rotateangle")		return EXPTYPE_FLOAT;
		else if (expName == "normalrandom")		return EXPTYPE_FLOAT;
		else if (expName == "cubic")			return EXPTYPE_FLOAT;
		else if (expName == "multisamples")		return EXPTYPE_INTEGER;
		else return EXPTYPE_NULL;
	}

	// System functions
	bool SysParamValid(CString expName, int paramNum, int numParams, ExpType type)
	{
		expName.MakeLower();

		if (expName == "loopindex" || expName == "looplength" || expName == "uppercase"
		 || expName == "lowercase" || expName == "vram")
			return IsStrictString(type);

		if (expName == "global")
			return type == EXPTYPE_VARIABLENAME;

		// Take any numerical values
		if (expName == "formatdecimal" || expName == "zeropad" || expName == "max" || expName == "min")
			return IsValueCastable(type);

		if (expName == "left" || expName == "right" || expName == "mid" || expName == "find") {
			if (paramNum == 0) return IsStrictString(type);
			else return IsValueCastable(type);
		}

		if (expName == "remove" || expName == "replace" || expName == "trim" || expName == "reverse"
		 || expName == "numtokens")
			return IsStrictString(type);

		if (expName == "gettoken") {
			if (paramNum == 1) return IsValueCastable(type);
			else return IsStrictString(type);
		}

		if (expName == "distance" || expName == "angle") {
			if (numParams == 2) return IsStrictArray(type);
			if (numParams == 4) return IsValueCastable(type);
		}

		if (expName == "clamp")
			return IsValueCastable(type);
	
		if (expName == "python")
			return IsValueCastable(type);

		if (expName == "countmatching")
			return type == EXPTYPE_INTEGER || type == EXPTYPE_STRING;

		if (expName == "rgb" || expName == "getred" || expName == "getgreen" || expName == "getblue")
			return IsValueCastable(type);

		if (expName == "layerzoomx" || expName == "layerzoomy" || expName == "layerscrollratex" || expName == "layerscrollratey"
			|| expName == "layeropacity" || expName == "layername" || expName == "mousex" || expName == "mousey"
			|| expName == "scrollxleft" || expName == "scrollxright" || expName == "scrollytop" || expName == "scrollybottom"
			|| expName == "totalobjects" || expName == "layerangle" || expName == "anglediff" || expName == "cosp"
			|| expName == "rotateangle" || expName == "normalrandom" || expName == "cubic")
			return IsValueCastable(type);

		if (expName == "lerp" || expName == "linearaim" || expName == "sign" || expName == "qarp")
			return IsValueCastable(type);

		return false;
	}

	bool CheckSysParamCount(CString expName, int numParams)
	{
		expName.MakeLower();

		if (expName == "mousex")				return numParams <= 1;
		else if (expName == "mousey")			return numParams <= 1;
		else if (expName == "loopindex")		return numParams == 0 || numParams == 1;
		else if (expName == "looplength")		return numParams == 0 || numParams == 1;
		else if (expName == "max")				return numParams > 1;
		else if (expName == "min")				return numParams > 1;
		else if (expName == "totalobjects")		return numParams <= 1;
		else if (expName == "global")			return numParams == 1;
		else if (expName == "formatdecimal")	return numParams == 2;
		else if (expName == "zeropad")			return numParams == 2;
		else if (expName == "newline")			return numParams == 0;
		else if (expName == "left")				return numParams == 2;
		else if (expName == "right")			return numParams == 2;
		else if (expName == "mid")				return numParams == 3;
		else if (expName == "uppercase")		return numParams == 1;
		else if (expName == "lowercase")		return numParams == 1;
		else if (expName == "find")				return numParams == 2 || numParams == 3;
		else if (expName == "remove")			return numParams == 2;
		else if (expName == "replace")			return numParams == 3;
		else if (expName == "trim")				return numParams == 1;
		else if (expName == "reverse")			return numParams == 1;
		else if (expName == "numtokens")		return numParams == 1 || numParams == 2;
		else if (expName == "gettoken")			return numParams == 2 || numParams == 3;
		else if (expName == "distance")			return numParams == 2 || numParams == 4;
		else if (expName == "angle")			return numParams == 2 || numParams == 4;
		else if (expName == "countmatching")	return numParams == 1;
		else if (expName == "scrollx")			return numParams == 0;
		else if (expName == "scrolly")			return numParams == 0;
		else if (expName == "displaywidth")		return numParams == 0;
		else if (expName == "displayheight")	return numParams == 0;
		else if (expName == "ps_version")		return numParams == 0;
		else if (expName == "rgb")				return numParams == 3;
		else if (expName == "getred")			return numParams == 1;
		else if (expName == "getgreen")			return numParams == 1;
		else if (expName == "getblue")			return numParams == 1;
		else if (expName == "tickcount")		return numParams == 0;
		else if (expName == "timer")			return numParams == 0;
		else if (expName == "layerzoomx")		return numParams == 1;
		else if (expName == "layerzoomy")		return numParams == 1;
		else if (expName == "layerscrollratex")	return numParams == 1;
		else if (expName == "layerscrollratey")	return numParams == 1;
		else if (expName == "layeropacity")		return numParams == 1;
		else if (expName == "layername")		return numParams == 1;
		else if (expName == "layercount")		return numParams == 0;
		else if (expName == "zoomx")			return numParams == 0;
		else if (expName == "zoomy")			return numParams == 0;
		else if (expName == "timedelta")		return numParams == 0;
		else if (expName == "scrollxleft")		return numParams <= 1;
		else if (expName == "scrollxright")		return numParams <= 1;
		else if (expName == "scrollytop")		return numParams <= 1;
		else if (expName == "scrollybottom")	return numParams <= 1;
		else if (expName == "displayangle")		return numParams == 0;
		else if (expName == "clamp")			return numParams == 3;
		else if (expName == "apppath")			return numParams == 0;
		else if (expName == "layoutwidth")		return numParams == 0;
		else if (expName == "layoutheight")		return numParams == 0;
		else if (expName == "python")			return numParams == 1;
		else if (expName == "lerp")				return numParams == 3;
		else if (expName == "vram")				return numParams <= 1;
		else if (expName == "displaydevice")	return numParams == 0;
		else if (expName == "timescale")		return numParams == 0;
		else if (expName == "layerangle")		return numParams == 1;
		else if (expName == "linearaim")		return numParams == 7;
		else if (expName == "layoutnumber")		return numParams == 0;
		else if (expName == "layoutname")		return numParams == 0;
		else if (expName == "sign")				return numParams == 1;
		else if (expName == "qarp")				return numParams == 4;
		else if (expName == "cosp")				return numParams == 3;
		else if (expName == "anglediff")		return numParams == 2;
		else if (expName == "rotateangle")		return numParams == 3;
		else if (expName == "normalrandom")		return numParams == 2;
		else if (expName == "cubic")			return numParams == 5;
		else if (expName == "multisamples")		return numParams == 0;
		else return false;
	}

	// Return 'true' if 'expName' is a system expression, eg. "MouseX"
	bool SystemExpressionExists(CString expName)
	{
		expName.MakeLower();

		if (expName == "mousex")				return true;
		else if (expName == "mousey")			return true;
		else if (expName == "loopindex")		return true;
		else if (expName == "looplength")		return true;
		else if (expName == "max")				return true;
		else if (expName == "min")				return true;
		else if (expName == "totalobjects")		return true;
		else if (expName == "global")			return true;
		else if (expName == "formatdecimal")	return true;
		else if (expName == "zeropad")			return true;
		else if (expName == "newline")			return true;
		else if (expName == "left")				return true;
		else if (expName == "right")			return true;
		else if (expName == "mid")				return true;
		else if (expName == "uppercase")		return true;
		else if (expName == "lowercase")		return true;
		else if (expName == "find")				return true;
		else if (expName == "remove")			return true;
		else if (expName == "replace")			return true;
		else if (expName == "trim")				return true;
		else if (expName == "reverse")			return true;
		else if (expName == "numtokens")		return true;
		else if (expName == "gettoken")			return true;
		else if (expName == "distance")			return true;
		else if (expName == "angle")			return true;
		else if (expName == "countmatching")	return true;
		else if (expName == "scrollx")			return true;
		else if (expName == "scrolly")			return true;
		else if (expName == "displaywidth")		return true;
		else if (expName == "displayheight")	return true;
		else if (expName == "ps_version")		return true;
		else if (expName == "rgb")				return true;
		else if (expName == "getred")			return true;
		else if (expName == "getgreen")			return true;
		else if (expName == "getblue")			return true;
		else if (expName == "tickcount")		return true;
		else if (expName == "timer")			return true;
		else if (expName == "layerzoomx")		return true;
		else if (expName == "layerzoomy")		return true;
		else if (expName == "layerscrollratex")	return true;
		else if (expName == "layerscrollratey")	return true;
		else if (expName == "layeropacity")		return true;
		else if (expName == "layername")		return true;
		else if (expName == "layercount")		return true;
		else if (expName == "zoomx")			return true;
		else if (expName == "zoomy")			return true;
		else if (expName == "timedelta")		return true;
		else if (expName == "scrollxleft")		return true;
		else if (expName == "scrollxright")		return true;
		else if (expName == "scrollytop")		return true;
		else if (expName == "scrollybottom")	return true;
		else if (expName == "displayangle")		return true;
		else if (expName == "clamp")			return true;
		else if (expName == "apppath")			return true;
		else if (expName == "layoutwidth")		return true;
		else if (expName == "layoutheight")		return true;
		else if (expName == "python")			return true;
		else if (expName == "lerp")				return true;
		else if (expName == "vram")				return true;
		else if (expName == "displaydevice")	return true;
		else if (expName == "timescale")		return true;
		else if (expName == "layerangle")		return true;
		else if (expName == "linearaim")		return true;
		else if (expName == "layoutnumber")		return true;
		else if (expName == "layoutname")		return true;
		else if (expName == "sign")				return true;
		else if (expName == "qarp")				return true;
		else if (expName == "cosp")				return true;
		else if (expName == "anglediff")		return true;
		else if (expName == "rotateangle")		return true;
		else if (expName == "normalrandom")		return true;
		else if (expName == "cubic")			return true;
		else if (expName == "multisamples")		return true;
		return false;
	}
#endif
};

#endif