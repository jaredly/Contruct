
#define EVENT_INDENT 20

// Serves as:
//	- An event
//	- A comment
//	- A group
//	- An include
// Todo: probably needs separate classes for each of these uses!

class CEditorEvent
{
	public:

		~CEditorEvent();

		int m_type; // 0 = event, 1 = comment, 2 = group, 3 = Include, 4 = script

		CRect m_rect_default;
		ChronoRectMap m_ED_rect;
		CRect& m_rect(EventSheetEditor* Ed = NULL); 

		CRect m_gridRec_default;
		ChronoRectMap m_ED_gridRec;
		CRect& m_gridRec(EventSheetEditor* Ed = NULL); 

		CRect m_handle_default;
		ChronoRectMap m_ED_handle;
		CRect& m_handle(EventSheetEditor* Ed = NULL); 

		CRect m_CondArea_default;
		ChronoRectMap m_ED_CondArea;
		CRect& m_CondArea(EventSheetEditor* Ed = NULL); 

		BOOL m_select_default; // BOOL
		ChronoBoolMap m_ED_select;
		BOOL& m_select(EventSheetEditor* Ed = NULL); 

		BOOL m_visible_default; // BOOL
		ChronoBoolMap m_ED_visible;
		BOOL& m_visible(EventSheetEditor* Ed = NULL); 

		int m_displayActionType_default ;// 0 = grid 1 = chrono 2 = list grid, 3 = list chrono
		ChronoIntMap m_ED_displayActionType;
		int& m_displayActionType(EventSheetEditor* Ed = NULL); 

		int m_displayConditionType_default ;// 0 = grid 1 = chrono 2 = list grid, 3 = list chrono
		ChronoIntMap m_ED_displayConditionType;
		int&  m_displayConditionType(EventSheetEditor* Ed = NULL); 

		int m_displayEventType_default ;// 0 = left and right, 1 = completely vertical
		ChronoIntMap m_ED_displayEventType;
		int&  m_displayEventType(EventSheetEditor* Ed = NULL); 

		BOOL m_open_default; //used like a tree + - box
		ChronoBoolMap m_ED_open;
		int&  m_open(EventSheetEditor* Ed = NULL); 

		bool freeMemory;

		CEditorEvent(bool _freeMemory = true) {
			m_displayActionType_default = 3;
			m_displayConditionType_default  = 3;
			m_displayEventType_default  = 1;
			freeMemory = _freeMemory;

			//set bookmark to false
			m_bBookmarked = false;
			m_bEnabled = true;
			mouseOverNewAction = false;
		}

		void ForceTriggersToTop();

		int m_number;
		int m_BreakPoint;

		bool m_bBookmarked;
		bool m_bEnabled;

		bool canAddEvents() 
		{
			switch (m_type)
			{
				case EVENT_TYPE:
					return true;
				case COMMENT_TYPE:
					return false;
				case GROUP_TYPE:
					return true;
				case INCLUDE_TYPE:
					return false;
				default:
					return false;
			}
		}
		bool canAddConditions() 
		{
			switch (m_type)
			{
				case EVENT_TYPE:
					return true;
				case COMMENT_TYPE:
					return false;
				case GROUP_TYPE:
					return false;
				case INCLUDE_TYPE:
					return false;
				default:
					return false;
			}
		}
		bool canAddActions() 
		{
			return canAddEvents();
		}

		bool IsElseEvent()
		{
			if(m_Conditions.size() > 0)
				if(m_Conditions.at(0)->oid == -1)
					if(m_Conditions.at(0)->m_Anim.m_bDestroyed == false)
						if(m_Conditions.at(0)->cndID == 17)
							return true;
			return false;
		}

	// For events
	public:
		ConditionVector m_Conditions;
		ActionVector m_Actions;
		EventVector m_EventList;

	// For Comments and python
	public:
		CString m_cText;
		COLORREF m_cTextBG;
		COLORREF m_cTextColour;
		CString m_cFontName;
		int m_cFontSize;
		BOOL m_cbFontBold;
		BOOL m_cbFontItalic;
		BOOL m_cbFontStrike;
		BOOL m_cbFontUnderline;
	


	// For Subevents
	public:
		CString m_sTitle;
		CString m_sDescription;

	// Group of events activated
		bool m_sActive;

	// for includes
	public:
		int event_sheet_include;
	public:

		void Init();
		bool DeleteSelectedEvents(SelectedEventVector& ParentEventList, int& parentIndex,EventSheetEditor& Ed, int* subtractfromanim);
		bool InstantDeleteSelectedEvents(SelectedEventVector& ParentEventList, int& parentIndex, EventSheetEditor* pEd);
		void Select(bool select, EventSheetEditor* pEd);
		//void Release();

		bool OnToggleBookmarks(CEditorEvent& Events, EventSheetEditor& Ed);

		void Remove(bool Animate, EventSheetEditor* pEd);
		bool PtInOnOffButton(CPoint pt, EventSheetEditor* pEd);
		bool Serialize( CArchive& ar,bool Save=false, EventSheetEditor* Ed = NULL);

		// Retrieve event at point
		CEditorEvent* EventAt(CPoint pt, EventSheetEditor* pEd, CEditorEvent** pRecieveParent = NULL);

		CEditorCondition* conditionAt(CPoint pt, EventSheetEditor* pEd);
		CEditorAction* actionAt(CPoint pt, EventSheetEditor* pEd);
		CEditorAction* actionAt(CPoint pt,SelectedActionVector& Actions, EventSheetEditor* pEd);
		
	//	void	m_action;

		int Render(CRect &Pos, CDC& dc, bool ParentSelected,EventSheetEditor& Ed,int& previousy, EventVector* pEventList, int index);

		CChronoAnimation m_Anim;

		bool mouseOverNewAction;
		bool mouseOverFooter;

};