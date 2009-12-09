// HTMLDrawer.cpp: implementation of the CHTMLDrawer class.
//
//////////////////////////////////////////////////////////////////////
// (c) Jerome Sopocko 2003
// this code worked last time I saw it
//////////////////////////////////////////////////////////////////////
// v1.0 : 25/3/2003 
//        First version
// v1.1 : 11/4/2003
//        Added support for <P> <A HREF> and <CENTER>
//        Corrected alignment of fonts of different sizes
////////////////////f<//////////////////////////////////////////////////
#include "stdafx.h"
#include "HTMLDrawer.h"

#include "HTMLAtom.h"
#include "..\..\Editors\Event Sheet Editor\Event Sheet Editor.h"
#include "..\..\Editors\Expressions\CExpression.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHTMLDrawer::CHTMLDrawer()
{

}

CHTMLDrawer::~CHTMLDrawer()
{
	CHTMLAtom::DeleteArray(m_arrDisplayAtoms);
}

CString Readable;

/////////////////////////////////////////////////////////////
// DrawText : static implementation
/////////////////////////////////////////////////////////////
// Parameters:
// pDC : Device context
// strText: Text to be drawn
// fntDefault : Default font to be used
// rctPosition : Rectangle in which the text is to be drawn
// nAlignment: 0 (left) or 1 (centered) or 2 (right)
/////////////////////////////////////////////////////////////
void CHTMLDrawer::DrawText(CDC * pDC, CString& strText,const CHTMLFont & fntDefault,CRect & rctPosition,short nAlignment, CChronoEventEditor* ed, ImageVector* special_images,  ParamVector* param)
{
	Readable = "";

	int param_number = 0;
	// Find where each item is to be drawn
	CObArray arrDisplayAtoms;
	FindDisplayAtoms(pDC, strText,fntDefault,rctPosition,nAlignment,arrDisplayAtoms);

	// Draw them (the highlight settings will not be used)
	DrawAtoms(pDC,arrDisplayAtoms,RGB(0,0,0),false, ed, special_images, param, param_number);

	strText.Format("%s", Readable);

	// Free memory
	CHTMLAtom::DeleteArray(arrDisplayAtoms);
}

/////////////////////////////////////////////////////////////
// Prepare Draw : caches the atoms so the repaint would be faster
/////////////////////////////////////////////////////////////
// Parameters:
// pDC : Device context
// strText: Text to be drawn
// fntDefault : Default font to be used
// rctPosition : Rectangle in which the text is to be drawn
// nAlignment: 0 (left) or 1 (centered) or 2 (right)
/////////////////////////////////////////////////////////////
int CHTMLDrawer::PrepareDraw(CDC * pDC, const CString & strText,const CHTMLFont & fntDefault,CRect & rctPosition,short nAlignment)
{
	// Free memory
	CHTMLAtom::DeleteArray(m_arrDisplayAtoms);
	m_arrAnchors.RemoveAll();

	// Find where each item is to be drawn
	return FindDisplayAtoms(pDC, strText,fntDefault,rctPosition,nAlignment,m_arrDisplayAtoms,&m_arrAnchors);
}

/////////////////////////////////////////////////////////////
// Draw : draws the cached atoms (call PrepareDraw before)
/////////////////////////////////////////////////////////////
// Parameters:
// pDC : Device context
/////////////////////////////////////////////////////////////
void CHTMLDrawer::Draw(CDC * pDC,COLORREF rgbHighlight,bool isUnderlineWhenHighlight, CChronoEventEditor* Ed, ImageVector* special_images,  ParamVector* param)
{
	// Draw them
	int params;
	DrawAtoms(pDC,m_arrDisplayAtoms,rgbHighlight,isUnderlineWhenHighlight, Ed, special_images,param, params);
}


int CHTMLDrawer::FindDisplayAtoms(CDC * pDC, const CString & strText,const CHTMLFont & fntDefault,CRect & rctPosition,short nAlignment,CObArray & arrDisplayAtoms,CObArray * parrAnchors/*=NULL*/)
{
	// Parse the text to break into atoms having the same settings
	CObArray arrAtoms;
	ParseHTMLText(strText,fntDefault,arrAtoms,nAlignment);

	// If no text return
	if ( arrAtoms.GetSize() == 0 )
		return 0;

	// Break into smaller atoms according to end of line
	BreakIntoLines(pDC,arrAtoms,arrDisplayAtoms,rctPosition,parrAnchors);

	CHTMLAtom::DeleteArray(arrAtoms);

	// Find the lowest position to center vertically
	int nLowest = ((CHTMLAtom *) arrDisplayAtoms.GetAt(0))->GetPosition().bottom;
	int nRightest = ((CHTMLAtom *) arrDisplayAtoms.GetAt(0))->GetPosition().right;
	for ( int nAtom = 0; nAtom < arrDisplayAtoms.GetSize(); nAtom++)
	{
		CHTMLAtom * pAtom = (CHTMLAtom *) arrDisplayAtoms.GetAt(nAtom);
		if ( pAtom->GetPosition().bottom > nLowest )
			nLowest = pAtom->GetPosition().bottom;
		if ( pAtom->GetPosition().bottom > nRightest )
			nRightest = pAtom->GetPosition().right;
	}


	int nShiftVertically = 0;
	//if ( nLowest < rctPosition.Height() )
	//	nShiftVertically = (rctPosition.Height()-nLowest) / 2;
	//else
		rctPosition.bottom = nLowest;
		//rctPosition.right = nRightest;
	ApplyAlignments(arrDisplayAtoms,nShiftVertically,rctPosition.right);

	return nLowest+nShiftVertically;

}


void CHTMLDrawer::ParseHTMLText(const CString & strSource,const CHTMLFont & fntDefault,CObArray & arrAtoms,short nDefaultAlignment)
{
	CString strText(strSource);
	CHTMLFont fntCurrent(fntDefault);

	CObArray arrFontStack;
	int nTagStart;
	short nAlignment=nDefaultAlignment;
	bool isLastAtomNewLine=true;
	CString strURL;
	while ( (nTagStart = strText.Find('<')) != -1 )
	{
		int nTagEnd = strText.Find('>',nTagStart);
		if ( nTagEnd == -1 )
		{
			break;
		}
		if ( AddText(arrAtoms,strText.Left(nTagStart),fntCurrent,nAlignment,strURL) )
			isLastAtomNewLine = false;

		CString strTag=strText.Mid(nTagStart+1,nTagEnd-nTagStart-1);

		bool isNewLine=false;
		UpdatePoliceAccordingToTag(strTag,fntCurrent,arrFontStack,isNewLine,nAlignment,nDefaultAlignment,isLastAtomNewLine,strURL);
		strText = strText.Mid(nTagEnd+1);
		if ( isNewLine )
		{
			AddText(arrAtoms,_T("\r\n"),fntCurrent,nAlignment,strURL);
			isLastAtomNewLine = true;
		}
	}
	if ( !strText.IsEmpty() )
	{
		AddText(arrAtoms,strText,fntCurrent,nAlignment,strURL);
	}
	CHTMLFont::DeleteArray(arrFontStack);
}

