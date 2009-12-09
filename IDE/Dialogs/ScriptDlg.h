#pragma once
#include "afxwin.h"
#include "..\resource.h"
#include "..\..\Utilities\anchor.h"
#include "..\..\Utilities\dlgman.h"
#include "Popup dialogs\Snippets Dlg.h"

class PyInfoClass;

class PyInfoFunc
{
public:
	PyInfoFunc();

public:
	PyInfoClass* pParent;
	CString name;
	CString help;
};

class PyInfoVar
{
public:
	PyInfoVar();
public:
	PyInfoClass* pParent;

	CString name;
	CString help;

	map<PyInfoClass*, int> classtypes;
};

class PyInfoKeyword
{
public:
	PyInfoKeyword();
public:
	PyInfoClass* pParent;

	CString name;
	CString help;
};

class PyInfoClass
{
public:
	PyInfoClass();
public:
	CString name;
	CString help;
	PyInfoClass* pParent;


	map<CString, PyInfoClass> classes;
	map<CString, PyInfoFunc> functions;
	map<CString, PyInfoVar> variables;
	map<CString, PyInfoKeyword> keywords; // for  while  etc
};

enum infoType
{
	PY_VARIABLE=0,
	PY_FUNCTION=1,
	PY_CLASS=2,
	PY_KEYWORD=3,
	PY_OPERATOR=4
};

typedef map<PyInfoClass*, infoType> PyInfoMapItem;
typedef map<CString, PyInfoMapItem> PyInfoMap;

class functiondata
{
public:
	map<PyInfoClass*, int> m_IntellMap;
	map<infoType, int> m_InfoType;
};

class CFunctionListBox : public CListBox
{
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

public:
	list<functiondata> data;
	
};
class CClassListBox : public CCheckListBox
{
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

public:
	list<functiondata> data;

};


class CScriptDlg : public CExtNCW<CExtResizableDialog>
{
	DECLARE_DYNAMIC(CScriptDlg)

public:
	CScriptDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CScriptDlg();

	BOOL OnInitDialog() ;
	CString m_script;
	CApplication* pApp;

	// Snippets
	CSnippetsDlg m_Snippets;

	CDlgAnchor dlgAnchor;
	CDlgMan dlgMan;

	CExtToolControlBar m_wndToolBar;

// Dialog Data
	enum { IDD = IDD_SCRIPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy() ;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	void OnOk();
	void OnModify();
	void OnCheckClass();

	DECLARE_MESSAGE_MAP()

public:
	CExtButton		m_OK;
	CExtButton		m_Cancel;
	CScintillaWnd	m_Script;
	CScintillaWnd	m_Help;
	CEdit			m_HelpLine;

	CExtEdit		m_VariableName;

	CCheckListBox m_Classes;
	CFunctionListBox m_Functions;

	HMODULE m_hDll;

	PyInfoClass m_Intell;
	PyInfoMap m_IntellMap;

	CString m_BeforeDotText;
	CString m_AfterDotText;
	CString m_OperatorText;
	bool m_Dot;

	void OnEnterVariable();
	void OnEnterFunction();
	void OnDblClickFunction();
	void UpdateHelpLine();
	void OnFunctionSelChange();

	void UpdateList();
	void UpdateClassList();

	void InitPythonInfo();
	void InitPythonForObjects();
	void InitPythonForObject(CObjType* pType, PyInfoClass* pClass);

	// Toolbar
	void OnNew();
	void OnLoad();
	void OnSave();
	void OnCut();
	void OnCopy();
	void OnPaste();
	void OnUndo();
	void OnRedo();
	void OnSnippets();

	// Sends a message to the Scintilla editor
	LRESULT SendEditor(UINT Msg, WPARAM wParam=0, LPARAM lParam=0) 
	{	return ::SendMessage(m_Script.m_hWnd, Msg, wParam, lParam); }

	void SetAStyle(int style, COLORREF fore, COLORREF back=RGB( 0, 0, 0 ), int size=-1, const char *face=0)
	{	SendEditor(SCI_STYLESETFORE, style, fore);
		SendEditor(SCI_STYLESETBACK, style, back);
		if (size >= 1)
			SendEditor(SCI_STYLESETSIZE, style, size);
		if (face) 
			SendEditor(SCI_STYLESETFONT, style, reinterpret_cast<LPARAM>(face));
	}

public:
	afx_msg void OnEnChangeScript();
	void HelpMe();
	void OnChar(char ch);
	CScintillaWnd* GetSelectedScintilla();

public:
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnEnChangeHelp();
};

#include "Popup dialogs\Snippets Dlg.h"