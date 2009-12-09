// SelColorDlg.h
//

#pragma once
#include "afxwin.h"
#include "resource.h"


// CSelColorDlg dialog

#define COLORSET_TOTAL			20
#define COLORSET_MAX			COLORSET_TOTAL - 1

class CColorSet
{
public:
	// classes constructor/deconstructor
	CColorSet() {
		// set all colours to white by default
		for(int nPos = 0; nPos < COLORSET_TOTAL; nPos++)
			m_rgbColors[nPos] = RGB(255, 255, 255);
	}

	// data storage/manipulation functions
	char GetCount() { return COLORSET_TOTAL; }
	void SetColor(int nPos, COLORREF rgbNewColor) {
		if(nPos < 0 || nPos > COLORSET_MAX) return;
		m_rgbColors[nPos] = rgbNewColor;
	}
	COLORREF GetColor(int nPos) {
		if(nPos < 0 || nPos > COLORSET_MAX) return NULL;
		return m_rgbColors[nPos];
	}
	void SetPos(int nPos, int nLeft, int nTop) {
		if(nPos < 0 || nPos > COLORSET_MAX) return;
		m_ptPos[nPos].x = nLeft;
		m_ptPos[nPos].y = nTop;
	}
	void SetLeft(int nPos, int nLeft) {
		if(nPos < 0 || nPos > COLORSET_MAX) return;
		m_ptPos[nPos].x = nLeft;
	}
	void SetTop(int nPos, int nTop) {
		if(nPos < 0 || nPos > COLORSET_MAX) return;
		m_ptPos[nPos].y = nTop;
	}
	POINT GetPos(int nPos) {
		return m_ptPos[nPos];
	}
	int GetLeft(int nPos) {
		if(nPos < 0 || nPos > COLORSET_MAX) return NULL;
		return m_ptPos[nPos].x;
	}
	int GetTop(int nPos) {
		if(nPos < 0 || nPos > COLORSET_MAX) return NULL;
		return m_ptPos[nPos].y;
	}

private:
	COLORREF m_rgbColors[COLORSET_TOTAL];		// custom colors
	POINT m_ptPos[COLORSET_TOTAL];				// custom color positions
};

#ifndef POINT_2F

struct POINT_2F {
	float x;
	float y;
};

#endif

class CSelColorDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelColorDlg)

public:
	// classes constructor/deconstructor
	CSelColorDlg(CWnd* pParent = NULL);   // standard constructor
	CSelColorDlg(BYTE nRedVal, BYTE nGreenVal, BYTE nBlueVal,
		CWnd *pParent = NULL);
	virtual ~CSelColorDlg();

	// data storage/manipulation
	void SetHLSValue(BYTE hue, BYTE lum, BYTE sat) {
		m_nHue = hue;
		m_nLum = lum;
		m_nSat = sat;
		Invalidate(FALSE);
	}
	void SetHueValue(BYTE hue) {
		m_nHue = hue;
		Invalidate(FALSE);
	}
	void SetLumValue(BYTE lum) {
		m_nLum = lum;
		Invalidate(FALSE);
	}
	void SetSatValue(BYTE sat)
	{
		m_nSat = sat;
		Invalidate(FALSE);
	}
	void SetColor(COLORREF color) {
		SelectColor(GetRValue(color), GetGValue(color), GetBValue(color));
	}

	BYTE GetHueValue() const { return m_nHue; }
	BYTE GetSatValue() const { return m_nSat; }
	BYTE GetLumValue() const { return m_nLum; }
	COLORREF GetColor() const {
		return RGB(m_nNewRed, m_nNewGreen, m_nNewBlue);
	}
	CString GetRefCode() const { return m_strRef; }

	// RGB -> HLS and HLS -> RGB conversion functions
	void RGBtoHLS(COLORREF lRGBColor, BYTE* H, BYTE* L, BYTE* S);
	WORD HueToRGB(WORD n1, WORD n2, WORD hue);
	DWORD HLStoRGB(WORD hue, WORD lum, WORD sat);

	// custom color manipulation
	COLORREF GetCustomColor(char nPos);
	void SetCustomColor(char nPos, COLORREF rgbColor);

// Dialog Data
	enum { IDD = IDD_COLSEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEnChangeHue();
	afx_msg void OnEnChangeSat();
	afx_msg void OnEnChangeLum();
	afx_msg void OnEnChangeColor();
	afx_msg void OnEnChangeRef();

	void DDX_TextEx(CDataExchange* pDX, UINT nID, BYTE nMin, BYTE nMax, BYTE &nValue);
	//void DDX_ValidateText(CDataExchange* pDX, UINT nID, BYTE nMax, BYTE &nValue);

	void InitCustomColors(bool bMonochrome = false);
	void InitColorSpectrum();
	void GenColorBar();

	void DrawGradBar(CDC* pDC);
	void DrawFrameBorder(CDC* pDC, LPRECT rect);
	void DrawColorSpectrum(CDC* pDC);
	void DrawColorBar(CDC* pDC);
	void DrawCurColorBox(CDC* pDC);
	void DrawNewColorBox(CDC* pDC);
	void DrawCustomColors(CDC* pDC);
	void DrawCross(CDC* pDC, LPPOINT ptPos);
	void DrawBarArrow(CDC* pDC, int nLum);

	void SelectColor(BYTE nRed, BYTE nGreen, BYTE nBlue);
	char GetCustomBox(LPPOINT ptPos);

	CBitmap m_bmpSpectrum;
	CBitmap m_bmpBar;
	CPen m_tempPen;
	CBrush m_tempBrush;
	bool m_bMouseMove;
	char nDragMode, nDragSrc;
	BYTE m_nDragRed, m_nDragGreen, m_nDragBlue;
	POINT m_ptPos;

	CString m_strRef;
	BYTE m_nHue, m_nSat, m_nLum;
	BYTE m_nRed, m_nGreen, m_nBlue;
	BYTE m_nNewRed, m_nNewGreen, m_nNewBlue;

	char m_nInsertPos;

public:
	afx_msg void OnBnClickedAddcolour();
};