bool CHTMLDrawer::AddText(CObArray & arrAtoms,const CString & strCodedText,CHTMLFont & polCurrent,short nAlignment,const CString & strURL)
{
	if ( strCodedText.IsEmpty() )
		return false;

	// Translate &quot; &lt; &gt; and &amp;
	CString strText(strCodedText);

	strText.Replace("amp;", "");
	strText.Replace("&lt;", "<");
	strText.Replace("&gt;", ">");

	CHTMLAtom * pAtom=new CHTMLAtom;
	pAtom->SetText(strText);
	pAtom->SetHTMLFont(polCurrent);
	pAtom->SetAlignment(nAlignment);
	pAtom->SetURL(strURL);
	arrAtoms.Add(pAtom);

	return true;
}

void CHTMLDrawer::UpdatePoliceAccordingToTag(CString & strTag,CHTMLFont & polCurrent,CObArray & arrFontStack,bool & isNewLine,short & nAlignment,short nDefaultAlignment,bool isLastAtomNewLine,CString & strURL)
{
	strTag.TrimLeft();
	strTag.TrimRight();
	if( strTag.CompareNoCase(_T("numparam")) == 0)
	{
		polCurrent.SetParamNum(true);
	}
	if( strTag.CompareNoCase(_T("/numparam")) == 0)
	{
		polCurrent.SetParamNum(false);
	}
	if( strTag.CompareNoCase(_T("tagparam")) == 0)
	{
		polCurrent.SetParam(true);
	}
	if( strTag.CompareNoCase(_T("/tagparam")) == 0)
	{
		polCurrent.SetParam(false);
	}
	if (strTag.CompareNoCase(_T("img")) == 0)
	{
		polCurrent.SetImage(true);
		polCurrent.m_DLL = false;
	}
	else if ( strTag.CompareNoCase(_T("/img")) == 0)
	{
		polCurrent.SetImage(false);
	}
	if (strTag.CompareNoCase(_T("special_image")) == 0)
	{
		polCurrent.SetSpecialImage(true);
		polCurrent.m_DLL = false;
	}
	else if ( strTag.CompareNoCase(_T("/special_image")) == 0)
	{
		polCurrent.SetSpecialImage(false);
	}


	if ( strTag.CompareNoCase(_T("imgd")) == 0)
	{
		polCurrent.SetImage(true);
		polCurrent.m_DLL = true;
	}
	else if ( strTag.CompareNoCase(_T("/imgd")) == 0)
	{
		polCurrent.SetImage(false);
		polCurrent.m_DLL = false;
	}
	else if ( strTag.CompareNoCase(_T("b")) == 0 )
	{
		polCurrent.SetBold(true);
	}
	else if ( strTag.CompareNoCase(_T("strong")) == 0 )
	{
		polCurrent.SetBold(true);
	}
	else if ( strTag.CompareNoCase(_T("/b")) == 0 )
	{
		polCurrent.SetBold(false);
	}
	else if ( strTag.CompareNoCase(_T("/strong")) == 0 )
	{
		polCurrent.SetBold(false);
	}
	else if ( strTag.CompareNoCase(_T("i")) == 0 )
	{
		polCurrent.SetItalic(true);
	}
	else if ( strTag.CompareNoCase(_T("/i")) == 0 )
	{
		polCurrent.SetItalic(false);
	}
	else if ( strTag.CompareNoCase(_T("em")) == 0 )
	{
		polCurrent.SetItalic(true);
	}
	else if ( strTag.CompareNoCase(_T("/em")) == 0 )
	{
		polCurrent.SetItalic(false);
	}
	else if ( strTag.CompareNoCase(_T("u")) == 0 )
	{
		polCurrent.SetUnderline(true);
	}
	else if ( strTag.CompareNoCase(_T("/u")) == 0 )
	{
		polCurrent.SetUnderline(false);
	}
	else if ( strTag.CompareNoCase(_T("br")) == 0 )
	{
		isNewLine = true;
	}
	else if ( strTag.Left(1).CompareNoCase(_T("p")) == 0 )
	{
		if ( !isLastAtomNewLine)
			isNewLine = true;

		if ( strTag.GetLength() == 1 )
			return;
		CStringArray arrNames,arrValues;
		FindTagAttributes(strTag.Mid(2),arrNames,arrValues);

		for ( int nAttribute = 0; nAttribute < arrNames.GetSize(); nAttribute++)
		{
			if ( arrNames.GetAt(nAttribute).CompareNoCase(_T("align")) == 0 )
			{
				CString strValue(arrValues.GetAt(nAttribute));
				if ( strValue.CompareNoCase(_T("left")) == 0 )
				{
					nAlignment=0;
				}
				else if ( strValue.CompareNoCase(_T("center")) == 0 )
				{
					nAlignment=1;
				}
				else if ( strValue.CompareNoCase(_T("right")) == 0 )
				{
					nAlignment=2;
				}
			}
		}
	}
	else if ( strTag.CompareNoCase(_T("/p")) == 0 )
	{
		if ( !isLastAtomNewLine)
			isNewLine = true;
		nAlignment=nDefaultAlignment;
	}
	else if ( strTag.CompareNoCase(_T("center")) == 0 )
	{
		if ( !isLastAtomNewLine)
			isNewLine = true;
		nAlignment=1;
	}
	else if ( strTag.CompareNoCase(_T("/center")) == 0 )
	{
		if ( !isLastAtomNewLine)
			isNewLine = true;
		nAlignment=nDefaultAlignment;
	}
	else if ( strTag.Left(1).CompareNoCase(_T("a")) == 0 )
	{
		if ( strTag.GetLength() == 1 )
			return;
		CStringArray arrNames,arrValues;
		FindTagAttributes(strTag.Mid(2),arrNames,arrValues);

		for ( int nAttribute = 0; nAttribute < arrNames.GetSize(); nAttribute++)
		{
			if ( arrNames.GetAt(nAttribute).CompareNoCase(_T("href")) == 0 )
			{
				strURL = arrValues.GetAt(nAttribute);
			}
		}
	}
	else if ( strTag.Left(2).CompareNoCase(_T("/a")) == 0 )
	{
		strURL.Empty();
	}
	else if ( strTag.Left(4).CompareNoCase(_T("font")) == 0 )
	{
		// Store previous police and color in stack
		arrFontStack.Add((CObject *) new CHTMLFont(polCurrent));

		CStringArray arrNames,arrValues;
		FindTagAttributes(strTag.Mid(4),arrNames,arrValues);

		for ( int nAttribute = 0; nAttribute < arrNames.GetSize(); nAttribute++)
		{
			CString strValue(arrValues.GetAt(nAttribute));
			if ( !strValue.IsEmpty() )
			{
				CString strName(arrNames.GetAt(nAttribute));

				// Update current police
				if ( strName.CompareNoCase(_T("face")) == 0 )
				{
					polCurrent.SetName(strValue);
				}
				else if ( strName.CompareNoCase(_T("size")) == 0 )
				{
					int nSizeBefore = polCurrent.GetHTMLSize();
					int nSize;
					if ( strValue[0] == '+' )
					{
						nSize= nSizeBefore + ToLong(strValue.Mid(1));
					}
					else if ( strValue[0] == '-' )
					{
						nSize= nSizeBefore - ToLong(strValue.Mid(1));
					}
					else
					{
						nSize= ToLong(strValue);
					}
					polCurrent.SetHTMLSize(nSize);
					polCurrent.SetSize(polCurrent.GetSize()/CHTMLFont::GetHTMLSizeCoeffient(nSizeBefore) * CHTMLFont::GetHTMLSizeCoeffient(nSize));
				}
				else if ( strName.CompareNoCase(_T("color")) == 0 )
				{
					// Try to find the color
					if ( strValue.CompareNoCase(_T("Light")) < 0 )
					{
						if ( strValue.CompareNoCase(_T("Deep")) < 0 )
						{
							if ( strValue.CompareNoCase(_T("Dark")) < 0 )
							{
								if ( strValue.CompareNoCase(_T("AliceBlue")) == 0 )
									strValue = _T("#F0F8FF");
								else if ( strValue.CompareNoCase(_T("AntiqueWhite")) == 0 )
									strValue = _T("#FAEBD7");
								else if ( strValue.CompareNoCase(_T("Aqua")) == 0 )
									strValue = _T("#00FFFF");
								else if ( strValue.CompareNoCase(_T("Aquamarine")) == 0 )
									strValue = _T("#7FFFD4");
								else if ( strValue.CompareNoCase(_T("Azure")) == 0 )
									strValue = _T("#F0FFFF");
								else if ( strValue.CompareNoCase(_T("Beige")) == 0 )
									strValue = _T("#F5F5DC");
								else if ( strValue.CompareNoCase(_T("Black")) == 0 )
									strValue = _T("#000000");
								else if ( strValue.CompareNoCase(_T("BlanchedAlmond")) == 0 )
									strValue = _T("#FFEBCD");
								else if ( strValue.CompareNoCase(_T("Blue")) == 0 )
									strValue = _T("#0000FF");
								else if ( strValue.CompareNoCase(_T("BlueViolet")) == 0 )
									strValue = _T("#8A2BE2");
								else if ( strValue.CompareNoCase(_T("Brown")) == 0 )
									strValue = _T("#A52A2A");
								else if ( strValue.CompareNoCase(_T("Bisque")) == 0 )
									strValue = _T("#FFE4C4");
								else if ( strValue.CompareNoCase(_T("BurlyWood")) == 0 )
									strValue = _T("#DEB887");
								else if ( strValue.CompareNoCase(_T("CadetBlue")) == 0 )
									strValue = _T("#5F9EA0");
								else if ( strValue.CompareNoCase(_T("Chartreuse")) == 0 )
									strValue = _T("#7FFF00");
								else if ( strValue.CompareNoCase(_T("Chocolate")) == 0 )
									strValue = _T("#D2691E");
								else if ( strValue.CompareNoCase(_T("Coral")) == 0 )
									strValue = _T("#FF7F50");
								else if ( strValue.CompareNoCase(_T("CornflowerBlue")) == 0 )
									strValue = _T("#6495ED");
								else if ( strValue.CompareNoCase(_T("CornSilk")) == 0 )
									strValue = _T("#FFF8DC");
								else if ( strValue.CompareNoCase(_T("Crimson")) == 0 )
									strValue = _T("#DC143C");
								else if ( strValue.CompareNoCase(_T("Cyan")) == 0 )
									strValue = _T("#00FFFF");
							}
							else
							{
								if ( strValue.CompareNoCase(_T("DarkBlue")) == 0 )
									strValue = _T("#00008B");
								else if ( strValue.CompareNoCase(_T("DarkCyan")) == 0 )
									strValue = _T("#008B8B");
								else if ( strValue.CompareNoCase(_T("DarkGoldenrod")) == 0 )
									strValue = _T("#B8860B");
								else if ( strValue.CompareNoCase(_T("DarkGray")) == 0 )
									strValue = _T("#A9A9A9");
								else if ( strValue.CompareNoCase(_T("DarkGreen")) == 0 )
									strValue = _T("#006400");
								else if ( strValue.CompareNoCase(_T("DarkKhaki")) == 0 )
									strValue = _T("#BDB76B");
								else if ( strValue.CompareNoCase(_T("DarkMagenta")) == 0 )
									strValue = _T("#8B008B");
								else if ( strValue.CompareNoCase(_T("DarkOliveGreen")) == 0 )
									strValue = _T("#556B2F");
								else if ( strValue.CompareNoCase(_T("DarkOrange")) == 0 )
									strValue = _T("#FF8C00");
								else if ( strValue.CompareNoCase(_T("DarkOrchid")) == 0 )
									strValue = _T("#9932CC");
								else if ( strValue.CompareNoCase(_T("DarkRed")) == 0 )
									strValue = _T("#8B0000");
								else if ( strValue.CompareNoCase(_T("DarkSalmon")) == 0 )
									strValue = _T("#E9967A");
								else if ( strValue.CompareNoCase(_T("DarkSeaGreen")) == 0 )
									strValue = _T("#8FBC8F");
								else if ( strValue.CompareNoCase(_T("DarkSlateBlue")) == 0 )
									strValue = _T("#483D8B");
								else if ( strValue.CompareNoCase(_T("DarkSlateGray")) == 0 )
									strValue = _T("#2F4F4F");
								else if ( strValue.CompareNoCase(_T("DarkTurquoise")) == 0 )
									strValue = _T("#00CED1");
								else if ( strValue.CompareNoCase(_T("DarkViolet")) == 0 )
									strValue = _T("#9400D3");
							}
						}
						else
						{
							if ( strValue.CompareNoCase(_T("DeepPink")) == 0 )
								strValue = _T("#FF1493");
							else if ( strValue.CompareNoCase(_T("DeepSkyBlue")) == 0 )
								strValue = _T("#00BFFF");
							else if ( strValue.CompareNoCase(_T("DimGray")) == 0 )
								strValue = _T("#696969");
							else if ( strValue.CompareNoCase(_T("DodgerBlue")) == 0 )
								strValue = _T("#1E90FF");
							else if ( strValue.CompareNoCase(_T("FireBrick")) == 0 )
								strValue = _T("#B22222");
							else if ( strValue.CompareNoCase(_T("FloralWhite")) == 0 )
								strValue = _T("#FFFAF0");
							else if ( strValue.CompareNoCase(_T("ForestGreen")) == 0 )
								strValue = _T("#228B22");
							else if ( strValue.CompareNoCase(_T("Fuchsia")) == 0 )
								strValue = _T("#FF00FF");
							else if ( strValue.CompareNoCase(_T("Gainsboro")) == 0 )
								strValue = _T("#DCDCDC");
							else if ( strValue.CompareNoCase(_T("GhostWhite")) == 0 )
								strValue = _T("#F8F8FF");
							else if ( strValue.CompareNoCase(_T("Gold")) == 0 )
								strValue = _T("#FFD700");
							else if ( strValue.CompareNoCase(_T("Goldenrod")) == 0 )
								strValue = _T("#DAA520");
							else if ( strValue.CompareNoCase(_T("Gray")) == 0 )
								strValue = _T("#808080");
							else if ( strValue.CompareNoCase(_T("Green")) == 0 )
								strValue = _T("#008000");
							else if ( strValue.CompareNoCase(_T("GreenYellow")) == 0 )
								strValue = _T("#ADFF2F");
							else if ( strValue.CompareNoCase(_T("Honeydew")) == 0 )
								strValue = _T("#F0FFF0");
							else if ( strValue.CompareNoCase(_T("HotPink")) == 0 )
								strValue = _T("#FF69B4");
							else if ( strValue.CompareNoCase(_T("IndianRed")) == 0 )
								strValue = _T("#CD5C5C");
							else if ( strValue.CompareNoCase(_T("Indigo")) == 0 )
								strValue = _T("#4B0082");
							else if ( strValue.CompareNoCase(_T("Ivory")) == 0 )
								strValue = _T("#FFFFF0");
							else if ( strValue.CompareNoCase(_T("Khaki")) == 0 )
								strValue = _T("#F0E86C");
							else if ( strValue.CompareNoCase(_T("Lavender")) == 0 )
								strValue = _T("#E6E6FA");
							else if ( strValue.CompareNoCase(_T("LavenderBlush")) == 0 )
								strValue = _T("#FFF0F5");
							else if ( strValue.CompareNoCase(_T("LawnGreen")) == 0 )
								strValue = _T("#7CFC00");
							else if ( strValue.CompareNoCase(_T("LemonChiffon")) == 0 )
								strValue = _T("#FFFACD");
						}
					}
					else
					{
						if ( strValue.CompareNoCase(_T("Pale")) < 0 )
						{
							if ( strValue.CompareNoCase(_T("Medium")) < 0 )
							{
								if ( strValue.CompareNoCase(_T("LightBlue")) == 0 )
									strValue = _T("#ADD8E6");
								else if ( strValue.CompareNoCase(_T("LightCoral")) == 0 )
									strValue = _T("#F08080");
								else if ( strValue.CompareNoCase(_T("LightCyan")) == 0 )
									strValue = _T("#E0FFFF");
								else if ( strValue.CompareNoCase(_T("LightGoldenrodYellow")) == 0 )
									strValue = _T("#FAFAD2");
								else if ( strValue.CompareNoCase(_T("LightGray")) == 0 )
									strValue = _T("#D3D3D3");
								else if ( strValue.CompareNoCase(_T("LightGreen")) == 0 )
									strValue = _T("#90EE90");
								else if ( strValue.CompareNoCase(_T("LightPink")) == 0 )
									strValue = _T("#FFB6C1");
								else if ( strValue.CompareNoCase(_T("LightSalmon")) == 0 )
									strValue = _T("#FFA07A");
								else if ( strValue.CompareNoCase(_T("LightSkyBlue")) == 0 )
									strValue = _T("#87CEFA");
								else if ( strValue.CompareNoCase(_T("LightSeaGreen")) == 0 )
									strValue = _T("#20B2AA");
								else if ( strValue.CompareNoCase(_T("LightSlateGray")) == 0 )
									strValue = _T("#778899");
								else if ( strValue.CompareNoCase(_T("LightSteelBlue")) == 0 )
									strValue = _T("#B0C4DE");
								else if ( strValue.CompareNoCase(_T("LightYellow")) == 0 )
									strValue = _T("#FFFFE0");
								else if ( strValue.CompareNoCase(_T("Lime")) == 0 )
									strValue = _T("#00FF00");
								else if ( strValue.CompareNoCase(_T("LimeGreen")) == 0 )
									strValue = _T("#32CD32");
								else if ( strValue.CompareNoCase(_T("Linen")) == 0 )
									strValue = _T("#FAF0E6");
								else if ( strValue.CompareNoCase(_T("Magenta")) == 0 )
									strValue = _T("#FF00FF");
								else if ( strValue.CompareNoCase(_T("Maroon")) == 0 )
									strValue = _T("#800000");
							}
							else
							{
								if ( strValue.CompareNoCase(_T("MediumAquamarine")) == 0 )
									strValue = _T("#66CDAA");
								else if ( strValue.CompareNoCase(_T("MediumBlue")) == 0 )
									strValue = _T("#0000CD");
								else if ( strValue.CompareNoCase(_T("MediumOrchid")) == 0 )
									strValue = _T("#BA55D3");
								else if ( strValue.CompareNoCase(_T("MediumPurple")) == 0 )
									strValue = _T("#9370DB");
								else if ( strValue.CompareNoCase(_T("MediumSeaGreen")) == 0 )
									strValue = _T("#3CB371");
								else if ( strValue.CompareNoCase(_T("MediumSlateBlue")) == 0 )
									strValue = _T("#7B68EE");
								else if ( strValue.CompareNoCase(_T("MediumSpringGreen")) == 0 )
									strValue = _T("#00FA9A");
								else if ( strValue.CompareNoCase(_T("MediumTurquoise")) == 0 )
									strValue = _T("#48D1CC");
								else if ( strValue.CompareNoCase(_T("MediumVioletRed")) == 0 )
									strValue = _T("#C71585");
								else if ( strValue.CompareNoCase(_T("MidnightBlue")) == 0 )
									strValue = _T("#191970");
								else if ( strValue.CompareNoCase(_T("MintCream")) == 0 )
									strValue = _T("#F5FFFA");
								else if ( strValue.CompareNoCase(_T("MistyRose")) == 0 )
									strValue = _T("#FFE4E1");
								else if ( strValue.CompareNoCase(_T("Mocassin")) == 0 )
									strValue = _T("#FFE4B5");
								else if ( strValue.CompareNoCase(_T("NavajoWhite")) == 0 )
									strValue = _T("#FFDEAD");
								else if ( strValue.CompareNoCase(_T("Navy")) == 0 )
									strValue = _T("#000080");
								else if ( strValue.CompareNoCase(_T("OldLace")) == 0 )
									strValue = _T("#FDF5E6");
								else if ( strValue.CompareNoCase(_T("Olive")) == 0 )
									strValue = _T("#808000");
								else if ( strValue.CompareNoCase(_T("OliveDrab")) == 0 )
									strValue = _T("#6B8E23");
								else if ( strValue.CompareNoCase(_T("Orange")) == 0 )
									strValue = _T("#FFA500");
								else if ( strValue.CompareNoCase(_T("OrangeRed")) == 0 )
									strValue = _T("#FF4500");
								else if ( strValue.CompareNoCase(_T("Orchid")) == 0 )
									strValue = _T("#DA70D6");
							}
						}
						else
						{
							if ( strValue.CompareNoCase(_T("Sea")) < 0 )
							{
								if ( strValue.CompareNoCase(_T("PaleGoldenrod")) == 0 )
									strValue = _T("#EEE8AA");
								else if ( strValue.CompareNoCase(_T("PaleGreen")) == 0 )
									strValue = _T("#98FB98");
								else if ( strValue.CompareNoCase(_T("PaleTurquoise")) == 0 )
									strValue = _T("#AFEEEE");
								else if ( strValue.CompareNoCase(_T("PaleVioletRed")) == 0 )
									strValue = _T("#DB7093");
								else if ( strValue.CompareNoCase(_T("PapayaWhip")) == 0 )
									strValue = _T("#FFEFD5");
								else if ( strValue.CompareNoCase(_T("PeachPuff")) == 0 )
									strValue = _T("#FFDAB9");
								else if ( strValue.CompareNoCase(_T("Peru")) == 0 )
									strValue = _T("#CD853F");
								else if ( strValue.CompareNoCase(_T("Pink")) == 0 )
									strValue = _T("#FFC0CB");
								else if ( strValue.CompareNoCase(_T("Plum")) == 0 )
									strValue = _T("#DDA0DD");
								else if ( strValue.CompareNoCase(_T("PowderBlue")) == 0 )
									strValue = _T("#B0E0E6");
								else if ( strValue.CompareNoCase(_T("Purple")) == 0 )
									strValue = _T("#800080");
								else if ( strValue.CompareNoCase(_T("Red")) == 0 )
									strValue = _T("#FF0000");
								else if ( strValue.CompareNoCase(_T("RosyBrown")) == 0 )
									strValue = _T("#BC8F8F");
								else if ( strValue.CompareNoCase(_T("RoyalBlue")) == 0 )
									strValue = _T("#4169E1");
								else if ( strValue.CompareNoCase(_T("SaddleBrown")) == 0 )
									strValue = _T("#8b4513");
								else if ( strValue.CompareNoCase(_T("Salmon")) == 0 )
									strValue = _T("#FA8072");
								else if ( strValue.CompareNoCase(_T("SandyBrown")) == 0 )
									strValue = _T("#F4A460");
							}
							else
							{
								if ( strValue.CompareNoCase(_T("SeaGreen")) == 0 )
									strValue = _T("#2E8B57");
								else if ( strValue.CompareNoCase(_T("SeaShell")) == 0 )
									strValue = _T("#FFF5EE");
								else if ( strValue.CompareNoCase(_T("Sienna")) == 0 )
									strValue = _T("#A0522D");
								else if ( strValue.CompareNoCase(_T("Silver")) == 0 )
									strValue = _T("#C0C0C0");
								else if ( strValue.CompareNoCase(_T("SpringGreen")) == 0 )
									strValue = _T("#00FF7F");
								else if ( strValue.CompareNoCase(_T("SkyBlue")) == 0 )
									strValue = _T("#87CEEB");
								else if ( strValue.CompareNoCase(_T("SlateBlue")) == 0 )
									strValue = _T("#6A5ACD");
								else if ( strValue.CompareNoCase(_T("SlateGray")) == 0 )
									strValue = _T("#708090");
								else if ( strValue.CompareNoCase(_T("Snow")) == 0 )
									strValue = _T("#FFFAFA");
								else if ( strValue.CompareNoCase(_T("SteelBlue")) == 0 )
									strValue = _T("#4682B4");
								else if ( strValue.CompareNoCase(_T("Tan")) == 0 )
									strValue = _T("#D2B48C");
								else if ( strValue.CompareNoCase(_T("Teal")) == 0 )
									strValue = _T("#008080");
								else if ( strValue.CompareNoCase(_T("Tomato")) == 0 )
									strValue = _T("#FF6347");
								else if ( strValue.CompareNoCase(_T("Turquoise")) == 0 )
									strValue = _T("#40E0D0");
								else if ( strValue.CompareNoCase(_T("Thistle")) == 0 )
									strValue = _T("#D8BFD8");
								else if ( strValue.CompareNoCase(_T("Violet")) == 0 )
									strValue = _T("#EE82EE");
								else if ( strValue.CompareNoCase(_T("Wheat")) == 0 )
									strValue = _T("#F5DEB3");
								else if ( strValue.CompareNoCase(_T("White")) == 0 )
									strValue = _T("#FFFFFF");
								else if ( strValue.CompareNoCase(_T("WhiteSmoke")) == 0 )
									strValue = _T("#F5F5F5");
								else if ( strValue.CompareNoCase(_T("Yellow")) == 0 )
									strValue = _T("#FFFF00");
								else if ( strValue.CompareNoCase(_T("YellowGreen")) == 0 )
									strValue = _T("#9ACD32");
							}
						}
					}

					if ( strValue[0] == '#' )
					{
						strValue = strValue.Mid(1);
						if ( strValue.GetLength() < 6 )
						{
							CString strTemp;
							strTemp.Format(_T("%0*d%s"),6-strValue.GetLength(),0,strValue);
							strValue = strTemp;
						}
						polCurrent.SetColor(RGB(FromHex(strValue.Left(2)),FromHex(strValue.Mid(2,2)),FromHex(strValue.Mid(4,2))));
					}
				}
			}
		}
	}
	else if ( strTag.Left(5).CompareNoCase(_T("/font")) == 0 )
	{
		if ( arrFontStack.GetSize() >0 )
		{
			CHTMLFont * pFont = (CHTMLFont *) arrFontStack.GetAt(arrFontStack.GetSize()-1);
			polCurrent = *pFont;
			delete pFont;
			arrFontStack.SetSize(arrFontStack.GetSize()-1);
		}
	}
}

