// Dialogs/EditEventGroup.cpp : implementation file
//

#include "stdafx.h"
#include "EditEventGroup.h"

// CEditEventGroup dialog
IMPLEMENT_DYNAMIC(CEditEventGroup, CDialog)

CEditEventGroup::CEditEventGroup(CWnd* pParent /*=NULL*/)
	: CExtWS<CDialog>(CEditEventGroup::IDD, pParent)
{
	m_Active = true;
}

void CEditEventGroup::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDOK, m_OK);
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEditEventGroup, CDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CEditEventGroup message handlers
BOOL CEditEventGroup::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Resizing anchors
	BOOL First = dlgMan.Load(this->m_hWnd, "Software\\Construct\\EditEventGroup");
	if (!First)
	{
		tips.Attach(this);
		tips.SetHighlightOnMouseOver();
		tips.SetResize();
		tips.SetWrapText();
		tips.SetText("An event group is a group of events which can be activated or deactivated at runtime.  This serves the purpose of not only keeping event sheets clean, but allowing certain segments to be toggled on and off depending on what's going on.");
	}

	// Translations
	m_Cancel.SetWindowText(CANCEL);
	GetDlgItem(IDC_TITLELABEL)->SetWindowText(EG_GROUPTITLE);
	GetDlgItem(IDC_DESCRIPTION)->SetWindowText(EG_DESCRIPTION);
	((CButton*)GetDlgItem(IDC_DISABLED))->SetWindowText(EG_DISABLEBYDEFAULT);
	CString Title;
	Title.Format("Construct : %s", EG_TITLE);
	SetWindowText(Title);

	// Editing
	if (!m_Active) ((CButton*)GetDlgItem(IDC_DISABLED))->SetCheck(BST_CHECKED);
	GetDlgItem(IDC_DESCRIPTION)->SetWindowText(m_Description);
	GetDlgItem(IDC_TITLE)->SetWindowText(m_Title);

	SubclassChildControls();

	return TRUE;
}

CEditEventGroup::~CEditEventGroup()
{
}

void CEditEventGroup::OnDestroy() 
{
	tips.Detach();

	CDialog::OnDestroy();
	
	dlgMan.Save();	
}

void CEditEventGroup::OnOK()
{
	m_Active = !((CButton*)GetDlgItem(IDC_DISABLED))->GetCheck();
	GetDlgItem(IDC_DESCRIPTION)->GetWindowText(m_Description);
	GetDlgItem(IDC_TITLE)->GetWindowText(m_Title);

	if (m_Title == "")
	{
		tips.Detach();
		tips.Attach(this);
		tips.SetHighlightOnMouseOver();
		tips.SetResize();
		tips.SetWrapText();
		tips.SetText("Please enter a name for this event group.");

		return;
	}

	CDialog::OnOK();
}