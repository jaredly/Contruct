#include "StdAfx.h"
#include "CLayer.h"

CLayer::CLayer(CString szName, long layerType) 
{
	SetName(szName);
	m_state = 0;
	m_bDelImage = FALSE;
	m_layerType = layerType;
	m_angle = 0;
	m_inherrit_layerID = -1;
	color = RGB(255,255,255);
	clearBack = false;
	backColor = RGB(0,0,0);
	forceOwnTexture = false;

	zoomXf = zoomYf = zoomX = zoomY = opacity = scrollXf = scrollYf = 1.0f;
	scrollX = scrollY = 0.0f;

	sampler = sample_default;
	enable3d = false;
}

CLayer::~CLayer() 
{
	if (m_bDelImage)
		::DeleteObject(m_iThumbnail);
}

bool CLayer::Serialize(CArchive& ar)
{
	CString ExpectedName = "CLayer";
	int     Version      = 6;

	if (!SerializeClassHeader(ExpectedName, Version, ar))
		return false;

	if(ar.IsLoading())
	{
		ar >> m_layerID >> m_layerType >> m_name >> m_state >> m_angle
		   >> color >> scrollXf >> scrollYf >> opacity >> clearBack >> backColor >> zoomX >> zoomY;

		// v2 addition: force own texture
		if (Version >= 2)
			ar >> forceOwnTexture;

		//v3 addition
		if (Version >= 3)
			ar >> zoomXf >> zoomYf >> scrollX >> scrollY;

		//v4 addition
		if (Version >= 4)
			ar >> m_inherrit_layerID;

		// v5
		if (Version >= 5) {
			int temp;
			ar >> temp;
			sampler = (LayerSampler)temp;
		}

		// v6
		if (Version >= 6)
			ar >> enable3d;

		int count;
		ar >> count;
		
		// Load effects
		effects.clear();
		for (int i = 0; i < count; i++) {
			effects.push_back(CEffect());

			if (!effects.back().Serialize(ar))
				return false;
		}

		// Load Z order
		ar >> count;
		m_zOrder.RemoveAll();
		for (int i = 0; i < count; i++) {
			long id;
			ar >> id;
			m_zOrder.AddTail(id);
		}

	}

	else
	{
		ar << m_layerID << m_layerType << m_name << m_state << m_angle
		   << color << scrollXf << scrollYf << opacity << clearBack << backColor << zoomX << zoomY;

		// v2 addition: force own texture
		ar << forceOwnTexture;

		ar << zoomXf << zoomYf << scrollX << scrollY << m_inherrit_layerID;

		// v5
		ar << (int)sampler;

		// v6
		ar << enable3d;

		// Export effects
		ar << effects.size();

		EffectIterator e = effects.begin();
		EffectConstIterator end = effects.end();

		for ( ; e != end; e++)
			e->Serialize(ar);

		// Export Z order
		ar << m_zOrder.GetCount();
		POSITION pos = m_zOrder.GetHeadPosition();
		for (int i = 0; i < m_zOrder.GetCount(); i++)
			ar << m_zOrder.GetNext(pos);
	}

	return true;
}

void CLayer::GetEveryObject(CObjList& objList,CLayout* pFrame, long parent)
{
	CObjList* objs = NULL;
	if(parent == -1)
		objs = &m_zOrder;
	else
	{
		CObj* o = NULL;
		pFrame->objects.Lookup(parent, o);
		if(o)
		{
			objs = &o->m_groupObjects;
			if(!o->m_bIsGroup)
			{
				objList.AddTail(parent);
			}
		}
	}

	POSITION pos = objs->GetHeadPosition();
	while(pos)
	{
		int id = objs->GetNext(pos);
		GetEveryObject(objList, pFrame, id);
	}

}