void CHTMLDrawer::FindTagAttributes(const CString & strAttributes,CStringArray & arrNames,CStringArray & arrValues)
{
	CString strInfo = strAttributes;
	while ( true )
	{
		int nPos = strInfo.Find(_T('='));
		if ( nPos == -1 )
			break;

		// Find the name of the property
		CString strName = strInfo.Left(nPos);
		strName.TrimLeft();
		strName.TrimRight();

		// Find the start of the value
		strInfo = strInfo.Mid(nPos+1);
		while ( strInfo[0] == ' ')
			strInfo = strInfo.Mid(1);

		// Find the value
		CString strValue;
		if ( strInfo[0] == '"' )
		{
			nPos = strInfo.Find('"',1);
			if ( nPos != -1 )
			{
				strValue=strInfo.Mid(1,nPos-1);
				strInfo = strInfo.Mid(nPos+1);
			}
		}
		else
		{
			nPos = strInfo.Find(' ');
			if ( nPos != -1 )
			{
				strValue=strInfo.Left(nPos);
				strInfo = strInfo.Mid(nPos+1);
			}
			else
			{
				strValue=strInfo;
				strInfo.Empty();
			}
		}

		arrNames.Add(strName);
		arrValues.Add(strValue);
	}
}


void CHTMLDrawer::BreakIntoLines(CDC * pDC,CObArray & arrAtoms,CObArray & arrDisplayAtoms,CRect & rctPosition,CObArray * parrAnchors)
{
	// Select the font
	CFont* pCurrentFont = NULL;
	CFont* pOldFont=NULL;

	int nY = rctPosition.top;
	int nX = rctPosition.left;
	int nXMax = rctPosition.right;
	int nStartLineAtom = 0;
	int nLowestAscent = 0;
	int nLine = 0;
	for ( int nAtom = 0; nAtom < arrAtoms.GetSize(); nAtom++)
	{
		CHTMLAtom * pAtom = (CHTMLAtom *) arrAtoms.GetAt(nAtom);

		// Find if there is a line break in the text
		bool isLineBreak = false;
		int nPos = pAtom->GetText().Find(_T("\r\n"));
		if ( nPos != -1 )
		{
			// Split this atom into 2
			CString strTextBefore = pAtom->GetText().Left(nPos);
			CString strTextAfter = pAtom->GetText().Mid(nPos+2);
			
			if ( !strTextAfter.IsEmpty() )
			{
				CHTMLAtom * pNewAtom = new CHTMLAtom(*pAtom);
				pNewAtom->SetText(strTextAfter);
				arrAtoms.InsertAt(nAtom+1,pNewAtom);
			}
			pAtom->SetText(strTextBefore);

			isLineBreak = true;
		}

		// Select the current font
		if ( pCurrentFont == NULL )
		{
			pCurrentFont = pAtom->GetHTMLFont().GetFont(pDC);
			pOldFont = pDC->SelectObject(pCurrentFont);
		}
		else
		{
			CFont * pNewFont = pAtom->GetHTMLFont().GetFont(pDC);
			pDC->SelectObject(pNewFont);
			//pCurrentFont->DeleteObject();
			//delete pCurrentFont;
			pCurrentFont = pNewFont;
		}

		// Find the size of the text
		CString strTextToWrite = pAtom->GetText();
		while ( !strTextToWrite.IsEmpty() )
		{
			CSize sizText = pDC->GetTextExtent(strTextToWrite);
			if(pAtom->GetHTMLFont().IsImage())
			{
				sizText.cx = 16;
				sizText.cy = 16;
			}
			if(pAtom->GetHTMLFont().IsSpecialImage())
			{
				sizText.cx = 16;
				sizText.cy = 16;
			}
			if(pAtom->GetHTMLFont().IsParamNumber())
			{
				sizText.cx = 0;
				sizText.cy = 0;
			}
			// If it does not fit in the current line
			CString strPart=strTextToWrite;
			bool isCreateNewLine = false;
			while ( nX + sizText.cx >= nXMax)
			{
				RemoveLastWord(strPart);
				sizText = pDC->GetTextExtent(strPart);
				if(pAtom->GetHTMLFont().IsImage())
				{
					sizText.cx = 16;
					sizText.cy = 16;
				}
				if(pAtom->GetHTMLFont().IsSpecialImage())
				{
					sizText.cx = 16;
					sizText.cy = 16;
				}
				if(pAtom->GetHTMLFont().IsParamNumber())
				{
					sizText.cx = 0;
					sizText.cy = 0;
				}
				isCreateNewLine = true;
				if ( strPart.IsEmpty())
					break;
			}

			// If nothing fits, 
			if ( strPart.IsEmpty() )
			{
				// If the current line is empty
				if ( nStartLineAtom == arrDisplayAtoms.GetSize() )
				{
					// We have to break up anywhere
					strPart=strTextToWrite[0];
					for ( int nChar = 1; nChar < strTextToWrite.GetLength(); nChar++)
					{
						strPart+=strTextToWrite[nChar];
						sizText = pDC->GetTextExtent(strPart);
						if(pAtom->GetHTMLFont().IsImage())
						{
							sizText.cx = 16;
							sizText.cy = 16;
						}
						if(pAtom->GetHTMLFont().IsSpecialImage())
						{
							sizText.cx = 16;
							sizText.cy = 16;
						}
						if(pAtom->GetHTMLFont().IsParamNumber())
						{
							sizText.cx = 0;
							sizText.cy = 0;
						}
						if ( nX + sizText.cx > nXMax)
						{
							strPart = strPart.Left(strPart.GetLength()-1);
							break;
						}
					}
				}
			}

			if ( !strPart.IsEmpty() )
			{
				// We can draw strPart
				sizText = pDC->GetTextExtent(strPart);
				if(pAtom->GetHTMLFont().IsImage())
				{
					sizText.cx = 16;
					sizText.cy = 16;
				}
				if(pAtom->GetHTMLFont().IsSpecialImage())
				{
					sizText.cx = 16;
					sizText.cy = 16;
				}
				if(pAtom->GetHTMLFont().IsParamNumber())
				{
					sizText.cx = 0;
					sizText.cy = 0;
				}
				CRect rctTextPosition(nX,nY,nX+sizText.cx,nY+sizText.cy);

				// Get the text metrics so we can align text of different sizes
				TEXTMETRIC tmMetrics;
				pDC->GetTextMetrics(&tmMetrics);

				CHTMLAtom * pDisplayAtom = new CHTMLAtom(*pAtom);
				pDisplayAtom->SetText(strPart);
				pDisplayAtom->SetPosition(rctTextPosition);
				pDisplayAtom->SetAscent(tmMetrics.tmAscent);
				pDisplayAtom->SetLine(nLine);
				arrDisplayAtoms.Add(pDisplayAtom);

				if ( !pDisplayAtom->GetURL().IsEmpty() && (parrAnchors!=NULL) )
					parrAnchors->Add(pDisplayAtom);

				nX = rctTextPosition.right;
				strTextToWrite = strTextToWrite.Mid(strPart.GetLength());

				// Find the lowest position on the lign to align properly
				if ( tmMetrics.tmAscent> nLowestAscent )
					nLowestAscent = sizText.cy;
			}

			if ( isCreateNewLine )
			{
				// Align to lowest position
				AlignLineAccordingToAscent(arrDisplayAtoms,nLowestAscent,nStartLineAtom);

				CreateNewLine(pDC,arrDisplayAtoms,nStartLineAtom,nY);
				nX=rctPosition.left;
				strTextToWrite.TrimLeft();
				nLine++;
			}

			
		}
		if ( isLineBreak )
		{
			// Align to lowest position
			AlignLineAccordingToAscent(arrDisplayAtoms,nLowestAscent,nStartLineAtom);
			nLowestAscent=0;

			CreateNewLine(pDC,arrDisplayAtoms,nStartLineAtom,nY);
			nX=rctPosition.left;
			nLine++;
		}
	}

	// Align to lowest position
	AlignLineAccordingToAscent(arrDisplayAtoms,nLowestAscent,nStartLineAtom);
	nLowestAscent=0;

	if ( pOldFont )
		pDC->SelectObject(pOldFont);
	if(pCurrentFont)
	{
		//pCurrentFont->DeleteObject();
		//delete pCurrentFont;
	}
}

