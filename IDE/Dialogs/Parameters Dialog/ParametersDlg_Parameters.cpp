/////////////////////////////////////////////////////////////////////////////
// Parameters dialog
/////////////////////////////////////////////////////////////////////////////

// Parameters file

// File includes
#include "StdAfx.h"
#include "..\..\Construct.h"
#include "..\ParametersDlg.h"
extern CMainFrame* g_MainFrame;

ObjectParameterButton::ObjectParameterButton(CApplication* application_, CLayout* layout_) :
	application(application_),
	layout(layout_)
{
}

void ObjectParameterButton::_OnClick(bool unused, bool unused_)
{
	// show object selector
	CString old;
	old.Format("%d", object);

	ObjectSelectorDialog select(application, layout, object, -1, true, true);

	select.CenterWindow(GetParent());

	if (select.DoModal() == IDOK)
	{
		if (select.selected.is_trait)
		{
			SetWindowText(select.selected.trait);
			this->SetIcon((HICON)NULL);

			object.Format("\"%s\"", select.selected.trait);
		}

		else if (select.selected.is_family)
		{
			SetWindowText(select.selected.family->name);

			// change the image
			CExtBitmap bitmap;
			bitmap.FromBitmap(select.selected.family->small_image.MakeBitmap());
			
			SetIcon(bitmap.CreateHICON());

			object.Format("%d", select.selected.family->identifier);
		}

		else
		{
			SetWindowText(select.selected.type->GetName());

			// change the image
			CExtBitmap bitmap;
			bitmap.FromBitmap(select.selected.type->small_image.MakeBitmap());
			
			SetIcon(bitmap.CreateHICON());

			object.Format("%d", select.selected.type->ObjectIdentifier);
		}
	}
}

// ScintillaSubclassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
// Subclass procedure 
LRESULT APIENTRY ScintillaSubclassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{ 
    if (uMsg == WM_GETDLGCODE) 
        return DLGC_HASSETSEL|DLGC_WANTCHARS|DLGC_WANTARROWS; // DLGC_WANTALLKEYS
 
    return CallWindowProc((WNDPROC)GetWindowLongPtr(hwnd, GWL_USERDATA), hwnd, uMsg, wParam, lParam);
} 

// CreateScintillaParameter(CWnd* pParent, LPCSTR DefaultText)
// Create a Scintilla parameter
CScintillaWnd* CParametersDlg::CreateScintillaParameter(CWnd* pParent, LPCSTR DefaultText, int iID)
{
	CScintillaWnd* pScintilla = new CScintillaWnd;
			
	pScintilla->Create("Scintilla", 
					   WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_VSCROLL, 
					   CRect(5, 86, 352, 185), pParent, 10000);

	pScintilla->Init();
		
	// Set colours and styles
	SetScintillaColour(pScintilla, 0xf8fff8);
	pScintilla->ID = iID;
	pScintilla->ModifyStyleEx(0, WS_EX_STATICEDGE, 0);
	pScintilla->SendMessage(SCI_SETWRAPMODE, SC_WRAP_WORD, 0);

	pScintilla->SendMessage(SCI_SETVSCROLLBAR, NULL, false);
	pScintilla->SendMessage(SCI_SETHSCROLLBAR, NULL, false);

	// Remove margins to the left
	pScintilla->SendMessage(SCI_SETMARGINWIDTHN, 0, 0);
	pScintilla->SendMessage(SCI_SETMARGINWIDTHN, 1, 0);

	// Tabbing
	pScintilla->SendMessage(SCI_ASSIGNCMDKEY, SCK_RETURN + (0 << 16), 0);
	pScintilla->SendMessage(SCI_AUTOCSETFILLUPS, 0, (LPARAM)"(),");
	pScintilla->SendMessage(SCI_AUTOCSETAUTOHIDE, FALSE, 0);

	pScintilla->SetLexer(SCLEX_PYTHON);
	pScintilla->SendMessage(SCI_STYLESETFORE, SCE_P_COMMENTLINE, RGB(0,128,0));
	pScintilla->SendMessage(SCI_STYLESETFORE, SCE_P_STRING, RGB(128,60,0));
	pScintilla->SendMessage(SCI_STYLESETFORE, SCE_P_CLASSNAME, RGB(0,0,0));
	pScintilla->SendMessage(SCI_STYLESETBOLD, SCE_P_CLASSNAME, 1);
	pScintilla->SendMessage(SCI_STYLESETFORE, SCE_P_DEFNAME, RGB(0,0,100));
	pScintilla->SendMessage(SCI_STYLESETBOLD, SCE_P_DEFNAME, 1);

	pScintilla->SendMessage(SCI_SETINDENTATIONGUIDES, FALSE, 0);
	pScintilla->SendMessage(SCI_SETINDENT,0,0);


	pScintilla->SetWindowText(DefaultText);






	return pScintilla;
}

