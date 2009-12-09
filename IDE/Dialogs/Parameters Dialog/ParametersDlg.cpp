/////////////////////////////////////////////////////////////////////////////
// Parameters dialog
/////////////////////////////////////////////////////////////////////////////

// Main file

// File includes
#include "StdAfx.h"
#include "..\..\Construct.h"
#include "..\ParametersDlg.h"
extern CMainFrame* g_MainFrame;

void CParametersDlg::OnFocusChange(UINT nID)
{
	CWnd* Window = GetFocus();

	for(int i = 0; i < m_Stack.m_arrPanes.GetSize(); i++)
	{
		if(m_Stack.m_arrPanes.GetAt(i)->content_window == Window)
		{
			pWndFocusParam = Window;
			// update description text for tusers

			CString paramDescription;
			paramDescription.Format("%s", m_pACEEntry->params[i].desc);

			if (m_lastDesc != paramDescription) {
				m_Desc.SetWindowText(paramDescription);
				this->Invalidate();
				m_lastDesc = paramDescription;
			}
		}	
	}
}

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParametersDlg dialog

CParametersDlg::CParametersDlg(CWnd* pParent /*=NULL*/)
: CExtResizableDialog(CParametersDlg::IDD, pParent)
{
	m_lastDesc = "";
	bForce = false;
}

void CParametersDlg::DoDataExchange(CDataExchange* pDX)
{
	CExtResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParametersDlg)
	DDX_Control(pDX, IDC_ARGDESC, m_Desc);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CParametersDlg, CExtResizableDialog)
	//{{AFX_MSG_MAP(CParametersDlg)
	//ON_BN_CLICKED(IDC_FINISH, OnFinish)
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_NOTIFY(NM_DBLCLK, 100, OnDblClick)
	ON_WM_TIMER()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParametersDlg message handlers

