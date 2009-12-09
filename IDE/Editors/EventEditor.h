#pragma once

#include "ChronoEventEditor\ChronoEventEditor.h"

// Insert a new trigger
#include "..\Dialogs\EventWizardDlg.h"

// Comment dialog
#include "..\Dialogs\AddCommentDlg.h"

// Edit group dialog
#include "..\Dialogs\EditEventGroup.h"

#include "..\Structure\SystemObject.h"

// Find & Replace
#include "..\Dialogs\FindReplaceDlg.h"
#include "..\Editors\Scintilla\include\Scintilla.h"
#include "..\Editors\Scintilla\ScintillaWnd.h"
/////////////////////////////////////////////////////////////////////////////
// CEventEditor view
class CParametersDlg;


class CEventEditorUndo;




class CEventEditor : public CChronoEventEditor
{
protected:           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEventEditor)

// Attributes
public:
	CEventEditor();
	virtual ~CEventEditor();
	CConstructDoc* GetDocument();
	CLayout*	m_pLayout;
	CApplication* m_app;
	CDragDropMgr ddm;
	int m_LastRibbon;
	HBITMAP GetIcon(int oid);
	
	void AddAction(CEditorEvent* pEvent, int index);
	CAction* PerformAction(CAction *action, BOOL bRepeat);
	void CreateUndo();

// Operations
public:

	// Necessary members
	CAddCommentDlg m_CommentDlg;
	CFont          m_HeaderFont;
	CImageList     m_ImageList;
	CImageList     m_Images;

	void Insert_ClickAfter();
	void Insert_ClickBefore();

	void RecurseEdit(CEditorEvent* pEvent, int iType);

	// Type checking

	bool              m_ParameterTooltipIsVisible;
	bool              m_QuotesOpen;
	int*			m_display_life;


	bool tts;
	bool tooltip;

	// Object IDs
	CStringArray objId;
	CStringArray objTypeId;

	// Comment colour
	COLORREF m_CommentColour;

	// Find & Replace
	CFindReplaceDlg m_FindReplaceDlg;

	// Helper functions
		// Add a condition
		void AddCondition (CEditorCondition* Parent = NULL, bool SubEvent = false, CEditorEvent* Event = NULL, bool IgnoreTrigger = false, int index = -1);
		void AddGroup(CEditorEvent* Parent = NULL);
		static void AppendParameters(ACESEntry2* ace, CStringArray& strings, vector<int> type,  ParamVector& params, int number, CParametersDlg& dlg);
		
		// Edit items
		void EditAction   (CEditorAction*    Action   );
		void EditCondition(CEditorCondition* Condition);
		void EditImage	  (CEditorAction*	Action, CEditorImage* Image);
		void EditImage	  (CEditorCondition*  Condition, CEditorImage* Image);
		bool EditImage    (CEditorImage* Image);

		CEditorImage* ImageAt(CPoint pt, CEditorAction* Action);
		CEditorImage* ImageAt(CPoint pt, CEditorCondition* Condition);

		// Replace %0 etc
		CString FormString(CStringArray& strings, CString string, int objID, LPCSTR ObjectName = "");

		// Handle condition column right click
		void HandleConditionColumnRightClick(CPoint point);

		// Handle grid right click
		void HandleGridRightClick(CPoint point);

		afx_msg void OnChar(NMHDR* pNMHDR, LRESULT* pResult);

		// Bookmarks
		virtual void OnBookmark();
		virtual void OnRemoveBookmarks();

		virtual void InlineEditCondition(CEditorCondition& condition, CPoint pt);
		virtual void InlineEditAction(CEditorAction& action, CPoint pt);
		virtual bool InlineEditParam(ParamVector& params, CPoint pt);
		CEditorParam* m_inline_param;
		CParamTooltipCtrl m_Tooltip;

		CString	m_Original;

		void ReformatAction(CEditorAction*	Action, bool ReworkParams = false);
		void ReformatCondition(CEditorCondition*	Condition, bool ReworkParams = false);
		void ReformatEvent(CEditorEvent*	Event);
		void Reformat();

		void AddEventSheet();
	
		virtual void OnToolbarUndo();
		virtual void OnToolbarRedo();	

		// Update scroll bars from Treeview
		void OnRibbonAdd();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEventEditor)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;

	// Generated message map functions
public:
	void DeleteIconBitmaps();
	void CreateIconBitmaps();
	//{{AFX_MSG(CEventEditor)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnAddComment();
	afx_msg void OnExpandComment();
	afx_msg void OnCommentColour();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnInitialUpdate();
	afx_msg void OnFilePreferences();
	afx_msg void OnShowPlugins();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFindReplace();
	afx_msg void OnAllChronological();
	afx_msg void OnAllFlash();
	afx_msg void OnAllGrid();
	afx_msg void OnAllList();
	afx_msg void OnAddCondition();
	afx_msg void OnAddSubEvent();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	bool KillFocusOnInlineEditor(void);
	void ReworkParameters();
};


class CEventEditorUndo
{
public:
	CArchive arc;
	CString m_text;


	void Save(CEventEditor* Ed, CString message)
	{
		
	}
	void Release()
	{

	}
	void Restore(CEventEditor* Ed)
	{
	
		// note that as the current selected layer is a pointer to the layer, that value should be updated as well
	}
};