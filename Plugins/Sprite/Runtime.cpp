// Runtime.cpp - Runtime implementation of object
//

// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Runtime functions
//////////////////////////////////////////////////////////////////////////////////
#ifdef RUN_ONLY

void UpdateAngleCallback(CRunObject* obj)
{
	((ExtObject*)obj)->UpdateCoords();
}

// ExtObject constructor:
// Only use for class initializer list.  Object initialisation must be done in OnCreate.
ExtObject::ExtObject(initialObject* editObject, VRuntime* pVRuntime)
:	renderer(pVRuntime->pRenderer)
{
	pRuntime = pVRuntime;
	info.editObject = editObject;
}

CRunAnimation* FindAnAnimation(VRuntime* pRuntime, CRunAnimation* pParent, CString animName)
{
	int size = pRuntime->GetSubAnimationCount(pParent);

	for(int i = 0; i < size; i++)
	{
		CRunAnimation* pAnim = pRuntime->GetSubAnimation(pParent, i);

		if (pAnim == NULL)
			continue;

		if(animName == "" || animName.CompareNoCase(pAnim->name) == 0)
		{
			int angles = pRuntime->GetSubAngleAnimationCount(pAnim);
			for(int a = 0; a < angles; a ++)
			{
				CRunAnimation* pAngle = pRuntime->GetSubAngleAnimation(pAnim, a);
				if(pRuntime->GetAnimationFrameCount(pAngle) > 0)
				{
					return pAngle;
				}
			}
		}

		pAnim = FindAnAnimation(pRuntime, pAnim, animName);

		if(pAnim)
			return pAnim;
	}
	return NULL;
}

// This is called just after the constructor.  You should perform all loading here.
void ExtObject::OnCreate()
{
	bin ar;
	ar.attach(info.editObject->eData, info.editObject->eSize);

	int Version = 0;
	ar >> Version;

	//int imgTexture;
	int collmode = 0;
	int animroot = 0;

//	ar >> info.angle;
//	ar >> info.filter;

	// Collision mode
	ar >> collmode;
	info.collMode = (CollisionMode)collmode;

	ar >> autoMirror;
	ar >> autoFlip;
	ar >> autoRotations;

	int rotationcombo;
	ar >> rotationcombo;

	if(rotationcombo == 0)
		autoRotations = -1;
	if(rotationcombo == 1)
		autoRotations = 1;
	if(rotationcombo == 2)
		if(autoRotations <= 0)
			autoRotations = 1;


	for(int a = 0; a < 4 ; a++)
		vertexcolors[a] = opaque_white;

	bool hideAtStart;
	ar >> hideAtStart;

	info.visible = !hideAtStart;

	// Animation
	ar >> animroot;
	m_AnimRoot = animroot;

	info.pInfo->skewX = info.pInfo->skewY = 0.0f;

	if (Version >= 3)
		ar >> info.pInfo->skewX >> info.pInfo->skewY;

	locked_animation_angles = true;

	if (Version >= 4)
		ar >> locked_animation_angles;

	CString animName; 
	int animFrame = 0;

	if (Version >= 5)
	{
		ar >> animName;
		ar >> animFrame;

	}


	ar.detach();

	// Use the texture and collision mask of this image handle
	info.curTexture = 0;
	info.imgHandle = 0;//imgTexture;
	info.w = info.editObject->eWidth;
	info.h = info.editObject->eHeight;

	info.x = info.editObject->eX;
	info.y = info.editObject->eY;
	info.pInfo->filter = info.editObject->eColor;
	info.angle = info.editObject->eAngle;

	info.UpdateAngle = UpdateAngleCallback;

	info.isFlipped = 0;
	info.isMirrored = 0;

	flashTime = 0;
	flashCounter = 0;
	flashInterval = 0;
	vertices_changed = false;

	// Setup private vars
	SetupPrivateVariableVector(pRuntime, this, privateVars);

	info.curAnim = &m_Animation;
	m_Animation.autoFlip = autoFlip!=0;
	m_Animation.autoMirror = autoMirror!=0;
	m_Animation.autoRotations = autoRotations;
	
	// Tempory thing so we have an animation at least :/
	pRoot = pRuntime->GetAnimationPtr(m_AnimRoot);

	// Load textures if settings require
	pRuntime->LoadAnimation(pRoot);

	// Either load up a default animation, or the one specified by animName
	CRunAnimation* pAnim = FindAnAnimation(pRuntime, pRoot, animName);
	if(!pAnim)
		pAnim = FindAnAnimation(pRuntime, pRoot, "");
	
	if(pAnim)
	{
		m_Animation.pAnim = pRuntime->SetAnimation(pAnim->handle, this);
		m_Animation.curFrame = animFrame - 1; // Normally you would use the runtime function, but we need the width/height to be unchanged
		m_Animation.playing = true;
	}
	else
	{
		m_Animation.playing = false;
		m_Animation.pAnim = 0;
	}

	// Test if we need to tick animations (if the object has 1 frame in 1 angle in 1 animation, no need)
	staticAnims = false;

	if (pRuntime->GetSubAnimationCount(pRoot) == 1) {

		CRunAnimation* onlyAnim = pRuntime->GetSubAnimation(pRoot, 0);

		if (pRuntime->GetSubAngleAnimationCount(onlyAnim) == 1) {

			CRunAnimation* onlyAngle = pRuntime->GetSubAngleAnimation(onlyAnim, 0);

			if (pRuntime->GetAnimationFrameCount(onlyAngle) == 1)
				staticAnims = true;
		}
	}
	
	pRuntime->TickAnimation(this);

	// Update bounding box
	pRuntime->UpdateBoundingBox(this);
	
}

