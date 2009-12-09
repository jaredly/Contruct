#pragma once

class CChildFrame;
class ObjectBarDialog : public CExtResizableDialog
{
public:

	ObjectBarDialog(CChildFrame&);

	enum { IDD = IDD_BARDIALOG };

	enum
	{
		ob_large_icons = 0,
		ob_small_icons,
		ob_small_icons_horizontal,
		ob_object_tree
	} view;

	enum
	{
		ob_sort_az = 0,
		ob_sort_za,
		ob_sort_zorder
	} sorting;

	bool show_only_selected_layer;
	bool show_nonlayout_objects;

	// current application & layout
	CApplication*		application;
	CLayout*			layout;

	// controls
	CListCtrl			objects;
	CXHtmlTree			object_tree;
	CImageList			large_images;
	CImageList			small_images;
	CExtToolControlBar	toolbar;

	// functions
	void SetLayout(CLayout* pLayout, CApplication* pApplication); 
	void Refresh(bool layer_changed = false); 

	// parent (not always guaranteed to be docked, so needs storing)
	CChildFrame&		parent;

protected:

	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void DoDataExchange(CDataExchange* pDX); 
	afx_msg void OnClickObject(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRClickObject(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblClickObject(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKeyDown(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};