BOOL CParametersDlg::OnInitDialog() 
{
	CExtResizableDialog::OnInitDialog();

	// Messes up window placement
	//SubclassChildControls();
	
	m_Tooltip.Create(this);
	m_ParameterTooltipIsVisible = false;

	// Stackers!
	CWnd* pWnd = GetDlgItem(IDC_STACKBORDER);

	CRect Position;
	pWnd->GetWindowRect(&Position);
	this->ScreenToClient(Position);

	pWnd->DestroyWindow();

	m_Stack.CreateEx(0/*WS_EX_STATICEDGE*/,NULL,NULL,WS_CHILD|WS_VISIBLE|WS_VSCROLL,Position,this,IDC_STACKBORDER);

	currentParameter = 0;

	// Resizing
    dlgAnchor.Init(this->m_hWnd);

	dlgAnchor.Add(IDC_ARGDESC, ANCHOR_LEFT | ANCHOR_TOP | ANCHOR_RIGHT);
	dlgAnchor.Add(IDC_ARGNUM, ANCHOR_LEFT | ANCHOR_TOP);
	dlgAnchor.Add(IDC_FINISH, ANCHOR_LEFT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDC_CANCEL, ANCHOR_LEFT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDC_TOOLBOX, ANCHOR_RIGHT | ANCHOR_BOTTOM);
	dlgAnchor.Add(IDC_STACK, ANCHOR_TOPLEFT | ANCHOR_BOTTOMRIGHT);
	dlgAnchor.Add(IDC_ACE, ANCHOR_LEFT|ANCHOR_RIGHT|ANCHOR_TOP);

	// Load panes
	CString					csRubric;
	CJazzUpTellTaleButton*	pbtJazzedUpRubric;
	CRect					rRect;
	UINT					nCtrlID = 0;

	imageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 3, 3);

	HBITMAP string_icon = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_STRING));
	HBITMAP custom_icon = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_CUSTOM));

	ImageList_Add(imageList, string_icon, NULL);
	ImageList_Add(imageList, custom_icon, NULL);

	DeleteObject(string_icon);
	DeleteObject(custom_icon);

	m_ObjectImages.Create(16, 16, ILC_COLOR24, 10, 10);
	m_ObjectImagesLarge.Create(32, 32, ILC_COLOR24, 10, 10);
	CObjTypeMap* objTypeMap = objectMap;

	POSITION pos;

	// Loop through and add the objects
	CBitmap image;
	image.LoadBitmapA(IDB_SYSTEM);
	m_ObjectImagesLarge.Add(&image, &image);

	vector<CObjType*> typesToAdd;

	// layout NULL for individual event sheets edited without a layout attached
	if (layout == NULL)
	{
		application->GetObjectTypes(typesToAdd);
		application->GetFamilyObjectTypes(typesToAdd);
	}
	else
	{
		layout->GetLocalObjectTypes(typesToAdd);
		application->GetFamilyObjectTypes(typesToAdd);
	}

	vector<CObjType*>::iterator t = typesToAdd.begin();

	// Add the global and layout object icons
	for ( ; t != typesToAdd.end(); t++)
	{
		CBitmap Bitmap;

		Bitmap.Attach((*t)->small_image.MakeBitmap());
		m_ObjectImages.Add(&Bitmap, &Bitmap);
		Bitmap.Detach();

		Bitmap.Attach((*t)->m_Image.MakeBitmap());
		m_ObjectImagesLarge.Add(&Bitmap, &Bitmap);
		Bitmap.Detach();
	}

	// Add family icons
	for(list<Family>::iterator i = application->families.begin(); i != application->families.end(); i++)
	{
		if(i->is_in_use)
		{
			CBitmap Bitmap;

			Bitmap.Attach(i->small_image.MakeBitmap());
			m_ObjectImages.Add(&Bitmap, &Bitmap);
			Bitmap.Detach();

			Bitmap.Attach(i->image.MakeBitmap());
			m_ObjectImagesLarge.Add(&Bitmap, &Bitmap);
			Bitmap.Detach();	
		}
	}

	bool add = true;

	if (strings.GetSize() != 0) 
		add = false;

	SetWindowLong(m_Stack.m_hWnd, GWL_USERDATA, (long)this);

	// Insert parameters
	for (int i = 0; i < m_pACEEntry->params.size(); i++)
	{
		TDS_PANE pane;
		m_Errors.Add("");

		CString paramName;
		paramName.Format("  %s", m_pACEEntry->params[i].name);

		int curItem = 0;

		csRubric = paramName;

		pbtJazzedUpRubric = new CJazzUpTellTaleButton;

		pbtJazzedUpRubric->Create( csRubric, WS_CHILD | BS_LEFT, rRect, &m_Stack, nCtrlID++ );
		pbtJazzedUpRubric->ModifyStyleEx(WS_EX_STATICEDGE,0);
		pbtJazzedUpRubric->SetFont( GetFont() );

		pbtJazzedUpRubric->SetIcon(imageList.ExtractIcon(m_pACEEntry->params[i].type - 1));
		
		if(strings.GetSize() <= i)
			strings.Add(m_pACEEntry->params[i].initStr);
		if(types.size() <= i)
			types.push_back(m_pACEEntry->params[i].type);

		CreateParameter(i, &m_Stack, pane, types.at(i) );

		m_Stack.AddPane(pbtJazzedUpRubric, pane);
	}
	
	CString paramDescription;
	if(currentParameter < m_pACEEntry->params.size())
		paramDescription.Format("%s", m_pACEEntry->params[currentParameter].desc);

	if(m_pACEEntry->params.size() == 0)
		paramDescription = "There are no parameters available for the selected item";

	m_Desc.SetWindowText(paramDescription);
	this->Invalidate();

	if(m_Stack.m_arrPanes.GetSize())
	{
		m_Stack.m_arrPanes.GetAt(0)->content_window->SetFocus();
		pWndFocusParam = m_Stack.m_arrPanes.GetAt(0)->content_window;
	
		pWndFocusParam->SendMessage(SCI_SELECTALL, NULL, false);
	}

	m_TypeChecker.objMap = objectMap;
	m_TypeChecker.pApp = application;

	Invalidate();

	pWnd->GetWindowRect(&Position);
	this->ScreenToClient(Position);

	CRect Rect;
	GetWindowRect(&Rect);
	int Width = Rect.Width();

	m_Stack.GetWindowRect(&Rect);
	m_Expand.Create(WS_CHILD | LVS_ICON | WS_BORDER, CRect(5, (Position.Height() / 2) + 5, Width - 10, (Rect.Height()/2) + 190), this, 100);
	Invalidate();
	m_Expand.Invalidate();
	m_Expand.SetImageList(&m_ObjectImagesLarge, 0);
	pos = objTypeMap->GetStartPosition();

	// Show object panel
	CRect window; 	 
	this->GetParent()->GetWindowRect(&window); 	 
	window.left += 3; 	 
	window.right += 3; 	 
	window.bottom -= 103; 	 
	MoveWindow(window); 	 

 
	         GetWindowRect(&Rect); 	 
	  	 
	         Rect.bottom += 102; 	 
	         Width = Rect.Width(); 	 
	  	 
	         dlgAnchor.Remove(IDC_STACK); 	 
	  	 
	         MoveWindow(&Rect); 	 
	  	 
	         CRect newRect; 	 
	         GetClientRect(&newRect); 	 
	  	 	 
	         Position; 	 
	         m_Stack.GetWindowRect(&Position); 	 
	         this->ScreenToClient(Position); 	 
	         Position.bottom = newRect.Height() - 102-4-4; 	 
	  	 
	         m_Stack.MoveWindow(Position); 	 
	  	 
	         m_Expand.SetWindowPos(NULL, 9, Position.bottom+2, Width - 16, 103, 0); 	 
	         Invalidate(); 	 
	         dlgAnchor.Add(IDC_STACK, ANCHOR_TOPLEFT | ANCHOR_BOTTOMRIGHT); 	 
	         dlgAnchor.Add(m_Expand, ANCHOR_BOTTOM | ANCHOR_LEFT | ANCHOR_RIGHT); 	 
	  	 
	         m_Expand.ShowWindow(SW_SHOW);

	int iter = 0;

	int item = m_Expand.InsertItem(0,application->m_System.GetName(), iter++);
	m_Expand.SetItemData(item, (DWORD_PTR)&application->m_System);
	
	t = typesToAdd.begin();

	for ( ; t != typesToAdd.end(); t++) {

		if(!(*t)->m_bIsGroupType)
		{
			int item = m_Expand.InsertItem(1, (*t)->GetName(), iter++);
			m_Expand.SetItemData(item, (DWORD_PTR)(*t));
		}
	}

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

