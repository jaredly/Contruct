#pragma once
#include "afxcmn.h"

#include "Tree\ResizablePage.h"

// A key that generates a line in the watch.
struct WatchKey {
	CString label;

	bool operator==(const WatchKey& other) const {
		return label == other.label;
	}
};

// The type of a watch item
enum WatchType {
	WATCH_OBJTYPE,					// Source is an object type
	WATCH_INSTANCE,					// Source is an object instance
	WATCH_INSTANCEPRIVATEVAR,		// Source is an object instance's private variable
	WATCH_GLOBALVAR,				// Source is a global variable
};

// An item being watched: an object instance, or a type.
struct WatchItem {
	WatchType type;

	// When type is WATCH_INSTANCEPRIVATEVAR, details of the private variable to watch
	int pvIndex;
	ExpStore* pPrivateVars;

	// Source type or instance
	union {
		CRunObject* obj;
		CRunObjType* objType;
	};

	bool operator<(const WatchItem& other) const {
		// Hackish way to define this operator, but the map requires it...
		__int64 lTotal = (__int64)type + pvIndex + (__int64)pPrivateVars + (__int64)obj;
		__int64 rTotal = (__int64)other.type + other.pvIndex + (__int64)other.pPrivateVars + (__int64)other.obj;
		return lTotal < rTotal;
	}
	bool operator==(const WatchItem& other) const {
		return type == other.type && obj == other.obj && pvIndex == other.pvIndex && pPrivateVars == other.pPrivateVars;
	}
};

typedef vector<WatchKey> KeyList;
typedef map<WatchItem, KeyList> Watch;

// A line being displayed in the watch
struct WatchLine {
	CString label;
	CString value;
	CString objIdent;		// Object identifier eg. Sprite #5
	bool readOnly;

	KeyList::iterator pItem;
	Watch::iterator pKey;
};

class CWatchPage : public CResizablePage
{
	DECLARE_DYNCREATE(CWatchPage)

// Construction
public:
	CWatchPage();
	~CWatchPage();

	Watch watch;

	vector<WatchLine> lastDisplay;

	void RefreshWatch();
	bool active;
	bool pausedUpdating;

	class CObjectsPage* pObjectsPage;
	CRuntime* pRuntime;

	LRESULT OnEditEnd(WPARAM nItem, LPARAM nSubItem);
	void OnClick(NMHDR* pNMHDR, LRESULT* pResult);

// Dialog Data
	//{{AFX_DATA(CMyPropertyPage1)
	enum { IDD = IDD_WATCHPAGE };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMyPropertyPage1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMyPropertyPage1)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CXListCtrl m_List;
public:
	afx_msg void OnWatchKeydown(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnRclickWatch(NMHDR *pNMHDR, LRESULT *pResult);
};