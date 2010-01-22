// Runtime.cpp - Runtime implementation of object
//

// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Runtime functions
//////////////////////////////////////////////////////////////////////////////////
#ifdef RUN_ONLY

// ExtObject constructor:
// Called when Construct creates an instance of your object.  Use your editTime data
// to initialise the object.
ExtObject::ExtObject(initialObject* editObject, VRuntime* pVRuntime)
{
	pRuntime = pVRuntime;
	info.editObject = editObject;

	info.x = editObject->eX;
	info.y = editObject->eY;
	info.w = editObject->eWidth;
	info.h = editObject->eHeight;
	info.angle = 0.0f;
}

// This is called just after the constructor, when Construct has added details to your
// object such as siblings and type pointer.  If you need this data on startup, it is
// valid to read here.
void ExtObject::OnCreate()
{
	bin ar;
	ar.attach(info.editObject->eData, info.editObject->eSize);

	int Version = 0;
	ar >> Version;

	CRect rc;
	rc.left = info.x;
	rc.top = info.y;
	rc.right = info.x + info.w;
	rc.bottom = info.y + info.h;

	// Read in your variables from the binary archive
	// (this is in the same format as the ETSerialize format)
	CString text;
	int autoHscroll, pass, enabled, Lowercase, Uppercase, Multiline, autoVscroll, vscroll, hscroll;
	ar>>text>>autoHscroll>>pass>>enabled>>Lowercase>>Uppercase>>vscroll>>hscroll>>Multiline>>autoVscroll;

	int border = 1;
	if (Version == 2)
		ar >> border;

	DWORD Border = WS_BORDER;
	if (border == 0) Border = 0;

	// Must be last line (after all reading code)!
	ar.detach();

	// Make sans serif font
	font.CreateFont(12, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");

	// Create edit
	CWnd parentWnd;
	parentWnd.Attach(pRuntime->GetFrameHwnd(pLayout));

	DWORD hScroll = autoHscroll ? ES_AUTOHSCROLL : 0;
	DWORD password = pass ? ES_PASSWORD : 0;
	DWORD Lower = Lowercase ? ES_LOWERCASE : 0;
	DWORD Upper = Uppercase ? ES_UPPERCASE : 0;
	DWORD vScroll = autoVscroll ? ES_AUTOVSCROLL : 0;
	DWORD vmultiline = Multiline ? ES_MULTILINE : 0;
	DWORD vvscroll = vscroll ? WS_VSCROLL : 0;
	DWORD vhscroll = hscroll ? WS_HSCROLL : 0;

	edit.Create(Lower | Upper | hScroll | vhscroll | vvscroll | password | vScroll | vmultiline | ES_LEFT | WS_CHILD | WS_VISIBLE | Border, rc, &parentWnd, 100);

	edit.SetFont(&font);
	edit.EnableWindow(enabled);
	edit.SetWindowText(text);
	edit.ShowWindow(SW_SHOW);
	edit.Invalidate();

	parentWnd.Detach();

	pRuntime->RegisterWindow(edit.GetSafeHwnd(), this);

	pRuntime->UpdateBoundingBox(this);
}

// Destructor: called when an instance of your object is destroyed.
ExtObject::~ExtObject()
{
	edit.DestroyWindow();
}

// Called every frame for you to update your object if necessary
// Return values: 1 (do not call RTOnFrame again), 0 (continue calling)
// The frame call order goes like this:
// run events -> OnFrame -> Draw -> OnFrame2 -> run events -> ...
BOOL ExtObject::OnFrame()
{
	edit.SetWindowPos(NULL, info.x, info.y, info.w, info.h, NULL);

	return 0;
}

BOOL ExtObject::OnFrame2()
{
	return 1;
}

// Draw: Construct calls this when it wants you to draw your object.  You can
// leave it blank if your extension doesn't draw anything.
void ExtObject::Draw()
{
	// Don't draw
}

// WindowProc:  called when a window message, or WM_COMMAND specifying your window,
// is sent to Construct.  You must call RegisterWindow() before Construct will send
// you messages for the given window.
LRESULT ExtObject::WindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage) {
	case WM_COMMAND:
		switch (HIWORD(wParam)) {
		case EN_CHANGE:
			pRuntime->GenerateEvent2(0);
			break;

		case EN_MAXTEXT:
			pRuntime->GenerateEvent2(1);
			break;
		}

		break;
	}

	return DefWindowProc(hWnd, uMessage, wParam, lParam);
}

BOOL ExtObject::PreTranslateMessage(MSG* msg)
{
	return 0;
}

// For global objects
void ExtObject::OnFrameChange(int oldFrame, int newFrame)
{
	// Do anything your global object needs when the frame changes
	// oldFrame is -1 on start of app, newFrame is -1 on end of app
}

void ExtObject::Serialize(bin& ar)
{
	int Version = 1;
	SerializeVersion(ar, Version);

	if (ar.loading) 
	{
		CString Text;
		ar >> Text;

		edit.SetWindowText(Text);
	}

	else
	{
		CString Text;
		edit.GetWindowText(Text);

		ar << Text;
	}
}

long ExtObject::GetData(int id, void* param)
{
	switch (id) {
	// If your extension uses a window, you can return it here
	case GET_WINDOW:
		return (long)&edit;
	}

	return 0;
}

long ExtObject::CallFunction(int id, void* param)
{
	return 0;
}

// Return the Default Value.  This is the value to be returned if your object name
// is used without an expression name, eg. in "Counter + 1".
// Parameters can be passed, eg. MyArray(3,7).
long ExtObject::ReturnDefaultValue(LPVAL theParams, ExpReturn& ret)
{
	CString text;
	edit.GetWindowText(text);
	return ret.ReturnString(pRuntime, text);
}

// Called for undefined expression names, if your extension allows it.
// Eg: MyObject.UndefinedExpression(3,4,5) calls this routine with
// "UndefinedExpression" as the expression name.
long ExtObject::ReturnUndefinedExpression(CString& expName, LPVAL theParams, ExpReturn& ret)
{
	return ret = 0;
}

// Debugging
void ExtObject::DebuggerUpdateDisplay(ExpStore*& pPrivateVars)
{
	CString text;
	edit.GetWindowText(text);
	pRuntime->AddDebuggerItem("Text", text, false);
}

void ExtObject::OnDebuggerValueChanged(const char* name, const char* value)
{
	// Convenience values
	CString label = name;
	CString text = value;

	if (label == "Text") {
		edit.SetWindowText(text);
	}
}

#else //ifdef RUN_ONLY

CRunObject* WINAPI RTCreateObject(bin& ar, VRuntime* pRuntime) 
{
	return NULL;
}

void WINAPI RTDestroyObject(ExtObject* object)
{
}

#endif