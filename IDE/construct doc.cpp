// Construct Doc.cpp
//

// File includes
#include "StdAfx.h"
#include "Construct.h"
#include "Construct Doc.h"
#include "Structure/SystemObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CConstructDoc *g_pDoc;

/////////////////////////////////////////////////////////////////////////////
// CConstructDoc

IMPLEMENT_DYNCREATE(CConstructDoc, CDocument)

BEGIN_MESSAGE_MAP(CConstructDoc, CDocument)
	//{{AFX_MSG_MAP(CConstructDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#include <shlwapi.h>

/////////////////////////////////////////////////////////////////////////////
// CConstructDoc construction/destruction

CConstructDoc::CConstructDoc()
{
}

CConstructDoc::~CConstructDoc()
{
}

BOOL CConstructDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CConstructDoc serialization

void CConstructDoc::Serialize(CArchive& ar)
{
//	m_level->Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CConstructDoc diagnostics

#ifdef _DEBUG
void CConstructDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CConstructDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CConstructDoc commands


/////////////////////////////////////////////////////////////////////////////
// CLevel

vector<ACESEntry2>* OINFO::GetTable(int type)
{
	if (type == ACTION)
		return &actions;
	if (type == CONDITION)
		return &conditions;
	if (type == EXPRESSION)
		return &expressions;

	return NULL;
}

ACESEntry* OINFO::GetACEEntry(int type, int index)
{
	if (index < 0 || index >= GetTable(type)->size()) return NULL;

	return &(GetTable(type)->at(index));
}

// Get a table's count
int CObjType::GetTableCount(int table)
{
	OINFO *oInfo;
	POSITION pos = g_dlls->GetStartPosition();
	DWORD nKey;

	// Find the DLL
	if(DLLIndex == -1)
	{
		return GetSystemTableCount(table);
	}

	else if(DLLIndex == -2)
	{
		if(table == ACTION)
			return m_pEffect->params.size() + 2;
		else
			return 0;
	}

	else
	{
		while (pos != NULL) 
		{
			g_dlls->GetNextAssoc(pos, nKey, oInfo);

			// Got it
			if (oInfo->oID == DLLIndex)
			{
				return oInfo->GetTable(table)->size();
			}
		}

	}
	return 0;
	
}
