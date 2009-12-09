// HTMLAtom.cpp: implementation of the CHTMLAtom class.
//
//////////////////////////////////////////////////////////////////////
// (c) Jerome Sopocko 2003
// this code worked last time I saw it
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HTMLAtom.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHTMLAtom::CHTMLAtom()
{
	m_nAscent = 0;
	m_nAlignment = 0;
	m_isHighlighted = false;

}

CHTMLAtom::CHTMLAtom(const CHTMLAtom & Atom)
{
	m_strText 		= Atom.m_strText;
	m_Font 			= Atom.m_Font;
	m_rctPosition 	= Atom.m_rctPosition;
	m_nAscent 		= Atom.m_nAscent;
	m_nAlignment 	= Atom.m_nAlignment;
	m_nLine 		= Atom.m_nLine;
	m_strURL 		= Atom.m_strURL;
	m_isHighlighted = Atom.m_isHighlighted;
}

CHTMLAtom::~CHTMLAtom()
{

}

void CHTMLAtom::DeleteArray(CObArray & arrArray)
{
	for ( int nObj=0; nObj < arrArray.GetSize(); nObj++)
		delete (CHTMLAtom *) arrArray[nObj];
	arrArray.RemoveAll();
}

