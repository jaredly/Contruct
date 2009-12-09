#pragma once

// File includes
#include "..\ChronoEventEditor\ChronoEventEditor.h"
#include "..\..\Dialogs\FindReplaceDlg.h"
#include "..\..\Editors\Scintilla\include\Scintilla.h"
#include "..\..\Editors\Scintilla\ScintillaWnd.h"
#include "..\..\Dialogs\Event Wizard Dialog\EventWizardDlg.h"
#include "..\..\Dialogs\ParametersDlg.h"
#include "..\..\Dialogs\ScriptDlg.h"
#include "..\..\Dialogs\EditEventGroup.h"
#include "..\..\MainFrm.h"
#include "..\..\Dialogs\AddCommentDlg.h"
#include "..\..\Structure\SystemObject.h"
#include "..\..\Structure\ObjectFunctions.h"
#include "..\..\Dialogs\ObjectSelector.h"
#include "..\..\UI Elements\ParamScintilla.h"

/////////////////////////////////////////////////////////////////////////////
// EventSheetEditor definition

class EventSheetEditor : public CChronoEventEditor
{
protected: 
	DECLARE_DYNCREATE(EventSheetEditor)


public:
	EventSheetEditor();
	~EventSheetEditor();

	// Lookup
	CEditorEvent* EventAt(CPoint pt, CEditorEvent** pRecieveParent = NULL);
	int EventNumberAt(CPoint pt);

	void RecursiveSelect(CEditorAction* action1, CEditorCondition* condition1, CEditorEvent* event1, CEditorAction* action2, CEditorCondition* condition2,
		CEditorEvent* event2,
		bool& inside_action, bool& inside_condition ,bool& inside_event, EventVector* m_pEventList);

	// Selections
	void CreateEventSelectionVectorLimited(SelectedEventVector& SelectedEventList, EventVector& EventList);
	void CreateEventSelectionVector(SelectedEventVector& SelectedEventList, EventVector& EventList);
	void CreateConditionSelectionVector(SelectedConditionVector& pSelectedConditionList, EventVector& EventList);
	void CreateActionSelectionVector(SelectedActionVector& pSelectedActionList, EventVector& EventList);
	
	void DeselectEvents();
	void DeselectConditions();
	void DeselectActions();

	void ExpandAllGroups(bool open, CEditorEvent* pParent = NULL);

	BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point); 

	// Mouse & keyboard
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlag);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint pt);

	// Clipboard
	afx_msg void OnToolbarCut();
	afx_msg void OnToolbarCopy();
	afx_msg void OnToolbarPaste();
	afx_msg void OnToolbarDelete();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);

	// Bookmarks
	void FindBookmarkEvent(CEditorEvent* pParent, bool& found, bool& found_first, bool& found_vis, bool& down, int& y, bool& closed);

	// This should be moved somewhere else like CMainFrame, and used by the project bar etc
	void OpenEventSheet(EventSheet* pEventSheet);

	// Drawing
	void OnDraw(CDC* pDC);

	CConstructDoc*	GetDocument();
	CLayout*		layout;
	CApplication*	application;
	CDragDropMgr ddm;
	int m_LastRibbon;
	HBITMAP GetIcon(int oid);
	
	void AddAction(CEditorEvent* pEvent, int index);
	CAction* PerformAction(CAction *action, BOOL bRepeat);
	void CreateUndo();
	void AddScript();
	void EditScript(CEditorEvent* pEvent);
	void EditComment(CEditorEvent* pComment);

