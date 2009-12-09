// Runtime.cpp - Runtime implementation of object
//

// Include StdAfx
#include "StdAfx.h"
#include "FontSize.h"

//////////////////////////////////////////////////////////////////////////////////
// Runtime functions
//////////////////////////////////////////////////////////////////////////////////
#ifdef RUN_ONLY

void ExtObject::UpdateFont()
{
	// Direct-X runtime only
	if (pRuntime->GetRuntimeType() == 2)
	{
		if (iFont != NULL)
			renderer->ReleaseCRFont(iFont);

		//CreateCRFont(PCTSTR font_face, INT height, UINT width = 0, UINT weight = FW_NORMAL, BOOL italic = false, DWORD quality = ANTIALIASED_QUALITY, BYTE charset = DEFAULT_CHARSET, BYTE pitch_and_family = DEFAULT_PITCH|FF_DONTCARE) = 0;
		iFont = renderer->CreateCRFont(fontface, pxSize, 0, bold ? FW_BOLD : FW_NORMAL, italics != 0);
	}

	// Application runtime only
	else
	{
		// Set font face/size
		m_Text.SetFont(fontface, GetFontPointSize(pxSize));

		// Set italics/bold
		//m_Text.SetItalic(italics != 0);
		//m_Text.SetBold(bold != 0);

		// Set colour
		m_Text.SetTextColor(m_Colour);
	}
}

// ExtObject constructor:
// Called when Construct creates an instance of your object.  Use your editTime data
// to initialise the object.
ExtObject::ExtObject(initialObject* editObject, VRuntime* pVRuntime)
: renderer(pVRuntime->pRenderer)
{
	pRuntime = pVRuntime;
	info.editObject = editObject;
}

// This is called just after the constructor, when Construct has added details to your
// object such as siblings and type pointer.  If you need this data on startup, it is
// valid to read here.
void ExtObject::OnCreate()
{
	COLORREF cref;

	float opacityf;
	int horizAlign, vertAlign;
	bool hide_at_start;

	bin ar;
	ar.attach(info.editObject->eData, info.editObject->eSize);

	int Version = 0;
	ar >> Version;

	ar>>text>>fontface>>pxSize>>italics>>bold>>cref>>opacityf>>horizAlign>>vertAlign;
	
	if (Version >= 2)
		ar >> hide_at_start;
	ar.detach();

	m_Colour = cref;

	info.x = info.editObject->eX;
	info.y = info.editObject->eY;
	info.w = info.editObject->eWidth;
	info.h = info.editObject->eHeight;
	info.HotSpotX = 0;
	info.HotSpotY = 0;
	info.HotSpotAngle = 0;
	info.HotSpotDist = 0;
	info.angle = 0.0f;
	info.displayangle = 0.0f;
	info.visible = true;
	info.collMode = COLLISIONMODE_BOX;

	// Direct-X runtime only
	if (pRuntime->GetRuntimeType() == 2)
	{
		iFont = NULL;

		info.pInfo->filter = cr::color(cref);
		info.pInfo->filter.a = opacityf;

		dwFlags = DT_WORDBREAK;

		switch (horizAlign) {
			case 0: dwFlags |= DT_LEFT; break;
			case 1: dwFlags |= DT_CENTER; break;
			case 2: dwFlags |= DT_RIGHT; break;
		}

		switch (vertAlign) {
			case 0: dwFlags |= DT_TOP; break;
			case 1: dwFlags |= DT_VCENTER; break;
			case 2: dwFlags |= DT_BOTTOM; break;
		}
	}

	// Application runtime only
	else
	{
		CWnd parentWnd;
		parentWnd.Attach(pRuntime->GetFrameHwnd(pLayout));

		m_Text.Create(text, WS_VISIBLE | WS_CHILD, OBJECTRECT, &parentWnd, 100);
		m_rc = OBJECTRECT; 

		CFont font;
		font.CreateFont(GetFontPointSize(pxSize), 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontface);
		m_Text.SetFont(&font);

		parentWnd.Detach();
	}

	// Setup private vars
	SetupPrivateVariableVector(pRuntime, this, privateVars);

	pRuntime->UpdateBoundingBox(this);

	UpdateFont();

	writing = false;
	write_speed = 0;

	if (hide_at_start) info.visible = false;
}

