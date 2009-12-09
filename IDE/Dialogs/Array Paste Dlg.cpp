// arraypastedlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\Construct.h"
#include "arraypastedlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CArrayPasteDlg dialog


CArrayPasteDlg::CArrayPasteDlg(CWnd* pParent /*=NULL*/) : CExtNCW<CExtResizableDialog>(CArrayPasteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CArrayPasteDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	bDialogInitialized = FALSE;
}


void CArrayPasteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CArrayPasteDlg)
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDOK, m_OK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CArrayPasteDlg, CDialog)
	//{{AFX_MSG_MAP(CArrayPasteDlg)
	ON_BN_CLICKED(IDC_AP_DIM1, OnApDim1)
	ON_BN_CLICKED(IDC_AP_DIM2, OnApDim2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_AP_COUNT_DIM1_SPIN, OnDeltaposApCountDim1Spin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_AP_COUNT_DIM2_SPIN, OnDeltaposApCountDim2Spin)
	ON_EN_CHANGE(IDC_AP_COUNT_DIM1, OnChangeApCountDim1)
	ON_EN_CHANGE(IDC_AP_COUNT_DIM2, OnChangeApCountDim2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArrayPasteDlg message handlers

BOOL CArrayPasteDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CSpinButtonCtrl *spinMoveX = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_MOVEX_SPIN);
	CSpinButtonCtrl *spinMoveY = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_MOVEY_SPIN);
	CSpinButtonCtrl *spinRotateX = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_ROTATEX_SPIN);
	CSpinButtonCtrl *spinScaleX = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_SCALEX_SPIN);
	CSpinButtonCtrl *spinScaleY = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_SCALEY_SPIN);
	CSpinButtonCtrl *spinMoveX2 = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_MOVEX_DIM2_SPIN);
	CSpinButtonCtrl *spinMoveY2 = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_MOVEY_DIM2_SPIN);
	CSpinButtonCtrl *spinCount1 = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_COUNT_DIM1_SPIN);
	CSpinButtonCtrl *spinCount2 = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_COUNT_DIM2_SPIN);

	spinMoveX->SetRange(UD_MINVAL, UD_MAXVAL);
	spinMoveY->SetRange(UD_MINVAL, UD_MAXVAL);
	spinRotateX->SetRange(UD_MINVAL, UD_MAXVAL);
	spinScaleX->SetRange(1, UD_MAXVAL);
	spinScaleX->SetPos(100);
	spinScaleY->SetRange(1, UD_MAXVAL);
	spinScaleY->SetPos(100);
	spinMoveX2->SetRange(UD_MINVAL, UD_MAXVAL);
	spinMoveY2->SetRange(UD_MINVAL, UD_MAXVAL);
	spinCount1->SetRange(1, UD_MAXVAL);
	spinCount1->SetPos(1);
	spinCount2->SetRange(1, UD_MAXVAL);
	spinCount2->SetPos(1);

	CEdit *editMoveX2 = (CEdit*)GetDlgItem(IDC_AP_MOVEX_DIM2);
	CEdit *editMoveY2 = (CEdit*)GetDlgItem(IDC_AP_MOVEY_DIM2);
	CEdit *editCount2 = (CEdit*)GetDlgItem(IDC_AP_COUNT_DIM2);
	CButton *radioMoveInc2 = (CButton*)GetDlgItem(IDC_AP_MOVE_INC_DIM2);
	CButton *radioMoveTot2 = (CButton*)GetDlgItem(IDC_AP_MOVE_TOT_DIM2);
	CButton *radioMoveInc = (CButton*)GetDlgItem(IDC_AP_MOVE_INC);
	CButton *radioRotateInc = (CButton*)GetDlgItem(IDC_AP_ROTATE_INC);
	CButton *radioScaleInc = (CButton*)GetDlgItem(IDC_AP_SCALE_INC);
	CButton *radioInstance = (CButton*)GetDlgItem(IDC_AP_INSTANCE);
	CButton *radioDim1 = (CButton*)GetDlgItem(IDC_AP_DIM1);

	radioMoveInc2->SetCheck(BST_CHECKED);
	radioMoveInc->SetCheck(BST_CHECKED);
	radioRotateInc->SetCheck(BST_CHECKED);
	radioScaleInc->SetCheck(BST_CHECKED);
	radioInstance->SetCheck(BST_CHECKED);
	radioDim1->SetCheck(BST_CHECKED);

	editMoveX2->EnableWindow(FALSE);
	editMoveY2->EnableWindow(FALSE);
	editCount2->EnableWindow(FALSE);
	radioMoveInc2->EnableWindow(FALSE);
	radioMoveTot2->EnableWindow(FALSE);

	CEdit *editObjectCount = (CEdit*)GetDlgItem(IDC_AP_OBJECT_COUNT);
	CString amt;
	amt.Format(_T("%li"), objCount);
	editObjectCount->SetWindowText(amt);

	bDialogInitialized = TRUE;

	m_Cancel.SetWindowText(CANCEL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CArrayPasteDlg::OnApDim1() 
{
	CEdit *editMoveX2 = (CEdit*)GetDlgItem(IDC_AP_MOVEX_DIM2);
	CEdit *editMoveY2 = (CEdit*)GetDlgItem(IDC_AP_MOVEY_DIM2);
	CEdit *editCount2 = (CEdit*)GetDlgItem(IDC_AP_COUNT_DIM2);
	CButton *radioMoveInc2 = (CButton*)GetDlgItem(IDC_AP_MOVE_INC_DIM2);
	CButton *radioMoveTot2 = (CButton*)GetDlgItem(IDC_AP_MOVE_TOT_DIM2);

	editMoveX2->EnableWindow(FALSE);
	editMoveY2->EnableWindow(FALSE);
	editCount2->EnableWindow(FALSE);
	radioMoveInc2->EnableWindow(FALSE);
	radioMoveTot2->EnableWindow(FALSE);

	CSpinButtonCtrl *spinMoveX2 = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_MOVEX_DIM2_SPIN);
	CSpinButtonCtrl *spinMoveY2 = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_MOVEY_DIM2_SPIN);
	CSpinButtonCtrl *spinCount1 = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_COUNT_DIM1_SPIN);
	CSpinButtonCtrl *spinCount2 = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_COUNT_DIM2_SPIN);

	spinMoveX2->RedrawWindow();
	spinMoveY2->RedrawWindow();
	spinCount2->RedrawWindow();

	CEdit *editObjectCount = (CEdit*)GetDlgItem(IDC_AP_OBJECT_COUNT);
	long total = objCount * spinCount1->GetPos();
	amt.Format(_T("%li"), total);
	editObjectCount->SetWindowText(amt);
}

