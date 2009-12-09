// CColourBox.cpp : Defines the class behaviors for the application.
//

#ifndef CCOLOURBOX_H
#define CCOLOURBOX_H

class CColourPopup : public CWnd
{
public:
	CColourPopup()
	{
		bla = 1;
	}

	BOOL Create(DWORD dwStyle, CWnd* pParentWnd, UINT nID)
	{
		return CWnd::CreateEx(0,AfxRegisterWndClass(CS_CLASSDC), NULL, dwStyle, CRect(0,0,100,20), pParentWnd, nID);
	}

	void Show(CPoint point);

private:
	int  bla;
	HWND parentWnd;

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

class CColourBox : public CWnd
{
public:
	// Constructor
	CColourBox();

	void            Initialise();
	void            AutoUpdate(bool doAutoUpdate) { autoUpdate = doAutoUpdate; }
	bool            IsAutoUpdateEnabled() { return autoUpdate; }

	void            SetEditFont(CString typeFace, int fontSize)
	{
		// Create the font
		CFont theFont;

		theFont.CreateFont(fontSize, 0, 0, 0, FW_NORMAL,
			               FALSE, FALSE, FALSE, 0, ANSI_CHARSET,
						   CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
						   DEFAULT_PITCH | FF_SWISS, typeFace);

		// Set it
		baseEdit.SetFont(&theFont, true);
	}

	void            SetCurrentValue(int newVal) { curValue = newVal; }
	int             GetCurrentValue() { return curValue; }

	void            SetProgressColour(int newR, int newG, int newB)
	{
		progR = newR;
		progG = newG;
		progB = newB;
	}

	void            SetBackgroundColour(int newR, int newG, int newB)
	{
		backR = newR;
		backG = newG;
		backB = newB;
	}

private:
	int             curValue;
	bool            autoUpdate;

	float           progressPosition;
	
	// Progress colour
	int             progR;
	int             progG;
	int             progB;

	// Background colour
	int             backR;
	int             backG;
	int             backB;

	// Is over?
	bool            isOver;

	CEdit           baseEdit;
	CSpinButtonCtrl spinControl;
	CButton         pushButton;
	CComboBox       comboControl;

	int             controlWidth;
	int             controlHeight;

	CColourPopup    thePopup;

	//{{AFX_MSG(CColourBox)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEditChange();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif
