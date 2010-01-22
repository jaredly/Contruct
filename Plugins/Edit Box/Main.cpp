// Main.cpp - Main implementation of object
//

// Include StdAfx
#include "StdAfx.h"

// If run only
#ifdef RUN_ONLY

//////////////////////////////////////////////////////////////////////////////////
// Conditions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::cOnChanged(LPVAL theParams)
{
	return TRUE;
}

long ExtObject::cCompareText(LPVAL params)
{
	CString l = params[0].GetString();

	CString text;
	edit.GetWindowText(text);

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

long ExtObject::cOnMaxText(LPVAL theParams)
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
// Actions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::aSetText(LPVAL theParams)
{
	CString text = theParams[0].GetString();
	text.Replace("\n", "\r\n");
	edit.SetWindowText(text);
	edit.Invalidate();

	return 0;
}

long ExtObject::aAppendText(LPVAL theParams)
{
	CString Text;
	edit.GetWindowText(Text);
	CString toAdd = theParams[0].GetString();
	toAdd.Replace("\n", "\r\n");

	Text += toAdd;

	edit.SetWindowText(Text);
	edit.Invalidate();

	return 0;
}

long ExtObject::aDisable(LPVAL theParams)
{
	edit.EnableWindow(false);
	edit.Invalidate();

	return 0;
}

long ExtObject::aEnable(LPVAL theParams)
{
	edit.EnableWindow(true);
	edit.Invalidate();

	return 0;
}

long ExtObject::aFocusOn(LPVAL theParams)
{
	edit.SetFocus();

	return 0;
}

long ExtObject::aFocusOff(LPVAL theParams)
{
	SetFocus(pRuntime->GetAppHwnd());

	return 0;
}

long ExtObject::aClearSelection(LPVAL theParams)
{
	edit.SetSel(-1, -1);

	return 0;
}

long ExtObject::aSelectAll(LPVAL theParams)
{
	edit.SetSel(0, -1);

	return 0;
}

long ExtObject::aSetSelection(LPVAL theParams)
{
	edit.SetSel(GetIntParam(theParams, 0), GetIntParam(theParams, 1));

	return 0;
}

long ExtObject::aCopy(LPVAL theParams)
{
	edit.Copy();

	return 0;
}

long ExtObject::aCut(LPVAL theParams)
{
	edit.Cut();

	return 0;
}

long ExtObject::aPaste(LPVAL theParams)
{
	edit.Paste();

	return 0;
}

long ExtObject::aLimitText(LPVAL theParams)
{
	edit.LimitText(GetIntParam(theParams, 0));

	return 0;
}

long ExtObject::aUndo(LPVAL theParams)
{
	edit.Undo();

	return 0;
}

long ExtObject::aClearUndoBuffer(LPVAL theParams)
{
	edit.EmptyUndoBuffer();

	return 0;
}

long ExtObject::aReplaceSelection(LPVAL theParams)
{
	edit.ReplaceSel(GetStringParam(theParams, 0), TRUE);

	return 0;
}

long ExtObject::aLoadFile(LPVAL theParams)
{
	CString text;

	FILE* pFile = fopen(GetStringParam(theParams, 0), "rb");
	fseek(pFile, 0, SEEK_END);
	int Length = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	char* Read = new char[Length + 1];
	fread(Read, 1, Length, pFile);
	Read[Length] = '\0';	// null terminate

	text = Read;
	delete[] Read;

	fclose(pFile);

	edit.SetWindowText(text);

	return 0;
}

long ExtObject::aSaveFile(LPVAL theParams)
{
	CString text;
	edit.GetWindowText(text);

	FILE* pFile = fopen(GetStringParam(theParams, 0), "w+");
	fwrite((LPCSTR)text, text.GetLength(), 1, pFile);
	fclose(pFile);

	return 0;
}

long ExtObject::aSetReadOnly(LPVAL theParams)
{
	if (GetIntParam(theParams, 0) == 0)
		edit.SetReadOnly();
	else
		edit.SetReadOnly(FALSE);

	return 0;
}

long ExtObject::aDestroy(LPVAL params)
{
	pRuntime->DestroyObject(this);
	return 0;
}

long ExtObject::aSetVisible(LPVAL params)
{
	bool show = params[0].GetBool();
	edit.ShowWindow(show ? SW_SHOW : SW_HIDE);
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
// Expressions
//////////////////////////////////////////////////////////////////////////////////

long ExtObject::eGetText(LPVAL theParams, ExpReturn& ret)
{
	CString text;
	edit.GetWindowText(text);
	return ret.ReturnString(pRuntime, text);
}

long ExtObject::eGetLineCount(LPVAL theParams, ExpReturn& ret)
{
	return ret = edit.GetLineCount();
}

long ExtObject::eGetTextLimit(LPVAL theParams, ExpReturn& ret)
{
	return ret = edit.GetLimitText();
}

long ExtObject::eGetSelectionStart(LPVAL theParams, ExpReturn& ret)
{
	int Start = 0;
	int End = 0;
	edit.GetSel(Start, End);

	return ret = Start;
}

long ExtObject::eGetSelectionEnd(LPVAL theParams, ExpReturn& ret)
{
	int Start = 0;
	int End = 0;
	edit.GetSel(Start, End);

	return ret = End;
}

long ExtObject::eGetSelection(LPVAL theParams, ExpReturn& ret)
{
	int Start = 0;
	int End = 0;
	edit.GetSel(Start, End);

	return ret = (End - Start);
}


#endif // #ifdef RUN_ONLY

//////////////////////////////////////////////////////////////////////////////////
// ACE tables
//////////////////////////////////////////////////////////////////////////////////

void DefineACES(MicroAceTime* at)
{
	/////////////////////////////
	// Conditions
	ADDCND("On text changed", "Notifications", "%o On text changed", &ExtObject::cOnChanged, "OnTextChanged", CND_TRIGGERED);

	ADDPARAM(PARAM_STRING, "Text to compare", "Enter the text to compare with the object's content");
	ADDPARAMDEF(PARAM_COMBO, "Case sensitive?", "Choose whether or not capital letters count as different", "No|Yes");
	ADDCND("Compare text", "Text", "%o Text is %0 (case: %1)", &ExtObject::cCompareText, "CompareText", 0);

	ADDCND("On text limit reached", "Notifications", "%o On text limit reached", &ExtObject::cOnMaxText, "OnTextLimitReached", CND_TRIGGERED);

	/////////////////////////////
	// Actions
	ADDACT("Disable", "Control", "Disable", &ExtObject::aDisable, "Disable", 0);
	ADDACT("Enable", "Control", "Enable", &ExtObject::aEnable, "Enable", 0);
	ADDACT("Focus on", "Control", "Focus on", &ExtObject::aFocusOn, "FocusOn", 0);
	ADDACT("Focus off", "Control", "Focus off", &ExtObject::aFocusOff, "FocusOff", 0);

	ADDPARAM(PARAM_STRING, "New text", "Enter the new edit text");
	ADDACT("Set text", "Text", "Set text to %0", &ExtObject::aSetText, "SetText", 0);

	ADDPARAM(PARAM_STRING, "Text", "Text to append");
	ADDACT("Append text", "Text", "Append text %0", &ExtObject::aAppendText, "AppendText", 0);

	ADDACT("Clear selection", "Control", "Clear selection", &ExtObject::aClearSelection, "ClearSelection", 0);
	ADDACT("Select all", "Control", "Select all", &ExtObject::aSelectAll, "SelectAll", 0);

	ADDPARAM(PARAM_VALUE, "Start", "Start of selection");
	ADDPARAM(PARAM_VALUE, "End", "End of selection");
	ADDACT("Set selection", "Control", "Select characters %0 to %1", &ExtObject::aSetSelection, "SetSelection", 0);

	// Copy
	ADDACT("Copy", "Control", "Copy", &ExtObject::aCopy, "Copy", 0);

	// Cut
	ADDACT("Cut", "Control", "Cut", &ExtObject::aCut, "Cut", 0);

	// Paste
	ADDACT("Paste", "Control", "Paste", &ExtObject::aPaste, "Paste", 0);

	// Undo
	ADDACT("Undo", "Control", "Undo", &ExtObject::aUndo, "Undo", 0);

	// Clear undo buffer
	ADDACT("Clear undo buffer", "Control", "Clear undo buffer", &ExtObject::aClearUndoBuffer, "ClearUndo", 0);

	// Limit text
	ADDPARAM(PARAM_VALUE, "Characters", "Character limit");
	ADDACT("Limit text length", "Control", "Limit text to %0 characters", &ExtObject::aLimitText, "LimitText", 0);

	// Replace selection
	ADDPARAM(PARAM_STRING, "Text", "Text to replace selection with");
	ADDACT("Replace selection", "Control", "Replace selection with %0", &ExtObject::aReplaceSelection, "ReplaceSelection", 0);

	// Load from file
	ADDPARAM(PARAM_STRING, "Path", "File path to load from");
	ADDACT("Load file", "Files", "Load file %0", &ExtObject::aLoadFile, "LoadFile", 0);

	// Save to file
	ADDPARAM(PARAM_STRING, "Path", "File path to save to");
	ADDACT("Save file", "Files", "Save file %0", &ExtObject::aSaveFile, "SaveFile", 0);

	// Set read only
	ADDPARAMCOMBO("Setting", "Enable/disable read only", "On|Off");
	ADDACT("Set read-only", "Control", "Set read only %0", &ExtObject::aSetReadOnly, "SetReadOnly", 0);

	ADDACT("Destroy", "Misc", "Destroy", &ExtObject::aDestroy, "Destroy", 0);

	ADDPARAMCOMBO("Visibility", "Set to show or hide the control.", "Invisible|Visible");
	ADDACT("Set visible", "Control", "Set %0", &ExtObject::aSetVisible, "SetVisible", 0);

	/////////////////////////////
	// Expressions

	ADDEXP("Get text", "Text", "Text", &ExtObject::eGetText, RETURN_STRING);
	ADDEXP("Get line count", "Text", "LineCount", &ExtObject::eGetLineCount, RETURN_VALUE);
	ADDEXP("Get text limit", "Text", "TextLimit", &ExtObject::eGetTextLimit, RETURN_VALUE);
	ADDEXP("Get selection start", "Text", "SelectionStart", &ExtObject::eGetSelectionStart, RETURN_VALUE);
	ADDEXP("Get selection end", "Text", "SelectionEnd", &ExtObject::eGetSelectionEnd, RETURN_VALUE);
	ADDEXP("Get selection length", "Text", "SelectionLength", &ExtObject::eGetSelection, RETURN_VALUE);
}