void CHTMLDrawer::AlignLineAccordingToAscent(CObArray & arrDisplayAtoms,int nLowestAscent,int nStartLineAtom)
{
	for ( int nAtom = nStartLineAtom; nAtom < arrDisplayAtoms.GetSize(); nAtom++)
	{
		CHTMLAtom * pAtom = (CHTMLAtom *) arrDisplayAtoms.GetAt(nAtom);
		if ( pAtom->GetAscent() != nLowestAscent )
		{
			CRect * pRect = pAtom->GetAdrPosition();
			*pRect+= CPoint(0,nLowestAscent-pAtom->GetAscent());
		}
	}
}


void CHTMLDrawer::CreateNewLine(CDC * pDC,CObArray & arrDisplayAtoms,int & nStartLineAtom,int & nY)
{
	// If no atom has been created for this line, create a dummy empty atom 
	if ( nStartLineAtom == arrDisplayAtoms.GetSize() )
	{
		CString strText=_T(" ");
		CSize sizText = pDC->GetTextExtent(strText);
		CRect rctTextPosition(0,nY,0+sizText.cx,nY+sizText.cy);

		CHTMLAtom * pDisplayAtom = new CHTMLAtom;
		pDisplayAtom->SetText(strText);
		pDisplayAtom->SetPosition(rctTextPosition);
		arrDisplayAtoms.Add(pDisplayAtom);

		nY=rctTextPosition.bottom+1;
		nStartLineAtom = arrDisplayAtoms.GetSize();

		return;
	}

	// Find the lowest atom on this line
	int nMaxBottom = nY;
	for ( int nAtom = nStartLineAtom; nAtom < arrDisplayAtoms.GetSize(); nAtom++)
	{
		CHTMLAtom * pLineAtom=(CHTMLAtom *) arrDisplayAtoms.GetAt(nAtom);
		if ( pLineAtom->GetPosition().bottom > nMaxBottom )
		{
			nMaxBottom = pLineAtom->GetPosition().bottom;
		}
	}
	/*
	// Align them to the lowest
	for ( nAtom = nStartLineAtom; nAtom < arrDisplayAtoms.GetSize(); nAtom++)
	{
		CHTMLAtom * pLineAtom=(CHTMLAtom *) arrDisplayAtoms.GetAt(nAtom);
		CRect rctPosition = pLineAtom->GetPosition();
		int nShift = nMaxBottom - rctPosition.bottom;
		rctPosition.top+=nShift;
		rctPosition.bottom+=nShift;
		pLineAtom->SetPosition(rctPosition);
	}
	*/
	nY=nMaxBottom+1;
	nStartLineAtom = arrDisplayAtoms.GetSize();
}

