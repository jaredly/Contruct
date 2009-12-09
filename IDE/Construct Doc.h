// Construct Doc.h : interface of the CConstructDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONSTRUCTDOC_H__0A20229A_4F25_4942_93B1_19B3A1AA598E__INCLUDED_)
#define AFX_CONSTRUCTDOC_H__0A20229A_4F25_4942_93B1_19B3A1AA598E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Structure.h"

class CConstructDoc : public CDocument
{
protected: // create from serialization only
	CConstructDoc();
	DECLARE_DYNCREATE(CConstructDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConstructDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CConstructDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:

// Generated message map functions
protected:
	//{{AFX_MSG(CConstructDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONSTRUCTDOC_H__0A20229A_4F25_4942_93B1_19B3A1AA598E__INCLUDED_)
