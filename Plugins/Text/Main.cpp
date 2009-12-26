// Main.cpp - Main implementation of object
//

// Include StdAfx
#include "StdAfx.h"

// If run only
#ifdef RUN_ONLY

#include "..\..\Common\CommonAceDef.hpp"

//////////////////////////////////////////////////////////////////////////////////
// Conditions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::cValueCmp(LPVAL params)
{
	const ExpStore& l = privateVars[params[0].GetVariableIndex(pRuntime, pType)];
	const ExpReturn& r = params[2];

	return DoComparison(params[1].GetInt(), (const ExpBase&)l, (const ExpBase&)r);
}

long ExtObject::cCompareText(LPVAL params)
{
	CString l = params[0].GetString();

	// Case sensitive?
	if (params[1].GetBool()) {
		return l == text;
	}
	// Case insensitive
	else {
		CString r = text;
		l.MakeLower();
		r.MakeLower();
		return l == r;
	}
}

long ExtObject::cWriteFinished(LPVAL params)
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
// Actions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::aSetText(LPVAL theParams)
{
	text = theParams[0].GetString();

	writing = false;

	if (pRuntime->GetRuntimeType() == 1)
		m_Text.SetWindowText(text);

	return 0;
}

long ExtObject::aWriteText(LPVAL theParams)
{
	text = "";
	write_text = theParams[0].GetString();

	current_letter = 0;
	write_speed = theParams[1].GetFloat() / 1000.0f;
	elapsed = 0;

	if (pRuntime->GetRuntimeType() == 1)
		m_Text.SetWindowText(text);
	else {
		pRuntime->CallOnFrame(this);
		writing = true;
	}

	return 0;
}

long ExtObject::aSetWriteSpeed(LPVAL theParams)
{
	write_speed = theParams[0].GetFloat() / 1000.0f;

	return 0;
}

long ExtObject::aSetTextColour(LPVAL theParams)
{
	COLORREF cref = theParams[0].GetColorRef();

	info.pInfo->filter = cr::color(cref);

	return 0;
}

long ExtObject::aSetTextSize(LPVAL theParams)
{
	int newSize = theParams[0].GetInt();
	Clamp(newSize, 1, 500);

	if (newSize != pxSize) {
		pxSize = newSize;
		UpdateFont();
	}

	return 0;
}

long ExtObject::aSetBold(LPVAL theParams)
{
	bool newBold = theParams[0].GetBool();

	if (newBold != (bold!=0)) {
		bold = newBold;
		UpdateFont();
	}

	return 0;
}

long ExtObject::aSetItalic(LPVAL theParams)
{
	bool newItalic = theParams[0].GetBool();

	if (newItalic != (italics!=0)) {
		italics = newItalic;
		UpdateFont();
	}

	return 0;
}

long ExtObject::aSetHorizAlign(LPVAL theParams)
{
	int horizAlign = theParams[0].GetInt();

	dwFlags &= ~DT_LEFT;
	dwFlags &= ~DT_CENTER;
	dwFlags &= ~DT_RIGHT;

	switch (horizAlign) {
		case 0: dwFlags |= DT_LEFT; break;
		case 1: dwFlags |= DT_CENTER; break;
		case 2: dwFlags |= DT_RIGHT; break;
	}

	return 0;
}

long ExtObject::aSetVertAlign(LPVAL theParams)
{
	int vertAlign = theParams[0].GetInt();

	dwFlags &= ~DT_TOP;
	dwFlags &= ~DT_VCENTER;
	dwFlags &= ~DT_BOTTOM;

	switch (vertAlign) {
		case 0: dwFlags |= DT_TOP; break;
		case 1: dwFlags |= DT_VCENTER; break;
		case 2: dwFlags |= DT_BOTTOM; break;
	}

	return 0;
}

long ExtObject::aSetValue(LPVAL params)
{
	// ExpStore operator=
	privateVars[params[0].GetVariableIndex(pRuntime, pType)] = params[1];

	return 0;
}

long ExtObject::aAddValue(LPVAL params)
{
	privateVars[params[0].GetVariableIndex(pRuntime, pType)] += params[1];

	return 0;
}

