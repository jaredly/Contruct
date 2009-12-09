/* @doc
 * @module Tokenizer.h | Definition of a String tokenizer class
 */
#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#define BOOL_TRUE  _T("true")
#define BOOL_FALSE  _T("false")

////////////////////////////////////
// @class CTokenizer | Class of a string tokenizer object
// @base public | --
//
class CTokenizer  
{
public:
// @access public constructor - destructor
// @cmember empty Constructor
	CTokenizer();
// @cmember Constructor
	CTokenizer(const CString string, CString strDelim = _T(" "));
// @cmember destructor
	virtual ~CTokenizer();

public:
// @access public functions
// @cmember returns next token
   virtual CString Next();
   virtual CString NextString(BOOL bRemoveQuotes = FALSE, CString strDefault = _T(""));
   virtual int     NextInt(int nDefault = 0);
   virtual long    NextLong(long nDefault = 0L);
   virtual float   NextFloat(float fDefault = 0.0);
   virtual double  NextDouble(double fDefault = 0.0);
   virtual BOOL    NextBool(BOOL bDefault = FALSE);
// @cmember tests for more tokens
   virtual BOOL HasMoreTokens();
// @cmember sets new delimiters
   virtual void SetDelimiter(CString strDelim){m_Delimiter = strDelim;};
// @cmember sets new string to examine
   virtual void SetString(const CString string);

protected:
// @access protected data members
// @cmember internal copy of the string
   CString m_Copy;
// @cmember internal copy of the delimiter string
   CString m_Delimiter;
// @cmember current index
   int m_nPosition;

};
#endif
