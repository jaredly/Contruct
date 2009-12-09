#pragma once

#include <afxtempl.h>
#include "..\Editors\Scintilla\ScintillaWnd.h"

#define		WM_RUBRIC_WND_CLICKED_ON		( WM_APP + 04100 )

class ObjectParameterButton : public CExtButton
{
public:
	
	ObjectParameterButton(CApplication* application_, CLayout* layout_);
	virtual void _OnClick(bool, bool);

	CApplication* application;
	CLayout* layout;

	CString object;
};

class TDS_PANE
{
public:
	TDS_PANE() :
		rubric(NULL),
		content_window(NULL),
		use_private_data(false),
		keep_string(false)
	{	
	}

	void SetParameter(int type_, CWnd* window)
	{
		content_window = window;
		type = type_;
	}

	CWnd*			rubric;
	CWnd*			content_window;
	int				type;

	bool			open;
	bool			use_private_data;
	bool			keep_string;

	CString GetParameterString()
	{
		CString data = "";

		switch (type)
		{
		case EDITORPARAM_INTEGER:
		case EDITORPARAM_STRING:
		case EDITORPARAM_UNDEFINED:
		case EDITORPARAM_ARRAY:
		case EDITORPARAM_LAYER:
		case EDITORPARAM_USERPARAM:
			{
				// get scintilla text, cast it to a scintilla window
				CScintillaWnd* scintilla = (CScintillaWnd*)content_window;
				scintilla->GetWindowTextA(data);
			}

			break;

		case EDITORPARAM_COLOUR:
			{
				CExtColorButton* button = (CExtColorButton*)content_window;
				COLORREF colour = button->m_clrSelected;
				data.Format("%d", colour);
			}

			break;

		case EDITORPARAM_KEYBOARD:
			{
				CHotKeyCtrl* hotkey = (CHotKeyCtrl*)content_window;
				data.Format("%s", hotkey->GetHotKeyName());
			}

			break;

		case EDITORPARAM_OBJECT:
			{
				ObjectParameterButton* button = (ObjectParameterButton*)content_window;
				data.Format("%s", button->object);
			}

			break;

		case EDITORPARAM_COMBOBOX:
		case EDITORPARAM_PRIVATEVARIABLE:
		case EDITORPARAM_MENURESOURCE:
		case EDITORPARAM_BINARYRESOURCE:
		case EDITORPARAM_TRANSITION:
		case EDITORPARAM_GLOBALVARIABLE:
			{
				CComboBox* combobox = (CComboBox*)content_window;

				// use the item data of the selected item rather than the name
				if(use_private_data)
					data.Format("%d", combobox->GetItemData(combobox->GetCurSel()));	

				// use the string
				else if (keep_string)
				{
					CString string;
					combobox->GetLBText(combobox->GetCurSel(), string);
					data.Format("\"%s\"", string);
				}

				else
					data.Format("%d", combobox->GetCurSel());
			}
		}

		return data;
	}
};
typedef TDS_PANE* PTDS_PANE ;

/////////////////////////////////////////////////////////////////////////////
// CStackedWndCtrl window

class CStackedWndCtrl : public CExtResizableDialog
{
// Construction
public:
	CStackedWndCtrl();
	void OnChange();
	CLayout* layout;

	bool updateTwice;

// Attributes

	PTDS_PANE m_pPane;
	int					m_iRubricHeight;

	CArray<PTDS_PANE, PTDS_PANE>		m_arrPanes;

// Operations
public:
	void			RearrangeStack		();

	virtual BOOL CreateScrollBarCtrl(DWORD dwStyle, UINT nID);

public:
	int				AddPane				( CWnd* pwndRubric, TDS_PANE Pane );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStackedWndCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT *pResult);
	
	void OnChar(NMHDR* pNMHDR, LRESULT* pResult);
	void OnChange(NMHDR* pNMHDR, LRESULT* pResult);
	void OnFocusChange(UINT id);
	int m_ScrollY;

	virtual ~CStackedWndCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStackedWndCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	afx_msg LRESULT OnRubricWndClicked(WPARAM wParam, LPARAM lParam);

	BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
};