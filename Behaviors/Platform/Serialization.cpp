// Include StdAfx.h
#include "StdAfx.h"

#ifdef RUN_ONLY

/////////////////////////////
// RUNTIME serialization
void ExtObject::Serialize(bin& ar)
{
	int version = 2;
	if (ar.loading) {
		ar >> version;
		ar >> grav_fall >> jump_strength >> floor_acc >> floor_dec >> max_floor_speed >> dx >> dy
			>> ignoringInput >> activated >> grav_dir >> jump_sustain >> lastjump;

		grav_jump = grav_fall;
		grav_jump_sustain = 0;

		if(version >= 2){
			ar >> grav_jump;
			ar >> grav_jump_sustain;
		}
		if (activated)
			pRuntime->CallOnFrame(this);
	}
	else {
		ar << version;
		ar << grav_fall << jump_strength << floor_acc << floor_dec << max_floor_speed << dx << dy
			<< ignoringInput << activated << grav_dir << jump_sustain << lastjump;
		ar << grav_jump << grav_jump_sustain;
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
	int Version = 5;
	SerializeVersion(ar, Version);

	if (ar.loading)
	{
		
		// Code to load your object here
		if(Version == 1)
		{
			
			bool aircontrol;
			ar >> floor_acc >> floor_dec >> max_floor_speed >> jump_strength >> grav_fall >> aircontrol >> jump_sustain;
			grav_jump = grav_fall;
			grav_jump_sustain = 0;
			if(aircontrol)
			{
				air_acc = floor_acc;
				air_dec = floor_dec;
			}
			else
				air_acc = air_dec = 0;
		
			max_air_speed = max_floor_speed;
		}
		else {
			autochangeangle = false;

			ar >> floor_acc >> floor_dec >> air_acc >> air_dec >> max_floor_speed >> max_air_speed;
			ar >> max_fall  >> jump_strength >> jump_sustain;
			ar >> grav_fall;
			grav_jump = grav_fall;
			grav_jump_sustain = 0;
			
			ar >> grav_dir;
			ar >> inv;
			ar >> bunnyhop;
			if(Version >= 2)
				ar >> CanPressDown;
			if (Version >= 3)
				ar >> player;
			if (Version >= 4)
			{
				ar >> grav_jump;
				ar >> grav_jump_sustain;
			}
			if( Version >= 5)
			{
				ar >> autochangeangle;
			}
		}

	}
	else
	{
		// Code to save your object here
		//ar << acc << dec << maxspeed << jumpstrength << g << aircontrol << jumpSustain;

		ar << floor_acc << floor_dec << air_acc << air_dec << max_floor_speed << max_air_speed;
		ar << max_fall  << jump_strength << jump_sustain;
		ar << grav_fall << grav_dir;
		ar << inv;
		ar << bunnyhop;

		ar << CanPressDown;
		ar << player;

		ar << grav_jump;
		ar << grav_jump_sustain;

		ar << autochangeangle;
	}
}

#endif // RUN_ONLY