void CHTMLDrawer::RemoveLastWord(CString & strText)
{
	bool isOneCharRemoved = false;
	while ( strText.GetLength() != 0 )
	{
		TCHAR car = strText[strText.GetLength()-1];
		if ( ( ( car < 'a' ) || ( car > 'z' ) ) && ( ( car < 'A' ) || ( car > 'Z' ) ) )
		{
			if (!isOneCharRemoved )
				strText = strText.Left(strText.GetLength()-1);
			break;
		}
		strText = strText.Left(strText.GetLength()-1);
		isOneCharRemoved = true;
	}
}

void CHTMLDrawer::ApplyAlignments(CObArray & arrDisplayAtoms,int nShiftVertically,int nRight)
{
	int nLine=-1;
	int nShiftX = 0;
	for ( int nAtom = 0; nAtom < arrDisplayAtoms.GetSize(); nAtom++)
	{
		CHTMLAtom * pAtom = (CHTMLAtom *) arrDisplayAtoms.GetAt(nAtom);

		// If we have changed line
		if ( nLine != pAtom->GetLine() )
		{
			nLine = pAtom->GetLine();
			// If there is an alignent for this line
			if ( pAtom->GetAlignment() == 0 )
			{
				nShiftX = 0;
			}
			else
			{
				// Find the extent of the line
				int nMaxRight = pAtom->GetPosition().right;
				for ( int nFollowingAtom = nAtom+1; nFollowingAtom < arrDisplayAtoms.GetSize(); nFollowingAtom++)
				{
					CHTMLAtom * pFollowingAtom = (CHTMLAtom *) arrDisplayAtoms.GetAt(nFollowingAtom);
					if ( nLine != pFollowingAtom->GetLine() )
						break;
					nMaxRight = pFollowingAtom->GetPosition().right;
				}
				if (pAtom->GetAlignment() == 1) 
					nShiftX = (nRight - nMaxRight)/2;
				else
					nShiftX = nRight - nMaxRight-1;
			}
		}

		// Change the position of the atom
		*pAtom->GetAdrPosition() += CPoint(nShiftX,nShiftVertically);
	}
}

