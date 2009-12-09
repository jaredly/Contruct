// Include StdAfx.h
#include "StdAfx.h"

#ifdef RUN_ONLY

/////////////////////////////
// RUNTIME serialization
void ExtObject::Serialize(bin& ar)
{
	int Version = 0;
	SerializeVersion(ar, Version);

	if (ar.loading) {
		//ar >> info >> privateVars 
		//	>> cr_colors.c1 >> cr_colors.c2 >> cr_colors.fill
		//	>> transparent >> padding;
		//pRuntime->SerializeObjectPtr(objAttach, ar);
	}
	else {
		//ar << info << privateVars 
		//	<< cr_colors.c1 << cr_colors.c2 << cr_colors.fill
		//	<< transparent << padding;
		//pRuntime->SerializeObjectPtr(objAttach, ar);

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

	if (ar.loading) 
	{
		pEditTime->SerializeImage(imgTexture, ar);

		ar >> vertex[0] >> vertex[1] >> vertex[2] >> vertex[3];
		ar >> vertex_opacity[0] >> vertex_opacity[1] >> vertex_opacity[2] >> vertex_opacity[3];
		ar >> image_left_margin	>> image_right_margin >> image_top_margin >> image_bottom_margin;

		ar >> hotspot_pos;
	}
	else 
	{
		pEditTime->SerializeImage(imgTexture, ar);

		ar << vertex[0] << vertex[1] << vertex[2] << vertex[3];
		ar << vertex_opacity[0] << vertex_opacity[1] << vertex_opacity[2] << vertex_opacity[3];
		ar << image_left_margin	<< image_right_margin << image_top_margin << image_bottom_margin;

		ar << hotspot_pos;
	}
}

#endif // RUN_ONLY