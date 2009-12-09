// HTMLFont.cpp: implementation of the CHTMLFont class.
//
//////////////////////////////////////////////////////////////////////
// (c) Jerome Sopocko 2003
// this code worked last time I saw it
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HTMLFont.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Additional: The font manager allows us to cache the fonts we use so we arn't creating new fonts all the time
extern CFontMgr gMyFontManager;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHTMLFont::CHTMLFont()
{
	m_rgbColor	= 0;
	m_nHTMLSize = 3;
	m_strName = _T("Times New Roman");
	m_dSize = 10;
	m_isBold = false;
	m_isItalic = false;
	m_isImage = false;
	m_isSpecialImage = false;
	m_DLL = false;
	m_isUnderline = false;
	m_nCharSet = 0;
	m_param = false;
	m_paramnum = false;
	m_isStrike = false;
}

CHTMLFont::~CHTMLFont()
{
	

}

CHTMLFont::CHTMLFont(const CHTMLFont & fnt)
{
	CopyProperties(fnt);
}

CHTMLFont & CHTMLFont::operator =(const CHTMLFont & fnt)
{
	if ( this != &fnt )
		CopyProperties(fnt);
	return *this;
}

void CHTMLFont::CopyProperties(const CHTMLFont & fnt)
{
	m_strName 		= fnt.m_strName;
	m_dSize 		= fnt.m_dSize;
	m_isBold 		= fnt.m_isBold;
	m_isItalic 		= fnt.m_isItalic;
	m_isUnderline 	= fnt.m_isUnderline;
	m_nCharSet 		= fnt.m_nCharSet;
	m_rgbColor 		= fnt.m_rgbColor;
	m_nHTMLSize 	= fnt.m_nHTMLSize;
	m_isImage		= fnt.m_isImage;
	m_isStrike		= fnt.m_isStrike;
	m_isSpecialImage = fnt.m_isSpecialImage;
	m_param = fnt.m_param;
	m_paramnum = fnt.m_paramnum;
}


void CHTMLFont::DeleteArray(CObArray & arrArray)
{
	for ( int nObj=0; nObj < arrArray.GetSize(); nObj++)
		delete (CHTMLFont *) arrArray[nObj];
	arrArray.RemoveAll();
}


double CHTMLFont::GetHTMLSizeCoeffient(int nHTMLSize)
{
	switch ( nHTMLSize )
	{
	case 1:
		return 0.625;
	case 2:
		return 0.8;
	case 3:
		return 1;
	case 4:
		return 1.125;
	case 5:
		return 1.5;
	case 6:
		return 2;
	case 7:
		return 3;
	default:
		if ( nHTMLSize > 7 )
			return nHTMLSize/7.*3.;
		// If you do not want the size to be limited, replace the 2 previous lines by this
		//if ( nHTMLSize > 7 )
		//	return 3;

	}
	return 1;
}

CFont* CHTMLFont::GetFont(CDC * pDC) const
{

	int lfHeight;
	if (pDC->m_hAttribDC == NULL )
		lfHeight =(int) -(m_dSize* 90 / 65.);
	else
		lfHeight =(int) -(m_dSize* pDC->GetDeviceCaps(LOGPIXELSY) / 65.);

	CFont * pFont =
	gMyFontManager.RequestFont(lfHeight , 0, 0, 0,m_isBold?FW_BOLD:FW_NORMAL, m_isItalic, m_isUnderline, m_isStrike, (BYTE) m_nCharSet,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH, m_strName);

	return pFont;
}
