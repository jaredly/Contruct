// CExpression
// Class to handle an expression
// By Tigerworks

#include "stdafx.h"
#include "ExpTree.h"
#include "CExpression.h"
#include "..\..\Utilities\Helper.h"

///////////////////////////////////
// CExpression functions
void CExpression::Evaluate(ExpReturn* eReturn)
{
	ExpTree->Evaluate(eReturn);
}

ExpType CExpression::TypeCheck()
{
	int step = 0;
	if (ExpTree)
		delete ExpTree;
	ExpTree = DoTree(step);
	Evaluate(&constantValue);
	return constantValue.Type();
}

void ProcessEdittimeExpressionInformation(vector<Token>& ttokens, CApplication* pApp, long oid)
{
	// Now process the tokens setting the sub types to find family and object names, and pointing variable names
	// to their owners.
	vector<Token>::iterator t = ttokens.begin();
	vector<Token>::iterator ttokens_end = ttokens.end();
	int index = 0;
	long lastOidOwner = -1;

	for ( ; t != ttokens_end; t++, index++) {

		switch(t->t) {
		case T_IDENTIFIER:
			// Identifier is the first token, or not preceded by a dot: looks like an object or family name
			if (index == 0 || (t - 1)->t != T_DOT) {

				CString objName = t->str;
				bool movementIdentifier = false;
				CString movName;

				// Look for '[' in identifier: signals a movement identifier eg. Sprite[Movement]
				int index = t->str.Find('[');

				// Movement found
				if (index > -1) {
					movementIdentifier = true;

					// Parse the names out of Obj[Mov]
					movName = objName;
					objName = objName.Left(index);
					movName.Delete(0, index+1);
					movName.Delete(movName.GetLength() - 1);
				}

				CObjType* pType = GetTypeFromName(pApp, objName);
				list<Family>::iterator family = find(pApp->families.begin(), pApp->families.end(), t->str);
				

				if (pType) {
					t->oidOwner = pType->ObjectIdentifier;
					lastOidOwner = pType->ObjectIdentifier;

					if (movementIdentifier) {
						t->tsub = TT_MOVEMENTNAME;
						t->id = pType->GetBehaviorByName(movName)->id;
					}
					else
						t->tsub = TT_OBJECTNAME;
				}

				else if (family != pApp->families.end()) 
				{
					if(family->is_in_use)
					{
						t->tsub = TT_FAMILYNAME;
						t->oidOwner = family->identifier;
						lastOidOwner = family->identifier;
					}
				}
			}//identifier
			break;
		case T_VARIABLENAME:

			// Previous token is an open bracket: use the last owner.
			// Otherwise, for standalone variable names, use the expression owner.
			bool isStandalone = true;

			// Looking for pattern <obj name> <open bracket>
			if (index >= 2) {
				if ((t - 1)->t == T_LEFTBRACKET
				&& (t - 2)->t == T_IDENTIFIER
				&& ((t - 2)->tsub == TT_OBJECTNAME
					|| (t - 2)->tsub == TT_FAMILYNAME))
				{
					isStandalone = false;
				}

				CString n = (t - 2)->str;
				n.MakeLower();

				// Look for a global variable name
				if ((t - 1)->t == T_LEFTBRACKET
				&& (t - 2)->t == T_IDENTIFIER
				&& n == "global")
				{
					t->oidOwner = -1;
					// now work out the id
					for(list<CApplication::GlobalVariable>::iterator i = pApp->global_variables.begin(); i!= pApp->global_variables.end(); i++)
					{
						CString l = i->name;
						CString r = t->str;
						l.MakeLower();
						r.MakeLower();

						if(l == r)
							t->id = i->identifier;
					}
					break;
				}
			}
			// Looking for pattern <dot> <identifier> <open bracket> var name <close bracket/comma>
			// eg .Value(...)
			if (index >= 3) {
				if ((t - 1)->t == T_LEFTBRACKET
				&& (t - 2)->t == T_IDENTIFIER
				&& (t - 3)->t == T_DOT)
				{
					if (t + 1 != ttokens_end && ((t + 1)->t == T_RIGHTBRACKET || (t + 1)->t == T_COMMA))
						isStandalone = false;
				}
			}

			// Use action/condition OID for standalones
			if (isStandalone)
				t->oidOwner = oid;
			// Use last OID for object variables eg. Sprite('Rofl')
			else
				t->oidOwner = lastOidOwner;
	
			// now work out the id
			CObjType* pType = pApp->FindObjTypeFromNumber(t->oidOwner);
			if(pType)
			{
				for(vector<PrivateValue>::iterator i = pType->m_PrivateValues.begin(); i!=  pType->m_PrivateValues.end(); i++)
				{
					CString l = i->name;
					CString r = t->str;
					l.MakeLower();
					r.MakeLower();

					if(l == r) {
						t->id = i->identifier;
						break;
					}
				}
			}
			break;
		}
	}
}

