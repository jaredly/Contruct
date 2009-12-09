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

	if (ar.loading)
	{
		// Code to load your object here
		ar >> accuracy >> obstacles;

		if(Version >= 2)
			ar >> range >> angleRange;
		else
		{
			range = 0; 
			angleRange = 360;
		}
	
	}
	else
	{
		// Code to save your object here
		ar << accuracy << obstacles << range << angleRange;
	}
}

#endif // RUN_ONLY