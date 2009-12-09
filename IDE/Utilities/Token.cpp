// Token.cpp: implementation of the CToken class.
//
//	MFC Simple CString Tokenizer (Version 1)
//	
//	Written by Richard Case (case@dcs.kcl.ac.uk)
//  Copyright (c) 1999
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// Expect bugs!
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 
//
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Token.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToken::CToken(CString str)
{
	m_strToToken = str;
	m_strLeft = str;
	m_strToken = " ";
	m_bFoundAny = FALSE;
	m_bFinalToken = FALSE;
}

CToken::~CToken()
{

}

BOOL CToken::MoreTokens()
{
	if ((m_strLeft.Find(m_strToken,0) == -1) && (m_bFinalToken))
	{
		return TRUE; // The string that is left over is a token
	}
	else if ((m_strLeft.Find(m_strToken,0) == -1) && (!m_bFinalToken))
	{
		return FALSE; // The string contains no tokens
	}
	else
		return TRUE;
}


CString CToken::GetNextToken()
{
	
	if(m_bFinalToken)
	{
		m_bFinalToken = FALSE;
		return m_strLeft;
	}
	

	int pos, len;
	CString newtok = m_strLeft; // Our own working copy of the string left
	len = newtok.GetLength();
	pos = newtok.Find(m_strToken,0);
	if (pos == -1)
	{
		// This should never happen
		AfxMessageBox("Opps. I did not think this would happen.\nPlease Contact Richard Case: case@dcs.kcl.ac.uk");
	}
	m_bFoundAny = TRUE;
	newtok = newtok.Left(pos);
	pos++;
	m_strLeft = m_strLeft.Right(len - pos);

	//Check to see if there anymore tokens
	if (m_strLeft.Find(m_strToken,0) == -1)
	{
		// No more tokens but the string left is the remaining token
		m_bFinalToken = TRUE;
	}

	return newtok;

}
