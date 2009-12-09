#pragma once

// ChronoEventEditor.h : header file
class CEditorEvent;
class CEditorCondition;
class CEditorAction;
class EventSheetEditor;
class CEditorParam;
class CEditorImage;

#define EVENT_TYPE		0
#define COMMENT_TYPE	1
#define GROUP_TYPE		2
#define INCLUDE_TYPE	3
#define SCRIPT_TYPE		4

typedef vector<CEditorEvent*> EventVector;
typedef vector<CEditorEvent*> SelectedEventVector;

typedef vector<CEditorCondition*> ConditionVector;
typedef vector<CEditorCondition*> SelectedConditionVector;

typedef vector<CEditorAction*> ActionVector;
typedef vector<CEditorAction*> SelectedActionVector;

typedef vector<CEditorParam*>	ParamVector;
typedef vector<CEditorImage>	ImageVector;


#include "..\Expressions\CExpression.h"
#include "..\..\Utilities\Undo.h"
#include "..\..\Editors\Scintilla\ScintillaWnd.h"
#include "..\..\UI Elements\HTML\HTMLFont.h"
#include "..\..\UI Elements\HTML\HTMLDrawer.h"

extern CHTMLDrawer g_HTMLDraw;

// Parameter types.
#define EDITORPARAM_USERPARAM			0
#define EDITORPARAM_INTEGER				1
#define EDITORPARAM_STRING				2
#define EDITORPARAM_COLOUR				3
#define EDITORPARAM_OBJECT				6
#define EDITORPARAM_KEYBOARD			7
#define EDITORPARAM_COMBOBOX			9
#define EDITORPARAM_UNDEFINED			10
#define EDITORPARAM_ARRAY				11
#define EDITORPARAM_LAYER				12
#define EDITORPARAM_GLOBALVARIABLE		13
#define EDITORPARAM_PRIVATEVARIABLE		14
#define EDITORPARAM_MENURESOURCE		15
#define EDITORPARAM_BINARYRESOURCE		16
#define EDITORPARAM_TRANSITION			17
#define EDITORPARAM_MUSIC				18
#define EDITORPARAM_SOUND				19

#include "ActionTooltip.h"

//////////////////////////
// Theme
//////////////////////////

// Events
  // Tab
#define EVENT_OUTLINE			RGB(145,155,156)
#define EVENT_OUTLINE_SEL		RGB(146,155,156)

#define EVENT_TAB_OUTLINE		RGB(102,102,102)
#define EVENT_TAB_BG			RGB(206,206,255)
#define EVENT_TAB_OUTLINE_SEL	RGB(230,139,44)
#define EVENT_TAB_BG_SEL		RGB(255,199,60)

#define PYTHON_MARGIN 16

// Information
#define EVENT_TAB_SIZE			19
#define EVENT_TAB_BUTTONX		6
#define EVENT_TAB_BUTTONY		13

// Actions
#define ACTION_BG				RGB(255,255,255)
#define ACTION_BG_EVSEL			RGB(255,255,250)
#define ACTION_BG_SEL			RGB(255,255,220)
#define ACTION_BG_SEL2			RGB(255,199,60)

// Conditions
#define CONDITION_BG			RGB(255,255,255)
#define CONDITION_BG_BOOKMARKED	RGB(192,255,255)
#define CONDITION_BG_EVSEL		RGB(255,255,255)
#define CONDITION_BG_SEL		RGB(255,255,220)
#define CONDITION_BG_SEL2		RGB(255,199,60)

// Header
#define HEADERBG				g_PaintManager->GetColor(COLOR_BTNFACE)//RGB(226,226,248)
#define HEADERBG2				RGB(255,255,255)
#define HEADEROUTLINE			RGB(145,155,156)

#define OBJECT_TAB_OUTLINE		RGB(102,102,102)
#define OBJECT_TAB_BG			RGB(206,206,255)
#define OBJECT_OUTLINE			RGB(145,155,156)
#define OBJECT_BG				RGB(255,255,255)

// Events
#define EVENT_OUTLINE			RGB(145,155,156)
#define EVENT_BG				RGB(244,244,255)
#define EVENT_BG_BOOKMARKED		RGB(255,192,255)
#define EVENT_TAB_OUTLINE		RGB(102,102,102)
#define EVENT_TAB_BG			RGB(206,206,255)

#define EVENT_OUTLINE_SEL		RGB(146,155,156)
#define EVENT_BG_SEL			RGB(255,255,192)
#define EVENT_TAB_OUTLINE_SEL	RGB(230,139,44)
#define EVENT_TAB_BG_SEL		RGB(255,199,60)

