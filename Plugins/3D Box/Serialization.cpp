// Include StdAfx.h
#include "StdAfx.h"

#ifdef RUN_ONLY

/////////////////////////////
// RUNTIME serialization
void ExtObject::Serialize(bin& ar)
{
	// TODO
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
void EditExt::Serialize(bin& ar)
{
	int Version = 3;
	SerializeVersion(ar, Version);

	if (ar.loading) {

		ar >> height >> z >> pInfo->objectAngle;

		for (int i = 0; i < 6; i++)
			pEditTime->SerializeImage(imageHandles[i], ar);

		if (Version >= 2)
			ar >> zbuffer;

		// V3: convert old Z values (0-8) to new Z values (0-100)
		if (Version < 3) {
			z *= (100.0 / 8.0);
			height *= (100.0 / 8.0);
		}
	}
	else {

		ar << height << z << pInfo->objectAngle;
		
		for (int i = 0; i < 6; i++)
			pEditTime->SerializeImage(imageHandles[i], ar);

		ar << zbuffer;

	}
}

#endif // RUN_ONLY