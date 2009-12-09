// MenuEditorDlg.h : header file
//

#pragma once

class MenuEditorItem;
class CMenuEditor;

class MenuEditorItem
{
public:
	int m_type;	// 0 = menu   1 = popup    2 = separator
	CRect m_rect;
	bool disabled;
	bool inserttext;
	vector<MenuEditorItem*> m_items;	// subitems

	int openned;

public:
	CString m_Text;
	bool m_Checked;
	int m_ID;
	bool m_Disabled;

public:
	void init(CMenuEditor* l_dlg)
	{
		m_type = 0;
		m_rect = CRect(0,0,0,0);
		disabled = false;
		m_Text = "Type Here";
		pEditor = l_dlg;
		openned = -1;
		inserttext = true;

		 m_Checked = 0;
		 m_ID = 0;
		 m_Disabled = 0;
	}

	void Paint(CDC& dc, CRect& rect);
	void PaintPopup(CDC& dc, CRect& rect);
	void PaintMenu(CDC& dc, CRect& rect);
	void PaintSeparator(CDC& dc, CRect& rect);

	CMenuEditor* pEditor;

	MenuEditorItem* GetPtr(){ return this;}

	void CreateFromMenuResource(MenuResourceItem& Item, CMenuEditor* dlg)
	{
		init(dlg);
		// Item info
		m_Text = Item.text;
		m_Checked = Item.checked;
		m_ID = Item.id;
		m_Disabled = Item.disabled;
		m_type = 1;
		openned = 0; 
		inserttext = false;
		for (int i = 0; i < Item.items.size(); i++)
		{
			MenuEditorItem* Sub = new MenuEditorItem;		
			Sub->CreateFromMenuResource(Item.items[i], dlg);
			m_items.push_back(Sub);
		}
		MenuEditorItem* Sub = new MenuEditorItem;		
		Sub->init(dlg);
		Sub->m_type = 1;
		m_items.push_back(Sub);

	}
	void SaveToMenuResource(MenuResourceItem& Item)
	{
		// Item info
		Item.text = m_Text;
		Item.checked = m_Checked;
		Item.id = m_ID;
		Item.disabled = m_Disabled;

		for (int i = 0; i < m_items.size(); i++)
		{
			if(m_items[i]->inserttext)
				continue;
			Item.items.push_back(MenuResourceItem());	 
			m_items[i]->SaveToMenuResource(Item.items[i]);
		}
	}

};


class CDragMenuItem : public CObject
{	
	virtual void Serialize( CArchive& ar ) ;


	int m_type;	// 0 = menu   1 = popup    2 = separator
	CRect m_rect;
	BOOL disabled;
	BOOL inserttext;
	vector<CDragMenuItem> m_items;	// subitems

	MenuEditorItem* src; // used for serialize function only

	CString m_Text;


};


class CEditEx : public CEdit
{
public:
	CEditEx()
	{
		bg = 0;

	}
	~CEditEx()
	{
		if(bg)
		delete bg;
	}

protected:

	DECLARE_MESSAGE_MAP()

	HBRUSH CtlColor(CDC* pDC, UINT nCtlColor); 


public:
	CBrush* bg;

};

class CMenuEditor : public CWnd
{
public:
	afx_msg void OnPaint();
	vector< vector<MenuEditorItem*>* > m_items;
	vector< CPoint > m_offsets;

	MenuEditorItem m_item;
	
	int nextoffsetx;
	int nextoffsety;

	void SetOffsetForNextPopup(int x, int y)
	{
		nextoffsetx = x;
		nextoffsety = y;
	}

	MenuEditorItem* selected;

	CEditEx m_edit;

	void OnEnter();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	void LoadFromResource(MenuResource* res);
	void SaveToResource(MenuResource* res);

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()
};

// CMenuEditorDlg dialog
class CMenuEditorDlg : public CDialog
{
// Construction
public:
	CMenuEditorDlg(CWnd* pParent = NULL);	// standard constructor
	MenuResource* m_pMenu;
	HACCEL m_Accelerators;

	CMenuEditor m_Editor;

	CExtCheckBox m_Checked;
	CExtCheckBox m_Separator;
	CExtCheckBox m_Disabled;

	CExtButton m_OK, m_Cancel;

// Dialog Data
	enum { IDD = IDD_MENUEDITOR };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
		afx_msg LRESULT OnGetDefID(WPARAM wp, LPARAM lp);
	afx_msg void OnEnter();
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