// CreateColourParameter(CWnd* pParent, LPCSTR DefaultText)
// Create a colour parameter
CExtColorButton* CParametersDlg::CreateColourParameter(CWnd* pParent, int iDefaultColour)
{
	CExtColorButton* pColour = new CExtColorButton;
			
	pColour->Create("", WS_TABSTOP | WS_CHILD | WS_VISIBLE, CRect(0, 0, 352, 25), pParent, 10000);
	pColour->m_clrSelected = iDefaultColour;

	return pColour;
}

// CreateComboBoxParameter(CWnd* pParent)
// Create a combobox parameter
CExtComboBox* CParametersDlg::CreateComboBoxParameter(CWnd* pParent, LPCSTR Items, bool bVariable)
{
	int ID = 10000;
	if (bVariable) ID = 555;

	CExtComboBox* pComboBox = new CExtComboBox;
	pComboBox->Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL, 
					  CRect(5, 86, 352,25), pParent, ID);

	pComboBox->SetFont(GetFont());

	CString ToAdd = Items;
	
	// Parse items and original value
	char  TokenCharacters[] = "|";
	char* CurrentToken;	

 	CurrentToken = strtok(ToAdd.GetBuffer(0), TokenCharacters);

	while(CurrentToken != NULL)
	{
		pComboBox->AddString(CurrentToken);
		CurrentToken = strtok(NULL, TokenCharacters);
	}

	return pComboBox;
}

// CreateColourParameter(CWnd* pParent, LPCSTR DefaultText)
// Create a colour parameter
CHotKeyCtrl* CParametersDlg::CreateHotKeyParameter(CWnd* pParent, int ID)
{
	CHotKeyCtrl* pHotKey = new CHotKeyCtrl;
			
	pHotKey->Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 352, 25), pParent, 10000);
	pHotKey->SetHotKey((WORD)ID, NULL);

	pHotKey->SetRules(HKCOMB_A,0);
	pHotKey->SetFont(GetFont());

	return pHotKey;
}

