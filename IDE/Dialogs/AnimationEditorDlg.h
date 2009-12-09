#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "..\UI Elements\EditSpinSlider.h"
// CAnimationEditorDlg dialog
#include "..\Utilities\anchor.h"
#include "..\Utilities\dlgman.h"

class CAnimationEditorDlg : public CExtNCW<CExtResizableDialog>
{
	DECLARE_DYNAMIC(CAnimationEditorDlg)

public:
	CAnimationEditorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAnimationEditorDlg();

	// Resizable
	CDlgAnchor dlgAnchor;
	CDlgMan dlgMan;   
	void UpdateAnimations();
	void AddAnimationToTree(CAnimation* anim, HTREEITEM parent);
	CAnimation* AddAnimation(CAnimation* parent);
	void UpdateAnimationSettings();
	void UpdateAnimationUISettings();

	CAnimation*		m_Animation;
	CImageList		m_ImageList;
	CApplication*	m_pApplication;
	CAnimation*		m_CurrentAnimation;

	CExtCheckBox    m_AnimForever,
					m_AnimPingPong;
	CExtButton      m_AddFrame,
					m_RemoveFrame,
					m_ImportFrame,
					m_OK,
					m_Cancel,
					m_AddAnim,
					m_AnimBackToSelect,
					m_AddDir,
					m_SubAnim,
					m_RemoveAnim,
					m_Copy,
					m_Paste,
					m_Import,
					m_Play;

	CExtEdit	m_FrameTime,
					m_AnimSpeed,
					m_AnimRepeat,
					m_AnimBackTo;
	CSliderCtrl		m_AnimSpeedSlider;

	CExtLabel	m_Label1, m_Label2, m_Label3, m_Label4;

	int        m_iFrame;
	int	       m_iPlayCount;
	bool	   m_iPlayForward;

	bool FirstRun;


	//Drag and drop stuff
	CImageList* m_pDragImage;	//For creating and managing the drag-image
	BOOL		m_bDragging;	//T during a drag operation
	int			m_nDragIndex;	//Index of selected item in the List we are dragging FROM
	int			m_nDropIndex;	//Index at which to drop item in the List we are dropping ON
	CWnd*		m_pDropWnd;		//Pointer to window we are dropping on (will be cast to CListCtrl* type)

	CListCtrl* m_pDragList;		//Which ListCtrl we are dragging FROM
	CListCtrl* m_pDropList;		//Which ListCtrl we are dropping ON

	void DropItemInTree();

	//drag animations
	
	BOOL m_bDragAnimation;

	CArchive*	m_ar;
	HGLOBAL		m_data;
	CSharedFile	m_sf;
	CMemFile	m_mf;

	// clipboard

	CDragDropMgr m_pDDMgr;
// Dialog Data
	enum { IDD = IDD_ANIMATIONEDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnAddFrame();

	virtual void OnRemoveFrame();
	virtual void UpdateList();
public:
	CLayout* m_pFrame;

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_AnimationTree;
	CListCtrl m_Frames;
	afx_msg void OnNMDblclkFilmstrip(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickFilmstrip(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnChangeFrameTime();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	CStatic m_Preview;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnTvnSelchangedAnimationtree(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnBnClickedImport();
public:
	afx_msg void OnBnClickedAddanimation();
public:
	afx_msg void OnBnClickedSubanimation();
public:
	afx_msg void OnBnClickedAdddirection();
public:
	afx_msg void OnBnClickedAnimationremove();
public:
	afx_msg void OnBnClickedBacktoSelect();
public:
	afx_msg void OnBnClickedForever();
public:
	afx_msg void OnBnClickedPingpong();
public:
	afx_msg void OnEnChangeRepeat();
public:
	afx_msg void OnEnChangeBackto();
public:
	afx_msg void OnEnChangeSpeed();
public:
	afx_msg void OnLvnBeginlabelEdit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndlabelEdit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBegindragFrames(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	
	void DropItemOnList(CListCtrl* pDragList, CListCtrl* pDropList);

	void FrameSerialize(CArchive& ar);
	void OnToolbarCopy();
	void OnToolbarPaste();
//	afx_msg void OnEnChangeTime();
public:
	afx_msg void OnBnClickedCopy();
public:
	afx_msg void OnBnClickedPaste();


	BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnBnClickedPlay();
public:
	afx_msg void OnTvnBegindragAnimationtree(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnKeyDownFilmStrip(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRClickFilmstrip(NMHDR *pNMHDR, LRESULT *pResult);
};