bool CParametersDlg::OnFinish() 
{
	if (bForce) return true;

	// Clear all strings
	strings.RemoveAll();
	types.clear();

	// Get all strings
	for (int i = 0; i < m_pACEEntry->params.size(); i++)
	{
		CString curStr = m_Stack.m_arrPanes.GetAt(i)->GetParameterString();

		int curType = m_Stack.m_arrPanes.GetAt(i)->type;
		
		// 0.97.3, add a quick check that object parameters are filled in
		if (m_pACEEntry->params[i].type == EDITORPARAM_OBJECT)
		{
			if (curStr == "0")
			{
				message_bar.Attach(this);
				message_bar.SetHighlightOnMouseOver();
				message_bar.SetText(_T("An object parameter has not been picked."));
		
				return false;
			}
		}

		// 0.97.6, check that private variable parameters are valid
		if (m_pACEEntry->params[i].type == EDITORPARAM_PRIVATEVARIABLE)
		{
			if (curStr == "-1")
			{
				message_bar.Attach(this);
				message_bar.SetHighlightOnMouseOver();
				message_bar.SetText(_T("A private variable has not been picked."));

				return false;
			}
		}

		CString err;
		bool valid = CheckParameterValid(curStr, err);

		if (!valid)
		{
			CString msg;
			msg.Format("Error in parameter '%d':\n\n%s", i+1, err);
			message_bar.Attach(this);
			message_bar.SetHighlightOnMouseOver();
			message_bar.SetWrapText();
			message_bar.SetText(msg);

			return false;
		}

		if(this->m_TypeChecker.m_UndefinedVariables.size() > 0)
		{
			CUndefinedVariable* pVar = &*m_TypeChecker.m_UndefinedVariables.begin();
			CString msg;

			if(pVar->global)
				msg.Format("The following global variable: '%s' is undefined. Would you like to define it?", pVar->Name);
			else
				msg.Format("The following variable: '%s' of '%s' is undefined. Would you like to define it?", pVar->Name, pVar->pType->GetName()); 
			
			if(IDYES == MessageBox(msg, "Undefined Variable", MB_ICONEXCLAMATION|MB_YESNO))
			{
				CObjType* pType = pVar->pType;
				CAddVariableDlg Dlg;
				Dlg.Name = pVar->Name;
				if (Dlg.DoModal() != IDOK)
					return false;

				if(pType)
				{
					// Check this variable name does not already exist
					if (exists_element(pType->m_PrivateValues.begin(), pType->m_PrivateValues.end(), Dlg.Name)) {
						MessageBox(CString("The variable name '") + Dlg.Name + "' is already used in this object.  Please choose another name.", "Private Variable", MB_ICONEXCLAMATION | MB_OK);
						return false;
					}
				}

				if(pVar->global) 
					application->AddGlobalVariable(CApplication::GlobalVariable(Dlg.Name, Dlg.Type, Dlg.Value, application->m_varID++));

				else
				{
					int varID = pType->VariableID++;
					pType->m_PrivateValues.push_back(PrivateValue(Dlg.Name, Dlg.Type, varID));

					PrivateVariableInstanceAdder updater(varID, Dlg.Value);
					ForEachObjectInstance(pType, application, updater);
				}

				SetTimer(500, 200, NULL);
			}

			return false;
		}
		
		strings.Add(curStr);
		types.push_back(curType);
	}

	while (!m_scintillaList.empty()) { // restore windowprocs
		HWND scn = m_scintillaList.back();
		m_scintillaList.pop_back();
		SetWindowLong(scn, GWL_WNDPROC, GetWindowLongPtr(scn, GWL_USERDATA));
	}

	// Save dialog size
	//dlgMan.Save();

	// Delete image list
	m_ObjectImages.DeleteImageList();

	// Exit
	CExtResizableDialog::OnOK();

	// Valid
	return true;
}

