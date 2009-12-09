#include "StdAfx.h"
#include "CLayer.h"

CLayer::CLayer(CString szName, long layerType) 
{
	SetName(szName);
	m_state = LAYSTATE_VISIBLE;
	m_bDelImage = FALSE;
	m_layerType = layerType;
	m_angle = 0;
	color = RGB(255,255,255);
	clearBack = false;
	backColor = RGB(0,0,0);

	opacity = scrollXf = scrollYf = 1.0f;
}

CLayer::~CLayer() 
{
	if (m_bDelImage)
		::DeleteObject(m_iThumbnail);
}

void CLayer::Serialize(CArchive& ar)
{
	if(ar.IsLoading())
	{
		ar >> m_layerID >> m_layerType >> m_name >> m_state >> m_angle
		   >> color >> scrollXf >> scrollYf >> opacity >> clearBack >> backColor;

		int count;
		ar >> count;
		
		// Load effects
		m_Effects.clear();
		for (int i = 0; i < count; i++) {
			m_Effects.push_back(CEffect());
			m_Effects.back().Serialize(ar);
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
		   << color << scrollXf << scrollYf << opacity << clearBack << backColor;

		// Export effects
		ar << m_Effects.size();

		EffectIterator e = m_Effects.begin();
		EffectConstIterator end = m_Effects.end();

		for ( ; e != end; e++)
			e->Serialize(ar);

		// Export Z order
		ar << m_zOrder.GetCount();
		POSITION pos = m_zOrder.GetHeadPosition();
		for (int i = 0; i < m_zOrder.GetCount(); i++)
			ar << m_zOrder.GetNext(pos);

	}
}