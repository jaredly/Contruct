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
		ar >> info >> privateVars 
			>> cr_colors.c1 >> cr_colors.c2 >> cr_colors.fill
			>> transparent >> padding;
		pRuntime->SerializeObjectPtr(objAttach, ar);
	}
	else {
		ar << info << privateVars 
			<< cr_colors.c1 << cr_colors.c2 << cr_colors.fill
			<< transparent << padding;
		pRuntime->SerializeObjectPtr(objAttach, ar);

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
	int Version = 4;
	SerializeVersion(ar, Version);

	if (ar.loading) {

		ar >> c1 >> c2 >> fill >> sysc1 >> sysc2 >> sysfill >> transparent;
		if(Version < 2)
		{
			float opacity; 
			COLORREF filter = 0xffffff;
			ar >> opacity;
			pInfo->filter = D3D_OPACITYCOLORREF((int)(opacity * 255.0), filter);
		} 		
		ar >> c1_opacityf >> c2_opacityf >> fill_opacityf >> pInfo->objectAngle;

		if (Version >= 3)
			ar >> smoothLines;
		if (Version >= 4)
			ar >> hotspot_pos;
	}
	else {
		ar << c1 << c2 << fill << sysc1 << sysc2 << sysfill << transparent;
		ar << c1_opacityf << c2_opacityf << fill_opacityf  << pInfo->objectAngle;
		ar << smoothLines << hotspot_pos;
	}
}

#endif // RUN_ONLY