//////////////////////////

class CChronoEventEditor;

class CChronoAnimation
{
//This class is used to make animation when the user deletes something
public:

	double distance;
	float Space; //stores height
	__int64 starttime;
	int StartSpace;

	bool m_bDestroyed;
	void Animate(int currenttime)
	{		

		distance = (currenttime - starttime ) / 300.0;
		if(distance > 1) distance = 1;
		distance = 1-distance;
		distance = distance*distance;
		distance = 1-distance;
		
		Space = StartSpace * (1-distance);
	
	}
	void Start(int currenttime)
	{
		m_bDestroyed = true;
		starttime = currenttime;
		StartSpace = Space;
		Space = Space;
	}
	void Destroy()
	{
		m_bDestroyed = true;
		starttime = 0;
		Space = 0;

	}

};

/////////////////////////////////////////////////////////////
// Font Manager Class
class CFontMgrItem
{
	public:
		CFont* m_myFont;
		int m_life;
};

class CFontMgr
{
	public:
		vector<CFontMgrItem> m_Fonts;

	public:
		CFont* RequestFont(int nHeight, int nWidth, int nEscapement, int nOrientation,
			int nWeight, BYTE bItalic, BYTE bUnderline, BYTE cStrikeOut, BYTE nCharSet,
			BYTE nOutPrecision, BYTE nClipPrecision, BYTE nQuality, BYTE nPitchAndFamily,
			LPCTSTR lpszFacename)
		{
			LOGFONT pLogFont;
			for(int a = 0; a < m_Fonts.size(); a++)
			{	
		
				m_Fonts[a].m_myFont->GetLogFont(&pLogFont);

				if(nHeight == pLogFont.lfHeight
					&& nWidth == pLogFont.lfWidth
					&& nEscapement == pLogFont.lfEscapement
					&& nOrientation == pLogFont.lfOrientation
					&& nWeight == pLogFont.lfWeight
					&& bItalic == pLogFont.lfItalic
					&& bUnderline == pLogFont.lfUnderline
					&& cStrikeOut == pLogFont.lfStrikeOut
					&& nCharSet == pLogFont.lfCharSet
					&& nOutPrecision == pLogFont.lfOutPrecision
					&& nClipPrecision == pLogFont.lfClipPrecision
					&& nQuality == pLogFont.lfQuality
					&& nPitchAndFamily == pLogFont.lfPitchAndFamily
					&& strcmp(lpszFacename, pLogFont.lfFaceName) == 0)
				{
					return m_Fonts[a].m_myFont;
				}
				if(m_Fonts[a].m_life <= 0)
				{
				//	m_Fonts.erase(m_Fonts.begin() + a);
				//	a --;
				}

				//if(m_
			}

			// Okay...we dont already have the font
			CFontMgrItem MyFont;
			MyFont.m_myFont = new CFont;

			MyFont.m_myFont->CreateFont(nHeight, nWidth, nEscapement,nOrientation,
				nWeight, bItalic, bUnderline, cStrikeOut, nCharSet,
				nOutPrecision, nClipPrecision, nQuality, nPitchAndFamily,
				lpszFacename);
			MyFont.m_life = 1;
			m_Fonts.push_back(MyFont);
			return MyFont.m_myFont;
		}


};

extern CFontMgr gMyFontManager;

/////////////////////////////////////////////////////////////
//  Event List Classes

/*
Sprite.X + 30

Sprite
.				-dot
X				-expression
 
+				-operator
				-whitespace
30				-number

*/


enum EditorParamTokenType {
	PTOK_STRING,
	PTOK_OBJECTNAME,
	PTOK_FAMILYNAME,
	PTOK_COLOR,
};

class CEditorParamToken
{
public:
	EditorParamTokenType type;	// 0 = string, 1 = object name, 2 = family name, 3 = color
	CString tok;
	bool trans;
};

class CExport;
class CApplication;
class CEditorCondition;
class CEditorAction;

#include "..\EditorParam.h"

class CEditorImage
{
public:

	CEditorImage()
	{
	//	type = 0;
		tok = 0;
	//	str = 0;


	}

	CRect m_Rect; // clickable region

	//int type; // 0 = display 1 = clickable and change object via string and or long.

	Token* tok;
//	CString* str;	// contains image in string
//	int* id;		// contains image in int
	
};

typedef map< CChronoEventEditor*, BOOL* > ChronoBoolMap;
typedef map< CChronoEventEditor*, CRect* > ChronoRectMap; 
typedef map< CChronoEventEditor*, int* >  ChronoIntMap; 

