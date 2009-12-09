#if !defined(AFX_ICONEDIT_H__AEA6DB87_1AB8_4BC0_8572_1A0ECB665F4A__INCLUDED_)
#define AFX_ICONEDIT_H__AEA6DB87_1AB8_4BC0_8572_1A0ECB665F4A__INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// CIconWnd window

class CIconWnd : public CStatic
{
// Construction/destruction
public:
	CIconWnd();
	virtual ~CIconWnd();

// Operations
public:
	void SetIcon( HICON icon );

protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

private:
// Attributes
	HICON	m_icon;
};

/////////////////////////////////////////////////////////////////////////////
// CIconEdit window

class CIconEdit : public CEdit
{
// Construction/destruction
public:
	CIconEdit();
	virtual ~CIconEdit();

// Operations
public:
	void SetIcon(HICON icon);
	void SetIcon(UINT iconres);

protected:
	virtual void PreSubclassWindow();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM lParam); // Maps to WM_SETFONT
	DECLARE_MESSAGE_MAP()

private:
	void		Prepare();
	void		CreateIcon();

// Attributes
	CIconWnd	m_icon;
	HICON		m_internalIcon;
};

#endif // !defined(AFX_ICONEDIT_H__AEA6DB87_1AB8_4BC0_8572_1A0ECB665F4A__INCLUDED_)