void CArrayPasteDlg::OnApDim2() 
{
	CEdit *editMoveX2 = (CEdit*)GetDlgItem(IDC_AP_MOVEX_DIM2);
	CEdit *editMoveY2 = (CEdit*)GetDlgItem(IDC_AP_MOVEY_DIM2);
	CEdit *editCount2 = (CEdit*)GetDlgItem(IDC_AP_COUNT_DIM2);
	CButton *radioMoveInc2 = (CButton*)GetDlgItem(IDC_AP_MOVE_INC_DIM2);
	CButton *radioMoveTot2 = (CButton*)GetDlgItem(IDC_AP_MOVE_TOT_DIM2);

	editMoveX2->EnableWindow(TRUE);
	editMoveY2->EnableWindow(TRUE);
	editCount2->EnableWindow(TRUE);
	radioMoveInc2->EnableWindow(TRUE);
	radioMoveTot2->EnableWindow(TRUE);

	CSpinButtonCtrl *spinMoveX2 = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_MOVEX_DIM2_SPIN);
	CSpinButtonCtrl *spinMoveY2 = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_MOVEY_DIM2_SPIN);
	CSpinButtonCtrl *spinCount1 = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_COUNT_DIM1_SPIN);
	CSpinButtonCtrl *spinCount2 = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_COUNT_DIM2_SPIN);

	spinMoveX2->RedrawWindow();
	spinMoveY2->RedrawWindow();
	spinCount2->RedrawWindow();

	CEdit *editObjectCount = (CEdit*)GetDlgItem(IDC_AP_OBJECT_COUNT);
	long total = objCount * spinCount1->GetPos() * spinCount2->GetPos();
	amt.Format(_T("%li"), total);
	editObjectCount->SetWindowText(amt);
}

