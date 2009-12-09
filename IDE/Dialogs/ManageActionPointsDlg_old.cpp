// Dialogs/ManageActionPointsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\Construct.h"
#include "ManageActionPointsDlg.h"
#include "ImageEditorDlg.h"

// CManageActionPointsDlg dialog

IMPLEMENT_DYNAMIC(CManageActionPointsDlg, CDialog)

CManageActionPointsDlg::CManageActionPointsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManageActionPointsDlg::IDD, pParent)
{

}

CManageActionPointsDlg::~CManageActionPointsDlg()
{
}

void CManageActionPointsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ActionPointsList);
	DDX_Control(pDX, IDC_EDIT1, m_ActionName_edit);

	DDX_Control(pDX, IDC_ADD, m_Add);
	DDX_Control(pDX, IDC_REMOVE, m_Remove);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
}


BEGIN_MESSAGE_MAP(CManageActionPointsDlg, CDialog)
	ON_BN_CLICKED(IDC_ADD, &CManageActionPointsDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_REMOVE, &CManageActionPointsDlg::OnRemove)
END_MESSAGE_MAP()


// CManageActionPointsDlg message handlers

void CManageActionPointsDlg::OnBnClickedButton1()
{
	// Verify it's not empty
	CString NewActionName; 
	m_ActionName_edit.GetWindowText(NewActionName);
	if (NewActionName == "")
	{
		CErrorDlg Dlg;
		Dlg.Error("Blank name", "Please enter a name for this image point to continue.");
		return;
	}

	CImageEditorDlg* imgDlg = m_pToolSettings->m_pImgEd->m_pImgEdDlg;

	CImageResource* currentImage = imgDlg->m_sourceImages.at(imgDlg->m_animation_index);

	(imgDlg->m_newAction[currentImage])[NewActionName] = imgDlg->m_newHotspots[currentImage];
	
	UpdateList();
}

void CManageActionPointsDlg::OnRemove()
{
	// TODO FOR DAVO
	
	UpdateList();
}

void CManageActionPointsDlg::UpdateList()
{
	m_ActionPointsList.ResetContent();
	//ADD
	CImageEditorDlg* imgDlg = m_pToolSettings->m_pImgEd->m_pImgEdDlg;

	
	map<CString, int> actions;
	imgDlg->GetActionNames(actions);
	
	for( map<CString, int>::iterator ai = actions.begin(); ai != actions.end(); ai++ )
	{
		m_ActionPointsList.AddString(ai->first);
	}
	// TODO: Add your control notification handler code here

}


BOOL CManageActionPointsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateList();
	return 0;
}
