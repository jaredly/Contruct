#pragma once

class CContainer
{
public:
	CString m_Name;
	vector<int> m_IDs;
	CApplication* application;	

	bool Serialize(CArchive& ar)
	{
		if (ar.IsLoading())
		{
			ar >> m_Name;

			int Count = 0;
			ar >> Count;

			m_IDs.clear();

			for(int i = 0; i < Count; i++)
			{
				int ID = 0;
				ar >> ID;
				m_IDs.push_back(ID);

				// Lookup this ID and update its pContainer to here
				long longID = ID;	// why do we mix and match int and long for IDs when
									// int& and long& cannot be converted?
				CObjType* pType = application->object_types[longID];

				pType->m_pContainer = this;
			}
		}
		else
		{
			ar << m_Name << (int)m_IDs.size();
			for (int i = 0; i < m_IDs.size(); i++)
				ar << m_IDs[i];
		}

		return true;
	}
};