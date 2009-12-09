#pragma once
#include "afxwin.h"

// CImportImagesDlg dialog
class CListBoxMulti : public CListBox
{
	DECLARE_DYNAMIC(CListBoxMulti)
public:
	CListBoxMulti();
	void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

	int lastclicked;





};






class CImportImagesDlg : public CExtNCW<CExtResizableDialog>
{
	DECLARE_DYNAMIC(CImportImagesDlg)

public:
	CImportImagesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImportImagesDlg();

// Dialog Data
	enum { IDD = IDD_IMPORT_IMAGES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBoxMulti m_ImageList;
	list<CImageResource> m_Images;
	bool m_frame;
	bool m_animation;
	CExtButton	m_Hotspot1, m_Hotspot2, m_Hotspot3, m_Hotspot4, m_Hotspot5, m_Hotspot6, m_Hotspot7, m_Hotspot8, m_Hotspot9,
		m_ColourMask, m_Mirror, m_Flip, m_Rotate1, m_Rotate2, m_Add, m_Remove, m_Up, m_Down, m_Tiles,
		m_ImportFrame, m_ImportAnimation, m_Cancel, m_Collision;

	CExtEdit	m_HotspotX, m_HotspotY, m_TilesetX, m_TilesetY, m_TilesetW, m_TilesetH;

	CExtGroupBox m_Group1, m_Group2, m_Group3;

	CExtLabel m_Static_X, m_Static_Y, m_Static_TileX, m_Static_TileY, m_Static_TileW, m_Static_TileH;
	CExtCheckBox m_KeepImage;

	CExtLabel m_Preview;
public:
	afx_msg void OnBnClickedHotspot1();
public:
	afx_msg void OnBnClickedHotspot2();
public:
	afx_msg void OnBnClickedHotspot3();
public:
	afx_msg void OnBnClickedHotspot4();
public:
	afx_msg void OnBnClickedHotspot5();
public:
	afx_msg void OnBnClickedHotspot6();
public:
	afx_msg void OnBnClickedHotspot7();
public:
	afx_msg void OnBnClickedHotspot8();
public:
	afx_msg void OnBnClickedHotspot9();
public:
	afx_msg void OnBnClickedColourMask();
public:
	afx_msg void OnBnClickedMirror();
public:
	afx_msg void OnBnClickedFlip();
public:
	afx_msg void OnBnClickedRotate1();
public:
	afx_msg void OnBnClickedRotate2();
public:
	afx_msg void OnBnClickedAdd();
public:
	afx_msg void OnBnClickedRemove();
public:
	afx_msg void OnBnClickedUp();
public:
	afx_msg void OnBnClickedDown();
public:
	afx_msg void OnBnClickedCutIntoTiles();
public:
	afx_msg void OnBnClickedImportframe();
public:
	afx_msg void OnBnClickedImportanimation();
public:
	afx_msg void OnBnClickedCancel2();
private:
	virtual BOOL OnInitDialog();

	void UpdateHotspot(int a, int b);
	void GetSelectedImages(vector<CImageResource*>& imgs);
	void UpdatePreview();
	void OnListSelChange();
	afx_msg void OnBnClickedSelectall();



public:
	// Drag and drop stuff
	CImageList* m_pDragImage;	//For creating and managing the drag-image
	BOOL		m_bDragging;	//T during a drag operation
	int			m_nDragIndex;	//Index of selected item in the List we are dragging FROM
	int			m_nDropIndex;	//Index at which to drop item in the List we are dropping ON
	CWnd*		m_pDropWnd;		//Pointer to window we are dropping on (will be cast to CListCtrl* type)

	CListBox* m_pDragList;		//Which ListCtrl we are dragging FROM
	CListBox* m_pDropList;		//Which ListCtrl we are dropping ON

	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	void DropItemOnList(CListBox* pDragList, CListBox* pDropList);

public:
	afx_msg void OnBnClickedColourMask2();
public:
	afx_msg void OnBnClickedCollision();
};