void ParseBehaviorExpression(CString ident, CString& objName, CString& movementName)
{
	int f = ident.Find('[');
	ident.MakeLower();

	// Search for and parse movement sub-names, ie. Sprite[Racecar]
	if (f != -1) {
		movementName = ident;
		movementName.Delete(0, f+1);
		movementName.Delete(movementName.GetLength() - 1);

		objName = ident.Left(f);
	}
	else {
		objName = ident;
		movementName = "";
	}
}


CExpression::~CExpression() {
	if (ExpTree)
		delete ExpTree;
}

// Is character a white space (newline, space, tab)
bool CExpression::IsWhitespace(char ch)
{
	return (ch == ' ' || ch == '	' || ch == '\n' || ch == '\r');
}

// Is character numerical digit
bool CExpression::IsChDigit(char ch)
{
	return ((ch <= '9' && ch >= '0') || ch == '.');
}

// Is character alpha (a-Z, _)
bool CExpression::IsChAlpha(char ch)
{
	return ((ch <= 'z' && ch >= 'a') || (ch <= 'Z' && ch >= 'A') || ch == '_');
}

// Is alphanumeric (a character that can start an identifier)
bool CExpression::IsChAlphaNumeric(char ch)
{
	if ((ch <= 'z' && ch >= 'a') || (ch <= 'Z' && ch >= 'A') || ch == '_' || (ch <= '9' && ch >= '0'))
		return true;

	return false;
}

void CExpression::NotifyError(const char* msg)
{
	throw runtime_error(msg);
}

// Add a token to the token vector
void CExpression::AppendToken(ExpToken t, CString str)
{
	Token tok;
	tok.str = str;
	tok.t = t;
	// First token?
	if (toks.empty())
		tok.length = str.GetLength() + whitespaceCount;
	else
		tok.length = str.GetLength();
	toks.push_back(tok);
}

CString ExpTypeToStr(ExpType t)
{
	switch (t) {
	case EXPTYPE_INTEGER:
		return "integer";
	case EXPTYPE_FLOAT:
		return "float";
	case EXPTYPE_STRING:
		return "string";
	case EXPTYPE_BINARY:
		return "binary";
	default:
		return "<unknown>";
	}
}
// Used to notify invalid type errors
void CExpression::WarnTypeMismatch(const char* op, ExpReturn* l, ExpReturn* r)
{
	// If types are unknown, don't warn
	if (l->eType == EXPTYPE_NULL) return;
	if (r) if (r->eType == EXPTYPE_NULL) return;

	CString msg;
	CString temp;

	if (r == NULL) {
		msg += "Cannot use '";
		msg += op;
		msg += "' with '";
		msg += ExpTypeToStr(l->eType);
		msg += "'.  The types are incompatible.";
	}
	else {
		msg = "Cannot use '";
		msg += op;
		msg += "' with ";
		msg += ExpTypeToStr(l->eType);
		msg += " and ";
		msg += ExpTypeToStr(r->eType);
		msg += ".  The types are incompatible.";
	}

	throw runtime_error((const char*)msg);

}

// Append a string as a token to the token vector
int CExpression::AppendString(const char* str, bool editorMode, bool singleQuotes)
{
	Token Tok;
	int i;

	for (i = 1; ; i++) {

		// End quote and not a "" double quote escape
		if (str[i] == '"') {
			// Double ""
			if (!singleQuotes && str[i+1] == '"') {
				Tok.str += '"';
				
				if(editorMode)
					Tok.str += '"';

				i++;		// Skip
			}
			else
				// Terminate
				break;
		}
		else if (singleQuotes && str[i] == '\'')
		{
			break;
		}

		// Null found inside string
		else if (str[i] == NULL) {

			// Report an error (string not closed)
			NotifyError("Syntax error: Unclosed string quote");
			i--;
			break;	// exit
		}
		else
			Tok.str += str[i];

	}//for

	// Append this token
	if (singleQuotes)
		Tok.t = T_VARIABLENAME;
	else {
		Tok.str.Replace("$n", "\n");
		Tok.str.Replace("$q", "\"");
		Tok.str.Replace("$t", "\t");
		Tok.t = T_STRINGLITERAL;
	}

	Tok.length = Tok.str.GetLength() + 2;	// Don't forget the quotes
	toks.push_back(Tok);

	return i;
}

// Run through a string copying digits to a buffer
int CExpression::ConsumeDigits(char* buf, const char* str)
{
	int i = 0;
	if (str[i] == '-') {
		buf[i] = str[i];
		i++;
	}

	for ( ; IsChDigit(str[i]); i++)
		buf[i] = str[i];

	buf[i] = NULL;
	return i-1;
}

// Run through a string copying ident to buffer
int CExpression::ConsumeAppendIdentifier(char* buf, const char* str)
{
	int i;

	bool alpha = false;
	bool floatNum = false;

	// Read chars up to first '.': if only digits, interpret as being a number and continue.  Else, use as identifier.
	for (i = 0; ; i++) {
		if (str[i] == '.')
			break;
		if (IsChAlpha(str[i]))
			alpha = true;
		else if (!IsChDigit(str[i]) && str[i] != '[' && str[i] != ']')
			break;
		buf[i] = str[i];
	}

	// Identifier found
	if (alpha) {
		buf[i] = NULL;
		AppendToken(T_IDENTIFIER, buf);
		CheckForKeywords(toks[toks.size() - 1]);
		return i - 1;
	}
	else if (str[i] == '.') {

		// Continue with dot
		buf[i++] = '.';
		floatNum = true;
	}

	// Only digits preceding .
	// Carry on reading digits
	for ( ; (str[i] <= '9' && str[i] >= '0'); i++)
		buf[i] = str[i];

	buf[i] = NULL;

	if (floatNum)
		AppendToken(T_FLOAT, buf);
	else
		AppendToken(T_INTEGER, buf);

	return i-1;
}

