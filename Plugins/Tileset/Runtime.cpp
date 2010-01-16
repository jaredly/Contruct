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
	int myValue;

	// Load the edittime data that was serialized.
	bin ar;
	ar.attach(info.editObject->eData, info.editObject->eSize);

	// Read the data.  Same format as you exported in EditExt::Serialize.
	int Version = 0;
	ar >> Version;
	
	/////////////////////////////////////////

	int size;
	ar >> size;
	tiles.reserve(size);
	for(int i = 0; i < size; i++)
	{
		int size;
		ar >> size;
		tiles.push_back(vector<CTile>());
		tiles.back().reserve(size);
		
		for(int j = 0; j < size; j++)
		{
			CTile tile;
			ar >> tile.index;
			tiles.back().push_back(tile);
		}
	}

	ar >> size;
	for(int i = 0; i < size; i++)
	{
		CMapPoint pt;
		ar >> pt.x;
		ar >> pt.y;
		int num;
		ar >> num;
		m_vt_sel[pt] = num;
	}

	ar >> m_vt_sel_x;
	ar >> m_vt_sel_y;
	ar >> m_vt_IsSel;
	ar >> m_vt_IsSelBox;
	ar >> m_vt_AddSel;

	ar >> size;
	for(int i = 0; i < size; i++)
	{
		CMapPoint pt;
		ar >> pt.x;
		ar >> pt.y;
		int num;
		ar >> num;
		m_sel[pt] = num;
	}

	ar >> m_sel_x;
	ar >> m_sel_y;
	ar >> m_IsSel;
	ar >> m_IsSelBox;
	ar >> m_AddSel;
	ar >> m_SelectingFromBoard;

	ar >> size;
	for(int i = 0; i < size; i++)
	{
		CMapPoint pt;
		ar >> pt.x;
		ar >> pt.y;
		int num;
		ar >> num;
		m_ghost[pt] = num;
	}

	// Animation
	int animroot;
	ar >> animroot;

	
#define ROUND(a,b) (((a)/(b))*(b))
	
	
	CollisionMask* mask = pRuntime->CreateCollisionMask(
		ROUND(info.editObject->eWidth,32),
		ROUND(info.editObject->eHeight,32));

	CRunAnimation* pAnim = pRuntime->GetAnimationPtr(animroot);

	int count = pRuntime->GetAnimationFrameCount(pAnim);

	for(int i = 0; i < count; i++)
	{
		CRunAnimationFrame* pFrame = pRuntime->GetAnimationFrame(pAnim, i);
		CollisionMask* tilemask = pRuntime->GetCollisionMaskFromImage((int)pFrame->imgHandle); // The runtime typecasts it back lol
		collision[i] = tilemask;
		textures[i] = pRuntime->pRenderer->CreateTextureFromHandle(pFrame->imgHandle);
	}


	int _x = 0;
	int _y = 0;
	for(vector< vector<CTile> >::iterator x = tiles.begin(); x!= tiles.end(); x++)
	{
		_y = 0;
		for(vector<CTile>::iterator y = x->begin(); y!= x->end(); y++)
		{
			int index = y->index;
			if(index >= 0 & index < collision.size())
			{
				CollisionMask* tilemask = collision.find(index)->second;
				pRuntime->AddMaskToMask(tilemask, mask, _x, _y);
			}
			_y += 32;	
		}
		_x += 32;
	}

	info.pInfo->collisionMaskHandle = mask;
	info.collMode = COLLISIONMODE_FINE;
	
	pRuntime->ExportCollisionToFile(mask, 1);




	ar >> m_EditMode;
	ar >> m_ViewTiles;

	ar >> tileWidth;
	ar >> tileHeight;



	/////////////////////////////////////////

	// Finished reading data
	ar.detach();

	// Set default dimensions
	info.x = info.editObject->eX;
	info.y = info.editObject->eY;
	info.w = info.editObject->eWidth;
	info.h = info.editObject->eHeight;
	info.pInfo->filter = info.editObject->eColor;
	info.angle = 0.0f;
	info.visible = true;

	// Update bounding box
	pRuntime->UpdateBoundingBox(this);


}

// Destructor: called when an instance of your object is destroyed.
ExtObject::~ExtObject()
{
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