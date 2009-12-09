#pragma once

class CDragObjects : public CObject
{
public:
	CDragObjects() 
	{
		clipboard = false;
		clone = false;
		nonlayout = false;	
	}

	~CDragObjects() {}
	CObjList		*m_pSel;
	CLayout			*layout;
	CApplication	*application;
	CLayoutEditor	*layout_editor;
	CPoint			m_curPt;
	virtual void Serialize( CArchive& ar );
	BOOL clipboard;

	bool clone;
	bool nonlayout;

	vector<long> layers;
};