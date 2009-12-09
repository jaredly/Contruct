#pragma once

class AnimatorBar : public CExtResizableDialog
{

public:

	AnimatorBar(); 

protected:

	CCtrlMessageBar message_bar;
	CXHtmlTree		animations;
	CListCtrl		film_strip;

	CApplication*	application;
	CLayout*		layout;


	bool m_LockChange;





public:

	CImageList		m_ImageList; // Images
	CImageList		m_Images; // Icons

	AnimatorBar*	m_pParent;
	CObjType*		m_pObjType;

	CFont			m_font;

	HTREEITEM		curHTreeItem;

	int m_iHandle;
	map<int, CAnimation*> m_SelectedItem;

	// Functions
	void		UpdateAnimations(CApplication* pApp, CLayout* pFrame, CObjType* pObjType, int iHandle);
	void		AddAnimationToTree(CAnimation* anim, HTREEITEM parent);
	void		UpdateFilmStrip();
	void		AnimationHasChanged();
	void		OnBnClickedPackage();

	// Animations
	CAnimation*	m_pAnimation; // root animation
	CAnimation* m_pCurrentAnimation; // animation in tree

	// Animation functions
	CAnimation*	AddAnimation(CAnimation* pParent);
	void		AddAnimation();
	void		AddSubAnimation();
	void		AddAngle();
	void		RemoveAnimation();

	void		AddFrame();
	void		RemoveFrame();
	void		FrameSerialize(CArchive& ar);

	CDragDropMgr m_pDDMgr;

	// Callbacks (MFC)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnAnimationChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAnimationLClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRClickAnimation(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblClickFilmStrip(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRClickFilmStrip(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickFilmStrip(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKeyDownFilmStrip(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	// Draggings
	afx_msg void OnBegindragFrames(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);	
	void DropItemOnList(CListCtrl* pDragList, CListCtrl* pDropList);

	// Animation dragging
	afx_msg LRESULT OnBeginDrag(WPARAM, LPARAM);
	afx_msg LRESULT OnEndDrag(WPARAM, LPARAM);
	afx_msg LRESULT OnDropHover(WPARAM, LPARAM);

	CImageList* m_pDragImage;	// For creating and managing the drag-image
	BOOL		m_bDragging;	// T during a drag operation
	int			m_nDragIndex;	// Index of selected item in the List we are dragging FROM
	int			m_nDropIndex;	// Index at which to drop item in the List we are dropping ON
	CWnd*		m_pDropWnd;		// Pointer to window we are dropping on (will be cast to CListCtrl* type)

	CListCtrl* m_pDragList;		// Which ListCtrl we are dragging FROM
	CListCtrl* m_pDropList;		// Which ListCtrl we are dropping ON

	DECLARE_MESSAGE_MAP()
};