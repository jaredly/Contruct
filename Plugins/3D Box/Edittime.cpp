// Edittime.cpp - Edittime implementation of object
//

// Include StdAfx and Properties.h
#include "StdAfx.h"

#ifndef RUN_ONLY

//////////////////////////////////////////////////////////////////////////////////
// Property table
//////////////////////////////////////////////////////////////////////////////////

// The property table defines the list of properties that appear in the property
// grid when your object is selected in the layout editor.
//
// See the SDK docs for full documentation, but to get you started:-
//
// PROPERTY_EDIT  (myVariable, "Label", "Description");		// A text field property.
// PROPERTY_VALUE(myVariable, "Label 2", "Description");	// An integer property.
// PROPERTY_FLOAT(myVariable, "Label 3", "Description");	// A double/float property
//
// The labels MUST be unique!

BEGIN_PROPERTY_TABLE();
	PROPERTY_FLOAT(height, "Depth", "The depth of the box.");
	PROPERTY_FLOAT(z, "Z", "The Z depth of the box.");
	PROPERTY_BOOL(zbuffer, "Z buffer", "Enabled for 3D rendering, disabled for boxes drawn on top of each other.");
PROPERTY_NEW_CATEGORY("Textures", "Edit the cube textures.");
	PROPERTY_BUTTON("Edit", "Front face", "Edit the texture for the front face", EditFace(FRONT_FACE));
	PROPERTY_BUTTON("Edit", "Top face", "Edit the texture for the top face", EditFace(TOP_FACE));
	PROPERTY_BUTTON("Edit", "Bottom face", "Edit the texture for the bottom face", EditFace(BOTTOM_FACE));
	PROPERTY_BUTTON("Edit", "Left face", "Edit the texture for the left face", EditFace(LEFT_FACE));
	PROPERTY_BUTTON("Edit", "Right face", "Edit the texture for the right face", EditFace(RIGHT_FACE));
	PROPERTY_BUTTON("Edit", "Back face", "Edit the texture for the back face", EditFace(BACK_FACE));
END_PROPERTY_TABLE  ();


/////////////////////////////////////////////////////////////////////////////////
// Edittime functions
//////////////////////////////////////////////////////////////////////////////////

// Constructor: when your object is created during edittime or application loaded.
EditExt::EditExt(VEditTime* pVEditTime, editInfo* pEInfo)
{
	// Save the edittime and einfo pointers
	pEditTime = pVEditTime;
	pInfo = pEInfo;
	drawInit = false;
	pVertices = NULL;
	pDevice = NULL;
	zbuffer = true;
}

void EditExt::EditFace(int face)
{
	pEditTime->EditImage(imageHandles[face]);
	textures[face] = pEditTime->AddImageAsTexture(imageHandles[face]);
	UpdateVertices();
}

// Destructor: when your object is deleted or the application closed:
// close any allocated resources here
EditExt::~EditExt()
{
	if (pVertices)
		pVertices->Release();
}

// Initialization:  when the frame editor is opened.  Create textures, fonts,
// and anything else DirectX volatile here.
void EditExt::Initialize()
{
}

// Called when any property has been changed.
void EditExt::OnPropertiesUpdate()
{
}

// When the object is inserted to the frame editor by the user.
void EditExt::OnPut()
{
	pInfo->objectWidth = 64;
	pInfo->objectHeight = 64;
	pInfo->originX = 0.5;
	pInfo->originY = 0.5;

	// Make AddImageFromResource load local resources
	USEMYRESOURCES();

	// Add an image for each face of the cube
	for (int i = 0; i < 6; i++)
		imageHandles[i] = pEditTime->AddImageFromResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_UNKNOWNPNG));

	height = 20.0f;
	z = 0.0f;
}


// When the object is removed from the frame editor by the user.
void EditExt::OnRemoved()
{

}

// Get any icons needed to show this ACEs menu
int EditExt::GetMenuIcons(int ACEMenu)
{
	// Todo: return the parameter number of a bitmap carrying an imagelist
	//       to show in the menu indicated by ACEMenu

	return -1; // -1 indicates no icons will be used
}


int EditExt::OnShowACEMenu(TABLE ACEType)
{
	// You can return the resource number indicating the menu to show for the
	//       ACE type indicated in ACEType
	// This overrides the default menu

	return -1;	// Return -1 for standard menu to be loaded
}

// On object resized
BOOL EditExt::OnSizeObject()
{
	return TRUE;
}

// Update vertices for the object position
void EditExt::UpdateVertices()
{
	UpdateTextures();

	void* vertexData;
	pVertices->Lock(0, 0, &vertexData, 0);

	// Calculate the UV coords for each face
	float imgU[6];
	float imgV[6];

	for (int i = 0; i < 6; i++) {
		SIZE imgArea = pEditTime->GetImageSize(imageHandles[i]);
		SIZE texArea;
		pEditTime->GetTextureSize(textures[i], &texArea);
		imgU[i] = (float)imgArea.cx / (float)texArea.cx;
		imgV[i] = (float)imgArea.cy / (float)texArea.cy;
	}

	// Copy the source cube vertices to the 
	TLVERTEX* vxSrc = g_cube_vertices;
	TLVERTEX* vxDest = currentVertices;
	TLVERTEX* end = vxSrc + NUMCUBEVERTICES;
	int index = 0;

	for ( ; vxSrc != end; vxSrc++, vxDest++, index++) {

		*vxDest = *vxSrc;

		// Update the UV coords to draw only this texture's image area
		if (vxDest->u == 1.0f)
			vxDest->u = imgU[index / 6];
		if (vxDest->v == 1.0f)
			vxDest->v = imgV[index / 6];
	}

	memcpy(vertexData, currentVertices, sizeof(currentVertices));
	pVertices->Unlock();
}