// String contains a character
// Todo: replace this function and all references with whatever ANSI thing
// does it that i cant be bothered to find
bool CExpression::StrContains(const char* str, char ch)
{
	for (int i = 0; str[i] != NULL; i++) {
		if (str[i] == ch)
			return true;
	}//for

	return false;
}

// Check for keywords like "Sin" and "Sqrt"
// Convert these from T_IDENTIFIERs to their token types e.g. T_SIN
void CExpression::CheckForKeywords(Token& t)
{
	if (t.t != T_IDENTIFIER)
		return;

	CString ident = t.str;
	ident.MakeLower();

	if (ident == "sin")
		t.t = T_SIN;
	else if (ident == "cos")
		t.t = T_COS;
	else if (ident == "tan")
		t.t = T_TAN;
	else if (ident == "sqrt")
		t.t = T_SQRT;
	else if (ident == "str")
		t.t = T_FUNC_STR;
	else if (ident == "int")
		t.t = T_FUNC_INT;
	else if (ident == "float" || ident == "num")
		t.t = T_FUNC_FLOAT;
	else if (ident == "and")
		t.t = T_AND;
	else if (ident == "or")
		t.t = T_OR;
	else if (ident == "asin")
		t.t = T_ASIN;
	else if (ident == "acos")
		t.t = T_ACOS;
	else if (ident == "atan")
		t.t = T_ATAN;
	else if (ident == "abs")
		t.t = T_ABS;
	else if (ident == "exp")
		t.t = T_EXP;
	else if (ident == "log" || ident == "ln")
		t.t = T_LN;
	else if (ident == "log10")
		t.t = T_LOG10;
	else if (ident == "floor")
		t.t = T_FUNC_INT;	// int conversion does floor conversion
	else if (ident == "ceil")
		t.t = T_CEIL;
	else if (ident == "round")
		t.t = T_ROUND;
	else if (ident == "random")
		t.t = T_RANDOM;
	else if (ident == "len")
		t.t = T_LEN;
	else if (ident == "at")
		t.t = T_AT;

	// Check for 'Self' to refer to owner object's name
	else if (ident == "self" || ident.Left(5) == "self[") {

		if (pCallerType->GetName() == "System")
			throw runtime_error("'Self' cannot be used in this context, because it would refer to the System object.");

		CString movPart = ident.Right(ident.GetLength() - 4);
		t.str = pCallerType->GetName() + movPart;
	}
}

