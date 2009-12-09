// Runtime.cpp - Runtime implementation of object
//

// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Runtime functions
//////////////////////////////////////////////////////////////////////////////////
#ifdef RUN_ONLY

// ExtObject constructor:
// Only use for class initializer list.  Object initialisation must be done in OnCreate.
// It is not safe to make runtime calls here: do so in OnCreate.
ExtObject::ExtObject(initialObject* editObject, VRuntime* pVRuntime)
{
	pRuntime = pVRuntime;
	info.editObject = editObject;
}

// This is called just after the constructor when your object has been created.  Construct has set
// up your object here so it is safe to make runtime calls.
void ExtObject::OnCreate()
{
	// Load the edittime data that was serialized.
	bin ar;
	ar.attach(info.editObject->eData, info.editObject->eSize);

	// Read the data.  Same format as you exported in EditExt::Serialize.
	// Your runtime loader must be able to load all versions!
	int Version = 0;
	ar >> Version;


	// Set default dimensions
	info.x = info.editObject->eX;
	info.y = info.editObject->eY;
	info.w = info.editObject->eWidth;
	info.h = info.editObject->eHeight;
	info.angle = 0.0f;

	// Update bounding box
	pRuntime->UpdateBoundingBox(this);

	for(int i = 0; i < 4; i++)
	{
		Player[i] = new CXBOXController(i+1);
	}

	Control editControl;
	for(int i = 0; i < CONTROLLERS; i++)
		for(int j = 0; j < BUTTONS; j++)
		{
			ar >> editControl;
			if(editControl.control != "")
			{
				controls.push_back(RunControl(editControl, i, (XBoxControl::Control)j));
			}
		}


	// Finished reading data
	ar.detach();
	//Player1->Vibrate(65535, 0);
}

// Destructor: called when an instance of your object is destroyed.
ExtObject::~ExtObject()
{
}

// Called every frame, before the events and after drawing, for you to update your object if necessary
// Return 1 (do not call again) or 0 (continue calling)
BOOL ExtObject::OnFrame()
{
	for(int i = 0; i < 4; i++)
	{
		CXBOXController* player = Player[i];
		player->valid_state = false; // make state update

		player->leftVib.Update(player);
		player->leftVib.time += pRuntime->GetTimeDelta() * 1000;

		player->rightVib.Update(player);
		player->rightVib.time += pRuntime->GetTimeDelta() * 1000;

		player->Vibrate(player->leftVib.curStrength, player->rightVib.curStrength);
	}

	vector<RunControl>::iterator i = controls.begin();
	for(; i!= controls.end(); i++)
	{
		float state = pRuntime->GetControlState(i->control.c_str(), i->player);
		state = max(state, Player[i->controller]->ButtonState((XBoxControl::Control)i->xboxControl));
		pRuntime->SetControlState(i->control.c_str(), i->player, state);
	}

	return 0;	
}

// Called every frame, after the events and before drawing, for you to update your object if necessary
// Return 1 (do not call again) or 0 (continue calling)
// It is not safe to destroy objects in OnFrame2().  If you have to do this, use OnFrame().
// If you are storing any pointers to CRunObjects, check the info.destroying flag here.  If it is true,
// you must reset the pointer to NULL as it is no longer valid.
BOOL ExtObject::OnFrame2()
{
	return 1;	// Do not call again
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

// User defined functions
long ExtObject::GetData(int id, void* param)
{
	return 0;
}

long ExtObject::CallFunction(int id, void* param)
{
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