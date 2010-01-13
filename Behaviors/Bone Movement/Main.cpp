// Main.cpp - Main implementation of object
//

// Include StdAfx
#include "StdAfx.h"

// If run only
#ifdef RUN_ONLY


// Conditions	
long ExtObject::eGetAnimFrame(LPVAL params, ExpReturn& ret)
{
	return ret = keyframe+1;
}

long ExtObject::eGetAnimName(LPVAL params, ExpReturn& ret)
{
	map<int, KeyAnimation>::iterator i = anim.find(animation);
	if(i != anim.end())
	{
		return ret.ReturnString(pRuntime, i->second.name);
	}
	return NULL;
}

// Actions


long ExtObject::aSetAnimFrame(LPVAL params)
{
	delaytillnextframe = params[0].GetFloat() / 1000.0;
	playing = true;
	pingpong = false;
	repeatcount = 0;
	keyframe = params[1].GetInt()-1;
	
	return 0;
}

long ExtObject::aPlayAnimation(LPVAL params)
{
	playing = true;

	return 0;
}

long ExtObject::aPauseAnimation(LPVAL params)
{
	playing = false;

	return 0;
}

long ExtObject::aSetAnimationSpeed(LPVAL params)
{
	map<int, KeyAnimation>::iterator i = anim.find(animation);
	if(i != anim.end())
	{
		i->second.animSpeed = params[0].GetFloat();
	}
	return 0;
}
long ExtObject::aSetAnimation(LPVAL params)
{
	int lastanim = animation;
	CString animname = params[1].GetString();
	map<int, KeyAnimation>::iterator i = anim.begin();
	KeyAnimation* pAnim = NULL;
	for( ; i!= anim.end(); i++){
		if(i->second.name.CompareNoCase(animname) == 0){
			animation = i->first;
			pAnim = &i->second;
		}
	}
	if(pAnim)
	{
		if(lastanim == animation)
			return 0; // pff, same animation :P

		keyframe = 0;
		delaytillnextframe = params[0].GetFloat() / 1000.0;

		pingpong = pAnim->animPingPong;
		repeatcount = pAnim->animRepeat ? -1 : pAnim->animRepeatNumber;
	}
	return 0;
}

long ExtObject::aSetScale(LPVAL params)
{
	scale = params[0].GetFloat();
	return 0;
}
long ExtObject::aSetFlipped(LPVAL params)
{
	flip = params[0].GetFloat() == 0? true : false;
	return 0;
}
long ExtObject::aSetMirrored(LPVAL params)
{
	mirror = params[0].GetFloat() == 0? true : false;
	return 0;
}
long ExtObject::aSetAngle(LPVAL params)
{
	angle = params[0].GetFloat();
	return 0;
}

long ExtObject::aForceBoneRepositioning(LPVAL params)
{
	limb.Update(pLink, false, this);
	return 0;
}














long ExtObject::cOnAnimationFinish(LPVAL params)
{
	return cAnimationPlaying(params);
}

long ExtObject::cAnimationPlaying(LPVAL params)
{
	map<int, KeyAnimation>::iterator i = anim.find(animation);
	if(i != anim.end())
	{
		CString l = params[0].GetString();
		CString r = i->second.name;
		l.MakeLower();
		r.MakeLower();
		return l == r;
	}

	return false;
}

long ExtObject::cCompareAnimFrame(LPVAL params)
{
	return DoComparison(params[0].GetInt(), keyframe, params[1].GetInt() - 1);
}






#endif // #ifdef RUN_ONLY

//////////////////////////////////////////////////////////////////////////////////
// ACE tables
//////////////////////////////////////////////////////////////////////////////////