// Tokenise a user's typed expression string
void CExpression::ParseUserString(const char* exp, int* pchpos, bool editorMode)
{
	toks.clear();

	whitespaceCount = 0;

	// Loop every char
	for (int i = 0; exp[i] != NULL; i++) {

		if (pchpos) *pchpos = i;

		char NextCh = exp[i+1];
		char CurCh = exp[i];
		char PrevCh = exp[(i == 0 ? 0 : i-1)];
		char strbuf[128];

		// Check this char
		switch (exp[i]) {

		case '+':
			AppendToken(T_ADD, "+");
			break;
		case '-':

			// If previous token is not operand, use as negative sign
			// Fix 25/4/07: random(4) - random(5) interprets '-' as integer: explicit check for right bracket
			if (toks.size() == 0 || ((!TokenFitsRule(toks.back().t, T_ANY_VALUE) || toks.back().t == T_LEFTBRACKET) && toks.back().t != T_RIGHTBRACKET)) {

				i += ConsumeDigits(strbuf, &(exp[i]));

				// Contains a dot: float
				if (StrContains(strbuf, '.'))
					AppendToken(T_FLOAT, strbuf);
				else	// Else integer
					AppendToken(T_INTEGER, strbuf);
			}
			else
				AppendToken(T_SUBTRACT, "-");

			break;
		case '*':
			AppendToken(T_MULTIPLY, "*");
			break;
		case '/':
			AppendToken(T_DIVIDE, "/");
			break;
		case '^':
			AppendToken(T_POWER, "^");
			break;
		case '%':
			AppendToken(T_MOD, "%");
			break;
		case '(':
			AppendToken(T_LEFTBRACKET, "(");
			break;
		case ')':
			AppendToken(T_RIGHTBRACKET, ")");
			break;
		case '{':
			AppendToken(T_LEFTCURLY, "{");
			break;
		case '}':
			AppendToken(T_RIGHTCURLY, "}");
			break;
		case '@':
			AppendToken(T_AT, "@");
			break;
		case ',':
			AppendToken(T_COMMA, ",");
			break;
		case '.':
			AppendToken(T_DOT, ".");
			break;
		case '"':
			i += AppendString(&(exp[i]), editorMode);
			break;
		case '\'':
			i += AppendString(&(exp[i]), editorMode, true);
			break;
		case '=':
			AppendToken(T_EQUAL, "=");
			break;
		case '<':
			if (NextCh == '=') {
				AppendToken(T_LESSEQUAL, "<=");
				i++;
			}
			else if (NextCh == '>') {
				AppendToken(T_NOTEQUAL, "<>");
				i++;
			}
			else
				AppendToken(T_LESS, "<");
			break;
		// Alternative not equal operator != <>
		case '!':
			if (NextCh == '=') {
				AppendToken(T_NOTEQUAL, "!=");
				i++;
			}
			else
				NotifyError("Syntax error: '!'");
			break;
		case '&':
			AppendToken(T_AND, "&");
			break;
		case '|':
			AppendToken(T_OR, "|");
			break;
		case '>':
			if (NextCh == '=') {
				AppendToken(T_GREATEREQUAL, ">=");
				i++;
			}
			else
				AppendToken(T_GREATER, ">");
			break;
		case '?':
			AppendToken(T_CONDITIONAL, "?");
			break;
		case ':':
			AppendToken(T_COLON, ":");
			break;
		default:

			// Parse numbers and idents
			if (IsChAlphaNumeric(CurCh))
				i += ConsumeAppendIdentifier(strbuf, &(exp[i]));

			// Skip over whitespace
			else if (IsWhitespace(CurCh)) {

				// In editor mode add a T_WHITESPACE token
				if (editorMode) {
					Token t;
					t.length = 0;
					t.t = T_WHITESPACE;

					while (IsWhitespace(exp[i])) {
						t.str += exp[i];
						i++;
					}

					// We want the next for loop iteration to see the next char
					i--;

					toks.push_back(t);
				}

				// Add to last token length
				if (toks.size() > 0)
					toks[toks.size() - 1].length++;
			}
			// Else unknown character
			else {
				string error = "Unknown character '";
				error += CurCh;
				error += "'";
				throw runtime_error(error.c_str());
			}
		}//switch

	}//for

	// Only perform preprocessing when not in edittime mode
	if (!editorMode)
		PreProcessTokStream();


	///////////////////////////
	// Final step:  parse to tree

	// Determine if the overall expression is constant, collapsing constant parts along
	// the way.
	// Runtime only
#ifdef RUNTIME
	// Make the tree
	int step = 0;
	ExpTree = DoTree(step);

	isConstant = ExpTree->Constant();

	// If constant, get the constant value.
	if (isConstant) {
		CollapseConstantTree(ExpTree);
		ExpTree->Evaluate(&constantValue);
	}
#endif

}

void CExpression::PreProcessTokStream()
{
	// Tokens preprocessor

	// TODO:
	// Search for standalone variable names and translate to default overload, eg:
	// 5 + 'Score' + 6
	// to
	// 5 + Object('Score') + 6
	vector<Token>::iterator i = toks.begin();
	vector<Token>::iterator toks_end = toks.end();
	vector<Token>::iterator toks_last = toks_end - 1;
	vector<Token>::iterator toks_begin = toks.begin();
	int index = 0;

	for ( ; i != toks_end; i++, index++) {
		// Check for var name token
		if (i->t == T_VARIABLENAME) {

			bool isStandalone = false;

			// No room for two tokens before it?
			if ((i - toks_begin) < 2)
				isStandalone = true;

			// Looking for no <identifier> <open bracket> 'Var name' meaning standalone
			// or <dot> <identifier> <open bracket>
			else {
				if ((i - 1)->t != T_LEFTBRACKET)
					isStandalone = true;
				else {
					// Check for <dot><identifier><open bracket>
					if (i - toks_begin >= 3 && (i - 3)->t == T_DOT)
						isStandalone = false;
					else {
						CString lwrstr = (i - 2)->str;
						lwrstr.MakeLower();

						if ((i - 2)->t != T_IDENTIFIER)
							isStandalone = true;
						else if (!ObjectNameExists(lwrstr) && lwrstr != "global")
							isStandalone = true;
					}
				}
			}

			if (isStandalone) {

				CString msg;
				msg.Format("Variablename expansion on token %d", index);
				//MessageBox(NULL, msg, "", NULL);
				Token t;
				t.t = T_RIGHTBRACKET;
				t.str = ")";
				t.length = 0;

				// Add right bracket
				i = toks.insert(i + 1, t) - 1;
				
				// Left bracket
				t.t = T_LEFTBRACKET;
				t.str = "(";
				i = toks.insert(i, t);

				// Obj name
				t.t = T_IDENTIFIER;
				if (pCallerType->GetName() == "System")
					t.str = "global";
				else
					t.str = pCallerType->GetName();

				i = toks.insert(i, t);

				toks_end = toks.end();
				toks_last = toks_end - 1;
				toks_begin = toks.begin();

				// Now i is pointing at the identifier.  Skip 2 tokens and continue to skip third
				i += 2;
				continue;
			}
		}
	}

	// Search for instances of ".X" and insert an identifier token for the object before it
	// 5 + . X + 6
	// to
	// 5 + object . X + 6

	// No room for a dot then expr
	if (toks.size() < 2)
		return;

	i = toks.begin();
	toks_end = toks.end();
	toks_last = toks_end - 1;
	toks_begin = toks.begin();

	for ( ; i != toks_end; i++) {

		// Is a dot not at the end?
		if (i->t == T_DOT && i != toks_last) {

			// Is the first token eg:    .X
			if (i == toks_begin) {
				if ((i+1)->t == T_IDENTIFIER) {
					// Insert the caller name before the dot
					Token tok;
					tok.length = 0;
					tok.str = pCallerType->GetName();
					tok.t = T_IDENTIFIER;
					i = toks.insert(toks_begin, tok);
					toks_end = toks.end();
					toks_last = toks_end - 1;
					toks_begin = toks.begin();
					continue;
				}
			}
			else {
				// Check for an identifier before and after the dot
				vector<Token>::iterator prev = i - 1;
				vector<Token>::iterator next = i + 1;

				// <not identifier> . <identifier> eg:    + .X
				if (next->t == T_IDENTIFIER && prev->t != T_IDENTIFIER) {
					// Insert the caller name before the dot
					Token tok;
					tok.length = 0;
					tok.str = pCallerType->GetName();
					tok.t = T_IDENTIFIER;
					i = toks.insert(i, tok);
					toks_end = toks.end();
					toks_last = toks_end - 1;
					toks_begin = toks.begin();
					continue;
				}
			}
		}
	}
}

