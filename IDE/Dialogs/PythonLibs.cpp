// Dialogs\PythonLibs.cpp : implementation file
//

#include "stdafx.h"
#include "..\Construct.h"
#include "PythonLibs.h"

extern vector<CString> g_PythonLib;

// CPythonLibs dialog

IMPLEMENT_DYNAMIC(CPythonLibs, CDialog)

CPythonLibs::CPythonLibs(CWnd* pParent /*=NULL*/)
	:  CExtNCW<CExtResizableDialog>(CPythonLibs::IDD, pParent)
{

}

CPythonLibs::~CPythonLibs()
{
}

void CPythonLibs::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Control(pDX, IDC_ALL, m_All);
	DDX_Control(pDX, IDC_NONE, m_None);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
}

BEGIN_MESSAGE_MAP(CPythonLibs, CDialog)
	ON_BN_CLICKED(IDOK, &CPythonLibs::OnBnClickedOk)
	ON_BN_CLICKED(IDC_ALL, &CPythonLibs::OnBnClickedAll)
	ON_BN_CLICKED(IDC_NONE, &CPythonLibs::OnBnClickedNone)
END_MESSAGE_MAP()


BOOL CPythonLibs::OnInitDialog()
{
	CExtNCW<CExtResizableDialog>::OnInitDialog();

	for(int i = 0; i < g_PythonLib.size(); i++)
	{
		CString file = g_PythonLib.at(i);
		int index = m_List.AddString(file);
		if(application->m_PythonDistrib.find(file) != application->m_PythonDistrib.end())
		{
			m_List.SetCheck(index, TRUE);
		}
	}

	return true;
}


void CPythonLibs::OnBnClickedOk()
{
	application->m_PythonDistrib.clear();

	for(int i = 0; i < m_List.GetCount(); i++)
	{
		CString file;
		m_List.GetText(i, file);

		if(m_List.GetCheck(i))
			application->m_PythonDistrib[file] = 0;
	}

	CDialog::OnOK();
}

void CPythonLibs::OnBnClickedAll()
{
	for(int i = 0; i < m_List.GetCount(); i++)
	{
		m_List.SetCheck(i,TRUE);
	}
}

void CPythonLibs::OnBnClickedNone()
{
	for(int i = 0; i < m_List.GetCount(); i++)
	{
		m_List.SetCheck(i,FALSE);
	}
}
