#if !defined(AFX_IMAGEEDITORDLG_H__52DAEC33_E1BC_4856_8E15_903783943E29__INCLUDED_)
#define AFX_IMAGEEDITORDLG_H__52DAEC33_E1BC_4856_8E15_903783943E29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageEditorDlg.h : header file
//
#include "..\resource.h"
#include "..\Editors/ImageEditor.h"
//#include "..\UTILITIES\CXIMAGE\CXIMAGE\ximage.h"	// Added by ClassView




class CImageEditorFrame : public CFrameWnd
{
public:

};


class CPicEdSettingsDlg : public CExtNCW<CExtResizableDialog>
{

public:
// Dialog Data
	//{{AFX_DATA(CImageEditorDlg)
	enum { IDD = IDD_PICED_SETTINGS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CPicEdSettingsDlg(CWnd* pParent = NULL);   // standard constructor
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageEditorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////////////////////////////////
// CImageEditorDlg dialog

class CImageEditorDlg : public CExtNCW<CExtResizableDialog>
{
// Construction
public:
	void OnFileOpen();
	void OnFileSave();

	void CropAll();
	~CImageEditorDlg()
	{
		if(m_Color_Mixer.pColor1)
		{
			delete m_Color_Mixer.pColor1;
			m_Color_Mixer.pColor1 = NULL;
		}
		if(m_Color_Mixer.pColor2)
		{
			delete m_Color_Mixer.pColor2;
			m_Color_Mixer.pColor2 = NULL;
		}
	}
	
	bool m_Animation; // used to determine if the animation bar should be shown
	bool m_Icon;

	CxImage* m_pMyImage;
	bool m_editCollision;

	CApplication* application;
	INT_PTR EditImage(CxImage* MyImage, bool noAlphaChannel); // use for editing arbitrary images such as icons
	INT_PTR EditImage(CImageResource* image, CApplication* pApp); 
	INT_PTR EditAnimation(int animation_id,int index, CApplication* pApp);
	CImageEditorDlg(CWnd* pParent = NULL);   // standard constructor
	CImageEditor* m_pXDVView;
	CFrameWnd* m_pFrameWnd;

	void AskToDeleteCollisionMask();

	CExtToolControlBar m_Tools;
	CExtToolControlBar m_Main;
	CColorMixer m_Color_Mixer;
	CToolSettings m_Tool_Settings;
	CAnimationToolbar m_Tool_Animation;
	CExtPanelControlBar	m_Color_Mixer_bar;

	CExtStatusControlBar* m_pStatusBar;


	vector<CImageResource*> m_sourceImages;

	map<CImageResource*, CxImage> m_newImages;
	map<CImageResource*, CxImage> m_newCollision;
	map<CImageResource*, CPoint> m_newHotspots;
	map<CImageResource*, map<CString, CPoint> > m_newAction;
	
	void ApplyHotspotToAll(int x, int y);
	
	int m_animation_frame_index;
//	CPaletteWindow paletteWindow;
	CPaletteBar m_PaletteBar;

	void OnEditPaste();


// Dialog Data
	//{{AFX_DATA(CImageEditorDlg)
	enum { IDD = IDD_BLANK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageEditorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
	//{{AFX_MSG(CImageEditorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg LRESULT OnKickIdle(WPARAM, LPARAM);
	afx_msg BOOL OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult);
	void nothing(){}
	void AnimLeft();
	void AnimRight();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	BOOL PreTranslateMessage(MSG* pMsg);

	HACCEL m_haccel;	

public:
	afx_msg void OnClose();
	void CopyNeededDataFromImages();
	void CopyNeededDataToImages();
	void GetActionNames(map<CString, int>& actions);
	void SaveImageInternally(bool updateThumbnails);
	void LoadImageInternally();
	void SetNoAlpha(bool val);
	bool m_result;

	CxImage* getOtherImageForThisFrame(); //If we are flipping or mirroring the image, we need to do the same to the collision mask

	bool m_noAlpha;


};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEEDITORDLG_H__52DAEC33_E1BC_4856_8E15_903783943E29__INCLUDED_)
