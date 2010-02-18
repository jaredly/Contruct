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

		if(Version == 1)
		{
			for(int i = 0; i < CONTROLLERS; i++)
				for(int j = 0; j < BUTTONS; j++) //26 was the old number
				{
					if(j == 14)
					{
						Control blah;
						ar >> blah;
						ar >> blah;
					}
					ar >> controls[i][j];
				}
		}
		else
		{
			for(int i = 0; i < CONTROLLERS; i++)
				for(int j = 0; j < BUTTONS; j++)
				{
					ar >> controls[i][j];
				}
		}
		ar >> controllerCombo ;
	}
	else {
		for(int i = 0; i < CONTROLLERS; i++)
			for(int j = 0; j < BUTTONS; j++)
				ar << controls[i][j];

		ar << controllerCombo ;
		
	}
			
}

#endif // RUN_ONLY