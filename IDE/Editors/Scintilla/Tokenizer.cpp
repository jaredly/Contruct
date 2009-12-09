/* @doc
 * @module Tokenizer.cpp | Implementation of a string tokenizer class.
 * Example:
 *   CTokenizer st(_T("\"aaa\";bb;100;200000;33.33;999999999.999999999;true"), _T(";"));
 *   TRACE(_T("NextString Test<\"aaa\";bb;100;200000;33.33;999999999.999999999;true>"));
 *   TRACE(_T("NextString=<%s>"), st.NextString(TRUE, _T("dddd")));
 *   TRACE(_T("NextString=<%s>"), st.NextString(TRUE, _T("eeee")));
 *   TRACE(_T("NextInt=<%d>"), st.NextInt(55));
 *   TRACE(_T("NextLong=<%ld>"), st.NextLong(55555555555));
 *   TRACE(_T("NextFloat=<%g>"), st.NextFloat(66.66));
 *   TRACE(_T("NextDouble=<%lg>"), st.NextDouble(666666666666.6666666666666));
 *   TRACE(_T("NextBool=<%d>"), st.NextBool(FALSE));
 */
#include "stdafx.h"
#include "Tokenizer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////
// @mfunc This is an empty constructor
// @rvalue void | not used
//
CTokenizer::CTokenizer()
{
   m_nPosition = 0;

}
/////////////////////////////////////
// @mfunc This is an constructor
// @rvalue void | not used
//
CTokenizer::CTokenizer(
                       const CString string, //@parm String to examine
                       CString strDelim)     //@parm String with single char delimiters
{
   m_Copy = string;
   m_Delimiter = strDelim;
   m_nPosition = 0;
}
/////////////////////////////////////
// @mfunc This is a destructor
// @rvalue void | not used
//
CTokenizer::~CTokenizer()
{
}
/////////////////////////////////////
// @mfunc This is an constructor
// @rvalue void | not used
//
void CTokenizer::SetString(
                           const CString string) //@parm String to examine
{
   m_Copy = string;
   m_nPosition = 0;
}
/////////////////////////////////////
// @mfunc Extracts the next String token - empty tokens are allowed
// @rvalue CString | next token
//
CString CTokenizer::Next()
{
   CString strReturn;
   int nLen = m_Copy.GetLength();
// if we have more characters in string
   while (nLen > m_nPosition)
   {
//    check all delimiters
      for (int i=0;i<m_Delimiter.GetLength();i++)
      {
//       if we find a delimiter or we find end of string return
         if (m_Copy.GetAt(m_nPosition) == m_Delimiter.GetAt(i))
         {
            m_nPosition++;
            return strReturn;
         }
      }
//    copy char
      strReturn += m_Copy.GetAt(m_nPosition);
      m_nPosition++;
   }
   return strReturn;
}
/////////////////////////////////////
// @mfunc Returns flag if more tokens are available
// @rvalue BOOL | TRUE if next token is available
//
BOOL CTokenizer::HasMoreTokens()
{
   if (m_nPosition == m_Copy.GetLength())
      return FALSE;
   return TRUE;
}
/////////////////////////////////////
// @mfunc Returs next token - if no token available returns default
// Delimiters are allowed within quotes!
// @rvalue CString | next token as string
//
CString CTokenizer::NextString(
                               BOOL bRemoveQuotes, 
                               CString strDefault)
{
   CString strReturn;
   int nLen = m_Copy.GetLength();
// at end - return default
   if (nLen <= m_nPosition)
      return strDefault;
// skip quotes if not wanted
   if (bRemoveQuotes)
   {
//    skip to first quote
      while (nLen > m_nPosition && m_Copy.GetAt(m_nPosition) != '\"')
         m_nPosition++;
      m_nPosition++; // skip quote
//    copy
      while (nLen > m_nPosition && m_Copy.GetAt(m_nPosition) != '\"')
      {
         strReturn += m_Copy.GetAt(m_nPosition);
         m_nPosition++;
      }
      m_nPosition++; // skip quote
//    skip to next delimiter
      while (nLen > m_nPosition)
      {
//       check all delimiters
         for (int i=0;i<m_Delimiter.GetLength();i++)
         {
//          if we find a delimiter or we find end of string return
            if (m_Copy.GetAt(m_nPosition) == m_Delimiter.GetAt(i))
            {
               m_nPosition++;
               return strReturn;
            }
         }
      }
      return strReturn;
   }
// if we have more characters in string
   while (nLen > m_nPosition)
   {
//    check all delimiters
      for (int i=0;i<m_Delimiter.GetLength();i++)
      {
//       if we find a delimiter or we find end of string return
         if (m_Copy.GetAt(m_nPosition) == m_Delimiter.GetAt(i))
         {
            m_nPosition++;
            return strReturn;
         }
      }
//    copy char 
      strReturn += m_Copy.GetAt(m_nPosition);
      m_nPosition++;
   }
// we did not find any tokens simply return 
   return strReturn;
}
/////////////////////////////////////
// @mfunc Returns next token translated as int
// @rvalue int | next token converted to int - or default at error
//
int CTokenizer::NextInt(
                        int nDefault)
{
   CString strReturn;
   int nLen = m_Copy.GetLength();
// if we have more characters in string
   while (nLen > m_nPosition)
   {
//    check all delimiters
      for (int i=0;i<m_Delimiter.GetLength();i++)
      {
//       if we find a delimiter or we find end of string return
         if (m_Copy.GetAt(m_nPosition) == m_Delimiter.GetAt(i))
         {
            m_nPosition++;
            int nReturn = nDefault;
            int nFields = ::sscanf ((LPCSTR)strReturn, _T("%d"), &nReturn);
            return nReturn;
         }
      }
//    copy char
      strReturn += m_Copy.GetAt(m_nPosition);
      m_nPosition++;
   }
   int nReturn = nDefault;
   int nFields = ::sscanf ((LPCSTR)strReturn, _T("%d"), &nReturn);
   return nReturn;
}
/////////////////////////////////////
// @mfunc Returns next token translated as long
// @rvalue int | next token converted to long - or default at error
//
long CTokenizer::NextLong(
                          long nDefault)
{
   CString strReturn;
   int nLen = m_Copy.GetLength();
// if we have more characters in string
   while (nLen > m_nPosition)
   {
//    check all delimiters
      for (int i=0;i<m_Delimiter.GetLength();i++)
      {
//       if we find a delimiter or we find end of string return
         if (m_Copy.GetAt(m_nPosition) == m_Delimiter.GetAt(i))
         {
            m_nPosition++;
            long nReturn = nDefault;
            int nFields = ::sscanf ((LPCSTR)strReturn, _T("%ld"), &nReturn);
            return nReturn;
         }
      }
//    copy char
      strReturn += m_Copy.GetAt(m_nPosition);
      m_nPosition++;
   }
   long nReturn = nDefault;
   int nFields = ::sscanf ((LPCSTR)strReturn, _T("%ld"), &nReturn);
   return nReturn;
}
/////////////////////////////////////
// @mfunc Returns next token translated as float
// @rvalue float | next token converted to float - or default at error
//
float CTokenizer::NextFloat(
                            float fDefault)
{
   CString strReturn;
   int nLen = m_Copy.GetLength();
// if we have more characters in string
   while (nLen > m_nPosition)
   {
//    check all delimiters
      for (int i=0;i<m_Delimiter.GetLength();i++)
      {
//       if we find a delimiter or we find end of string return
         if (m_Copy.GetAt(m_nPosition) == m_Delimiter.GetAt(i))
         {
            m_nPosition++;
            float fReturn = fDefault;
            strReturn.Replace(',','.'); // replace comma with dot for german numbers
            int nFields = ::sscanf ((LPCSTR)strReturn, _T("%g"), &fReturn);
            return fReturn;
         }
      }
//    copy char
      strReturn += m_Copy.GetAt(m_nPosition);
      m_nPosition++;
   }
   float fReturn = fDefault;
   strReturn.Replace(',','.'); // replace comma with dot for german numbers
   int nFields = ::sscanf ((LPCSTR)strReturn, _T("%g"), &fReturn);
   return fReturn;
}
/////////////////////////////////////
// @mfunc Returns next token translated as double
// @rvalue float | next token converted to double - or default at error
//
double CTokenizer::NextDouble(
                              double fDefault)
{
   CString strReturn;
   int nLen = m_Copy.GetLength();
// if we have more characters in string
   while (nLen > m_nPosition)
   {
//    check all delimiters
      for (int i=0;i<m_Delimiter.GetLength();i++)
      {
//       if we find a delimiter or we find end of string return
         if (m_Copy.GetAt(m_nPosition) == m_Delimiter.GetAt(i))
         {
            m_nPosition++;
            double fReturn = fDefault;
            strReturn.Replace(',','.'); // replace comma with dot for german numbers
            int nFields = ::sscanf ((LPCSTR)strReturn, _T("%lg"), &fReturn);
            return fReturn;
         }
      }
//    copy char
      strReturn += m_Copy.GetAt(m_nPosition);
      m_nPosition++;
   }
   double fReturn = fDefault;
   strReturn.Replace(',','.'); // replace comma with dot for german numbers
   int nFields = ::sscanf ((LPCSTR)strReturn, _T("%lg"), &fReturn);
   return fReturn;
}
/////////////////////////////////////
// @mfunc Returns next token as boolean value where FALSE can be coded as the string "false" or as the number 0
// and TRUE as string "true" or as number != 0.
// @rvalue BOOL | next token as boolean value or default if no token found
//
BOOL CTokenizer::NextBool(
                          BOOL bDefault)
{
   CString strReturn;
   int nLen = m_Copy.GetLength();
// if we have more characters in string
   while (nLen > m_nPosition)
   {
//    check all delimiters
      for (int i=0;i<m_Delimiter.GetLength();i++)
      {
//       if we find a delimiter or we find end of string return
         if (m_Copy.GetAt(m_nPosition) == m_Delimiter.GetAt(i))
         {
            m_nPosition++;
            int nReturn = 0;
            int nFields = ::sscanf ((LPCSTR)strReturn, _T("%d"), &nReturn);
//          if we cant translate to number try keyword
            if (nFields < 1)
            {
               if (strReturn.CompareNoCase(BOOL_TRUE) == 0)
                  return TRUE;
               else if (strReturn.CompareNoCase(BOOL_FALSE) == 0)
                  return FALSE;
               else
                  return bDefault;
            }
            else if (nReturn == 0)
               return FALSE;
            else
               return TRUE;
         }
      }
//    copy char
      strReturn += m_Copy.GetAt(m_nPosition);
      m_nPosition++;
   }
   int nReturn = 0;
   int nFields = ::sscanf ((LPCSTR)strReturn, _T("%d"), &nReturn);
// if we cant translate to number try keyword
   if (nFields < 1)
   {
      if (strReturn.CompareNoCase(BOOL_TRUE) == 0)
         return TRUE;
      else if (strReturn.CompareNoCase(BOOL_FALSE) == 0)
         return FALSE;
      else
         return bDefault;
   }
   else if (nReturn == 0)
      return FALSE;
   else
      return TRUE;
}