BOOL& GetMapItemFunction_bool(  ChronoBoolMap *map, BOOL& def, CChronoEventEditor* ed);
int& GetMapItemFunction_int( ChronoIntMap *map, int& def, CChronoEventEditor* ed);
CRect& GetMapItemFunction_CRect( ChronoRectMap *map, CRect& def, CChronoEventEditor* ed);

#include "..\EditorCondition.h"
#include "..\EditorAction.h"



#include "..\EditorEvent.h"

EventVector* FindEventListEventExistsIn(CEditorEvent* Event, EventVector* pEventList,int& index);
CEditorEvent* FindEventConditionExistsIn(CEditorCondition* Condition, EventVector* pEventList,int& index);
CEditorEvent* FindEventActionExistsIn(CEditorAction* Action, EventVector* pEventList,int& index);



/////////////////////////////////////////////////////////////
// Header Classes


class CEditorObject
{
/// we would normally have a pointer in here to the actual object.
public:
	int width;
	int oid;
	bool family;
	CRect m_rect;
	void Serialize(CArchive& ar)
	{
		if(ar.IsLoading())
		{
			ar >> width;
			ar >> oid;
			ar >> family;
		}
		else
		{
			ar << width;
			ar << oid;
			ar << family;
		}


	}
	CEditorObject()
	{
		width = 32;		
	}


};

class CEditorHeader
{
public:
	void Serialize(CArchive& ar);

	CEditorHeader()
	{

		m_Split = 200;
		m_isDrag = false;

	}
	CEditorObject* GetObjectAtX(long x);
	int m_DragNumber;
	bool m_isDrag;
	int m_Split;
	void OnLeftUp(CPoint pt);
	void OnMouseMove(CPoint pt);
	void OnLeftClick(CPoint pt);
	CRect m_rect;
	vector<CEditorObject> m_Objects;
	void Render(CRect &Pos, CDC& dc, CChronoEventEditor& Ed);
};

/////////////////////////////////////////////////////////////////////////////
// Dragging and dropping and copying classes


class CEditorDragEvents : public CObject
{
public:

	EventVector* m_pEventListPart; // event list to drop into
//	EventVector* m_pEventList; // main event list!
	SelectedEventVector* m_pSelectedEvents;
	int index;
	bool GoingUp;

	EventSheetEditor* pEd;

	virtual void Serialize( CArchive& ar ) ;
};




class CEditorDragActions : public CObject
{
public:

	ActionVector* m_pActions; // list to drop Actions into
	SelectedActionVector* m_pSelectedActions;
	int index;
	bool GoingUp;

	EventSheetEditor* pEd;


	virtual void Serialize( CArchive& ar ) ;
};



class CEditorDragObject : public CObject
{
public:
	virtual void Serialize( CArchive& ar ) ;
	int ObjectIdentifier;
	EventSheetEditor* pEd;
};



class CEditorDragConditions : public CObject
{
public:

	ConditionVector* m_pConditions; // list to drop conditions into
	SelectedConditionVector* m_pSelectedConditions;
	int index;
	bool GoingUp;

	EventSheetEditor* pEd;

	virtual void Serialize( CArchive& ar ) ;
};



/////////////////////////////////////////////////////////////////////////////
// CChronoEventEditor window

class CChronoEventEditor : public CScrollView
{
friend class CChronoEventEDViewView;
friend class CEditorHeaderTooltip;
friend class CFlatSplitterWnd;
// Construction

public:
	CChronoEventEditor();
	virtual ~CChronoEventEditor();

	CPoint m_Mouse;
	CDragDropMgr* m_pDDMgr;

	CScintillaWnd	m_pInlineScintilla;

	virtual void OnDraw(CDC *)
	{
		//
	}

	DECLARE_DYNCREATE(CChronoEventEditor)

// Attributes
public:
	CActionTooltip m_ActionTooltip;
	CHoverButton m_InsertBefore;
	CHoverButton m_InsertAfter;
	CHoverButton m_InsertSub;

	virtual void Insert_ClickAfter();
	virtual void Insert_ClickBefore();

	CEditorHeaderTooltip m_ObjectHeaderTooltip;
// Operations
public:
	int scrolllock;
	void LockScrollUpdate();
	void RefreshHeader();

	void UnlockScrollUpdate();
	vector<HBITMAP> m_hbitmaps;
	CMap<int,int,int,int> m_objecthbitmaps;
	CMap<int,int,int,int> m_familyhbitmaps;

