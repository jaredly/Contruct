// HTMLAtom.h: interface for the CHTMLAtom class.
//
//////////////////////////////////////////////////////////////////////
// (c) Jerome Sopocko 2003
// this code worked last time I saw it
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTMLATOM_H__6114FDBF_AD37_4EAA_800B_6175F8F0DAA9__INCLUDED_)
#define AFX_HTMLATOM_H__6114FDBF_AD37_4EAA_800B_6175F8F0DAA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HTMLFont.h"

class CHTMLAtom : public CObject  
{
public:
	CHTMLAtom();
	CHTMLAtom(const CHTMLAtom & Atom);
	virtual ~CHTMLAtom();
protected:
	CString	m_strText;
	CHTMLFont m_Font;
	CRect	m_rctPosition;
	int		m_nAscent;
	short	m_nAlignment;
	int		m_nLine;
	CString m_strURL;
	bool	m_isHighlighted;
	// For images, we simply use the text to get the number,
	// and the font contains an m_IsImage member to check it :)

public:
	static void DeleteArray(CObArray & arrArray);

	const CString& GetText() const 
	{
		return m_strText;
	}
	void SetText(LPCTSTR strText)
	{
		m_strText = strText;
	}
	//
	const CHTMLFont& GetHTMLFont() const 
	{
		return m_Font;
	}
	void SetHTMLFont(const CHTMLFont& Font)
	{
		m_Font = Font;
	}
	//
	const CRect& GetPosition() const 
	{
		return m_rctPosition;
	}
	CRect * GetAdrPosition()
	{
		return &m_rctPosition;
	}
	void SetPosition(const CRect& rctPosition)
	{
		m_rctPosition = rctPosition;
	}
	//
	int GetAscent() const 
	{
		return m_nAscent;
	}
	void SetAscent(int nAscent)
	{
		m_nAscent = nAscent;
	}
	//
	short GetAlignment() const 
	{
		return m_nAlignment;
	}
	void SetAlignment(short nAlignment)
	{
		m_nAlignment = nAlignment;
	}
	//
	int GetLine() const 
	{
		return m_nLine;
	}
	void SetLine(int nLine)
	{
		m_nLine = nLine;
	}
	//
	const CString& GetURL() const 
	{
		return m_strURL;
	}
	void SetURL(LPCTSTR strURL)
	{
		m_strURL = strURL;
	}
	//
	bool IsHighlighted() const 
	{
		return m_isHighlighted;
	}
	void SetHighlighted(bool isHighlighted)
	{
		m_isHighlighted = isHighlighted;
	}
	//
};

#endif // !defined(AFX_HTMLATOM_H__6114FDBF_AD37_4EAA_800B_6175F8F0DAA9__INCLUDED_)
