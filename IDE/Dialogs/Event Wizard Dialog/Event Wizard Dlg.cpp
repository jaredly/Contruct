/////////////////////////////////////////////////////////////////////////////
// Event Wizard Dialog
/////////////////////////////////////////////////////////////////////////////

// File includes
#include "StdAfx.h"
#include "..\..\Construct.h"
#include "EventWizardDlg.h"
#include "..\Object Info Dlg.h"
#include "..\ParametersDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////
// CEventWizardDlg dialog

CEventWizardDlg::CEventWizardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEventWizardDlg::IDD, pParent)
{
	
	m_Params = NULL;
	m_iStep = 1;
	m_aceIndex = -1;
	m_childid = 0;
	m_pSelectedObject = -2;
	m_aceType = 0;
	m_locked = false;
	m_bOK = false;
	m_InitialStage = 0;
	m_InitialSelect = 0;

	tabSelectionIndex = 0;
}

void CEventWizardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventWizardDlg)
	DDX_Control(pDX, IDC_BACK, m_Back);
	DDX_Control(pDX, IDC_NEXT, m_Next);
	DDX_Control(pDX, IDC_CANCEL, m_Cancel);
	DDX_Control(pDX, IDC_OBJECTLIST, m_List);
	DDX_Control(pDX, IDC_ACELIST, m_ACEList);
	DDX_Control(pDX, IDC_FIND, m_Find);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEventWizardDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_DESTROY()

	ON_BN_CLICKED(IDC_NEXT, &CEventWizardDlg::OnBnClickedNext)
	ON_BN_CLICKED(IDC_BACK, &CEventWizardDlg::OnBnClickedBack)
	ON_WM_PAINT()

	ON_NOTIFY(NM_DBLCLK, IDC_OBJECTLIST, &CEventWizardDlg::OnHdnItemdblclickObjectlist)
	ON_NOTIFY(NM_DBLCLK, IDC_ACELIST, &CEventWizardDlg::OnHdnItemdblclickObjectlist)
	ON_BN_CLICKED(IDC_CANCEL, &CEventWizardDlg::OnBnClickedCancel)

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ACELIST, &CEventWizardDlg::OnUpdateButtons)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_OBJECTLIST, &CEventWizardDlg::OnUpdateButtons)

	ON_EN_CHANGE(IDC_FIND, &CEventWizardDlg::OnFilterChange)
END_MESSAGE_MAP()

bool ObjTypeSortFunction(const CObjType* typeA, const CObjType* typeB)
{
	CString l = typeA->GetName();
	CString r = typeB->GetName();

	l.MakeLower();
	r.MakeLower();

	return l < r;
}

/////////////////////////////////////////////////////////////////////////////
// CEventWizardDlg message handlers