// Destructor: called when an instance of your object is destroyed.
ExtObject::~ExtObject()
{
	// DirectX runtime only
	if (pRuntime->GetRuntimeType() == 2) {
		if (iFont != NULL)
				renderer->ReleaseCRFont(iFont);
	}
}

// Called every frame for you to update your object if necessary
// Return values: 1 (do not call RTOnFrame again), 0 (continue calling)
// The frame call order goes like this:
// run events -> OnFrame -> Draw -> OnFrame2 -> run events -> ...
BOOL ExtObject::OnFrame()
{
	// Application runtime only
	if (pRuntime->GetRuntimeType() == 1)
	{
		CRect rc(info.x, info.y, info.x + info.w, info.y + info.h);
		if (rc != m_rc)
			m_Text.SetWindowPos(NULL, info.x, info.y, info.w, info.h, NULL);
		m_rc = rc;

		return 0;
	}

	if (writing)
	{
		elapsed += pRuntime->GetTimeDelta();

		// check if we've elapsed to the next
		if (elapsed >= write_speed)
		{
			current_letter++;
			elapsed -= write_speed;
		}

		if (current_letter == write_text.GetLength())
		{
			writing = false;
			pRuntime->GenerateEvent2(2);
		}

		text = write_text.Left(current_letter);

		if (writing)
			return 0;	// keep calling
		else
			return 1;	// done
	}
	else
		return 1;	// nothing to do
}

BOOL ExtObject::OnFrame2()
{
	return 1;
}

// Draw: Construct calls this when it wants you to draw your object.  You can
// leave it blank if your extension doesn't draw anything.
void ExtObject::Draw()
{
	// Direct-X runtime only
	if (pRuntime->GetRuntimeType() == 2)
	{
		RECTF rc;
		rc.left = info.x;
		rc.top = info.y;
		rc.right = info.x + info.w;
		rc.bottom = info.y + info.h;

		renderer->DrawTextCR(iFont, text, rc, dwFlags, info.pInfo->filter);
	}
}

// WindowProc:  called when a window message, or WM_COMMAND specifying your window,
// is sent to Construct.  You must call RegisterWindow() before Construct will send
// you messages for the given window.
LRESULT ExtObject::WindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
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

void ExtObject::DebuggerUpdateDisplay(class ExpStore*& pPrivateVars)
{
	pRuntime->AddDebuggerItem("Text", text, false);

	if (!privateVars.empty())
		pPrivateVars = &(privateVars[0]); 
}

void ExtObject::OnDebuggerValueChanged(const char* name, const char* value)
{
	// Check if a private variable was modified
	if (CheckDebugVariableChange(name, value, privateVars, this, pRuntime))
		return;

	CString label = name;

	if (label == "Text")
		text = value;
}

void ExtObject::Serialize(bin& ar)
{
	int Version = 1;
	SerializeVersion(ar, Version);

	if (ar.loading) {
		ar >> info;
		ar >> text >> fontface >> pxSize >> bold >> italics >> dwFlags
			>> privateVars;

		UpdateFont();

	}
	else {
		ar << info;
		ar << text << fontface << pxSize << bold << italics << dwFlags
			<< privateVars;
	}
}

long ExtObject::GetData(int id, void* param)
{
	switch (id) {
	// If your extension uses a window, you can return it here
	case GET_WINDOW:
		break;
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
	// No parameters: give text
	if (theParams[0].Type() == EXPTYPE_NULL)
		return ret.ReturnString(pRuntime, text);

	// A variable parameter:  acces pvs
	else if (theParams[0].Type() == EXPTYPE_VARIABLENAME)
		return ret.ReturnCustom(pRuntime, privateVars[theParams[0].GetVariableIndex(pRuntime, pType)]);

	else return ret = 0;
}

// Called for undefined expression names, if your extension allows it.
// Eg: MyObject.UndefinedExpression(3,4,5) calls this routine with
// "UndefinedExpression" as the expression name.
long ExtObject::ReturnUndefinedExpression(CString& expName, LPVAL theParams, ExpReturn& ret)
{
	return ret = 0;
}

#else //ifdef RUN_ONLY

CRunObject* WINAPI RTCreateObject(bin& ar, VRuntime* pRuntime) 
{
	return NULL;
}

void WINAPI RTDestroyObject(ExtObject* object)
{
	//
}

#endif