void CParametersDlg::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// Mouse position
	POINT cursorPosition;
	GetCursorPos(&cursorPosition);

	CExtResizableDialog::OnRButtonUp(nFlags, point);
}

void CParametersDlg::OnSize(UINT nType, int cx, int cy) 
{
	CExtResizableDialog::OnSize(nType, cx, cy);
	m_Stack.RearrangeStack();
	
	dlgAnchor.OnSize();
}

void CParametersDlg::OnCancel() 
{
	//dlgMan.Save();
	CExtResizableDialog::OnCancel();
}

void CParametersDlg::OnTimer(UINT nIDEvent) 
{	
	// Event 500: Reevaluate current scintilla
	if (nIDEvent == 500)
	{
		KillTimer(500); // Don't continue indefinitely
		CString Text;
		m_pWindow->GetText(Text);

		CString Unused;

		if (CheckParameterValid(Text, Unused))
		{
			if(m_TypeChecker.m_UndefinedVariables.size() > 0)
				SetScintillaColour(m_pWindow, 0xf0ffff);	// YELLOW: Undefined variables but otherwise fine
			else
				SetScintillaColour(m_pWindow, 0xf8fff8);	// GREEN: all is good
		}
		else 
			SetScintillaColour(m_pWindow, 0xf4f4ff);		// RED: syntax error

	}
	if( nIDEvent == 15)
	{
		KillTimer(15);
		m_Stack.RearrangeStack();
	}
}

void CParametersDlg::OnChange(NMHDR* pNMHDR, LRESULT* pResult,CScintillaWnd& scintWin)
{
	m_pWindow = &scintWin;
	SetTimer(500, 200, NULL);
	m_Stack.RearrangeStack();
}

void CParametersDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// Has no effect for the tooltips; is probably sent to scintilla window instead -Ash
    switch( nChar ) {
        case VK_UP:
			if(m_Tooltip.IsWindowVisible())
				m_Tooltip.ShowPrevMethod();
			else
			{
				CWnd* focus = GetFocus();
				int index = 0;
				for(int i = 0; i < m_Stack.m_arrPanes.GetSize(); i++)
				{
					if(focus == m_Stack.m_arrPanes.GetAt(i)->content_window)
						index = i;
				}
				index --;
				if(index >= 0)
				{
					m_Stack.m_arrPanes.GetAt(index)->content_window->SetFocus();
					m_Stack.m_arrPanes.GetAt(index)->content_window->SendMessage(SCI_SELECTALL,0,0);
				}
			}
            break;
        case VK_DOWN:
			if(m_Tooltip.IsWindowVisible())
				m_Tooltip.ShowNextMethod();
			else
			{
				CWnd* focus = GetFocus();
				int index = 0;
				for(int i = 0; i < m_Stack.m_arrPanes.GetSize(); i++)
				{
					if(focus == m_Stack.m_arrPanes.GetAt(i)->content_window)
						index = i;
				}
				index ++;
				if(index < m_Stack.m_arrPanes.GetSize())
				{
					m_Stack.m_arrPanes.GetAt(index)->content_window->SetFocus();
					m_Stack.m_arrPanes.GetAt(index)->content_window->SendMessage(SCI_SELECTALL,0,0);
				}
			}
            break;
		case VK_RETURN:
			CEventWizardDlg* parent = ((CEventWizardDlg*)GetParent());
			parent->OnBnClickedNextClicked();
			return;

			//parent->SendMessage(WM_COMMAND, MAKELONG(IDC_NEXT, BN_CLICKED), (LPARAM)parent->GetDlgItem(IDC_NEXT)->GetSafeHwnd());
			break;
     }
	
	CExtResizableDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

CObjType* CParametersDlg::GetObjectType()
{
	// no longer does much really
	return m_ID;
}
	
int CParametersDlg::GetType()
{
	// First get the object
	CObjType* myobj = GetObjectType();

	// Now loop through all conditions
	for(int type = 0; type < 2; type ++)
	{
		for(int ace = 0; ace < myobj->GetTableCount(type); ace++)
		{
			if(myobj->GetACESEntry(type, ace) == m_pACEEntry)	// ACE
				return type;

			for(BehaviorIterator m = myobj->behaviors.begin() ; m != myobj->behaviors.end(); m++)	// Behaviors
			{
				for(int ace = 0; ace < myobj->GetTableCount(type); ace++)
				{
					vector<ACESEntry2>* table =	m->oInfo->GetTable(type);
					for(vector<ACESEntry2>::iterator i = table->begin(); i != table->end(); i++)
						if(&*i == m_pACEEntry)
							return type;
				}
			}
			// TODO Effects 
			// TODO custom aces
		}
	}

	MessageBox("Unable to work out which type it is!");

	return -1;
}



