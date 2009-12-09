// HTMLFont.h: interface for the CHTMLFont class.
//
//////////////////////////////////////////////////////////////////////
// (c) Jerome Sopocko 2003
// this code worked last time I saw it
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTMLFONT_H__84F80EAE_21F9_4BD3_853E_7AA130226700__INCLUDED_)
#define AFX_HTMLFONT_H__84F80EAE_21F9_4BD3_853E_7AA130226700__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHTMLFont : public CObject
{
public:
	CHTMLFont();
	CHTMLFont(const CHTMLFont & Police);
	virtual ~CHTMLFont();

	CHTMLFont & operator = (const CHTMLFont & Police);

	CString		m_strName;
	double		m_dSize;
	bool 		m_isBold;
	bool 		m_isItalic;
	bool 		m_isUnderline;
	short		m_nCharSet;
	COLORREF	m_rgbColor;
	int			m_nHTMLSize;
	bool		m_isImage;
	bool		m_isStrike;

	bool		m_param;
	bool		m_paramnum;	
	bool		m_isSpecialImage;	// construct - special image
	bool        m_DLL;
protected:
	void CopyProperties(const CHTMLFont & fnt);

public:
	CFont* GetFont(CDC * pDC) const;
	static double GetHTMLSizeCoeffient(int nHTMLSize);
	static void DeleteArray(CObArray & arrTableau);

	const CString& GetName() const 
	{
		return m_strName;
	}
	void SetName(LPCTSTR strName)
	{
		m_strName = strName;
	}
	//
	double GetSize() const 
	{
		return m_dSize;
	}
	void SetSize(double dSize)
	{
		m_dSize = dSize;
	}
	//
	bool IsBold() const 
	{
		return m_isBold;
	}
	void SetBold(bool isBold)
	{
		m_isBold = isBold;
	}
	void SetStrike(bool isStrike)
	{
		m_isStrike = isStrike;
	}
	//
	bool IsItalic() const 
	{
		return m_isItalic;
	}
	void SetItalic(bool isItalic)
	{
		m_isItalic = isItalic;
	}
	//
	bool IsUnderline() const 
	{
		return m_isUnderline;
	}
	void SetUnderline(bool isUnderline)
	{
		m_isUnderline = isUnderline;
	}
	//
	short GetCharSet() const 
	{
		return m_nCharSet;
	}
	void SetCharSet(short nCharSet)
	{
		m_nCharSet = nCharSet;
	}
	//
	COLORREF GetColor() const 
	{
		return m_rgbColor;
	}
	void SetColor(COLORREF nColor)
	{
		m_rgbColor = nColor;
	}
	//
	int GetHTMLSize() const 
	{
		return m_nHTMLSize;
	}
	void SetHTMLSize(int nHTMLSize)
	{
		m_nHTMLSize = nHTMLSize;
	}
	void SetImage(bool isImage)
	{
		m_isImage = isImage;
	}
	void SetParamNum(bool istrue)
	{
		m_paramnum = istrue;
	}
	void SetParam(bool istrue)
	{
		m_param = istrue;
	}
	bool IsImage() const 
	{
		return m_isImage;
	}
	void SetSpecialImage(bool isSpecialImage)
	{
		m_isSpecialImage = isSpecialImage;
	}
	bool IsSpecialImage() const 
	{
		return m_isSpecialImage;
	}
	bool IsParam() const
	{
		return m_param;
	}
	bool IsParamNumber() const
	{
		return m_paramnum;
	}
	//
	//

};

#endif // !defined(AFX_HTMLFONT_H__84F80EAE_21F9_4BD3_853E_7AA130226700__INCLUDED_)
