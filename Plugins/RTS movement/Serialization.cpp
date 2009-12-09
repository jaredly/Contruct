// Include StdAfx.h
#include "StdAfx.h"

#ifdef RUN_ONLY

/////////////////////////////
// RUNTIME serialization
void ExtObject::Serialize(bin& ar)
{
	if (ar.loading) {
		
		int waypointCount;
		int i;

		ar >> speed >> mAngle >> params.av >> params.acc >> params.dec >> params.maxspeed >> moving >> targetX >> targetY >> quickStop
			>> params.pathfinding >> params.blockedspeed >> activated >> params.rotate >> waypointCount;

		waypoints.clear();

		for (i = 0; i < waypointCount; i++) {
			CPoint pt;
			ar >> pt.x >> pt.y;
			waypoints.push_back(pt);
		}

		pfCollideTypes.resize(0);
		int count;
		ar >> count;
		for (i = 0; i < count; i++) {
			CRunObjType* pType;
			pRuntime->SerializeObjectType(pType, ar);
			pfCollideTypes.push_back(pType);
		}

		typesAvoid.resize(0);

		// Load types to avoid
		ar >> count;

		for (i = 0; i < count; i++) {
			CRunObjType* pType;
			pRuntime->SerializeObjectType(pType, ar);
			typesAvoid.push_back(pType);
		}

		if (activated)
			pRuntime->CallOnFrame(this);
			
	}
	else {
		
		ar << speed << mAngle << params.av << params.acc << params.dec << params.maxspeed << moving << targetX << targetY << quickStop
			<< params.pathfinding << params.blockedspeed << activated << params.rotate << (int)(waypoints.size());

		list<CPoint>::iterator i = waypoints.begin();
		for ( ; i != waypoints.end(); i++)
			ar << i->x << i->y;

		// Navigation types
		ar << pfCollideTypes.size();

		ObjTypeIterator t = pfCollideTypes.begin();

		for ( ; t != pfCollideTypes.end(); t++)
			pRuntime->SerializeObjectType(*t, ar);

		// Save types to avoid
		ar << typesAvoid.size();

		t = typesAvoid.begin();
		for ( ; t != typesAvoid.end(); t++)
			pRuntime->SerializeObjectType(*t, ar);
			
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
	params.Serialize(ar);
}

#endif // RUN_ONLY