// Operations
public:

	// Necessary members
	CAddCommentDlg m_CommentDlg;
	CFont          m_HeaderFont;
	CImageList     m_ImageList;
	CImageList     m_Images;

	void Insert_ClickAfter();
	void Insert_ClickBefore();
	void Insert_ClickSub();

	void RecurseEdit(CEditorEvent* pEvent, int iType);
	EventVector* FindEventListParent(CEditorEvent* pEvent, EventVector* eventlist);
	CEditorEvent* FindEventParent(CEditorEvent* pEvent, CEditorEvent* pEventParent = NULL);

	// Type checking

	bool              m_ParameterTooltipIsVisible;
	bool              m_QuotesOpen;


	bool tts;
	bool tooltip;

	// Object IDs
	CStringArray objId;
	CStringArray objTypeId;

	// Comment colour
	COLORREF m_CommentColour;

	// Find & Replace
	CFindReplaceDlg m_FindReplaceDlg;

	//////////////////////////////////
	// New to 0.96.3
	//////////////////////////////////
	int		m_InitialStage;
	int		m_InitialSelect;
	CString	m_OldName;

	// for scrolling while drag/select/resize
	UINT    m_nTimerID;
	DWORD	m_lastTick;
	///////////////////////////////////

	// Helper functions
	// Add a condition
	bool AddCondition (bool bPreset = false, CEditorCondition* Parent = NULL, bool SubEvent = false, CEditorEvent* Event = NULL, bool IgnoreTrigger = false, int index = -1);
	void AddGroup(CEditorEvent* Parent = NULL, CEditorEvent* pAfter = NULL);
	static void AppendParameters(ACESEntry2* ace, CStringArray& strings, vector<int> type,  ParamVector& params, int number, CParametersDlg& dlg);
	
	// Edit items
	void EditAction   (CEditorAction*    Action, int step = 3);
	void EditCondition(CEditorCondition* Condition, int step = 3);
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

	// When you switch to the layout editor
	void OnSwitchToLayoutEditor();

	afx_msg void OnChar(NMHDR* pNMHDR, LRESULT* pResult);

	// Bookmarks
	virtual void OnBookmark();
	void RemoveBookmarks();
	void NextBookmark();
	void PreviousBookmark();

	// Toggle event
	void OnToggleEvent();

	virtual void InlineEditCondition(CEditorCondition& condition, CPoint pt);
	virtual void InlineEditAction(CEditorAction& action, CPoint pt);
	virtual bool InlineEditParam(ParamVector& params, CPoint pt);
	CEditorParam* m_inline_param;
	CParamTooltipCtrl m_Tooltip;
	CString m_Original;

	void ReformatAction(CEditorAction*	Action, bool ReworkParams = false);
	void ReformatCondition(CEditorCondition*	Condition, bool ReworkParams = false);
	void ReformatEvent(CEditorEvent*	Event);
	void Reformat();

	void AddEventSheet();

	virtual void OnToolbarUndo();
	virtual void OnToolbarRedo();	

	// Update scroll bars from Treeview
	void OnRibbonAdd();

	// Serializing
	void RegisterObjectID(int& oid, CArchive& ar);

	class ObjectIDTranslator // class used
	{
	public:
		ObjectIDTranslator(){use = false;}
		void translate_id_save(int &oid);
		void translate_id_load(int &oid);

		map<int, int> translate_to_table;
		set<int> used_items;

		void serialize_objects(CArchive& ar, CApplication* pApp);

		void begin();
		void end();

		bool use;

	} object_id_translator;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EventSheetEditor)
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
	//{{AFX_MSG(EventSheetEditor)
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
	afx_msg void OnDestroy();
	afx_msg void OnInitialUpdate();
	afx_msg void OnFilePreferences();
	afx_msg void OnShowPlugins();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFindReplace();
	afx_msg void OnAllChronological();
	afx_msg void OnAllList();
	afx_msg void OnAddCondition();
	afx_msg void OnAddAction();
	afx_msg void OnAddSubEvent();
	afx_msg void OnAddEvent();
	afx_msg void OnAddGroup();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	CString StripHTML(LPCSTR String);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	bool KillFocusOnInlineEditor(void);
};


class EventSheetEditorUndo
{
public:
	CArchive arc;
	CString m_text;


	void Save(EventSheetEditor* Ed, CString message)
	{
		
	}
	void Release()
	{

	}
	void Restore(EventSheetEditor* Ed)
	{
	
		// note that as the current selected layer is a pointer to the layer, that value should be updated as well
	}
};