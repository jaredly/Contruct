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
		ar >> info >> privateVars;

		pRuntime->SerializeObjectPtr(objAttach, ar);

		ar >> padding; // Padding for attached object
		int temp;
		ar >> temp;
		hotspot_pos = (hotspot_position)temp;

		ar >> hs_xf >> hs_yf >> oldw >> oldh;

		ar >> vertex_filter[0] >> vertex_filter[1] >> vertex_filter[2] >> vertex_filter[3];
		ar >> image_left_margin >> image_right_margin >> image_top_margin >> image_bottom_margin;

		//TextureHandle iTexture; - Needs to be a way to serialize texture :(
	}
	else {
		ar << info << privateVars;

		pRuntime->SerializeObjectPtr(objAttach, ar);

		ar << padding;				// Padding for attached object
		ar << (int)hotspot_pos;
		ar << hs_xf << hs_yf << oldw << oldh;

		ar << vertex_filter[0] << vertex_filter[1] << vertex_filter[2] << vertex_filter[3];
		ar << image_left_margin << image_right_margin << image_top_margin << image_bottom_margin;

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