bool CExpression::DetermineIfASpaceNeedsToBeInsertedHereForClarityForTheUserWhenConstructingAUserStringAgain(int i)
{
	if (i == 0)
		return false;
	
	ExpToken prevType = toks[i-1].t;
	ExpToken curType = toks[i].t;

	if (curType == T_COMMA)
		return false;
	if (curType == T_DOT || prevType == T_DOT)
		return false;
	if (prevType == T_LEFTBRACKET)
		return false;
	if (curType == T_RIGHTBRACKET)
		return false;
	if (curType == T_LEFTBRACKET && (prevType == T_IDENTIFIER
								||   prevType == T_COS
								||   prevType == T_SIN			||   prevType == T_TAN	
								||   prevType == T_SQRT			||	 prevType == T_FUNC_INT
								||	 prevType == T_FUNC_FLOAT	||	 prevType == T_FUNC_STR))
		return false;

	return true;

}

// Convert token stream back to user expression string
void CExpression::GetUserString(CString& str)
{
	str = "";

	for (int i = 0; i < toks.size(); i++) {

		// Determine if a space needs to go before this token
		if (DetermineIfASpaceNeedsToBeInsertedHereForClarityForTheUserWhenConstructingAUserStringAgain(i))
			str += " ";

		if (toks[i].t == T_STRINGLITERAL)
			str += "\"";

		str += toks[i].str;

		if (toks[i].t == T_STRINGLITERAL)
			str += "\"";

	}//for
}