// Destructor: called when an instance of your object is destroyed.
ExtObject::~ExtObject()
{
	// No longer using this animation
	pRuntime->ReleaseAnimation(pRoot);
}

// Called every frame for you to update your object if necessary
// Return values: 1 (do not call RTOnFrame again), 0 (continue calling)
// The frame call order goes like this:
// run events -> OnFrame -> Draw -> OnFrame2 -> run events -> ...
BOOL ExtObject::OnFrame()
{
	TickAnimRet status = pRuntime->TickAnimation(this);

	switch (status) {
	case TA_FINISHED:
		pRuntime->GenerateEvent(info.oid, 7, this);		// On Any Animation Finished
		pRuntime->GenerateEvent(info.oid, 6, this);		// On Animation Finished
		break;
	case TA_FRAMECHANGED:
	case TA_LOOPED:
	case TA_PONGED:
		pRuntime->GenerateEvent(info.oid, 13, this);
		break;
	}

	if (flashTime > 0) 
	{
		flashTime -= pRuntime->GetTimeDelta();
		flashCounter += pRuntime->GetTimeDelta();

		if (flashCounter >= flashInterval) {
			info.visible = !info.visible;
			flashCounter -= flashInterval;			
		}
		if (flashTime <= 0) {
			info.visible = true;
			pRuntime->GenerateEvent(info.oid, 14, this);	// on flash finished
		}
	}
	// Single-framed animations don't need to be ticked and improve performance
	else if (staticAnims)
		return 1;


	return 0;
}

BOOL ExtObject::OnFrame2()
{
	// Check objects held in collision map aren't destroyed
	if (!collisionMemory.empty()) {

		map<void*, set<CRunObject*> >::iterator i = collisionMemory.begin();
		map<void*, set<CRunObject*> >::iterator end = collisionMemory.end();

		for ( ; i != end; ++i) {
			
			if (!i->second.empty()) {
				
				set<CRunObject*>::iterator j = i->second.begin();
				
				for ( ; j != i->second.end(); ) {
					if ((*j)->info.destroying)
						j = i->second.erase(j);
					else
						++j;
				}
			}
		}
	}

	return 0;
}

// Draw: Construct calls this when it wants you to draw your object.  You can
// leave it blank if your extension doesn't draw anything.

void LimitAngle(float& angle)
{
	while(angle >= 360.0f)
		angle -= 360.0f;
	while(angle < 0.0f)
		angle += 360.0f;
}

