// Include StdAfx.h
#include "StdAfx.h"

#ifdef RUN_ONLY

/////////////////////////////
// RUNTIME serialization
void ExtObject::Serialize(bin& ar)
{
	int Version = 1;
	SerializeVersion(ar, Version);

	if (ar.loading) {

		//

	}
	else {

		//

	}
}

#else // RUN_ONLY

/////////////////////////////
// EDITTIME serialization
// Save and load all your object's edittime data here.
// If you change the serialization format after plugin release, change 'Version' and check it while loading.
// You can then use the old serialization routine if 'Version' is still 1, and set new features to their defaults.
// You never need to check 'Version' while saving.
void EditExt::Serialize(bin& ar)
{
	int Version = 2;
	SerializeVersion(ar, Version);

	if (ar.loading) {

		int size;
		ar >> size;
		tiles.reserve(size);
		for(int i = 0; i < size; i++)
		{
			int size;
			ar >> size;
			tiles.push_back(vector<CTile>());
			tiles.back().reserve(size);
			
			for(int j = 0; j < size; j++)
			{
				CTile tile;
				ar >> tile.index;
				tiles.back().push_back(tile);
			}
		}

		ar >> size;
		for(int i = 0; i < size; i++)
		{
			CMapPoint pt;
			ar >> pt.x;
			ar >> pt.y;
			int num;
			ar >> num;
			m_vt_sel[pt] = num;
		}

		ar >> m_vt_sel_x;
		ar >> m_vt_sel_y;
		ar >> m_vt_IsSel;
		ar >> m_vt_IsSelBox;
		ar >> m_vt_AddSel;

		ar >> size;
		for(int i = 0; i < size; i++)
		{
			CMapPoint pt;
			ar >> pt.x;
			ar >> pt.y;
			int num;
			ar >> num;
			m_sel[pt] = num;
		}

		ar >> m_sel_x;
		ar >> m_sel_y;
		ar >> m_IsSel;
		ar >> m_IsSelBox;
		ar >> m_AddSel;
		ar >> m_SelectingFromBoard;

		ar >> size;
		for(int i = 0; i < size; i++)
		{
			CMapPoint pt;
			ar >> pt.x;
			ar >> pt.y;
			int num;
			ar >> num;
			m_ghost[pt] = num;
		}

		pEditTime->SerializeAnimation(m_iAnimationRoot, ar);
		ar >> m_EditMode;
		ar >> m_ViewTiles;

		if(Version >= 2)
		{
			ar >> tileWidth;
			ar >> tileHeight;
		}
		else
		{
			tileWidth = 32;
			tileHeight = 32;
		}

	}
	else {
	
		ar << (int)tiles.size();
		for(vector<vector<CTile> >::iterator i = tiles.begin(); i != tiles.end(); i++)
		{
			ar << (int)i->size();
			for(vector<CTile>::iterator j = i->begin(); j!= i->end(); j++)
			{
				ar << j->index;
			}
		}

		ar << (int)m_vt_sel.size();
		for(map<CMapPoint, int, less_point>::iterator i = m_vt_sel.begin(); i!= m_vt_sel.end(); i++)
		{
			ar << i->first.x;
			ar << i->first.y;
			ar << i->second;
		}

		ar << m_vt_sel_x;
		ar << m_vt_sel_y;
		ar << m_vt_IsSel;
		ar << m_vt_IsSelBox;
		ar << m_vt_AddSel;

		ar << (int)m_sel.size();
		for(map<CMapPoint, int, less_point>::iterator i = m_sel.begin(); i!= m_sel.end(); i++)
		{
			ar << i->first.x;
			ar << i->first.y;
			ar << i->second;
		}

		ar << m_sel_x;
		ar << m_sel_y;
		ar << m_IsSel;
		ar << m_IsSelBox;
		ar << m_AddSel;
		ar << m_SelectingFromBoard;

		ar << (int)m_ghost.size();
		for(map<CMapPoint, int, less_point>::iterator i = m_ghost.begin(); i!= m_ghost.end(); i++)
		{
			ar << i->first.x;
			ar << i->first.y;
			ar << i->second;
		}

		pEditTime->SerializeAnimation(m_iAnimationRoot, ar);
		ar << m_EditMode;
		ar << m_ViewTiles;

		ar << tileWidth;
		ar << tileHeight;

	}
}

#endif // RUN_ONLY