// Turn a token number to a character position
int CExpression::TokenToChar(int t)
{
	int chpos = 0;

	// Count up token lengths
	for (int i = 0; i < t; i++)
		chpos += toks[i].length;

	return chpos;
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

#define EXP_NUM_RULES 46
// The Expression Validation Rules
ExpToken ExpValidationRules [EXP_NUM_RULES+1][10] = {

	// Token type		Types that are allowed to follow
	{T_IDENTIFIER,		T_ANY_BINARY_OPERATOR, T_LEFTBRACKET, T_RIGHTBRACKET, T_COMMA, T_RIGHTCURLY, T_DOT, T_CONDITIONAL, T_COLON, T_NULL},
	{T_INTEGER,			T_ANY_BINARY_OPERATOR, T_RIGHTBRACKET, T_COMMA, T_RIGHTCURLY, T_CONDITIONAL, T_COLON, T_NULL},
	{T_FLOAT,			T_ANY_BINARY_OPERATOR, T_RIGHTBRACKET, T_COMMA, T_RIGHTCURLY, T_COLON, T_NULL},
	{T_STRINGLITERAL,	T_ADD, T_AND, T_RIGHTBRACKET, T_COMMA, T_RIGHTCURLY, T_COLON, T_NULL},
	{T_VARIABLENAME,	T_RIGHTBRACKET, T_COMMA, T_COLON, T_NULL},
	{T_LEFTBRACKET,		T_ANY_VALUE, T_ANY_FUNCTION, T_SUBTRACT, T_NULL},
	{T_RIGHTBRACKET,	T_RIGHTBRACKET, T_ANY_BINARY_OPERATOR, T_COMMA, T_RIGHTCURLY, T_CONDITIONAL, T_COLON, T_NULL},
	{T_RIGHTCURLY,		T_RIGHTBRACKET, T_ANY_BINARY_OPERATOR, T_COMMA, T_RIGHTCURLY, T_CONDITIONAL, T_COLON, T_NULL},
	{T_COMMA,			T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_DOT,				T_IDENTIFIER, T_NULL},
	{T_ADD,				T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_SUBTRACT,		T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_MULTIPLY,		T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_DIVIDE,			T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_MOD,				T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_POWER,			T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_SIN,				T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_COS,				T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_TAN,				T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_SQRT,			T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_FUNC_INT,		T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_FUNC_FLOAT,		T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_FUNC_STR,		T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_CEIL,			T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_AND,				T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_OR,				T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_ASIN,			T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_ACOS,			T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_ATAN,			T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_ABS,				T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_EXP,				T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_LN,				T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_LOG10,			T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_ROUND,			T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_RANDOM,			T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_LEN,				T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_AT,				T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_LEFTCURLY,		T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_CONDITIONAL,		T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_COLON,			T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_EQUAL,			T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_LESS,			T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_GREATER,			T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_LESSEQUAL,		T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_GREATEREQUAL,	T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_NOTEQUAL,		T_ANY_VALUE, T_ANY_FUNCTION, T_NULL},
	{T_NULL,			T_NULL},
};

int CExpression::GetRuleForToken(ExpToken t)
{
	for (int i = 0; i < EXP_NUM_RULES; i++)
		if (t == ExpValidationRules[i][0])
			return i;

	return EXP_NUM_RULES;
}

bool CExpression::TokenFitsRule(ExpToken t, ExpToken rule)
{
	if (t == rule)
		return true;

	if (rule == T_ANY_BINARY_OPERATOR) {
		switch (t) {
		case T_ADD:		case T_SUBTRACT:		case T_MULTIPLY:		case T_DIVIDE:		case T_MOD:
		case T_POWER:		case T_AND:		case T_OR:		case T_AT:		case T_LESS:		case T_EQUAL:
		case T_NOTEQUAL:		case T_LESSEQUAL:	case T_GREATER:		case T_GREATEREQUAL:
			return true;
		}
	}

	if (rule == T_ANY_FUNCTION) {
		switch (t) {
		case T_SIN: case T_COS: case T_TAN: case T_SQRT: case T_FUNC_INT: case T_FUNC_FLOAT:
		case T_FUNC_STR: case T_ASIN: case T_ACOS: case T_ATAN: case T_ABS: case T_EXP: case T_LN:
		case T_LOG10: case T_ROUND: case T_RANDOM: case T_LEN: case T_CEIL:
			return true;
		default:
			return false;
		}
	}

	if (rule == T_ANY_VALUE && (t == T_IDENTIFIER || t == T_INTEGER || t == T_FLOAT || t == T_STRINGLITERAL || t == T_VARIABLENAME || t == T_LEFTBRACKET || t == T_LEFTCURLY))
		return true;

	return false;
}

// Validate the tokenised expression
#ifndef RUNTIME
void CExpression::Validate(int& chpos)
{
	chpos = -1;

	ExpToken curTok;
	ExpToken prevTok;

	// Null entry not allowed
	if (toks.empty()) {
		chpos = 0;
		throw runtime_error("You must enter an expression");
	}

	// If first or last tokens are a binary operator, disallow
	if (TokenFitsRule(toks.front().t, T_ANY_BINARY_OPERATOR) || toks.front().t == T_DOT) {
		chpos = TokenToChar(0);
		CString msg = "'";
		msg += toks.front().str;
		msg += "' not valid here";
		throw runtime_error((const char*)msg);
	}
	// If first or last tokens are a binary operator, disallow
	if (TokenFitsRule(toks.back().t, T_ANY_BINARY_OPERATOR) || toks.back().t == T_DOT) {
		chpos = TokenToChar((int)toks.size() - 1);
		CString msg = "'";
		msg += toks.back().str;
		msg += "' not valid here";
		throw runtime_error((const char*)msg);
	}


	int LBrackets = 0, RBrackets = 0, LastLBracket = 0, LastRBracket = 0;
	int LBrackets2 = 0, RBrackets2 = 0, LastLBracket2 = 0, LastRBracket2 = 0;

	for (int i = 0; i < toks.size(); i++) {

		if (toks[i].t == T_LEFTBRACKET) {
			LBrackets++;
			LastLBracket = i;
		}
		if (toks[i].t == T_RIGHTBRACKET) {
			RBrackets++;
			LastRBracket = i;

			// Too many close brackets
			if (RBrackets > LBrackets) {
				chpos = TokenToChar(i);
				throw runtime_error("Too many close brackets");
			}
		}

		if (toks[i].t == T_LEFTCURLY) {
			LBrackets2++;
			LastLBracket2 = i;
		}
		if (toks[i].t == T_RIGHTCURLY) {
			RBrackets2++;
			LastRBracket2 = i;

			// Too many close brackets
			if (RBrackets2 > LBrackets2) {
				chpos = TokenToChar(i);
				throw runtime_error("Too many closing '}'");
			}
		}

		// Identifier token
	if (i == 0 || toks[i-1].t != T_DOT)
		if (toks[i].t == T_IDENTIFIER) {

			// Tokens follow
			if (i + 1 < toks.size()) {

				// Not part of object.*
				if (toks[i+1].t != T_DOT) {

					// Invalid unless sys exp
					if (!SystemExpressionExists(toks[i].str) && !ObjectHasDefaultValue(toks[i].str)) {
						chpos = TokenToChar(i);
						CString err = "'";
						err += toks[i].str;
						err += "' is not a recognised system expression or usable object name.";
						throw runtime_error((const char*)err);
					}
				}
				// Next token is dot: check object name exists
				else if (!ObjectNameExists(toks[i].str) && !StrContains(toks[i].str, '[')) {
					chpos = TokenToChar(i);
					CString err = "'";
					err += toks[i].str;
					err += "' is not an object name";
					throw runtime_error((const char*)err);
				}
			}//if not last token
			// Last token
			else {
				// Invalid unless sys exp
				if (!SystemExpressionExists(toks[i].str) && !ObjectHasDefaultValue(toks[i].str) && !StrContains(toks[i].str, '[')) {
					chpos = TokenToChar(i);
					CString err = "'";
					err += toks[i].str;
					err += "' is not a recognised expression";
					throw runtime_error((const char*)err);
				}
			}
		}//if identifier

		// Rest of code is not safe for i=0
		if (i == 0) continue;

		// If identifier preceded by dot
		if (toks[i].t == T_IDENTIFIER && toks[i-1].t == T_DOT) {
			// Check expression name exists
			if (!ObjectExpressionExists(toks[i-2].str, toks[i].str)) {
				chpos = TokenToChar(i);
				CString err = "'";
				err += toks[i].str;
				err += "' is not an expression in '";
				err += toks[i-2].str;
				err += "'";
				throw runtime_error((const char*)err);
			}
		}

		curTok = toks[i].t;
		prevTok = toks[i - 1].t;

		int rule = GetRuleForToken(prevTok);
		int step = 1;
		bool allowed = false;

		while (ExpValidationRules[rule][step] != T_NULL) {

			if (TokenFitsRule(curTok, ExpValidationRules[rule][step])) {
				allowed = true;
				break;
			}
			step++;

		}//while

		// This token combo not allowed
		if (!allowed) {
			CString err = "'";
			err += toks[i].str;
			err += "' is not allowed after '";
			err += toks[i-1].str;
			err += "'";
			chpos = TokenToChar(i);
			throw runtime_error((const char*)err);
		}

	}//for

	// Some unclosed brackets
	if (LBrackets > RBrackets) {

		// Find the unclosed bracket
		int LBracketNumber = 0;
		int tokenNumber = 0;

		int i;
		for (i = toks.size() - 1; i >= 0; i--) {
			if (toks[i].t == T_LEFTBRACKET)
				LBracketNumber--;
			if (toks[i].t == T_RIGHTBRACKET)
				LBracketNumber++;
			if (LBracketNumber == -1)
				break;
		}

		chpos = TokenToChar(i);

		throw runtime_error("Too many open brackets");
	}

	// Some unclosed brackets
	if (LBrackets2 > RBrackets2) {

		// Find the unclosed bracket
		int LBracketNumber = 0;
		int tokenNumber = 0;

		int i;
		for (i = toks.size() - 1; i >= 0; i--) {
			if (toks[i].t == T_LEFTCURLY)
				LBracketNumber--;
			if (toks[i].t == T_RIGHTCURLY)
				LBracketNumber++;
			if (LBracketNumber == -1)
				break;
		}

		chpos = TokenToChar(i);

		throw runtime_error("Too many opening '{'");
	}

}
#endif

CExpression::CExpression()
{
	ExpTree = NULL;
	Depth = 0;
	SolID = 0;
	pSelectedObject = NULL;
	// Constant until proven variable
	isConstant = true;
	constantValue.eType = EXPTYPE_NULL;
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

#define CSTR_BUF(a) ((const char*)a)

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

		// Check object name exists
		if (!ObjectNameExists(ident)) {
			CString msg;
			msg.Format("'%s' is not an object name", ident);
			throw runtime_error((const char*)msg);
		}
		// Check this movement name exists
		if (!ObjectBehaviorExists(ident, movementName)) {
			CString msg;
			msg.Format("'%s' is not a behavior in '%s'", movementName, ident);
			throw runtime_error((const char*)msg);
		}
	}
}