void ExtObject::UpdateCoords()
{
	// If animation angle is locked to the object angle update it here.
	if (locked_animation_angles) {
		if(pRuntime->GetSubAngleAnimationCount(info.curAnim->pAnim->parent) != 1)
		{
			int anim = pRuntime->GetAnimationByNearestAngle(info.curAnim->handle, this->info.angle);
			if(info.curAnim->handle != anim)
				pRuntime->SetAnimation(anim, this);	
			pRuntime->UpdateAnimationFrame(this);
		}
	}

	float x = info.x;
	float y = info.y;
	float w = info.w * (info.isMirrored?-1:1);
	float h = info.h * (info.isFlipped?-1:1);

	int flip = 1;
	int mirror = 1;

	LimitAngle(info.angle);


	float angle = info.angle;

	if(m_Animation.autoRotations != -1)
	{
		if(m_Animation.autoRotations == 1)
			angle = 0;
		else
		{
			int tmp = angle / (float)(360.0 / m_Animation.autoRotations) + 0.5f;
			angle = tmp * (360.0/m_Animation.autoRotations);
		}
	}

	
	// If 'No rotation' used, don't allow angle to change from 0
	if (m_Animation.autoRotations != 1)
		angle -= m_Animation.pAnim->angle;	// 0 normally, other values when facing different directions

	angle += m_Animation.pAnim->angle;
	LimitAngle(angle);

	if(m_Animation.autoMirror)
	{
		if(info.angle > 90 && info.angle < 270 && m_Animation.autoRotations == 1)
		{
			mirror *= -1;
			w *= -1;
		}
		else if(angle > 90 && angle < 270)
		{
			flip *= -1;
			h *= -1;
			angle += m_Animation.pAnim->angle*2;
		}
	}

	if(m_Animation.autoFlip)
	{
		if(info.angle > 180 && info.angle < 360 && m_Animation.autoRotations == 1)
		{
			flip *= -1;
			h *= -1;	
		}
		else if(angle > 180 && angle < 360)
		{
			flip *= -1;
			h *= -1;
			angle += m_Animation.pAnim->angle*2;
		} 
	}
	angle -= m_Animation.pAnim->angle;
	LimitAngle(angle);

	info.isFlipped = (flip == -1);
	info.isMirrored = (mirror == -1);
	info.w = w;
	info.h = h;
	info.displayangle = angle;
}

// WindowProc:  called when a window message, or WM_COMMAND specifying your window,
// is sent to Construct.  You must call RegisterWindow() before Construct will send
// you messages for the given window.
LRESULT ExtObject::WindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, uMessage, wParam, lParam);
}

BOOL ExtObject::PreTranslateMessage(MSG* msg)
{
	return 0;
}

// For global objects
void ExtObject::OnFrameChange(int oldFrame, int newFrame)
{
	// Do anything your global object needs when the frame changes
	// oldFrame is -1 on start of app, newFrame is -1 on end of app
}


long ExtObject::GetData(int id, void* param)
{
	switch (id) {
	// If your extension uses a window, you can return it here
	case 1:
		return (long)pRoot;
		break;
	}

	return 0;
}

long ExtObject::CallFunction(int id, void* param)
{
	if (id == 1) // Image manipulator gives sprite a texture handle.
	{
		TextureHandle th = (TextureHandle)param;

		// Swap out the old texture for the new one
		renderer->ReplaceTexture(info.curTexture, th);

		CRunAnimationFrame* pAnimFrame = pRuntime->GetAnimationFrame(m_Animation.pAnim, m_Animation.curFrame);

		float xScale = info.w / pAnimFrame->width;
		float yScale = info.h / pAnimFrame->height;

		pAnimFrame->width = info.curTexture->image_widthf;
		pAnimFrame->height = info.curTexture->image_heightf;
		info.w = xScale * info.curTexture->image_widthf;
		info.h = yScale * info.curTexture->image_heightf;
		pRuntime->UpdateBoundingBox(this);
	}

	return 0;
}

#else //ifdef RUN_ONLY

CRunObject* WINAPI RTCreateObject(bin& ar, VRuntime* pRuntime) 
{
	return NULL;
}

void WINAPI RTDestroyObject(ExtObject* object)
{
}

#endif