// CheckParameterValid(const CString& In, CString& Message)
// Evaluate a parameter for validity.
void CParametersDlg::CreateParameter(int i, CWnd* pParent, TDS_PANE& Pane, int type, CString DefaultString)
{
	// Get parameter
	PARAM& Parameter = m_pACEEntry->params[i];

	// Get type
	int iType = Parameter.type;
	if (type != -1)
		iType = type;

	// Setup a default value for this parameter
	CString Default ;
	if(DefaultString == "")
		Default = strings[i];
	else
		Default = DefaultString;

	// There is no default value, create one based on type
	if (Default == "")
	{
		switch (iType)
		{
		case EDITORPARAM_INTEGER:
			Default = "0";
			break;
		case EDITORPARAM_STRING: 
			Default = "\"\"";
			break;
		case 11: 
			Default = "\"\""; 
			break; // NEED TO FIND OUT WHAT THIS IS
		default: 
			Default = "0"; 
			break;
		}
	}

	CWnd* content;

	// Do we require a scintilla control?
	if (iType == EDITORPARAM_USERPARAM
		      || iType == EDITORPARAM_INTEGER 
			  || iType == EDITORPARAM_STRING
			  || iType == EDITORPARAM_UNDEFINED
			  || iType == EDITORPARAM_ARRAY 
			  || iType == EDITORPARAM_LAYER)
	{
		CScintillaWnd* pScintillaWindow = CreateScintillaParameter(pParent, Default, i);

		content = pScintillaWindow;

		// Setup window proc
		WNDPROC wpOrigProc = (WNDPROC) SetWindowLongPtr(pScintillaWindow->GetSafeHwnd(), 
														GWL_WNDPROC, (LONG)ScintillaSubclassProc);

		SetWindowLongPtr(pScintillaWindow->GetSafeHwnd(), GWL_USERDATA, (LONG)wpOrigProc);

		m_scintillaList.push_back(pScintillaWindow->GetSafeHwnd());

		// Resize scintilla to default
		ResizeScintilla(*pScintillaWindow);
	}

	// Colour parameter
	if (iType == EDITORPARAM_COLOUR)
	{
		CExtColorButton* pColourWindow = CreateColourParameter(pParent, atoi(Default));

		content = pColourWindow;
	}

	// Object parameter
	if (iType == EDITORPARAM_OBJECT)
	{
		ObjectParameterButton* button = new ObjectParameterButton(application, layout);

		// set default data
		CObjType* type = application->FindObjTypeFromNumber(atoi(Default));

		button->Create("Pick an object", WS_CHILD | WS_VISIBLE, CRect(0, 0, 352, 25), pParent, 100);

		if (type)
		{
			// change the label on this button
			button->SetWindowText(type->GetName());

			// change the image
			CExtBitmap bitmap;
			bitmap.FromBitmap(type->small_image.MakeBitmap());
			button->SetIcon(bitmap.CreateHICON());
			
			button->object = Default;
		}
		else
		{
			vector<CString>::iterator i = application->traits.begin();
			for(;i!= application->traits.end(); i++)
			{
				CString trait;
				trait.Format("\"%s\"", *i);
				if(Default.CompareNoCase(trait) == 0)
				{
					Default = Default.Mid(1, Default.GetLength()-2);
					button->SetWindowText(Default);
					button->object = trait;				
				}
			}
		}


		button->SetFont(GetFont());

		content = button;
	}

	// Keyboard parameter
	if (iType == EDITORPARAM_KEYBOARD)
	{
		CHotKeyCtrl* pHotKeyCtrl = CreateHotKeyParameter(pParent, atoi(Default));

		content = pHotKeyCtrl;

		::SendMessage(pHotKeyCtrl->m_hWnd, 
                HKM_SETRULES, 
                (WPARAM) HKCOMB_NONE | HKCOMB_S,   // invalid key combinations 
                MAKELPARAM(HOTKEYF_ALT, 0));       // add ALT to invalid entries 
	}

	// ComboBox parameter
	if (iType == EDITORPARAM_COMBOBOX)
	{
		CExtComboBox* pComboBox = CreateComboBoxParameter(pParent, Parameter.initStr);

		int pos = atoi(Default);
		if(pos < 0 || pos >= pComboBox->GetCount()){
			pos = 0;
		}
		pComboBox->SetCurSel(pos);
		

		content = pComboBox;
	}

	// Private value parameter
	if (iType == EDITORPARAM_PRIVATEVARIABLE)
	{
		CExtComboBox* pComboBox = CreateComboBoxParameter(pParent, NULL, true);

		content = pComboBox;
		Pane.use_private_data = true;

		int SelectedID = atoi(Default);

		// Add items and select initial item
		for (int i = 0; i < m_ID->m_PrivateValues.size(); i++)
		{
			pComboBox->AddString(m_ID->m_PrivateValues[i].name);
			pComboBox->SetItemData(i, m_ID->m_PrivateValues[i].identifier);

			if(m_ID->m_PrivateValues[i].identifier == SelectedID)
				pComboBox->SetCurSel(i);
		}

		pComboBox->AddString("<Add new...>");

		m_Stack.layout = layout;
	}

	// Menu resource parameter
	if (iType == EDITORPARAM_MENURESOURCE)
	{
		CExtComboBox* pComboBox = CreateComboBoxParameter(pParent);

		SetWindowLong(pComboBox->m_hWnd, GWL_USERDATA, 15);

		// Add menus
		for(list<MenuResource>::iterator i = application->resources.menu_resources.begin(); 
			i != application->resources.menu_resources.end(); i++)
			pComboBox->AddString(i->GetName());

		content = pComboBox;
	}

	// file parameter
	if (iType == EDITORPARAM_BINARYRESOURCE)
	{
		CExtComboBox* pComboBox = CreateComboBoxParameter(pParent);

		SetWindowLong(pComboBox->m_hWnd, GWL_USERDATA, 15);

		// Add binary resources
		for(list<BinaryResource>::iterator i = application->resources.file_resources.begin();
			i != application->resources.file_resources.end(); i++)
			pComboBox->AddString(i->GetName());

		Default.Remove('\"');

		if (pComboBox->FindStringExact(0, Default) != CB_ERR)
			pComboBox->SetCurSel(pComboBox->FindStringExact(0, Default));
		else
			pComboBox->SetCurSel(0);

		Pane.keep_string = true;
		content = pComboBox;
	}

	// music parameter
	if (iType == EDITORPARAM_MUSIC)
	{
		CExtComboBox* pComboBox = CreateComboBoxParameter(pParent);

		SetWindowLong(pComboBox->m_hWnd, GWL_USERDATA, 15);

		for(list<BinaryResource>::iterator i = application->resources.music_resources.begin();
			i != application->resources.music_resources.end(); i++)
			pComboBox->AddString(i->GetName());

		Default.Remove('\"');

		if (pComboBox->FindStringExact(0, Default) != CB_ERR)
			pComboBox->SetCurSel(pComboBox->FindStringExact(0, Default));
		else
			pComboBox->SetCurSel(0);

		Pane.keep_string = true;
		content = pComboBox;
	}

	// sound parameter
	if (iType == EDITORPARAM_SOUND)
	{
		CExtComboBox* pComboBox = CreateComboBoxParameter(pParent);

		SetWindowLong(pComboBox->m_hWnd, GWL_USERDATA, 15);

		for(list<BinaryResource>::iterator i = application->resources.sound_resources.begin();
			i != application->resources.sound_resources.end(); i++)
			pComboBox->AddString(i->GetName());

		Default.Remove('\"');

		if (pComboBox->FindStringExact(0, Default) != CB_ERR)
			pComboBox->SetCurSel(pComboBox->FindStringExact(0, Default));
		else
			pComboBox->SetCurSel(0);

		Pane.keep_string = true;
		content = pComboBox;
	}

	// Transition parameter
	if (iType == EDITORPARAM_TRANSITION)
	{
		CExtComboBox* pComboBox = CreateComboBoxParameter(pParent);

		// Add default, no transition
		pComboBox->AddString("None");

		// Add transitions in application
		list<CTransition>::iterator i = application->m_Transitions.begin();
		for (; i != application->m_Transitions.end(); i++)
			pComboBox->AddString(i->m_Name);

		Default.Remove('\"');
		if (pComboBox->FindStringExact(0, Default) != CB_ERR)
			pComboBox->SetCurSel(pComboBox->FindStringExact(0, Default));
		else
			pComboBox->SetCurSel(0);

		Pane.keep_string = true;
		content = pComboBox;
	}

	// Global variable parameter
	if (iType == EDITORPARAM_GLOBALVARIABLE)
	{
		CExtComboBox* pComboBox = CreateComboBoxParameter(pParent, NULL, true);

		int SelectedID = atoi(Default);

		for(list<CApplication::GlobalVariable>::iterator i = application->global_variables.begin(); 
			i !=  application->global_variables.end(); i++)
		{
			pComboBox->AddString(i->name);
			pComboBox->SetItemData(pComboBox->GetCount() - 1, i->identifier);

			// If it's this one, select it
			if(i->identifier == SelectedID)
				pComboBox->SetCurSel(pComboBox->GetCount() - 1);
		}

		pComboBox->AddString("<Add new...>");

		Pane.use_private_data = true;

		m_Stack.layout = layout;

		content = pComboBox;
	}

	Pane.SetParameter(iType, content);
}

// CheckParameterValid(const CString& In, CString& Message)
// Evaluate a parameter for validity.
bool CParametersDlg::CheckParameterValid(const CString& In, CString& Message)
{
	int chPos;
	bool Valid = true;

	// To link 'Self' to the owner object
	m_TypeChecker.m_UndefinedVariables.clear();
	m_TypeChecker.pCallerType = m_selfType;

	try {
		m_TypeChecker.ParseUserString(In, &chPos);
	}

	catch (runtime_error e) {
		Message = e.what();
		return false;
	}

	catch (...)	{
		Message = "Unparsable expression.  Please check your expression is correctly written.";
		return false;
	}

	try {
		m_TypeChecker.Validate(chPos);
	}

	catch (runtime_error e)	{
		Message = e.what();
		return false;
	}

	catch (...){
		Message = "Unvalidatable expression.  Please check your expression is correctly written.";
		return false;
	}

	ExpType Type;

	try{
		Type = m_TypeChecker.TypeCheck();
	}
	catch (runtime_error e){
		Message = e.what();
		return false;
	}
	catch (...) {		
		Message = "Uncheckable expression.  Please check your expression is correctly written.";
		return false;
	}

	// Expression OK
	return true;
}