// Initialise the dialog
//
BOOL CEventWizardDlg::OnInitDialog() 
{
	// Startup
	CDialog::OnInitDialog();

	int currentPosition = 1;

	// Create an m_ObjectImages
	m_ObjectImages.DeleteImageList();

	bool bSmallImages = false;

	// Check what size images we'll need
	// 32x32 without families or 16x16 with
	if (application->families.size() == 0)
	{
		m_ObjectImages.Create(32, 32, ILC_COLOR24, 3, 3);
		m_List.SetImageList(&m_ObjectImages, LVSIL_NORMAL);

		HBITMAP SystemBitmap = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_SYSTEM));
		ImageList_Add(m_ObjectImages, SystemBitmap, SystemBitmap);
		DeleteObject(SystemBitmap);
	}

	else
	{
		// If it's small, also set the view to small images
		m_List.SetView(LV_VIEW_SMALLICON);

		m_ObjectImages.Create(16, 16, ILC_COLOR24, 3, 3);
		m_List.SetImageList(&m_ObjectImages, LVSIL_SMALL);

		HBITMAP SystemBitmap = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_SYSTEMC));
		ImageList_Add(m_ObjectImages, SystemBitmap, SystemBitmap);	
		DeleteObject(SystemBitmap);

		bSmallImages = true;
	}

	// Prepare to iterate all objects to add icons
	POSITION pos = objMap->GetStartPosition();
	long mapKey;
	CObjType* ObjectType;

	// Add '(System)' because system uses it
	object_folders["(System)"] = 0;

	// Loop through and add all objects to the otList
	while (pos != NULL) 
	{
		objMap->GetNextAssoc(pos, mapKey, ObjectType);

		if(!ObjectType->m_bIsGroupType)
		{
			CObj *o;
			bool bAdd = false;

			// If global, its okay to add...if its not global..and we have a layout...we need to check
			if (ObjectType->m_bGlobal) 
			{
				bAdd = true;
			}
			else if(layout)
			{
				POSITION pos = layout->objects.GetStartPosition();

				for (int i = 0; i < layout->objects.GetCount(); i++) 
				{
					long ID;
					layout->objects.GetNextAssoc(pos, ID, o);
						
					if (o->GetGlobalID() == ObjectType->ObjectIdentifier)
					{
						bAdd = true;	
					}
				}
			}
			else // we have no layout to this event sheet
			{
				bAdd = true;
			}

			// Dont add if it doesn't have conditions/actions
			bool hasAces = false;
			for (int i = 0; i < ObjectType->GetTableCount(m_aceType); i++)
			{
				if (ObjectType->GetACESEntry(m_aceType, i)->aceListName == "")
				{
					hasAces = true;
					break;
				}
				if (hasAces)
					break;
			}
			if(!hasAces)
				bAdd = false;

			// Okay cool...look like its okay to add...just check if is showabout in the event sheet editor
			if(bAdd && ObjectType->m_bShowInEventSheetEditor)
			{
				// Possibly add a new category
				if(object_folders.find(ObjectType->GetFolder()) == object_folders.end())
				{
					object_folders[ObjectType->GetFolder()] = object_folders.size();
				}
				m_otList.push_back(ObjectType);
			}
		}
	}

	// Sort
	sort(m_otList.begin(), m_otList.end(), ObjTypeSortFunction);

	// Now loop the otList backwards to display each object icon
	for(vector<CObjType*>::iterator i = m_otList.begin(); i != m_otList.end(); i++)
	{	
		HBITMAP Bitmap;

		if (bSmallImages)
			Bitmap = (*i)->small_image.MakeBitmap();
		else
			Bitmap = (*i)->m_Image.MakeBitmap();

		ImageList_Add(m_ObjectImages, Bitmap, Bitmap);

		DeleteObject(Bitmap);
	}	

	pos = objMap->GetStartPosition();

	// Family icons
	list<Family>::iterator f = application->families.begin();

	for ( ; f != application->families.end(); f++)
	{
		if(f->is_in_use)
		{
			HBITMAP Bitmap = f->small_image.MakeBitmap();
			ImageList_Add(m_ObjectImages, Bitmap, Bitmap);
			
			DeleteObject(Bitmap);
		}
	}
	
	// Title
	CString Title;

	if (m_aceType == CONDITION)
		Title.Format("Construct : New condition");
	else if (m_aceType == ACTION)
		Title.Format("Construct : New action");
	else if (m_aceType == EXPRESSION)
		Title.Format("Construct : New expression");

	SetWindowText(Title);

	CRect Rect;
	m_ACEList.GetWindowRect(&Rect);
	ScreenToClient(&Rect);
	Rect.top = Rect.bottom - 1;
	Rect.bottom = Rect.top + 18;
	m_CategoryTabs.Create(this, Rect, 1001, WS_CHILD | WS_VISIBLE, __ETWS_ORIENT_BOTTOM);

	m_ACEList.GetWindowRect(&Rect);
	ScreenToClient(&Rect);
	Rect.top -= 17;
	Rect.bottom = Rect.top + 18;
	m_Tabs.Create(this, Rect, 1002, WS_CHILD | WS_VISIBLE, __ETWS_ORIENT_TOP);

	GetClientRect(&Rect);

	Rect.bottom = Rect.top + 22;
	Rect.right += 48;

	// Resizing anchors
	BOOL First = dlgMan.Load(this->m_hWnd, "Software\\Construct\\EventWizardDialogs");
	if (!First)
	{
		first_run.Attach(this);
		first_run.SetHighlightOnMouseOver();
		first_run.SetResize();
		first_run.SetWrapText();

		if (m_aceType == ACTION)
			first_run.SetText("Events are created via this wizard.  Choose an object from the list below.  A list of actions will then appear, some of which require parameters: for example 'Create object' under System.  Once all parameters are filled out, click Finish to insert your action.");
		if (m_aceType == CONDITION)
			first_run.SetText("Events are created via this wizard.  Choose an object from the list below.  A list of conditions will then appear, some of which require parameters: for example 'Compare' under System.  Once all parameters are filled out, click Finish to insert your condition.");

	}

    dlgAnchor.Init(this->m_hWnd);

	dlgAnchor.Add(IDC_OBJECTLIST, ANCHOR_TOPLEFT | ANCHOR_BOTTOMRIGHT);
	dlgAnchor.Add(IDC_ACELIST, ANCHOR_TOPLEFT | ANCHOR_BOTTOMRIGHT);
	dlgAnchor.Add(IDC_FIND, ANCHOR_TOP | ANCHOR_RIGHT);

	dlgAnchor.Add(1001, ANCHOR_LEFT | ANCHOR_BOTTOMRIGHT);
	dlgAnchor.Add(1002, ANCHOR_TOPLEFT | ANCHOR_RIGHT);
	dlgAnchor.Add(1003, ANCHOR_TOPLEFT | ANCHOR_RIGHT);
	
	dlgAnchor.Add(IDC_BACK, ANCHOR_BOTTOM | ANCHOR_RIGHT);
	dlgAnchor.Add(IDC_CANCEL, ANCHOR_BOTTOM | ANCHOR_LEFT);
	dlgAnchor.Add(IDC_NEXT, ANCHOR_BOTTOM | ANCHOR_RIGHT);
	
	m_Cancel.SetWindowText(CANCEL);

	// Set ACE margins
	CRect ListRect;
	m_ACEList.GetClientRect(&ListRect);
	ListRect.left += 5;
	ListRect.top += 5;
	m_ACEList.SetWorkAreas(1, &ListRect);
	m_ACEList.SetColumnWidth(0, 200);		// Spaces everything out nicely for step 2.

	int initStep = m_iStep;

	if (m_InitialStage == 0)
	{
		m_iStep = 1;
		ChangeStep();
	}

	else
	{
		// New to 0.96.3
		m_pSelectedObject = m_InitialSelect;
		initStep = 2;
	}

	// The below is used so we set the default selected items (eg. if we are editing)
	if(m_pSelectedObject != -2)
	{
		// Have an object already selected
		for(int i = 0; i < m_List.GetItemCount(); i++)
			if(m_List.GetItemData(i) == m_pSelectedObject)
				m_List.SetItemState(i,LVIS_SELECTED, LVIS_SELECTED);
	}

	if(m_aceIndex != -1)
	{
		PreChangeStep();
		m_iStep = 2;
		int aceIndex = m_aceIndex;

		ChangeStep();
		if(m_childid != 0)
		{
			CObjType* pType = application->FindObjTypeFromNumber(m_pSelectedObject);
			if(pType)
			{
				int index = 1;
				list<CBehavior>::iterator i = pType->behaviors.begin();
				list<CBehavior>::iterator end = pType->behaviors.end();

				for(; i!= end; i++)
				{
					if(i->id == m_childid)
					{
						m_Tabs.SelectionSet(index);
						break;
					}

					index ++;
				}

				list<CEffect>::iterator e = pType->effects.begin();
				for(;e!= pType->effects.end(); e++)
				{
					if(e->m_effectID == -m_childid)				
						m_Tabs.SelectionSet(index);

					index ++;
				}
			}

			ChangeStep();
		}

		m_aceIndex = aceIndex;

		for(int i = 0; i < m_ACEList.GetItemCount(); i++)
		{
			if(m_ACEList.GetItemData(i) == m_aceIndex)
				m_ACEList.SetItemState(i,LVIS_SELECTED, LVIS_SELECTED);
		}		
	}

	if(initStep != m_iStep)
	{
		PreChangeStep();
		m_iStep = initStep;
		ChangeStep();
	}

	UpdateButtons();

	// Expression dialogs will move slightly to make it clear it's on top of the existing parameters dialog.
	if (m_aceType == EXPRESSION) {
		RECT rc;
		this->GetWindowRect(&rc);
		rc.left += 50;
		rc.top += 50;
		this->SetWindowPos(NULL, rc.left, rc.top, 0, 0, SWP_NOSIZE);
	}

	// New drawing code
	LOGFONT* m_pLF;
	m_pLF=(LOGFONT*)calloc(1,sizeof(LOGFONT));
	strncpy(m_pLF->lfFaceName,"Arial",6);
	m_pLF->lfHeight=14;
	m_pLF->lfWeight=600;
	m_pLF->lfItalic=0;
	m_pLF->lfUnderline=0;
	m_FontBold.CreateFontIndirect(m_pLF);

	m_pLF->lfWeight = 0;
	m_Font.CreateFontIndirect(m_pLF);

	m_Find.SetIcon(IDI_SEARCH);

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// Size: resize the dialog thru dlgMan
void CEventWizardDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	dlgAnchor.OnSize();

	Invalidate();
}