long ExtObject::aSubValue(LPVAL params)
{
	privateVars[params[0].GetVariableIndex(pRuntime, pType)] -= params[1];

	return 0;
}


long ExtObject::aSetFont(LPVAL params)
{
	CString newFont = params[0].GetString();

	if (newFont != fontface) {
		fontface = newFont;
		UpdateFont();
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
// Expressions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::eGetText(LPVAL theParams, ExpReturn& ret)
{
	return ret.ReturnString(pRuntime, text);
}

long ExtObject::eGetValue(LPVAL params, ExpReturn& ret)
{
	// Convert the value name to lowercase
	return ret.ReturnCustom(pRuntime, privateVars[params[0].GetVariableIndex(pRuntime, pType)]);
}

long ExtObject::eGetTextWidth(LPVAL params, ExpReturn& ret)
{
	cr::rect rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = 0;
	rc.bottom = 0;

	renderer->GetTextSize(iFont, GetStringParam(params, 0), rc);

	return ret = rc.right;
}

long ExtObject::eGetTextHeight(LPVAL params, ExpReturn& ret)
{
	cr::rect rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = 0;
	rc.bottom = 0;

	renderer->GetTextSize(iFont, GetStringParam(params, 0), rc);

	return ret = rc.bottom;
}

long ExtObject::eGetWriteSpeed(LPVAL theParams, ExpReturn& ret)
{
	return ret = write_speed * 1000.0f;
}

#endif // #ifdef RUN_ONLY

//////////////////////////////////////////////////////////////////////////////////
// ACE tables
//////////////////////////////////////////////////////////////////////////////////

void DefineACES(MicroAceTime* at)
{
	// Define your ACE tables here.
	// Use ADDPARAM before each ADDACT/ADDCND/ADDEXP to add formal parameters.
	
	// Format:
	// ADDPARAM(type, title, description)
	//
	// type = the type of parameter.  See the PARAMS enum.
	// title = title of parameter, in expression editor
	// description = text to go in the text box describing this parameter.  Try to give
	//				 a helpful description.

	/////////////////////////////
	// Conditions

	// Format:
	// ADDCND(menuID, routine, scriptname, displaytext, flags)
	//
	// menuID = ID in the menu resource, must start at 2000
	// routine = address of the function associated
	// scriptname = reserved for future script use, e.g. "Set X position" script name could be "SetX"
	// displaytext = text to display in event editor, %0 is first param, %1 second etc
	// flags: can be
	// 0: ordinary evaluated event
	// CND_TRIGGERED: ordinary event trigger


	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to test.");
	ADDPARAMCOMBO("Comparison", "Select the comparison to make", "Equal to|Not equal to|Less than|Less or equal|Greater than|Greater or equal");
	ADDPARAM(PARAM_VALUE, "Value", "Value to compare to");
	ADDCND("Comapre private variable", "Private variables", "%o Value %0 %1 %2", &ExtObject::cValueCmp, "CompareValue", 0);

	ADDPARAM(PARAM_STRING, "Text to compare", "Enter the text to compare with the object's content.");
	ADDPARAMDEF(PARAM_COMBO, "Case sensitive?", "Choose whether or not capital letters count as different.", "No|Yes");
	ADDCND("Compare text", "Text", "%o Text is %0 (case: %1)", &ExtObject::cCompareText, "CompareText", 0);

	ADDCND("Writing finished", "Text", "%o Text writing finished", &ExtObject::cWriteFinished, "WriteFinished", CND_TRIGGERED);

	/////////////////////////////
	// Actions

	// ADDACT(menuID, routine, scriptname, displaytext)
	// Same format as ADDCND, but actions have no flags.

	ADDPARAM(PARAM_STRING, "Text", "Text to display in object");
	ADDACT("*Set text", "Text", "Set text to %0", &ExtObject::aSetText, "SetText", 0);

	ADDPARAM(PARAM_COLOR, "Colour", "New text colour to display");
	ADDACT("Set text colour", "Text", "Set text colour to %0", &ExtObject::aSetTextColour, "SetTextColour", 0);

	ADDPARAM(PARAM_VALUE, "Text size", "New text size");
	ADDACT("Set text size", "Text", "Set text size to %0", &ExtObject::aSetTextSize, "SetTextSize", 0);

	ADDPARAMDEF(PARAM_COMBO, "Bold", "Whether or not to show bold text.", "No|Yes");
	ADDACT("Set bold", "Text", "Set bold: %0", &ExtObject::aSetBold, "SetBold", 0);

	ADDPARAMDEF(PARAM_COMBO, "Italics", "Whether or not to show italic text.", "No|Yes");
	ADDACT("Set italic", "Text", "Set italic: %0", &ExtObject::aSetItalic, "SetItalic", 0);

	ADDPARAMDEF(PARAM_COMBO, "Horizontal alignment", "Choose the text horizontal alignment.", "Left|Center|Right");
	ADDACT("Set horizontal alignment", "Text", "Set horizontal aligmnent to %0", &ExtObject::aSetHorizAlign, "SetHorizAlign", 0);

	ADDPARAMDEF(PARAM_COMBO, "Vertical alignment", "Choose the text vertical alignment.", "Top|Center|Bottom");
	ADDACT("Set vertical alignment", "Text", "Set vertical aligmnent to %0", &ExtObject::aSetVertAlign, "SetVertAlign", 0);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to modify.");
	ADDPARAM(PARAM_VALUE, "Value", "Value to store");
	ADDACT("Set value", "Private variables", "Set %0 to %1", &ExtObject::aSetValue, "SetValue", 0);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to modify.");
	ADDPARAM(PARAM_VALUE, "Value", "Value to add.");
	ADDACT("Add to value", "Private variables", "Add %1 to %0", &ExtObject::aAddValue, "AddValue", 0);

	ADDPARAM(PARAM_PRIVATEVARIABLE, "Private variable", "Select the private variable to modify.");
	ADDPARAM(PARAM_VALUE, "Value", "Value to subtract.");
	ADDACT("Subtract from value", "Private variables", "Subtract %1 from %0", &ExtObject::aSubValue, "SubValue", 0);

	ADDPARAM(PARAM_STRING, "Font face", "Enter the desired font, eg. \"Verdana\".");
	ADDACT("Set font", "Text", "Set font to %0", &ExtObject::aSetFont, "SetFont", 0);

	ADDPARAM(PARAM_STRING, "Text", "Text to write");
	ADDPARAM(PARAM_VALUE, "Speed", "Milliseconds per letter written");
	ADDACT("Write text", "Text", "Write text <i>%0</i> at 1 letter per <i>%1</i> milliseconds", &ExtObject::aWriteText, "WriteText", 0);

	ADDPARAM(PARAM_VALUE, "Speed", "Milliseconds per letter written");
	ADDACT("Set write speed", "Text", "Set write speed to 1 letter per <i>%0</i> milliseconds", &ExtObject::aSetWriteSpeed, "SetWriteSpeed", 0);

	/////////////////////////////
	// Expressions

	// ADDEXP(menuID, routine, expressionName, displaytext, flags)
	// Same format as conditions, except with the expression editor text.
	// expressionName = the name in the expression editor, e.g "Width" would
	// associate this expression when the user types YourObject.Width
	// Flags:  RETURN_VALUE, RETURN_STRING, RETURN_UNDEFINED

	ADDEXP("Get text", "Text", "Text", &ExtObject::eGetText, RETURN_STRING);

	ADDPARAM(PARAM_STRING, "Value", "Name of value");
	ADDEXP("Get private variable", "Private variables", "Value", &ExtObject::eGetValue, 0);

	ADDPARAM(PARAM_STRING, "Text", "Text to calculate");
	ADDEXP("Get text width", "Text size", "TextWidth", &ExtObject::eGetTextWidth, RETURN_VALUE);

	ADDPARAM(PARAM_STRING, "Text", "Text to calculate");
	ADDEXP("Get text height", "Text size", "TextHeight", &ExtObject::eGetTextHeight, RETURN_VALUE);

	ADDEXP("Get write speed", "Text", "GetWriteSpeed", &ExtObject::eGetWriteSpeed, RETURN_VALUE);

#include "..\..\Common\CommonAceTable.hpp"
}
