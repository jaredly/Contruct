// Include StdAfx.h
#include "StdAfx.h"

#ifdef RUN_ONLY

/////////////////////////////
// RUNTIME serialization
void ExtObject::Serialize(bin& ar)
{
	if (ar.loading) 
	{
		timers.resize(0);

		int count = 0;
		ar >> count;

		for (int i = 0; i < count; i++)
		{
			ObjectTimer new_timer;
			ar >> new_timer.name;
			ar >> new_timer.repeat;
			ar >> new_timer.time;
			ar >> new_timer.initial_length;
			ar >> new_timer.destroy;

			timers.push_back(new_timer);
		}
	}

	else 
	{
		ar << timers.size();

		vector<ObjectTimer>::iterator i = timers.begin();

		for ( ; i != timers.end(); ) 
		{
			ar << i->name;
			ar << i->repeat;
			ar << i->time;
			ar << i->initial_length;
			ar << i->destroy;
		}
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
	int Version = 1;
	SerializeVersion(ar, Version);
}

#endif // RUN_ONLY