void DefineACES(MicroAceTime* at)
{
	// Conditions
	ADDPARAM(PARAM_STRING, "Animation name", "Enter the name of the animation.");
	ADDCND("On animation finished", "Animations", "%o Animation %0 finished", &ExtObject::cOnAnimationFinish, "AnimFinished", CND_TRIGGERED);

//	ADDCND("On any animation finished", "Animations", "%o Any animation finished", &ExtObject::cTrigger, "AnyAnimFinished", CND_TRIGGERED);

	ADDPARAM(PARAM_STRING, "Animation name", "Enter the name of the animation.");
	ADDCND("An animation is playing", "Animations", "%o Animation %0 is playing", &ExtObject::cAnimationPlaying, "AnimPlaying", 0);

	ADDPARAMCOMBO("Comparison", "Select the comparison to make.", "Equal to|Not equal to|Less than|Less or equal|Greater than|Greater or equal");
	ADDPARAMDEF(PARAM_VALUE, "Animation frame", "Animation frame to compare to", "1");
	ADDCND("Compare animation frame", "Animations", "%o Animation frame %0 %1", &ExtObject::cCompareAnimFrame, "CompareAnimFrame", 0);

	// Actions
	ADDACT("Play animation", "Animation", "Play current animation", &ExtObject::aPlayAnimation, "PlayAnim", 0);
	ADDACT("Pause animation", "Animation", "Pause current animation", &ExtObject::aPauseAnimation, "PauseAnim", 0);

	ADDPARAM(PARAM_VALUE, "Twean time", "Transission time");
	ADDPARAM(PARAM_STRING, "Name", "Enter the name of the animation");
	ADDACT("Set animation", "Animation", "Set animation to %1 (time: %0 ms)", &ExtObject::aSetAnimation, "SetAnim", 0);
	
	ADDPARAM(PARAM_VALUE, "Twean time", "Transission time");
	ADDPARAM(PARAM_VALUE, "Frame number", "Animation frame number to change to, starting at 1.");
	ADDACT("Set animation frame", "Animation", "Set animation frame to %0", &ExtObject::aSetAnimFrame, "SetAnimFrame", 0);

	ADDPARAM(PARAM_VALUE, "Speed", "Enter the speed of the animation (frames per second)");
	ADDACT("Set animation speed", "Animation", "Set animation speed to %0", &ExtObject::aSetAnimationSpeed, "SetAnimSpeed", 0);


	ADDPARAM(PARAM_VALUE, "Scale", "Enter the scale of the bone strucutre");
	ADDACT("Set scale", "Size & Angle", "Set scale to %0", &ExtObject::aSetScale, "SetScale", 0);

	ADDPARAMCOMBO("Mirror", "Select if the bone structure should be mirrored", "Yes|No");
	ADDACT("Set mirror", "Size & Angle", "Set mirrored? - %0", &ExtObject::aSetMirrored, "SetMirrored", 0);

	ADDPARAMCOMBO("Flipped", "Select if the bone structure should be flipped", "Yes|No");
	ADDACT("Set flip", "Size & Angle", "Set flipped? - %0", &ExtObject::aSetFlipped, "SetFlipped", 0);

	ADDPARAM(PARAM_VALUE, "Angle", "Enter the angle offset in degrees");
	ADDACT("Set angle", "Size & Angle", "Set angle to %0", &ExtObject::aSetAngle, "SetHeightScale", 0);

	ADDACT ("Force bone repositioning", "Bone", "Force bone repositioning", &ExtObject::aForceBoneRepositioning, "ForceBoneRepositioning", 0);



	// Expressions	
	ADDEXP("Get animation frame", "Animation", "AnimFrame", &ExtObject::eGetAnimFrame, RETURN_VALUE);
	ADDEXP("Get animation name", "Animation", "AnimName", &ExtObject::eGetAnimName, RETURN_STRING);



}

#define LAZY(x) if (ar.loading){ar >> x;} else{ar << x;}
void LimbData::Serialize(bin& ar)
{
	int Version = 1;
	SerializeVersion(ar, Version);

	LAZY(cx); //origin from hotspot to pivot (for child)
	LAZY(cy); 
	
	LAZY(px); //origin from hotspot to pivot (for parent)
	LAZY(py);
	
	LAZY(w); //width scale
	LAZY(h);

	LAZY(a);

	LAZY(opacity);
	LAZY(filter);
}

int GetLimbIndex(Limb* pLimb, vector<Limb*>& allLimbs)
{
	int index = 0;
	for(vector<Limb*>::iterator i = allLimbs.begin(); i!= allLimbs.end(); i++)
	{
		if(*i == pLimb)
			return index;
		index++;
	}
	return -1;
}