void EditExt::CreateZBuffer()
{
	// Z buffer not yet created or lost in lost device
	if (!pEditTime->LayoutKeyExists("zbuffer") || pEditTime->GetLayoutKey("zbuffer") == NULL) {

		SIZE s;
		s.cx = GetSystemMetrics(SM_CXSCREEN);
		s.cy = GetSystemMetrics(SM_CYSCREEN);

		hr = pDevice->CreateDepthStencilSurface(s.cx, s.cy, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, &pDepthStencil, NULL);

		if (FAILED(hr)) {
			MessageBox(NULL, "3D Cube object: Failed to create depth stencil", "3D Cube", MB_OK | MB_ICONHAND);
			return;
		}

		hr = pDevice->SetDepthStencilSurface(pDepthStencil);

		if (FAILED(hr)) {
			MessageBox(NULL, "3D Cube object: Failed to set depth stencil", "3D Cube", MB_OK | MB_ICONHAND);
			return;
		}

		hr = pDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

		if (FAILED(hr)) {
			MessageBox(NULL, "3D Cube object: Failed to clear depth stencil", "3D Cube", MB_OK | MB_ICONHAND);
			return;
		}

		pEditTime->SetLayoutKey("zbuffer", pDepthStencil, false);
		pEditTime->SetLayoutKey("zbufferclearframe", 0, false);
	}
	else
		pDepthStencil = (IDirect3DSurface9*)pEditTime->GetLayoutKey("zbuffer");
}

// Startup D3D resources
void EditExt::D3DInit()
{
	// Get the D3D device used by the runtime
	pDevice = (IDirect3DDevice9*)pEditTime->GetDisplayDevice();

	if (pDevice == NULL) {
		MessageBox(NULL, "No DirectX device available", "3D Cube Error", MB_OK | MB_ICONHAND);
		return;
	}

	// Set up the cube vertices
	hr = pDevice->CreateVertexBuffer(sizeof(TLVERTEX) * 36, D3DUSAGE_WRITEONLY, D3DFVF_TLVERTEX, D3DPOOL_MANAGED,
								&pVertices, NULL);

	if (FAILED(hr)) {
		MessageBox(NULL, "Failed to create vertex buffer", "3D Cube Error", MB_OK | MB_ICONHAND);
		return;
	}

	UpdateVertices();

	//Here we build our View Matrix, think of it as our camera.
	//First we specify that our viewpoint is 8 units back on the Z-axis
	eye_vector = D3DXVECTOR3( 0.0f, 0.0f, 8.0f );

	//We are looking towards the origin
	lookat_vector = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	//The "up" direction is the positive direction on the y-axis
	up_vector = D3DXVECTOR3(0.0f,1.0f,0.0f);

	D3DXMatrixLookAtLH(&view_matrix,&eye_vector,
								   &lookat_vector,
								   &up_vector);

	SIZE s;
	s.cx = GetSystemMetrics(SM_CXSCREEN);
	s.cy = GetSystemMetrics(SM_CYSCREEN);
	aspect=((float)s.cx / (float)s.cy);

	D3DXMatrixPerspectiveFovLH(&projection_matrix, //Result Matrix
							  D3DX_PI/4,          //Field of View, in radians.
							  aspect,             //Aspect ratio
							  1.0f,               //Near view plane
							  100.0f );           //Far view plane

	D3DXMatrixIdentity(&worldMatrix);
	D3DXMatrixIdentity(&rotMatrix);
	D3DXMatrixIdentity(&transMatrix);
	D3DXMatrixIdentity(&scaleMatrix);

	// The vectors used in screen projection
	orig.x = 0;
	orig.y = 0;
	orig.z = 0;
	normal.x = 0;
	normal.y = 0;
	normal.z = 1;		// Normal facing the view

	CreateZBuffer();
}

#else //ifndef RUN_ONLY

BOOL WINAPI ETCreateObject(editInfo* editObject) {return FALSE;}
BOOL WINAPI ETInitializeObject(editInfo* editObject, VEditTime* pVEditTime) {return FALSE;}
BOOL WINAPI ETDestroyObject(editInfo* editObject) {return FALSE;}
int WINAPI ETDrawObject(editInfo *editObject) {return 0;}
void ETOnPropertyChanged(editInfo* editObject) {}
long LinkPropTable() {return 0;}
void WINAPI ETSerialize(editInfo* editObject, CArchive& ar) {}
void WINAPI ETPutObject(editInfo* editObject) {}
void WINAPI ETRemoveObject(editInfo* editObject) {}
void WINAPI GetInfo(LPOINFO objectInfo) {}
int WINAPI ETGetMenuIcons(editInfo* editObject, int ACEMenu) {return 0;}
int WINAPI ETShowACEMenu(editInfo* editObject, TABLE ACEType)  {return 0;}
BOOL WINAPI ETSizeObject(editInfo* editObject)  {return FALSE;}

#endif // RUN_ONLY