	void NumerateEventList(EventVector* m_pEventList, int& num);

	void ResetScrollBar();

	BOOL Create(int n, int x, DWORD dwStyle,CRect WndSize, CWnd* pParentWnd, UINT nID, CCreateContext* p)
	{
		return CScrollView::Create(0,AfxRegisterWndClass(CS_CLASSDC), dwStyle,WndSize, pParentWnd, nID, p);
	}

	virtual HBITMAP GetIcon(int oid)
	{
		return NULL;
	}

	EventVector* m_pEventList;
	CEditorHeader* m_pHeader;

public:
	bool m_ShiftDown;
	CEditorCondition* m_FocusCondition;
	bool m_ClickedSelectedCondition;
	CEditorAction* m_FocusAction;
	bool m_ClickedSelectedAction;
	CEditorEvent* m_FocusEvent;
	bool m_ClickedSelectedEvent;
	bool m_CtrlDown;


	// Retrieve information for a point
	CEditorEvent* EventAt(CPoint pt, CEditorEvent** pRecieveParent = NULL);
	int EventNumberAt(CPoint pt);
	virtual void RegisterObjectID(int& oid, CArchive& ar){};

public:
	CUndo*			m_undo;


protected:

	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();

	virtual void InlineEditCondition(CEditorCondition& condition,CPoint pt);
	virtual void InlineEditAction(CEditorAction& action, CPoint pt);
	virtual bool KillFocusOnInlineEditor(void);
	virtual void CreateUndo(){}

	BOOL OnMouseWheel(UINT fFlags, short zDelta, CPoint point);
public:
	CEditorEvent* FindNextVisibleEvent(CEditorEvent* pEvent);

	CRect m_FocusRect;
	bool m_leftDown;

	struct DragHelper
	{
		int dragtype;
		CPoint StoredPos;
		bool IsDragging;
		bool GoingUp;
		bool CtrlDown;
	};

	DragHelper m_Drag;

	bool m_bScrollBarMessage;
	long m_tmpScrollCount;
	long m_Ellapsed;
	bool Animate(CEditorEvent& EventList);
	CFont MyFont;

// Generated message map functions
protected:
	//{{AFX_MSG(CChronoEventEditor)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);


	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);

	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	
	void OnNextBookmark();
	void OnPreviousBookmark();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CPoint ScrollPos;

////////////////////
// BLOCK - Special Per View Scrolling code

		virtual void UpdateScrollbars(bool forced = false);
	// if one of the views is activated we update the scrollbars
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView,
					CView* pDeactiveView)
	{
		CScrollView::OnActivateView( bActivate,  pActivateView,
					 pDeactiveView);
		if(bActivate)
		{
			this->SetRedraw(false);
			UpdateScrollbars(true);
			CScrollView::SetScrollPos(0,ScrollPos.x);
			CScrollView::SetScrollPos(1,ScrollPos.y);
			UpdateScrollbars(true);
			this->SetRedraw(true);
			//
		}

		if(bActivate)
			Invalidate();
	}

	CPoint GetScrollPosition()
	{
		//return CScrollView::GetScrollPosition();
		return ScrollPos;
	}

	int SetScrollPos(int nBar, int nPos, BOOL bRedraw = TRUE)
	{
		BOOL ret;
		ret = CScrollView::SetScrollPos(nBar,  nPos,  bRedraw);
		if(nBar ==0)
			ScrollPos.x = CScrollView::GetScrollPos(0);
		if(nBar ==1)
			ScrollPos.y = CScrollView::GetScrollPos(1);


		return ret;
	}

private:
	void CenterOnPoint(CPoint ptCenter)
	{
		CScrollView::CenterOnPoint( ptCenter);
		ScrollPos = CScrollView::GetScrollPosition();
	}

public:
	void ScrollToPosition(POINT pt)
	{
		CScrollView::ScrollToPosition( pt);
		ScrollPos = CScrollView::GetScrollPosition();
		if(!CScrollView::GetScrollBarCtrl(0)->IsWindowEnabled())
		{
			ScrollPos.x = 0;
			GetScrollBarCtrl(0)->SetScrollPos(0);
		}
		if(!CScrollView::GetScrollBarCtrl(1)->IsWindowEnabled())
		{
			ScrollPos.y = 0;
			GetScrollBarCtrl(1)->SetScrollPos(0);
		}
	}

	int GetScrollPos(int bar)
	{
		if(bar == SB_HORZ)
			return ScrollPos.x;
		else
			return ScrollPos.y;
	}
};