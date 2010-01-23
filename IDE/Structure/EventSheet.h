#pragma once

#include "CApplication.h"

class EventSheet
{
public:

	EventSheet(CApplication* application_) :
		application(application_),
		changed(false),
		second_tree(NULL)
	{}

	bool operator == (const EventSheet& b)
	{
		return (identifier == b.identifier);
	}

	CApplication* application;
	int identifier;

	int m_ExportIndex; // export only
	EventVector m_EventList;

	CEditorHeader m_Header;
	
	// Name functions
	void SetName(LPCSTR name_);
	CString GetName() { return name; }

	// Changed functions
	void SetChanged(bool);
	bool GetChanged() { return changed; }

	// tree items
	map<CLayout*, HTREEITEM> tree;
	HTREEITEM second_tree;

	bool Serialize(CArchive& ar)
	{
		if(ar.IsLoading())
		{
			ar >> identifier;
			ar >> name;

			unsigned int esize;
			ar >> esize;

			for(int e = 0; e < esize; e ++ )
			{
				CEditorEvent* Evt = new CEditorEvent;
				Evt->Init();				
				if (!Evt->Serialize(ar, true, NULL))
					return false;

				m_EventList.push_back(Evt);
			}

			m_Header.Serialize(ar);
		}

		else
		{
			ar << identifier;
			ar << name;
	
			ar << m_EventList.size();
			for(int e = 0; e < m_EventList.size(); e ++ )
				m_EventList[e]->Serialize(ar,true, NULL);

			m_Header.Serialize(ar);

			changed = false;
		}

		return true;
	}

protected:

	bool changed;
	CString name;
};