int GetFrameIndex(KeyFrame* pFrame, map<int, KeyFrame>& allframes)
{
	for(map<int, KeyFrame>::iterator i = allframes.begin(); i!= allframes.end(); i++)
	{
		if(&i->second == pFrame)
			return i->first;
	}
	return -1;
}

void KeyAnimation::RemoveAllFramesThatUseKeyframe(KeyFrame* pFrame)
{
	for(int i = 0; i < frames.size(); i++)
	{
		if(frames[i] == pFrame)
		{
			frames.erase(frames.begin() + i);
			i--;
		}
	}
}

void KeyAnimation::Serialize(bin& ar, map<int, KeyFrame>& allframes)
{
	int Version = 1;
	SerializeVersion(ar, Version);

	if (ar.loading)
	{
		int size;
		ar >> size;

		for(int i = 0; i < size; i++)
		{
			int index;
			ar >> index;

			map<int, KeyFrame>::iterator found = allframes.find(index);
			if(found != allframes.end())
			{
				frames.push_back(&found->second);
			}
			else
			{
				// Error detected
				CString error; 
				error.Format("Frame %d in the animation %s could not be found, ignoring it", index, name);
				MessageBox(0, error, "Bone Movement",0);
			}
			index ++;
		}

		ar >> name >> animSpeed >> animRepeat >> animRepeatNumber >> animRepeatTo >> animPingPong;
	}
	else
	{
		ar << (int)frames.size();
		int index = 0;
		for(vector<KeyFrame*>::iterator i = frames.begin(); i != frames.end(); i++)
		{
			int found = GetFrameIndex(*i, allframes);
			if(found != -1)
			{
				ar << found;
			}
			else
			{
				// Error detected
				CString error; 
				error.Format("Frame %d in the animation %s could not be found: writing 0", index, name);
				MessageBox(0, error, "Bone Movement",0);
				ar << (int)0;
			}
			index ++;
		}

		ar << name << animSpeed << animRepeat << animRepeatNumber << animRepeatTo << animPingPong;
	}




}

void KeyFrame::Serialize(bin& ar, vector<Limb*>& allLimbs)
{
	int Version = 1;
	SerializeVersion(ar, Version);

	if (ar.loading)
	{
		int size;
		ar >> size;

		for(int i = 0; i < size; i++)
		{
			int index;
			ar >> index;

			if(index >= 0 && index < allLimbs.size())
			{
				limb[allLimbs[index] ].Serialize(ar);
			}
			else
			{
				MessageBox(0, "Limb could not be found, information in frame skipped", "Bone Movement",0);
			}
		}

		ar >> name >> speed;
	}
	else
	{
		ar << (int)limb.size();
		int index = 0;
		for(map< Limb*, LimbData>::iterator i = limb.begin(); i != limb.end(); i++)
		{
			int found = GetLimbIndex(i->first, allLimbs);
			if(found != -1)
			{
				ar << found;
				i->second.Serialize(ar);
			}
			else
			{
				// Error detected
				MessageBox(0, "Limb in frame could not be found", "Bone Movement",0);
				ar << (int)0;
				LimbData blank;
				blank.Serialize(ar);
			}
			index ++;
		}
		ar << name << speed;
	}
}

Limb* Limb::LookForParent(Limb* child)
{
	for(list<Limb>::iterator i = limbs.begin(); i != limbs.end(); i++)
	{	
		if(&*i == child)
			return this;
			
		Limb* ret = i->LookForParent(child);
		if(ret)
			return ret;
	}
	return false;
}

void Limb::SetBonesToFrame(KeyFrame* pFrame)
{
	map< Limb*, LimbData>::iterator i = pFrame->limb.find(this);
	if(i != pFrame->limb.end())
	{
		a = i->second.a;
		cx = i->second.cx;
		cy = i->second.cy;
		px = i->second.px;
		py = i->second.py;
		w = i->second.w;
		h = i->second.h;
		filter = i->second.filter;
		opacity = i->second.opacity;
	}
	for(list<Limb>::iterator i = limbs.begin(); i != limbs.end(); i++)
	{
		i->SetBonesToFrame(pFrame);
	}
}