void CEventWizardDlg::OnSwitchTab()
{
	if (m_bOK) return;

	if(m_locked)
		return;
	PreChangeStep();
	ChangeStep();
}

// Save dialog settings
void CEventWizardDlg::OnDestroy() 
{
	first_run.Detach();

	CDialog::OnDestroy();
	
	if (m_aceType != EXPRESSION)
		dlgMan.Save();	
}


void CEventWizardDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	g_PaintManager->PaintDockerBkgnd( true, dc, this );

	CRect rect;
	GetClientRect(&rect);

	dc.FillSolidRect(0, 0, rect.Width(), 46, RGB(255, 255, 255));
	dc.FillSolidRect(0, 46, rect.Width(), 1, RGB(0, 0, 0));

	dc.FillSolidRect(0, rect.bottom - 40, rect.Width(), 1, RGB(158, 158, 158));
	dc.FillSolidRect(0, rect.bottom - 39, rect.Width(), 1, RGB(255, 255, 255));

	CFont* pOldFont = NULL;

	if (m_iStep == 1)
	{
		pOldFont = dc.SelectObject(&m_FontBold);
		dc.DrawText("Select an object.", CRect(7, 7, 200, 21), 0);
	}

	if (m_iStep == 2)
	{
		pOldFont = dc.SelectObject(&m_FontBold);

		CObjType* pType = application->FindObjTypeFromNumber(m_pSelectedObject);

		CString text;
		if (m_aceType == CONDITION)
			text.Format("Conditions for '%s'", pType->GetName());
		if (m_aceType == ACTION)
			text.Format("Actions for '%s'", pType->GetName());
		if (m_aceType == EXPRESSION)
			text.Format("Expressions for '%s'", pType->GetName());

		dc.DrawText(text, CRect(7, 7, 200, 21), 0);
	}

	if (m_iStep == 3)
	{
		pOldFont = dc.SelectObject(&m_FontBold);

		CObjType* pType = application->FindObjTypeFromNumber(m_pSelectedObject);
		CString text;
		text.Format("Parameters for '%s - %s'", pType->GetName(), pAce->aceListName);

		dc.DrawText(text, CRect(7, 7, 400, 21), 0);
	}

	if (pOldFont) dc.SelectObject(pOldFont);
}