// Lookup a function name
void CExpression::LookupExtExpression(int oid, CString fname, ExpDot* pDot)
{
#ifdef RUNTIME
	CRunObjType* pType = pRuntime->pOid(oid);

	fname.MakeLower();

	vector<CString>* pvNames = pType->ext->pvExpNames;

	// Check against all expression names
	for (int i = 0; i < pvNames->size(); i++) {

		// Name matches
		CString n = pvNames->at(i);
		if (n == fname) {

			// Get corresponding expression routine
			pDot->expRoutine = pType->ext->pvExpRoutines->at(i);
			return;
		}
	}

	// Not found; error
	CString msg = "Error:  expression \"" + fname + "\" not found in object \"";
	msg += pType->Name;
	msg += "\"";
	throw runtime_error((const char*)msg);
#endif
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
#define NEW_UNARY_OPERATOR(a, b) new a; doUnaryExpPart(this, step, LeftPart, b); b->p = DoTree(step, true)

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

	Token curTok;

	while (step < toks.size()) {

		curTok = toks[step];

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
			pInt->value = _atoi64(CSTR_BUF(curTok.str));

			// Return on close bracket or comma
			CHECK_TERMINATE();

			break;

		case T_FLOAT:

			pFloat = NEW_UNARY_EXP(ExpFloat, pFloat);
			pFloat->value = atof(CSTR_BUF(curTok.str));

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

			pVariableName = NEW_UNARY_EXP(ExpVariableName, pVariableName);

			// Store var name
			pVariableName->name = curTok.str;
			pVariableName->name.MakeLower();
			
			// Return on close bracket or comma
			CHECK_TERMINATE();

			break;

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
			if(!LeftPart)
			{
				throw runtime_error("Syntax Error: Conditional operator a ? b : c\npart a is missing");
			}
			step++;

			// Get TRUE part (b)
			/*if (toks[step].t == T_LEFTBRACKET)
				pConditional->b = DoBrackets(step);
			else*/
				pConditional->b = DoTree(step);
				if(!pConditional->b)
				{
					throw runtime_error("Syntax Error: Conditional operator a ? b : c\npart b is missing");
				}
				pConditional->b = PostSortTree(pConditional->b);

			// Step over colon
			step++;

			// Get FALSE part (c)
			/*if (toks[step].t == T_LEFTBRACKET)
				pConditional->c = DoBrackets(step);
			else*/
				pConditional->c = DoTree(step);
				if(!pConditional->c)
				{
					throw runtime_error("Syntax Error: Conditional operator a ? b : c\npart c is missing");
				}
				pConditional->c = PostSortTree(pConditional->c);

			// (never has precedence)

			return pConditional;

			// Identifier, eg. call without a dot: Object Object(5) MouseX
		case T_IDENTIFIER:

			// Idents cause variability
			isConstant = false;

			pIdent = NEW_UNARY_EXP(ExpIdent, pIdent);
			pIdent->ident = curTok.str;
			//pIdent->pRuntime = pRuntime;
			//pIdent->pSystemObject = &(pRuntime->system);

			// This will store the correct OID for this name, else -1 and load an expression routine
			LookupIdentifier(curTok.str, pIdent);

			// Check for OID caller match
			pIdent->isCallerOid = (pIdent->oid == this->oidCaller);

			// True if dot precedes this identifier (bit of a hack, but hey)
			// We can't check for unary mode, because unary operators use it and it would break "sin Array(1)"
			dotIdentifier = false;
			if (step > 1) if (toks[step-2].t == T_DOT) dotIdentifier = true;

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
					pIdent->parameters.push_back(param);
				}

				// Step over right bracket
				step++;
			}

			// Now we have params parsed, but if this isn't a system expression, there isn't yet a routine stored for it.
			// This means it is the default return format, eg. Array(5).  Use the Default Return Value address.
			if (this->ObjectNameExists(pIdent->ident)) {
				pIdent->pType = 1;	// nonzero
				//pIdent->pTypeInstances = &(pIdent->pType->instances);
				//pIdent->routine = &CRunObject::ReturnDefaultValue;
			}
			else
				pIdent->pType = NULL;

			pIdent->hasParameters = !pIdent->parameters.empty();

			// Return on close bracket or comma
			CHECK_TERMINATE();

			// Return if function call (followed by lbracket)
			if (toks[step].t == T_LEFTBRACKET)
				return LeftPart;

			break;

		case T_DOT:

			// Plugin returns cause variability
			isConstant = false;

			pDot = new ExpDot;
			pDot->t = T_DOT;
			// Object name part
			pDot->l = (ExpIdent*)LeftPart;
			pDot->pCExp = this;
			//pDot->pRuntime = pRuntime;
			pDot->bracketdepth = Depth;
			pDot->oid = ((ExpIdent*)(pDot->l))->oid;
			//pDot->pType = pRuntime->objects[pDot->oid];
			//pDot->pTypeInstances = &(pDot->pType->instances);
			pDot->unnamedExp = false;
			pDot->objName = ((ExpIdent*)(pDot->l))->ident;

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
					pDot->parameters.push_back(param);
				}

				// Step over right bracket
				step++;
			}

			// Has parameters?
			pDot->hasParameters = !pDot->parameters.empty();

			// Store function name for type checker
			pDot->fname = ((ExpIdent*)(pDot->r))->ident;

			// Dot will have precedence over most operators
			if (HasPrecedence(pDot, pDot->r)) {

				ExpPart* newParent = SwapTree(pDot);

				// Look up the plugin expression routine
				LookupExtExpression(((ExpIdent*)(pDot->l))->oid, ((ExpIdent*)(pDot->r))->ident, pDot);

				return newParent;
			}

			// Look up the plugin expression routine
			LookupExtExpression(((ExpIdent*)(pDot->l))->oid, ((ExpIdent*)(pDot->r))->ident, pDot);

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
			{
				CString msg = "Syntax error: '";
				msg += curTok.str;
				msg += "' in expression:\n\n";
				CString userstr;
				GetUserString(userstr);
				msg += userstr;
				throw runtime_error((const char*)msg);
			}

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