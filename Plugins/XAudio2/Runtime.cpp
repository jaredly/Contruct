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
	
	data.Serialize(ar);
	data.commandBatching = CXAudio2::CBT_NEXTTICK;		// Batched commands

	// Finished reading data
	ar.detach();

	// Initialise XAudio2
	try {
		XAudio2.Init(data);
	}
	catch (CXAudio2Error e) {
		OnXAudio2Error(e);
	}

	lastAutoplayChannel = 0;
	loopMp3 = false;

	// Set default dimensions
	info.x = info.editObject->eX;
	info.y = info.editObject->eY;
	info.w = info.editObject->eWidth;
	info.h = info.editObject->eHeight;
	info.angle = 0.0f;

	// Positioned sounds
	listenerX = listenerY = 0.0f;
	positionedSounds.resize(data.numChannels);
	listenerObj = NULL;

	// Update bounding box
	pRuntime->UpdateBoundingBox(this);
}

// Destructor: called when an instance of your object is destroyed.
ExtObject::~ExtObject()
{
	XAudio2.Close();

	// Delete all temp extracted music
	map<int, CString>::iterator i = extractedMusic.begin();

	for ( ; i != extractedMusic.end(); i++)
		_unlink(i->second);
}

void ExtObject::SetChannelPositioned(int channel, float x, float y, CRunObject* obj)
{
	if (channel < 0 || channel >= data.numChannels) return;

	PositionedSound& ps = positionedSounds[channel];
	ps.active = true;
	ps.obj = obj;

	if (obj) {
		ps.x = ps.obj->info.x;
		ps.y = ps.obj->info.y;
	}
	else {
		ps.x = x;
		ps.y = y;
	}
}

// Called every frame, before the events and after drawing, for you to update your object if necessary
// Return 1 (do not call again) or 0 (continue calling)
BOOL ExtObject::OnFrame()
{
	return 1;	// Do not call again
}

// Called every frame, after the events and before drawing, for you to update your object if necessary
// Return 1 (do not call again) or 0 (continue calling)
// It is not safe to destroy objects in OnFrame2().  If you have to do this, use OnFrame().
// If you are storing any pointers to CRunObjects, check the info.destroying flag here.  If it is true,
// you must reset the pointer to NULL as it is no longer valid.
BOOL ExtObject::OnFrame2()
{
	// Check for finished music and loop
	if (loopMp3)
		mp3.CheckLoop();

	// Update listener
	if (listenerObj) {
		listenerX = listenerObj->info.x;
		listenerY = listenerObj->info.y;
	}

	// Update positioned sounds
	PositionedSoundIterator i = positionedSounds.begin();
	PositionedSoundIterator end = positionedSounds.end();
	int channel = 0;

	for ( ; i != end; i++, channel++) {
		if (i->active) {
			if (XAudio2.IsPlaying(channel)) {
				
				// Update if object
				if (i->obj) {
					i->x = i->obj->info.x;
					i->y = i->obj->info.y;

					if (i->obj->info.destroying)
						i->obj = NULL;
				}

				// Calculate volume for this sound
				float curVol = data.maxVol;

				float dx = i->x - listenerX;
				float dy = i->y - listenerY;
				float sqdist = dx*dx + dy*dy;

				if (sqdist > (data.minDist * data.minDist)) {
					float db_atten = 10 * log10((data.minDist * data.minDist) / sqdist);

					curVol = data.maxVol + (db_atten * data.rolloff);

					if (curVol < data.minVol)
						curVol = data.minVol;
				}

				if (XAudio2.GetVolume(channel) != curVol)
					XAudio2.SetVolume(channel, curVol);

				// Calculate pan for this sound
				float pan = dx / data.panDist;
				Clamp(pan, -1.0f, 1.0f);
				pan *= data.maxPan;

				if (XAudio2.GetPan(channel) != pan)
					XAudio2.SetPan(channel, pan);
			}
			// No longer playing; set positioned sound inactive
			else
				i->active = false;
		}
	}

	// Update audio engine (leave timescale at 1 if scaling disabled)
	XAudio2.Tick(data.enableTimescale ? pRuntime->GetTimeScale() : 1.0f);

	// Check the listener object isn't dead
	if (listenerObj && listenerObj->info.destroying)
		listenerObj = NULL;

	return 0;	// Call again
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

// Error messages are sent to the debugger
void ExtObject::OnXAudio2Error(const CXAudio2Error& e)
{
	CString msg;
	msg.Format("XAudio2: %s (%#x)", e.GetName(), e.GetHR());
	pRuntime->AddDebugLogMessage(msg);
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