void Limb::SetFrameToBones(KeyFrame* pFrame)
{
	if(!pFrame)
		return;
	pFrame->limb[this]; //add element if it doesn't exist
	map< Limb*, LimbData>::iterator i = pFrame->limb.find(this);
	if(i != pFrame->limb.end())
	{
		i->second.a = a;
		i->second.cx = cx;
		i->second.cy = cy;
		i->second.px = px;
		i->second.py = py;
		i->second.w = w;
		i->second.h = h;
		i->second.a = a;
		i->second.filter = filter;
		i->second.opacity = opacity;
	}
	for(list<Limb>::iterator i = limbs.begin(); i != limbs.end(); i++)
	{
		i->SetFrameToBones(pFrame);
	}
}

float angle_lerp(float start, float end, float ratio)
{
	float cs = cos(RADIANS(start));
	float ce = cos(RADIANS(end));
	float ss = sin(RADIANS(start));
	float se = sin(RADIANS(end));

	float dot = cs*ce + ss*se;
	float step = DEGREES(acos(dot))* ratio;
	float z = cs*se - ss*ce;
	float angle = DEGREES(acos(dot));

	if(angle > step)
	if(z > 0)
	return start + step;
	else
	return start - step;
	else
	return end;
}

void Limb::Tick(KeyFrame* pFrame, double ratio)
{

	map< Limb*, LimbData>::iterator i = pFrame->limb.find(this);
	if(i != pFrame->limb.end())
	{
		a = angle_lerp(i->second.a, a, ratio);


		cx = lerp(i->second.cx, cx, ratio);
		cy = lerp(i->second.cy, cy, ratio);
		px = lerp(i->second.px, px, ratio);
		py = lerp(i->second.py, py, ratio);
		w = lerp(i->second.w, w, ratio);
		h = lerp(i->second.h,h, ratio);
		//filter = i->second.filter;
		opacity = lerp(i->second.opacity, opacity, ratio);
	}
	for(list<Limb>::iterator i = limbs.begin(); i != limbs.end(); i++)
	{
		i->Tick(pFrame, ratio);
	}
}


void Limb::Serialize(bin& ar)
{
	int Version = 1;
	SerializeVersion(ar, Version);

	if (ar.loading)
	{
		ar >> cx; 
		ar >> cy; 
		
		ar >> px; 
		ar >> py;
		
		ar >> w; 
		ar >> h;
		ar >> filter;
		ar >> opacity;

		ar >>  a;

		ar >>  pivot_pixelx;
		ar >>  pivot_pixely;

		ar >>  instanceID;
		ar >>  ObjectIdentifier;
		ar >>  globalID;
		ar >>  object_name;

		ar >> save;

		int count = limbs.size();
		ar >> count;
		for(int i = 0; i < count; i++)
		{
			limbs.push_back(Limb());
			limbs.back().Serialize(ar);
		}
	}
	else
	{
		ar << cx; 
		ar << cy; 
		
		ar << px; 
		ar << py;
		
		ar << w;
		ar << h;
		ar << filter;
		ar << opacity;

		ar <<  a;


		ar <<  pivot_pixelx;
		ar <<  pivot_pixely;

		ar <<  instanceID;
		ar <<  ObjectIdentifier;
		ar <<  globalID;
		ar <<  object_name;
		ar << save;

		int count = limbs.size();
		ar << count;
		for(list<Limb>::iterator i = limbs.begin(); i != limbs.end(); i++)
		{
			i->Serialize(ar);
		}
	}
}

void Limb::UpdateAllLimbs(vector<Limb*>& allLimbs)
{
	allLimbs.push_back(this);

	for(list<Limb>::iterator i = limbs.begin(); i != limbs.end(); i++)
		i->UpdateAllLimbs(allLimbs);

}





#ifdef RUN_ONLY

