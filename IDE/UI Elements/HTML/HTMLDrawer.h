// HTMLDrawer.h: interface for the CHTMLDrawer class.
//
//////////////////////////////////////////////////////////////////////
// (c) Jerome Sopocko 2003
// this code worked last time I saw it
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTMLDRAWER_H__DCF0B41F_7E7D_472A_BDDB_E8902BE074D6__INCLUDED_)
#define AFX_HTMLDRAWER_H__DCF0B41F_7E7D_472A_BDDB_E8902BE074D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "..\..\Editors\ChronoEventEditor\ChronoEventEditor.h"

class CHTMLFont;
class CHTMLAtom;

class CHTMLDrawer  
{
public:
	CHTMLDrawer();
	virtual ~CHTMLDrawer();
protected:
	CObArray	m_arrDisplayAtoms;
	CObArray	m_arrAnchors;


public:

	static void DrawText(CDC * pDC, CString & strText,const CHTMLFont & fntDefault,CRect & rctPosition,short nAlignment, class CChronoEventEditor* ed, vector<class CEditorImage>* pParameters,  vector<class CEditorParam*>* param);
	
	int PrepareDraw(CDC * pDC, const CString & strText,const CHTMLFont & fntDefault,CRect & rctPosition,short nAlignment);
	void Draw(CDC * pDC,COLORREF rgbHighlight,bool isUnderlineWhenHighlight, class CChronoEventEditor* ed, vector<class CEditorImage>* pParameters, vector<class CEditorParam*>* param);
	bool IsPointInAnchor(CPoint & pntMouse,CHTMLAtom *& pAnchor);
	bool HighlightAnchor(CHTMLAtom * pHighlight,CRect & rctToInvalidate);

	int GetAnchorCount() const
	{
		return m_arrAnchors.GetSize();
	}

protected:
	static int FindDisplayAtoms(CDC * pDC, const CString & strText,const CHTMLFont & fntDefault,CRect & rctPosition,short nAlignment,CObArray & arrDisplayAtoms,CObArray * parrAnchors=NULL);
	static void ParseHTMLText(const CString & strSource,const CHTMLFont & Police,CObArray & arrAtoms,short nDefaultAlignment);
	static void FindTagAttributes(const CString & strAttributes,CStringArray & arrNames,CStringArray & arrValues);
	static bool AddText(CObArray & arrAtoms,const CString & strText,CHTMLFont & polCurrent,short nAlignment,const CString & strURL);
	static void UpdatePoliceAccordingToTag(CString & strTag,CHTMLFont & polCurrent,CObArray & arrFontStack,bool & isNewLine,short & nAlignment,short nDefaultAlignment,bool isLastAtomNewLine,CString & strURL);
	static void BreakIntoLines(CDC * pDC,CObArray & arrAtoms,CObArray & arrDisplayAtoms,CRect & rctPosition,CObArray * parrAnchors);
	static void CreateNewLine(CDC * pDC,CObArray & arrDisplayAtoms,int & nStartLineAtom,int & nY);
	static void RemoveLastWord(CString & strText);
	static void ApplyAlignments(CObArray & arrDisplayAtoms,int nShiftVertically,int nRight);
	static void DrawAtoms(CDC * pDC,CObArray & arrDisplayAtoms,COLORREF rgbHighlight,bool isUnderlineWhenHighlight, CChronoEventEditor* ed, vector<class CEditorImage>* pParameters, vector<class CEditorParam*>* param, int& param_number);
	static long FromHex(const CString & strHex);
	static void AlignLineAccordingToAscent(CObArray & arrDisplayAtoms,int nLowestAscent,int nStartLineAtom);
	static long ToLong(LPCTSTR lpszText);

};

#endif // !defined(AFX_HTMLDRAWER_H__DCF0B41F_7E7D_472A_BDDB_E8902BE074D6__INCLUDED_)
