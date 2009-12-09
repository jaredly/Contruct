// Include StdAfx.h
#include "StdAfx.h"

#ifdef RUN_ONLY

/////////////////////////////
// RUNTIME serialization

void DistortInfo::Serialize(bin& ar)
{
	int Version = 2;
	SerializeVersion(ar, Version);

	if (ar.loading) {
		ar >> x >> y >> u >> v >> filter >> z;
	}
	else {
		ar << x << y << u << v << filter << z;
	}
}

void ExtObject::Serialize(bin& ar)
{
	int Version = 3;
	SerializeVersion(ar, Version);

	if (ar.loading) {
		ar >> info >> privateVars >> flashTime >> flashCounter >> flashInterval
			>> autoMirror >> autoFlip >> autoRotations;
		ar >> vertexcolors[0] >> vertexcolors[1] >> vertexcolors[2] >> vertexcolors[3];
		ar >> m_Animation;
		collisionMemory.clear();

		if(Version >= 2)
			ar >> m_AnimRoot;
		m_Animation.pAnim = pRuntime->GetAnimationPtr(m_Animation.handle);

		if (Version >= 3) {
			int ci, cj;
			ar >> ci >> cj;

			distort.resize(ci);

			for (int i = 0; i < ci; ++i) {

				distort[i].resize(cj);

				for (int j = 0; j < cj; ++j) {
					distort[i][j].Serialize(ar);
				}
			}
		}
	}
	else {
		ar << info << privateVars << flashTime << flashCounter << flashInterval
			<< autoMirror << autoFlip << autoRotations;
		ar << vertexcolors[0] << vertexcolors[1] << vertexcolors[2] << vertexcolors[3];
		ar << m_Animation;
		ar << m_AnimRoot;

		// Save distort state
		ar << distort.size();				// first dimension

		if (distort.empty())
			ar << 0;
		else
			ar << distort.front().size();	// second dimension

		for (int i = 0; i < distort.size(); ++i) {
			for (int j = 0; j < distort[i].size(); ++j) {
				distort[i][j].Serialize(ar);
			}
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
	int Version = 5;
	SerializeVersion(ar, Version);

	if (ar.loading) {

		// Serialize D3DCOLOR, but process with COLORREF and OPACITY values at edittime.
		//D3DCOLOR f;
		if(Version < 2)
		{
			ar >> pInfo->objectAngle;
			ar >> pInfo->filter;
		}
		ar >> collMode;
		ar >> autoMirror;
		ar >> autoFlip;
		ar >> autoRotations;
		ar >> autoRotationsCombo;
		ar >> hideAtStart;
		pEditTime->SerializeAnimation(m_iAnimationRoot,ar);

		if (Version >= 3)
			ar >> skewX >> skewY;

		if (Version >= 4)
			ar >> locked_animation_angles;

		if (Version >= 5)
		{
			ar >> startAnim;
			ar >> startFrame;
		}

	}
	else {
		// Code to save your object here
		ar << collMode;
		ar << autoMirror;
		ar << autoFlip;
		ar << autoRotations;
		ar << autoRotationsCombo;
		ar << hideAtStart;
		pEditTime->SerializeAnimation(m_iAnimationRoot,ar);
		ar << skewX << skewY;

		ar << locked_animation_angles;
		ar << startAnim;
		ar << startFrame;

	}
}

#endif // RUN_ONLY