void Limb::Update(CRunObject* pParent, bool beforeevents, ExtObject* behavior)
{
	// Todo: Better picking of limb. What if the user deletes this
	// particular instance...instead use object id's and such

	CRunObject* pChild = object;
	if(pChild == NULL){
		CRunObjType* pType = pParent->pRuntime->GetTypeFromName(object_name);
		pChild = pParent->pRuntime->GetPairedObject(pType, pParent);
	}
	if(pChild)
	{
		double x;
		double y;

		double cospa = cos(Rad(pParent->info.displayangle));
		double sinpa = sin(Rad(pParent->info.displayangle));
		double cosca = cos(Rad(pChild->info.displayangle));
		double sinca = sin(Rad(pChild->info.displayangle));

		x = pParent->info.x;
		y = pParent->info.y;
		x += cospa * pParent->info.w * px - sinpa * pParent->info.h * py;
		y += sinpa * pParent->info.w * px + cospa * pParent->info.h * py;

		pivot_pixelx = x;
		pivot_pixely = y;

		x -= cosca * pChild->info.w * cx - sinca * pChild->info.h * cy;
		y -= sinca * pChild->info.w * cx + cosca * pChild->info.h * cy;

		if(beforeevents)
		{

			pChild->info.w *= w;
			pChild->info.h *= h;

			if(behavior->mirror != behavior->prev_mirror)
				pChild->info.w *= -1;

			if(behavior->flip != behavior->prev_flip)
				pChild->info.h *= -1;

			if(behavior->prev_scale != 0)
			{
				pChild->info.w *= behavior->scale / behavior->prev_scale;
				pChild->info.h *= behavior->scale / behavior->prev_scale;
			}

			pChild->info.angle = a + behavior->angle;

			if(behavior->mirror)
				pChild->info.angle = 360 - pChild->info.angle;
			if(behavior->flip)
				pChild->info.angle = 360 - pChild->info.angle;

			pChild->info.x = x;
			pChild->info.y = y;
			
		}
		else
		{
			pChild->info.x = x;
			pChild->info.y = y;
		}
		pChild->UpdateBoundingBox();
		//TODO: change pChilds filter to be multiplied by filter and opacity

		for(list<Limb>::iterator i = limbs.begin(); i != limbs.end(); i++)
		{
			i->Update(pChild, beforeevents, behavior);
		}
	}
}


#else

void Limb::RemoveLimb(Limb* pRemove)
{
	for(list<Limb>::iterator i = limbs.begin(); i != limbs.end(); i++)
	{
		if(&*i == pRemove)
		{
			limbs.erase(i);
			return;
		}
		else
			i->RemoveLimb(pRemove);
	}
}


void Limb::Update(editInfo* pParent, EditExt* ext)
{
	// Todo: Better picking of limb. What if the user deletes this
	// particular instance...instead use object id's and such

	editInfo* pChild = ext->pEditTime->FindObjectByAnID(instanceID, ObjectIdentifier);
	object_name = ext->pEditTime->GetName(ObjectIdentifier);
	if(pChild)
	{
		ext->m_Backup.push_back(ObjectBackup(pChild));

		double x;
		double y;

		double cospa, sinpa, cosca, sinca;
		cr::sincosf(Rad(pParent->objectAngle), &sinpa, &cospa );
		cr::sincosf(Rad(a), &sinca, & cosca);


		x = pParent->objectX;
		y = pParent->objectY;
		x += cospa * pParent->objectWidth * px - sinpa * pParent->objectHeight * py;
		y += sinpa * pParent->objectWidth * px + cospa * pParent->objectHeight * py;

		pivot_pixelx = x * ext->pEditTime->GetZoom();
		pivot_pixely = y * ext->pEditTime->GetZoom();

		x -= cosca * pChild->objectWidth * cx - sinca * pChild->objectHeight * cy;
		y -= sinca * pChild->objectWidth * cx + cosca * pChild->objectHeight * cy;

		pChild->objectX = x;
		pChild->objectY = y;
		pChild->objectWidth *= w;
		pChild->objectHeight *= h;
		pChild->objectAngle = a;

		if(mouseover && !(GetKeyState(VK_RBUTTON) >> 2))
		{
			pChild->filter = 0xff4444ff;
		}
		//TODO: change pChilds filter to be multiplied by filter and opacity

		for(list<Limb>::iterator i = limbs.begin(); i != limbs.end(); i++)
		{
			i->Update(pChild, ext);
		}

	}
}
#endif