void CArrayPasteDlg::OnOK() 
{
	SetCursor(LoadCursor(NULL, IDC_WAIT));
	CSpinButtonCtrl *spinMoveX = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_MOVEX_SPIN);
	CSpinButtonCtrl *spinMoveY = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_MOVEY_SPIN);
	CSpinButtonCtrl *spinRotateX = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_ROTATEX_SPIN);
	CSpinButtonCtrl *spinScaleX = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_SCALEX_SPIN);
	CSpinButtonCtrl *spinScaleY = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_SCALEY_SPIN);
	CSpinButtonCtrl *spinMoveX2 = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_MOVEX_DIM2_SPIN);
	CSpinButtonCtrl *spinMoveY2 = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_MOVEY_DIM2_SPIN);
	CSpinButtonCtrl *spinCount1 = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_COUNT_DIM1_SPIN);
	CSpinButtonCtrl *spinCount2 = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_COUNT_DIM2_SPIN);

	moveX = spinMoveX->GetPos();
	moveY = spinMoveY->GetPos();
	rotateX = spinRotateX->GetPos();
	scaleX = spinScaleX->GetPos();
	scaleY = spinScaleY->GetPos();
	moveX2 = spinMoveX2->GetPos();
	moveY2 = spinMoveY2->GetPos();
	count1 = spinCount1->GetPos();
	count2 = spinCount2->GetPos();

	if (moveX > 32768)
		moveX -= 65536;
	if (moveY > 32768)
		moveY -= 65536;
	if (rotateX > 32768)
		rotateX -= 65536;
	if (moveX2 > 32768)
		moveX2 -= 65536;
	if (moveY2 > 32768)
		moveY2 -= 65536;

	CButton *radioMoveInc = (CButton*)GetDlgItem(IDC_AP_MOVE_INC);
	CButton *radioRotateInc = (CButton*)GetDlgItem(IDC_AP_ROTATE_INC);
	CButton *radioScaleInc = (CButton*)GetDlgItem(IDC_AP_SCALE_INC);
	CButton *radioMoveInc2 = (CButton*)GetDlgItem(IDC_AP_MOVE_INC_DIM2);
	CButton *radioInstance = (CButton*)GetDlgItem(IDC_AP_INSTANCE);
	CButton *radioDim1 = (CButton*)GetDlgItem(IDC_AP_DIM1);

	bMoveTotal = (radioMoveInc->GetCheck() != BST_CHECKED);
	bRotateTotal = (radioRotateInc->GetCheck() != BST_CHECKED);
	bScaleTotal = (radioScaleInc->GetCheck() != BST_CHECKED);
	bMoveTotal2 = (radioMoveInc2->GetCheck() != BST_CHECKED);
	bDuplicate = (radioInstance->GetCheck() != BST_CHECKED);
	b2D = (radioDim1->GetCheck() != BST_CHECKED);

	layout_editor->ArrayPaste(this);

	SetCursor(LoadCursor(NULL, IDC_ARROW));
	CDialog::OnOK();
}

void CArrayPasteDlg::OnDeltaposApCountDim1Spin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CSpinButtonCtrl *spinCount1 = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_COUNT_DIM1_SPIN);
	CSpinButtonCtrl *spinCount2 = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_COUNT_DIM2_SPIN);
	CEdit *editObjectCount = (CEdit*)GetDlgItem(IDC_AP_OBJECT_COUNT);
	CButton *radioDim1 = (CButton*)GetDlgItem(IDC_AP_DIM1);
	b2D = (radioDim1->GetCheck() != BST_CHECKED);

	long total = b2D ? (objCount * (pNMUpDown->iPos + pNMUpDown->iDelta) * spinCount2->GetPos())
						: (objCount * (pNMUpDown->iPos + pNMUpDown->iDelta));
	amt.Format(_T("%li"), total);
	editObjectCount->SetWindowText(amt);
	
	*pResult = 0;
}

void CArrayPasteDlg::OnDeltaposApCountDim2Spin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CSpinButtonCtrl *spinCount1 = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_COUNT_DIM1_SPIN);
	CSpinButtonCtrl *spinCount2 = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_COUNT_DIM2_SPIN);
	CEdit *editObjectCount = (CEdit*)GetDlgItem(IDC_AP_OBJECT_COUNT);
	CButton *radioDim1 = (CButton*)GetDlgItem(IDC_AP_DIM1);
	b2D = (radioDim1->GetCheck() != BST_CHECKED);

	long total = b2D ? (objCount * spinCount1->GetPos() * (pNMUpDown->iPos + pNMUpDown->iDelta))
							: (objCount * spinCount1->GetPos());
	amt.Format(_T("%li"), total);
	editObjectCount->SetWindowText(amt);
	
	*pResult = 0;
}

void CArrayPasteDlg::OnChangeApCountDim1() 
{
	if (bDialogInitialized) {
		CSpinButtonCtrl *spinCount1 = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_COUNT_DIM1_SPIN);
		CSpinButtonCtrl *spinCount2 = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_COUNT_DIM2_SPIN);
		CEdit *editObjectCount = (CEdit*)GetDlgItem(IDC_AP_OBJECT_COUNT);
		CButton *radioDim1 = (CButton*)GetDlgItem(IDC_AP_DIM1);

		long total = b2D ? (objCount * spinCount1->GetPos() * spinCount2->GetPos())
								: (objCount * spinCount1->GetPos());
		amt.Format(_T("%li"), total);
		editObjectCount->SetWindowText(amt);
	}
}

void CArrayPasteDlg::OnChangeApCountDim2() 
{
	if (bDialogInitialized) {
		CSpinButtonCtrl *spinCount1 = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_COUNT_DIM1_SPIN);
		CSpinButtonCtrl *spinCount2 = (CSpinButtonCtrl*)GetDlgItem(IDC_AP_COUNT_DIM2_SPIN);
		CEdit *editObjectCount = (CEdit*)GetDlgItem(IDC_AP_OBJECT_COUNT);
		CButton *radioDim1 = (CButton*)GetDlgItem(IDC_AP_DIM1);

		long total = b2D ? (objCount * spinCount1->GetPos() * spinCount2->GetPos())
								: (objCount * spinCount1->GetPos());
		amt.Format(_T("%li"), total);
		editObjectCount->SetWindowText(amt);
	}
}