void CEventWizardDlg::OnHdnItemdblclickObjectlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	*pResult = 0;

	OnBnClickedNext();	
}

void CEventWizardDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CEventWizardDlg::OnUpdateButtons(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	UpdateButtons();
}

void CEventWizardDlg::UpdateButtons()
{
	if(m_iStep == 1)
	{
		int sel = m_List.GetSelectedItem();
		m_Next.SetWindowText("Next");
		if(sel == -1)
			m_Next.EnableWindow(FALSE);
		
		else
			m_Next.EnableWindow();
		
		m_Back.EnableWindow(FALSE);
	}

	if(m_iStep == 2)
	{
		int sel = m_ACEList.GetSelectedItem();

		if(sel == -1) //nothing selected...
		{
			m_Next.SetWindowText("Next");
			m_Next.EnableWindow(false);
		}

		else
		{
			aceIndex = m_ACEList.GetItemData(sel);
			CObjType* pType = application->FindObjTypeFromNumber(m_pSelectedObject);

			ACESEntry2* pACE = NULL;

			if(pType)
			{
				int sel = m_Tabs.SelectionGet();
				if(sel == 0) // object
				{
					m_childid = -1;	
					pACE = pType->GetACESEntry(m_aceType, aceIndex);
				}

				else if(sel <= pType->behaviors.size())
				{
					list<CBehavior>::iterator i = pType->behaviors.begin();
					advance(i, sel-1);
					m_childid = i->id;
					CObjType tmp;
					tmp.CreateFromBehavior(&*i);

					pACE = tmp.GetACESEntry(m_aceType, aceIndex);			
				}
				else
				{
					sel -= pType->behaviors.size() + 1;
					list<CEffect>::iterator i = pType->effects.begin();
					advance(i, sel);
					m_childid = -i->m_effectID;
				
					CObjType tmp;
					tmp.CreateFromEffect(&*i);

					pACE = tmp.GetACESEntry(m_aceType, aceIndex);

				}
			}
		
			if (pACE != NULL)
			{
				if (pACE->params.size() == 0 || m_aceType == EXPRESSION)
					m_Next.SetWindowText("Finish");
			}

			else
				m_Next.SetWindowText("Next");

			m_Next.EnableWindow();
		}

		m_Back.EnableWindow();	
	}

	if(m_iStep == 3)
	{
		m_Next.SetWindowText("Finish");
		m_Next.EnableWindow();
		m_Back.EnableWindow();
	}
}