#include "..\..\Structure\ObjectFunctions.h"

void CHTMLDrawer::DrawAtoms(CDC * pDC,CObArray & arrDisplayAtoms,COLORREF rgbHighlight,bool	isUnderlineWhenHighlight, CChronoEventEditor* ed, ImageVector* special_images, ParamVector* param, int& param_number)
{
	if(param)	// empty our rects
	{
		for(int p = 0; p < param->size(); p++)
		{
			((*param)[p])->displayrects.clear();
		}

	}

	// Prepare the settings to be restored
	CFont* pCurrentFont = NULL;
	CFont* pOldFont;
	COLORREF rgbOldColor;
	pDC->SetBkMode(TRANSPARENT);

	for ( int nAtom = 0; nAtom < arrDisplayAtoms.GetSize(); nAtom++)
	{
		CHTMLAtom * pAtom = (CHTMLAtom *) arrDisplayAtoms.GetAt(nAtom);

		CHTMLFont htmlFont(pAtom->GetHTMLFont());
		if ( pAtom->IsHighlighted() )
		{
			htmlFont.SetColor(rgbHighlight);
			htmlFont.SetUnderline(isUnderlineWhenHighlight);
		}

		// Select the font
		if ( pCurrentFont == NULL )
		{
			pCurrentFont = htmlFont.GetFont(pDC);
			pOldFont = pDC->SelectObject(pCurrentFont);
			rgbOldColor  = pDC->SetTextColor(htmlFont.GetColor());
		}
		else
		{
			CFont * pNewFont = htmlFont.GetFont(pDC);
			pDC->SelectObject(pNewFont);
			//pCurrentFont->DeleteObject();
			//delete pCurrentFont;
			pCurrentFont = pNewFont;
			pDC->SetTextColor(htmlFont.GetColor());
		}

		if (htmlFont.IsImage())
		{
			CString sImage = pAtom->GetText();
			sImage.Replace("<object>", "");
			int Image = atoi(sImage);

			pAtom->GetAdrPosition()->top--;
			pAtom->GetAdrPosition()->top--;
			pAtom->GetAdrPosition()->bottom--;
			pAtom->GetAdrPosition()->bottom--;

			pDC->Draw3dRect(pAtom->GetAdrPosition(),0,0);
		
			static HDC dcMem = CreateCompatibleDC(pDC->m_hDC);

			HBITMAP ToDraw;
			ToDraw = ed->GetIcon(Image);

			HBITMAP old = (HBITMAP)SelectObject(dcMem, ToDraw);
			BitBlt(pDC->m_hDC,pAtom->GetAdrPosition()->left,pAtom->GetAdrPosition()->top
				,pAtom->GetAdrPosition()->Width()
				,pAtom->GetAdrPosition()->Height(),dcMem,0,0,SRCCOPY);
			SelectObject(dcMem, old);
			//DeleteDC(dcMem);

			if(htmlFont.IsParam())
			{
				CRect pos = *pAtom->GetAdrPosition();
				((*param)[param_number])->displayrects.push_back(pos);
			}
		}
		else if(htmlFont.IsSpecialImage())
		{
			
			CString sImage = pAtom->GetText();
			int number = atoi(sImage);
			if(special_images)
				if(special_images->size() > number)
				{
					CEditorImage* img = &special_images->at(number);

					pAtom->GetAdrPosition()->top--;
					pAtom->GetAdrPosition()->top--;
					pAtom->GetAdrPosition()->bottom--;
					pAtom->GetAdrPosition()->bottom--;

					pDC->Draw3dRect(pAtom->GetAdrPosition(),0,0);
		
					HBITMAP ToDraw;
					int image = 0;
					if(img->tok)
					{

						image = img->tok->oidOwner;
						//if(img->tok->t == T_IDENTIFIER)// && img->tok->tsub == TT_OBJECTNAME)
							ed->m_objecthbitmaps.Lookup(image, image);
						//else if(img->tok->t == T_IDENTIFIER && img->tok->tsub == TT_FAMILYNAME)
						//	ed->m_familyhbitmaps.Lookup(image, image);


						if(img->tok->t == T_COLOR) // color
						{
							int colour = img->tok->id;
							CRect bound(pAtom->GetAdrPosition()->left,pAtom->GetAdrPosition()->top
								,pAtom->GetAdrPosition()->left + pAtom->GetAdrPosition()->Width()
								,pAtom->GetAdrPosition()->top + pAtom->GetAdrPosition()->Height());

							pDC->FillSolidRect(bound, colour);
							pDC->Draw3dRect(bound, 0, 0);
						}
						else
						{
							static HDC dcMem = CreateCompatibleDC(pDC->m_hDC);
							
							ToDraw = ed->GetIcon(image);

							HBITMAP old = (HBITMAP)SelectObject(dcMem, ToDraw);
							BitBlt(pDC->m_hDC,pAtom->GetAdrPosition()->left,pAtom->GetAdrPosition()->top
								,pAtom->GetAdrPosition()->Width()
								,pAtom->GetAdrPosition()->Height(),dcMem,0,0,SRCCOPY);

							img->m_Rect = pAtom->GetAdrPosition();

							SelectObject(dcMem, old);
							//DeleteDC(dcMem);
						}
					}
				}

			if(htmlFont.IsParam())
			{
				CRect pos = *pAtom->GetAdrPosition();
				((*param)[param_number])->displayrects.push_back(pos);
			}
		}

		else if(htmlFont.IsParamNumber())
		{
			param_number = atoi(pAtom->GetText());
		}

		else	//normal text
		{
			pAtom->GetAdrPosition()->top--;
			pAtom->GetAdrPosition()->bottom--;
			pDC->DrawText(pAtom->GetText(),pAtom->GetAdrPosition(),DT_LEFT |DT_NOPREFIX |DT_SINGLELINE);
			pAtom->GetAdrPosition()->top++;
			pAtom->GetAdrPosition()->bottom++;
			if(htmlFont.IsParam())
			{
				CRect pos = *pAtom->GetAdrPosition();
				((*param)[param_number])->displayrects.push_back(pos);
			}

			Readable += pAtom->GetText();
		}
	
	}

	// Restore the settings
	pDC->SelectObject(pOldFont);
	pDC->SetTextColor(rgbOldColor);
	if(pCurrentFont)
	{
	//	pCurrentFont->DeleteObject();
//		delete pCurrentFont;
	}
}


