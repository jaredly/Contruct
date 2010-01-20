// Runtime.cpp - Runtime implementation of object
//

// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Runtime functions
//////////////////////////////////////////////////////////////////////////////////
#ifdef RUN_ONLY

// ExtObject constructor:
// Called when Construct creates an instance of your object.  Use your editTime data
// to initialise the object.
ExtObject::ExtObject(initialObject* editObject, VRuntime* pVRuntime)
{
	pRuntime = pVRuntime;


	// Load settings
	bin ar;
	ar.attach(editObject->eData, editObject->eSize);

	int version;
	ar >> version;

	limb.Serialize(ar);

	// Update our allLimbs list
	allLimbs.clear();
	limb.UpdateAllLimbs(allLimbs);

	int size;
	ar >> size;
	frame.clear();
	for(int f = 0; f < size; f++)
	{
		int index;
		ar >> index;
		frame[index].Serialize(ar, allLimbs);
	}

	ar >> size;
	anim.clear();
	for(int a = 0; a < size; a++)
	{
		int index;
		ar >> index;
		anim[index].Serialize(ar, frame);
	}

	ar.detach();


	keyframe = 0;
	animation = 1;
	delaytillnextframe = 1;
	playing = true;
	pingpong = false;
	
	repeatcount = 0;
	delaytillnextframe = 0;

	map<int, KeyAnimation>::iterator i = anim.find(animation);
	if(i != anim.end())
	{
		KeyAnimation* pAnim = &i->second;
		repeatcount = pAnim->animRepeat ? -1 : pAnim->animRepeatNumber;
	}

	 scale = 1;
	 prev_scale = 1;

	 flip = false;
	 mirror = false;
	 prev_flip = false;
	 prev_mirror = false;
	 angle = 0;
}

// This is called just after the constructor, when Construct has added details to your
// object such as siblings and type pointer.  If you need this data on startup, it is
// valid to read here.
void ExtObject::OnCreate()
{
}

// Destructor: called when an instance of your object is destroyed.
ExtObject::~ExtObject()
{
	//
}

// Called every frame for you to update your object if necessary
// Return values: 1 (do not call RTOnFrame again), 0 (continue calling)

// Change actual co-ordinates and cause rotation in OnFrame since these must both be
// overridable in the following events
BOOL ExtObject::OnFrame()
{
	limb.Update(pLink, true, this);
	
	 prev_scale = scale;
	 prev_flip = flip;
	 prev_mirror = mirror;



	double olddelaytillnextframe = delaytillnextframe;
	double td = pRuntime->GetTimeDelta();
	delaytillnextframe -= td;

	map<int, KeyAnimation>::iterator ipAnim = anim.find(animation);
	if(ipAnim != anim.end())
	{
		KeyAnimation* pAnim = &ipAnim->second;
		while(delaytillnextframe < 0)
		{
			Clamp<int>(keyframe, 0, pAnim->frames.size()-1);
			KeyFrame* pFrame = pAnim->frames[keyframe];
			limb.Tick(pFrame, 0);

			// Advance a frame
			if(pingpong) keyframe --;
			if(!pingpong) keyframe ++;
			if(keyframe < 0){
				keyframe = 1;
				pingpong = false;
			}
			else if(keyframe >= pAnim->frames.size())
			{
				if(repeatcount){
					if(pAnim->animPingPong)
					{
						 keyframe = pAnim->frames.size()-2;
						 pingpong = true;
					}
					else
					{
						keyframe = 0;
						pingpong = false;
					}
					repeatcount --;
				}
				else{
					keyframe = pAnim->frames.size() - 1;
					pRuntime->GenerateEvent(info.oid, 0, this);
				}
				
			 }

			if(pAnim->frames.size() == 1)
				keyframe = 0;
			
			double timeshift = pFrame->speed / pAnim->animSpeed;
			olddelaytillnextframe = 0;

			olddelaytillnextframe += timeshift;
			delaytillnextframe    += timeshift;

		}
		Clamp<int>(keyframe, 0, pAnim->frames.size()-1);
		KeyFrame* pFrame = pAnim->frames[keyframe];
		double ratio;
		if(olddelaytillnextframe != 0)
			ratio = delaytillnextframe / olddelaytillnextframe;
		else
			ratio = 0;
		limb.Tick(pFrame, ratio);
		
	}





	return 0;
}

BOOL ExtObject::OnFrame2()
{
	limb.Update(pLink, false, this);

	return 0;
}

// Draw: Construct calls this when it wants you to draw your object.  You can
// leave it blank if your extension doesn't draw anything.
void ExtObject::Draw()
{
	//
}

// WindowProc:  called when a window message, or WM_COMMAND specifying your window,
// is sent to Construct.  You must call RegisterWindow() before Construct will send
// you messages for the given window.
LRESULT ExtObject::WindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	return -2;
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

void ExtObject::Serialize(bin& ar)
{
	// Implement saving/loading of your object

}

long ExtObject::GetData(int id, void* param)
{
	switch (id) {
	// If your extension uses a window, you can return it here
	case GET_WINDOW:
		break;
	}

	return 0;
}

long ExtObject::CallFunction(int id, void* param)
{
	return 0;
}

// Return the Default Value.  This is the value to be returned if your object name
// is used without an expression name, eg. in "Counter + 1".
// Parameters can be passed, eg. MyArray(3,7).
long ExtObject::ReturnDefaultValue(LPVAL theParams, ExpReturn& ret)
{
	return ret = 0;
//	ReturnInt(0);
}

// Called for undefined expression names, if your extension allows it.
// Eg: MyObject.UndefinedExpression(3,4,5) calls this routine with
// "UndefinedExpression" as the expression name.
long ExtObject::ReturnUndefinedExpression(CString& expName, LPVAL theParams, ExpReturn& ret)
{
	return ret = 0;
	//ReturnInt(0);
}

#else //ifdef RUN_ONLY

CRunObject* WINAPI RTCreateObject(bin& ar, VRuntime* pRuntime) 
{
	return NULL;
}

void WINAPI RTDestroyObject(ExtObject* object)
{
	//
}

#endif