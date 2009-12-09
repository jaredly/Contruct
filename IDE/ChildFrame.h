// ChildFrm.h : interface of the CChildFrame class
#pragma once

#include "Stdafx.h"
#include "Structure.h"

#include "Editors\LayoutEditor.h"
#include "Editors\Event Sheet Editor\Event Sheet Editor.h"
#include "Editors\ImageEditor.h"
#include "Bars\Object Bar.h"

#include "UI Elements\Controls\CustomTabCtrl.h"

class CChildFrame : public  CExtNCW <CMDIChildWnd>
{
public:

	CChildFrame() :
		application(NULL),
		layout(NULL),
		event_sheet(NULL),
		object_bar(*this)
	{
	}

	CChildFrame(CApplication*, CLayout*, EventSheet*);

	CLayoutEditor*		layout_editor[2][2];
	EventSheetEditor*	m_pEventView[2][2];

	CExtControlBar object_bar_parent;
	ObjectBarDialog object_bar;

	void InitialUpdateAllEditors();

	// PER APPLICATION MENU OPTIONS GO HERE
	afx_msg void OnPlugins();
	afx_msg void OnPreview();
	afx_msg void OnPreviewLayout();
	afx_msg void OnDebug();
	afx_msg void OnDebugLayout();
	afx_msg void OnExportEXE();
	afx_msg void OnFileNew();
	afx_msg void OnFilePreferences();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnEventEditor();
	afx_msg void OnFrameEditor();
	afx_msg void OnScriptEditor();
	afx_msg void OnLayerBar();
	afx_msg void OnProjectBar();
	afx_msg void OnPropertiesBar();
	afx_msg void OnResourceBar();
	afx_msg void OnHideAll();
	afx_msg void OnAddLayout();
	afx_msg void OnRemoveLayout();
	afx_msg void OnExchange();
	afx_msg void OnManageTransitions();
	afx_msg void OnManageVariables();
	afx_msg void OnManageEventSheets();
	afx_msg void OnManageFamilies();
	afx_msg void OnManageObjectFolders();
	int oldview;

	// Hide all the editors
	void HideFrames();

	// Old view ID
	UINT GetOldView();

private:

	DECLARE_DYNCREATE(CChildFrame)

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	CFlatSplitterWnd splitter;
	BOOL m_bInitSplitter;

	class CEditorTab : public CExtTabFlatWnd
	{
		virtual bool OnTabWndSelectionChange(LONG nOldItemIndex, LONG nNewItemIndex, bool bPreSelectionTest)
		{
			bool bRetVal =     CExtTabFlatWnd::OnTabWndSelectionChange(nOldItemIndex,
																			nNewItemIndex,
																			bPreSelectionTest);
			if( bRetVal && !bPreSelectionTest)
			{  
				LRESULT End = 0;
				if(	((CChildFrame*)(this->GetParent()->GetParent()))->m_pEventView		!= NULL)
					((CChildFrame*)(this->GetParent()->GetParent()))->OnSelChangeTab(NULL, &End);
			}
			return bRetVal;
		}
	} m_tabs;


	void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) ;

	int id;

	CLayout*		layout;
	CApplication*	application;
	EventSheet*	event_sheet;

	void InsertObject();

// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
public:
	//{{AFX_MSG(CChildFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	afx_msg void OnSelChangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
public:
	// we have changed editors or child windows
	void SwitchEditor(void);
	void UpdateAllEditors(CLayoutEditor* donotupdate);

	void OnLayoutEditor();
	void OnEventSheetEditor();
};