long CHTMLDrawer::FromHex(const CString & strHex)
{
	CString strHexa(strHex);
	strHexa.TrimRight();
	strHexa.TrimLeft();
	long nRes=0;
	long nBase=1;
	for ( int nChar = strHexa.GetLength()-1; nChar >= 0; nChar--)
	{
		switch (strHexa[nChar])
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			nRes+=(strHexa[nChar]-'0')*nBase;
			break;
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			nRes+=(10+strHexa[nChar]-'a')*nBase;
			break;
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			nRes+=(10+strHexa[nChar]-'A')*nBase;
			break;
		}
		nBase*=16;
	}
	return nRes;
}


long CHTMLDrawer::ToLong(LPCTSTR lpszText)
{
	ASSERT(lpszText != NULL);

	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;
	TCHAR chFirst = lpszText[0];
	long l;
	l = _tcstol(lpszText, (LPTSTR*)&lpszText, 10);

	if (l == 0 && chFirst != '0')
		return 0;   // could not convert

	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;
	if (*lpszText != '\0')
		return 0;   // not terminated properly

	// all ok
	return l;
}


bool CHTMLDrawer::IsPointInAnchor(CPoint & pntMouse,CHTMLAtom *& pAnchor)
{
	for ( int nAnchor = 0; nAnchor < m_arrAnchors.GetSize(); nAnchor++)
	{
		pAnchor = (CHTMLAtom *) m_arrAnchors.GetAt(nAnchor);
		if( pAnchor->GetPosition().PtInRect(pntMouse) )
			return true;
	}

	return false;
}

bool CHTMLDrawer::HighlightAnchor(CHTMLAtom * pHighlight,CRect & rctToInvalidate)
{
	bool isInvalidate = false;
	for ( int nAnchor = 0; nAnchor < m_arrAnchors.GetSize(); nAnchor++)
	{
		CHTMLAtom * pAnchor = (CHTMLAtom *) m_arrAnchors.GetAt(nAnchor);
		if ( pHighlight == pAnchor )
		{
			if ( !pAnchor->IsHighlighted() )
			{
				pAnchor->SetHighlighted(true);
				rctToInvalidate.UnionRect(rctToInvalidate,pAnchor->GetPosition());
				isInvalidate = true;
			}
		}
		else if ( pAnchor->IsHighlighted() )
		{
			pAnchor->SetHighlighted(false);
			rctToInvalidate.UnionRect(rctToInvalidate,pAnchor->GetPosition());
			isInvalidate = true;
		}
	}

	return isInvalidate;
}