// Dialogs\AddVariableDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\Construct.h"
#include "AddVariableDlg.h"

// CAddVariableDlg dialog

IMPLEMENT_DYNAMIC(CAddVariableDlg, CDialog)

CAddVariableDlg::CAddVariableDlg(CWnd* pParent /*=NULL*/)
	: CExtNCW<CExtResizableDialog>(CAddVariableDlg::IDD, pParent)
{
	Type = 0;
	Value = "0";
}

CAddVariableDlg::~CAddVariableDlg()
{
}

void CAddVariableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NAME, m_Name);
	DDX_Control(pDX, IDC_VARIABLE, m_Value);
	DDX_Control(pDX, IDC_COMBO1, m_Type);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
}


BEGIN_MESSAGE_MAP(CAddVariableDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAddVariableDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddVariableDlg message handlers

void CAddVariableDlg::OnBnClickedOk()
{
	m_Name.GetWindowTextA(Name);

	if (Name == "")
	{
		tips.Attach(this);
		tips.SetHighlightOnMouseOver();
		tips.SetResize();
		tips.SetWrapText();
		tips.SetText("Please enter a name for this variable.");

		return;
	}

	m_Value.GetWindowTextA(Value);
	Type = m_Type.GetCurSel();

	// If a number type (0), convert the value to a float then back to text.  This sets the value to 0
	// if the user accidentally types text for a number variable.
	if (Type == 0) {
		Value.Format("%f", atof(Value));
		
		while(Value.Right(1) == "0")
			Value = Value.Left(Value.GetLength() - 1);

		if (Value.Right(1) == ".")
			Value = Value.Left(Value.GetLength() - 1);
	}

	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL CAddVariableDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (Caption != "")
		SetWindowText(Caption);

	m_Type.SetCurSel(Type);
	m_Name.SetWindowText(Name);
	m_Value.SetWindowText(Value);

	SubclassChildControls();

	return TRUE;
}