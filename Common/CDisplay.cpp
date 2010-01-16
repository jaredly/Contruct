#include "StdAfx.h"

//////////////////////////////////////////////
// CD3DDISPLAY FUNCTIONS
#ifndef _CDISPLAY_CPP_
#define _CDISPLAY_CPP_

#ifndef APPRUNTIME

#define m_mipmaplevels 0
#define m_mipmap_filter D3DX_FILTER_NONE

// Texture memory trackers
int CD3DDisplay::vramtexturebytes;
int CD3DDisplay::vramtargetbytes;

// Constructor
CD3DDisplay::CD3DDisplay()
{
	// Null default values
	d3d = NULL;	d3dDevice = NULL;
	d3d9Dll = NULL; batchBuffer = NULL;
	hr = curTextureId = bbWidth = bbHeight = 0;
	indexBuffer = NULL;
	d3dPresents = NULL;

	globalcolor = 0xFFFFFFFF;

	renderTex = -1;
	curTexture = -1;
	pDisplaySurface = NULL;
	pBackBuffer = NULL;

	trans_PanX = 0;
	trans_PanY = 0;
	trans_Angle = 0;
	trans_ScaleX = 1;
	trans_ScaleY = 1;

	vramtargetbytes = 0;
	vramtexturebytes = 0;

	m_pD3DXSprite = 0;
	haveIGotMyDevice = true;
	batch_lastdraw = 0;

	// Create the first memory chunk
	memChunks.push_back(CDisplayHeapChunk());
	curChunk = memChunks.begin();
	chunkPtr = &(curChunk->mem[0]);

	textQuality = ANTIALIASED_QUALITY;
	multiMonitorMode = MMM_SINGLESCREEN;
	changedBackBuffer = false;
	lineDrawer = NULL;
}

// Destructor
CD3DDisplay::~CD3DDisplay()
{
	// Shut down
	Close();
}

void CD3DDisplay::PreInit()
{
	// Load d3d9.dll
	if (d3d9Dll == NULL)
		d3d9Dll = LoadLibrary("d3d9.dll");

	if (d3d9Dll == NULL)
		throw CD3DError("Failed to load d3d9.dll.  Please check your DirectX version.");

	LPDIRECT3DCREATE9 D3DCreate9 = (LPDIRECT3DCREATE9)GetProcAddress(d3d9Dll, "Direct3DCreate9");

	if (D3DCreate9 == NULL)
		throw CD3DError("Failed to initialise Direct3D 9!");

	// Initialise D3D 9
	if (d3d == NULL)
		d3d = D3DCreate9(D3D_SDK_VERSION);

	if (d3d == NULL)
		throw CD3DError("Failed to initialise Direct3D 9!");

	//Get device capabilities
    ZeroMemory (&d3dCaps, sizeof(d3dCaps));
    hr = d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps);
	
	if (FAILED(hr)) throw CD3DError("Failed to initialise Direct3D 9!");
}

// Initialise the DirectX 9 engine
bool CD3DDisplay::Init(CD3DDisplaySetup* settings)
{
	PreInit();

	// Store info
	CD3DDisplay::hWnds = settings->hWnds;

	//Setup present parameters
	delete[] d3dPresents;
	d3dPresents = new D3DPRESENT_PARAMETERS[GetMultiHeadCount()];
    ZeroMemory(d3dPresents, sizeof(D3DPRESENT_PARAMETERS) * GetMultiHeadCount());

	// Setup first d3dPresent
	D3DPRESENT_PARAMETERS& d3dPresent = d3dPresents[0];

    d3dPresent.SwapEffect = settings->swapEffect;
    d3dPresent.hDeviceWindow = settings->hWnds.front();
    d3dPresent.BackBufferCount = 1;						// Single back buffer
	d3dPresent.MultiSampleType = D3DMULTISAMPLE_NONE;	// No multi-sampling
	d3dPresent.MultiSampleQuality = 0;					// No multi-sampling
	d3dPresent.EnableAutoDepthStencil = FALSE;			// No depth/stencil buffer
	d3dPresent.Flags = 0;

	// VSynced FPS
	if (settings->fpsMode == CD3DFPS_VSYNCED)
		d3dPresent.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	// Unlimited or fixed FPS use immediate display, will produce tearing
	if (settings->fpsMode == CD3DFPS_UNLIMITED || settings->fpsMode == CD3DFPS_FIXED)
		d3dPresent.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// Fullscreen mode enabled: fullscreen backbuffer
	if (settings->bFullscreen) {

		d3dPresent.Windowed = false;
		d3dPresent.BackBufferWidth = settings->resWidth;
		d3dPresent.BackBufferHeight = settings->resHeight;
		d3dPresent.BackBufferFormat = settings->resFormat;
	}
	// Windowed mode
	else {

		// Get display mode
		D3DDISPLAYMODE d3ddm;
		d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

		// Get window bounds
		RECT rWindow;
		GetClientRect(hWnds.front(), &rWindow);

		// Set up backbuffer in window client area
		d3dPresent.Windowed = true;
		d3dPresent.BackBufferFormat = settings->resFormat;
		d3dPresent.BackBufferWidth = rWindow.right - rWindow.left;
		d3dPresent.BackBufferHeight = rWindow.bottom - rWindow.top;
	}

	UpdatePresentsArray();

	DWORD extraFlags = 0;
	if (multiMonitorMode != MMM_SINGLESCREEN)	extraFlags |= D3DCREATE_ADAPTERGROUP_DEVICE;

	// *** Setup vertex processor ***
	// Check if hardware vertex processing is available
    if (d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
    {    
        //Create device with hardware vertex processing
        hr = d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL, settings->hFocusWnd,
            D3DCREATE_HARDWARE_VERTEXPROCESSING | extraFlags, d3dPresents, &d3dDevice);        
    }
    else
    {
        //Create device with software vertex processing
        hr = d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL, settings->hFocusWnd,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING | extraFlags, d3dPresents, &d3dDevice);
    }

	//Make sure device was created
    if (FAILED(hr) || d3dDevice == NULL)
        throw CD3DError(hr);

	bbWidth = settings->resWidth;
	bbHeight = settings->resHeight;

	// Initialise the device with renderstates
	SetupDevice();

    // Successfully initalized Direct3D
    return true;
}

// Initialise the D3D Device
void CD3DDisplay::SetupDevice()
{

	//Set vertex shader
    d3dDevice->SetVertexShader(NULL);
    d3dDevice->SetFVF(D3DFVF_TLVERTEX);

	// Create the batching buffer
	d3dDevice->CreateVertexBuffer(sizeof(TLVERTEX) * CD3D_MAX_BATCH_SIZE, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_TLVERTEX, D3DPOOL_DEFAULT,
									&batchBuffer, NULL);

	// Create index buffer
	d3dDevice->CreateIndexBuffer(CD3D_MAX_INDEX_SIZE * 2, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFMT_INDEX16, D3DPOOL_DEFAULT,
									&indexBuffer, NULL);

	// Initialise batching
	ResetBatch();
	
	// Initialise 2D renderstates
	Setup2DMode();

    if( FAILED( hr = D3DXCreateSprite( d3dDevice, &m_pD3DXSprite ) ) )
		return;

	// Get the display rendertarget for access to backbuffer surface
	d3dDevice->GetRenderTarget(0, &(pDisplaySurface));
	pBackBuffer = pDisplaySurface;

	SetTexture(-1);
	SetRenderTarget(-1);
}

// Reset the device
void CD3DDisplay::Reset()
{
	UpdatePresentsArray();

	// Release our vertex buffers
	if (batchBuffer)	batchBuffer->Release();
	if (indexBuffer)	indexBuffer->Release();

	batchBuffer = NULL;
	indexBuffer = NULL;

	// Release access to backbuffer
	if(m_pD3DXSprite)
		m_pD3DXSprite->Release();
	
	pDisplaySurface->Release();
	if(pDisplaySurface != pBackBuffer)
		pBackBuffer->Release();

	// Find and free all rendertarget textures
	map<int, TextureInfo*>::iterator t = textures.begin();
	vector<int> toRemove;

	for ( ; t != textures.end(); t++) {
		if (t->second->rendertarget)
			toRemove.push_back(t->first);
	}

	vector<int>::iterator v = toRemove.begin();

	for ( ; v != toRemove.end(); v++)
		RemoveTexture(*v);

	// Reset
	HRESULT hr = d3dDevice->Reset(d3dPresents);

	if (FAILED(hr))
		throw CD3DError(hr);

	bbWidth = d3dPresents[0].BackBufferWidth;
	bbHeight = d3dPresents[0].BackBufferHeight;

	// Setup the device again
	SetupDevice();
}

bool CD3DDisplay::TextureExists(int iTexture)
{
	return textures.find(iTexture) != textures.end();
}

void CD3DDisplay::OnLostDevice()
{
	if (m_pD3DXSprite)
		m_pD3DXSprite->OnLostDevice();

	if (lineDrawer)
		lineDrawer->OnLostDevice();

	vector<CD3DDisplayFont*>::iterator f = fontRefs.begin();

	for ( ; f != fontRefs.end(); f++)
		if ((*f)->d3dxfont)
			(*f)->d3dxfont->OnLostDevice();
}

void CD3DDisplay::OnResetDevice()
{
	if (m_pD3DXSprite)
		m_pD3DXSprite->OnResetDevice();

	if (lineDrawer)
		lineDrawer->OnResetDevice();

	vector<CD3DDisplayFont*>::iterator f = fontRefs.begin();

	for ( ; f != fontRefs.end(); f++)
		if ((*f)->d3dxfont)
			(*f)->d3dxfont->OnResetDevice();
}

void CD3DDisplay::Setup2DMode()
{
	EndBatch();

	d3dDevice->SetStreamSource(0, batchBuffer, 0, sizeof(TLVERTEX));
	d3dDevice->SetIndices(indexBuffer);

    // Set up rendering states for 2D drawing
    d3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	d3dDevice->SetRenderState(D3DRS_LASTPIXEL,TRUE);
	d3dDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);		// thin pixel lines
	d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); // renders back faces so horizontal flips work

	// Mipmapping stuff
	d3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	d3dDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 1);
	float bias = -0.5f;
	d3dDevice->SetSamplerState(0, D3DSAMP_MIPMAPLODBIAS, *((LPDWORD)(&bias)));

	// Prevent bleeding
	d3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU,D3DTADDRESS_MIRROR);
	d3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV,D3DTADDRESS_MIRROR);

	// Retrieve all current renderstates
	for (int i = 0; i < CD3D_NUM_RENDERSTATES; i++)
		d3dDevice->GetRenderState((D3DRENDERSTATETYPE)i, &(renderStates[i]));

	// Retrieve all current samplerstates
	for (int i = 0; i < CD3D_NUM_SAMPLERSTATES; i++)
		d3dDevice->GetSamplerState(0,(D3DSAMPLERSTATETYPE)i, &(samplerStates[i]));

	D3DXMATRIX Identity;
	D3DXMATRIX Rotate;
	D3DXMatrixIdentity(&Identity);

	d3dDevice->SetTransform(D3DTS_PROJECTION, &Identity);
	d3dDevice->SetTransform(D3DTS_WORLD, &Identity);
	d3dDevice->SetTransform(D3DTS_VIEW, &Identity);


	for(int i = 0; i < CD3D_NUM_TRANSFORMS; i++)
		d3dDevice->GetTransform((D3DTRANSFORMSTATETYPE)i, &transforms[i]);
		

	renderTex = -1;
	curTexture = -1;

	Update3dCoords();

}

// Shut down DirectX 9 engine
bool CD3DDisplay::Close()
{
	ClearBatchData();

	// Release textures
	ClearTextures();

	// Release fonts
	fontRefs.resize(0);

	if (lineDrawer != NULL)
		lineDrawer->Release();

    // Release vertex buffers
	if (batchBuffer)
		batchBuffer->Release();
	if (indexBuffer)
		indexBuffer->Release();

	if(m_pD3DXSprite)
		m_pD3DXSprite->Release();
	if(pDisplaySurface)
		pDisplaySurface->Release();
	if(pDisplaySurface != pBackBuffer)
		pBackBuffer->Release();

    // Release device
    if (d3dDevice)
        d3dDevice->Release();

    // Release d3d object
    if (d3d)
        d3d->Release();

	FreeLibrary(d3d9Dll);

	delete[] d3dPresents;
	d3dPresents = NULL;

	// All have been released
	batchBuffer = NULL;
	indexBuffer = NULL;
	d3dDevice = NULL;
	d3d = NULL;
	pDisplaySurface = NULL;
	pBackBuffer = NULL;
	m_pD3DXSprite = NULL;
	lineDrawer = NULL;

    //Successfully closed Direct3D
    return true;
}

// For multihead: update all present structures in the array
void CD3DDisplay::UpdatePresentsArray()
{
	int i;

	// Assign presentation structures
	for (i = 1; i < GetMultiHeadCount(); i++)
		d3dPresents[i] = d3dPresents[0];

	// Assign windows
	for (i = 0; i < GetMultiHeadCount(); i++)
		d3dPresents[i].hDeviceWindow = hWnds[i];
}

int CD3DDisplay::GetMultiHeadCount() const
{
	// Single screen mode uses only 1 swap chain/backbuffer/window
	if (multiMonitorMode == MMM_SINGLESCREEN)
		return 1;
	// All other multimonitor modes use the number of adapters
	else
		return d3dCaps.NumberOfAdaptersInGroup;
}

void CD3DDisplay::SetMultiMonitorMode(MultiMonitorMode mmm)
{
	multiMonitorMode = mmm;
}

MultiMonitorMode CD3DDisplay::GetMultiMonitorMode() const
{
	return multiMonitorMode;
}

// Memory management
void* CD3DDisplay::BatchAllocate(unsigned int s)
{
	// Check if we can fit this size in to the current chunk
	unsigned int bytesfree = CDISPLAY_HEAP_CHUNK_SIZE - (chunkPtr - curChunk->mem);

	// Will fit in this chunk
	if (s < bytesfree) {
		// Return some memory from this chunk
		void* ret = chunkPtr;
		chunkPtr += s;
		return ret;
	}
	// Will not fit in this chunk
	else {
		// Does another chunk exist?
		list<CDisplayHeapChunk>::iterator next = curChunk;
		next++;

		// Need to allocate a new chunk
		if (next == memChunks.end())
			memChunks.push_back(CDisplayHeapChunk());

		// Move on to the next chunk
		curChunk++;
		chunkPtr = &(curChunk->mem[0]);

		// Return some fresh data from this chunk
		void* ret = chunkPtr;
		chunkPtr += s;
		return ret;
	}
}

void* CBatchBase::operator new(unsigned int num_bytes, CD3DDisplay* pDisplay)
{
	return pDisplay->BatchAllocate(num_bytes);
}

void CD3DDisplay::BatchMemoryReset()
{
	// Reset memory for new allocations
	curChunk = memChunks.begin();
	chunkPtr = &(curChunk->mem[0]);
}

// Begin drawing operations
void CD3DDisplay::BeginScene()
{
	batchesPerPresent = 0;
	d3dDevice->BeginScene();
}

// Get a TextureInfo pointer from a handle
TextureInfo* CD3DDisplay::GetTexture(int index)
{
	map<int, TextureInfo*>::iterator i = textures.find(index);

	if (i != textures.end())
		return i->second;
	else {
		MessageBox(NULL, "An invalid texture was referenced.\n\nThis is a bug, please report it!", "CD3DDisplay", MB_OK | MB_ICONHAND);
		throw;
	}
}

// End drawing operations
void CD3DDisplay::EndScene()
{
	EndBatch();

	// End the scene
	d3dDevice->EndScene();
}

// Show frame
void CD3DDisplay::Present(RECT* pRec, HWND hWndOverride)
{
	EndBatch();
	hr = d3dDevice->Present(pRec, NULL, hWndOverride, NULL);

	if (FAILED(hr))
		throw CD3DError(hr);
}

// Add a texture which can be used as a rendertarget
int CD3DDisplay::AddTextureRT(int w, int h, D3DFORMAT format, DWORD usage, D3DPOOL pool)
{
	// Can't call during lost device
	if (!haveIGotMyDevice)
		return -1;
	
	EndBatch();

	TextureInfo* tex = new TextureInfo;

	// Create render-target texture
	hr = D3DXCreateTexture(d3dDevice, w, h, 1, usage,
						format, pool, &(tex->texture));

	if (tex->texture == NULL || FAILED(hr))
		throw CD3DError(hr);

	D3DSURFACE_DESC surfaceDesc;
	tex->texture->GetLevelDesc(0, &surfaceDesc);

	tex->rendertarget = true;
	tex->format = format;

	tex->texwidth = surfaceDesc.Width;
	tex->texheight = surfaceDesc.Height;
	tex->imgwidth = w;
	tex->imgheight = h;

	tex->xf = float(tex->imgwidth) / float(tex->texwidth);
	tex->yf = float(tex->imgheight) / float(tex->texheight);

	textures[curTextureId] = tex;
	curTextureId++;

	vramtargetbytes += tex->texwidth * tex->texheight * 4;

	return curTextureId - 1;
}

// Clear rendertarget
void CD3DDisplay::ClearRT(D3DCOLOR color)
{
	CBatch_ClearRT* pBatch = new (this) CBatch_ClearRT(color);
	batch.push_back((CBatchBase*)pBatch);

	batch_lastdraw = batch.size() - 1;
}

// Clear rendertarget
void CD3DDisplay::ClearRT(D3DCOLOR color, const D3DRECT& rect)
{
	CBatch_ClearRT* pBatch = new (this) CBatch_ClearRT(color, rect);
	batch.push_back((CBatchBase*)pBatch);

	batch_lastdraw = batch.size() - 1;
}

// Resize a rendertarget texture
bool CD3DDisplay::ResizeTextureRT(int& iTexture, int w, int h)
{
	EndBatch();
	TextureInfo* pTex = TexInfo(iTexture);
	
	if (!pTex->rendertarget) return false;

	int oldTex = curTexture;
	int oldRT = renderTex;

	int newTex = AddTextureRT(w, h, pTex->format);

	SetTexture(iTexture);
	SetRenderTarget(newTex);
	ClearRT();
	Blit(0,0);

	RemoveTexture(iTexture);

	//vramtargetbytes += w * h * 4;

	if (oldRT == iTexture)
		SetRenderTarget(newTex);
	else
		SetRenderTarget(oldRT);

	if (oldTex == iTexture)
		SetTexture(newTex);
	else
		SetTexture(oldTex);

	iTexture = newTex;

	return true;
}

// Add texture from disk file
int CD3DDisplay::AddTextureFromFile(const char* filename)
{
	EndBatch();

	TextureInfo* tex = new TextureInfo;

	D3DXIMAGE_INFO SrcInfo;      //Optional

	// Load image from file
	hr = D3DXCreateTextureFromFileEx(d3dDevice, filename, 0, 0, m_mipmaplevels, 0, 
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, m_mipmap_filter, D3DX_DEFAULT, 
		CD3DDISPLAY_TRANSPARENT, &SrcInfo, NULL, &(tex->texture));

	if (FAILED(hr))
		throw CD3DError(hr);

	// Get the tex width/height
	D3DSURFACE_DESC surfaceDesc;
	tex->texture->GetLevelDesc(0, &surfaceDesc);

	tex->texwidth = surfaceDesc.Width;
	tex->texheight = surfaceDesc.Height;
	tex->imgwidth = SrcInfo.Width;
	tex->imgheight = SrcInfo.Height;
	tex->rendertarget = false;

	vramtexturebytes += tex->texwidth * tex->texheight * 4;

	tex->xf = float(tex->imgwidth) / float(tex->texwidth);
	tex->yf = float(tex->imgheight) / float(tex->texheight);

	// Add the new texture to the textures array
	textures[curTextureId] = tex;
	curTextureId++;

	// Return texture index
	return curTextureId - 1;
}

int CD3DDisplay::AddTextureFromFileInMemory(LPCVOID pData, UINT len)
{
	EndBatch();

	TextureInfo* tex = new TextureInfo;

	D3DXIMAGE_INFO SrcInfo;      //Optional

	// Load image from file
	hr = D3DXCreateTextureFromFileInMemoryEx(d3dDevice, pData, len, 0, 0, m_mipmaplevels, 0, 
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, m_mipmap_filter, D3DX_DEFAULT, 
		CD3DDISPLAY_TRANSPARENT, &SrcInfo, NULL, &(tex->texture));
 
	if (FAILED(hr))
		throw CD3DError(hr);

	// Get the tex width/height
	D3DSURFACE_DESC surfaceDesc;
	tex->texture->GetLevelDesc(0, &surfaceDesc);

	tex->texwidth = surfaceDesc.Width;
	tex->texheight = surfaceDesc.Height;
	tex->imgwidth = SrcInfo.Width;
	tex->imgheight = SrcInfo.Height;
	tex->rendertarget = false;

	vramtexturebytes += tex->texwidth * tex->texheight * 4;

	tex->xf = float(tex->imgwidth) / float(tex->texwidth);
	tex->yf = float(tex->imgheight) / float(tex->texheight);

	// Add the new texture to the textures array
	textures[curTextureId] = tex;
	curTextureId++;

	// Return texture index
	return curTextureId - 1;
}

// Add texture from file in a Data resource
int CD3DDisplay::AddTextureFromResource(HMODULE hModule, LPCTSTR pResource)
{
	EndBatch();

	TextureInfo* tex = new TextureInfo;

	D3DXIMAGE_INFO SrcInfo;      //Optional

	// Load image from file
	hr = D3DXCreateTextureFromResourceEx(d3dDevice, hModule, pResource, 0, 0, m_mipmaplevels, 0, 
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, m_mipmap_filter, D3DX_DEFAULT, 
		CD3DDISPLAY_TRANSPARENT, &SrcInfo, NULL, &(tex->texture));

	if (FAILED(hr))
		throw CD3DError(hr);

	// Get the tex width/height
	D3DSURFACE_DESC surfaceDesc;
	tex->texture->GetLevelDesc(0, &surfaceDesc);

	tex->texwidth = surfaceDesc.Width;
	tex->texheight = surfaceDesc.Height;
	tex->imgwidth = SrcInfo.Width;
	tex->imgheight = SrcInfo.Height;
	tex->rendertarget = false;

	vramtexturebytes += tex->texwidth * tex->texheight * 4;

	tex->xf = float(tex->imgwidth) / float(tex->texwidth);
	tex->yf = float(tex->imgheight) / float(tex->texheight);

	// Add the new texture to the textures array
	textures[curTextureId] = tex;
	curTextureId++;

	// Return texture index
	return curTextureId - 1;
}

#ifndef CD3D_NOCXIMAGE

int CD3DDisplay::AddTextureFromCxImage(CxImage& cx)
{
	EndBatch();

	

	BYTE *buffer = 0;
	long size;
	cx.Encode(buffer,size,CXIMAGE_FORMAT_PNG);
	int ret =  AddTextureFromBuffer(buffer, size);
	cx.FreeMemory(buffer);

	return ret;
}


#endif

int CD3DDisplay::AddTextureFromBuffer(BYTE *buffer, long size)
{
	EndBatch();

	TextureInfo* tex = new TextureInfo;

	D3DXIMAGE_INFO SrcInfo;      //Optional

	int	hr = D3DXCreateTextureFromFileInMemoryEx(d3dDevice,buffer,size,0,0,m_mipmaplevels,0,D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, m_mipmap_filter, D3DX_DEFAULT, 
		CD3DDISPLAY_TRANSPARENT, &SrcInfo, NULL, &(tex->texture));

	if (FAILED(hr))
		throw CD3DError(hr);

	// Get the tex width/height
	D3DSURFACE_DESC surfaceDesc;
	tex->texture->GetLevelDesc(0, &surfaceDesc);

	tex->texwidth = surfaceDesc.Width;
	tex->texheight = surfaceDesc.Height;
	tex->imgwidth = SrcInfo.Width;
	tex->imgheight = SrcInfo.Height;
	tex->rendertarget = false;

	vramtexturebytes += tex->texwidth * tex->texheight * 4;

	tex->xf = float(tex->imgwidth) / float(tex->texwidth);
	tex->yf = float(tex->imgheight) / float(tex->texheight);

	// Add the new texture to the textures array
	textures[curTextureId] = tex;
	curTextureId++;

	// Return texture index
	return curTextureId - 1;
}

void CD3DDisplay::ApplyTextureGutter(int iTexture)
{
	TextureInfo* pTex = TexInfo(iTexture);
	if(pTex)
	{
		pTex->imgwidth --;
		pTex->imgheight --;

		pTex->xf = float(pTex->imgwidth) / float(pTex->texwidth);
		pTex->yf = float(pTex->imgheight) / float(pTex->texheight);
	}
}

// Release all textures
void CD3DDisplay::ClearTextures()
{

	EndBatch();
	map<int, TextureInfo*>::iterator i = textures.begin();

	for ( ; i != textures.end(); i++) {
		i->second->texture->Release();
		delete i->second;
	}

	textures.clear();
}

// Retrieve texture at index
IDirect3DTexture9* CD3DDisplay::TextureAt(int index)
{
	return GetTexture(index)->texture;
}

// Set render target
bool CD3DDisplay::SetRenderTarget(int iTexture)
{
	// If SetBackBuffer has been called, skip the parameter check
	if (changedBackBuffer)
		changedBackBuffer = false;
	else if(iTexture == renderTex)
		return true; // no change

	//EndBatch();

	renderTex = iTexture;

	IDirect3DTexture9* pD3DTex = NULL;

	if(iTexture != -1)
	{
		TextureInfo* pTex = TexInfo(iTexture);
		if (pTex->rendertarget) {
			pD3DTex = pTex->texture;
		}
		else
		{
			MessageBox(NULL, "A non-rendertarget texture was set as render target.  Please report this bug to Scirra!", "CD3DDisplay", MB_ICONHAND);
			throw;
		}
	}


	//d3dDevice->SetRenderTarget(0, pSurf);
	Update3dCoords();

	//return true;

	int &indexCommand = iSetRenderTarget;
	int &lastindexCommand = lastiSetRenderTarget;


#ifdef BATCH_OPTIMISE
	if(indexCommand > batch_lastdraw)
	{
		CBatch_SetRenderTarget* Command = dynamic_cast<CBatch_SetRenderTarget*>(batch[indexCommand]);
		if(lastindexCommand != -1)
		{
			CBatch_SetRenderTarget* LastCommand = dynamic_cast<CBatch_SetRenderTarget*>(batch[lastindexCommand]);

			if(LastCommand->SameAs(pD3DTex))
			{
				batch[indexCommand] = NULL;
				while(batch.size() && batch.back() == NULL)
					batch.pop_back();
				indexCommand = lastindexCommand;
				lastindexCommand = -1;
				return true;
			}
		}
		Command->tex = pD3DTex;
	}
	else
#endif
	{
		lastindexCommand = indexCommand;
		indexCommand = batch.size();

		CBatch_SetRenderTarget* pBatch = new (this) CBatch_SetRenderTarget(pD3DTex);

		batch.push_back((CBatchBase*)pBatch);
	}

	return true;

}

// Handle 2D display transformations using DirectX's 3D features
void CD3DDisplay::Update3dCoords(bool translate)
{
#ifdef USE_3D_CORDS
	
	int iTexture = GetRenderTarget();
	float Width = bbWidth;
	float Height = bbHeight;
	if (iTexture >= 0) 
	{
		TextureInfo* pTex = TexInfo(iTexture);
		
		 Width = pTex->texwidth;
		 Height = pTex->texheight;
	}
	float l = 0;
	float t = 0;
	float r = Width;
	float b = Height;
	if(translate)
	{
		r /= trans_ScaleX;
		b /= trans_ScaleY;

#ifndef ORTHMATRIX

		Width /= trans_ScaleX;
		Height /= trans_ScaleY;
#endif
	}

	l += 0.5;
	t += 0.5;
	r += 0.5;
	b += 0.5;

	D3DXMATRIX Identity;
	D3DXMATRIX Rotate;
	D3DXMatrixOrthoOffCenterRH(&Ortho2D, l, r, b, t, 0, 1);
	//D3DXMatrixIdentity(&Identity);

	if(translate)
	{
		//D3DXMatrixIdentity(&Rotate);
		D3DXMatrixRotationYawPitchRoll(&Rotate, 0, 0, Rad(trans_Angle));
		D3DXMatrixMultiply(&Ortho2D, &Rotate, &Ortho2D);
	}


	SetTransform(D3DTS_PROJECTION, &Ortho2D);
/*
	int &indexCommand = iUpdate3dCoords;
	int &lastindexCommand = lastiUpdate3dCoords;


#ifdef BATCH_OPTIMISE
	if(indexCommand > batch_lastdraw)
	{
		CBatch_Update3dCoords* Command = dynamic_cast<CBatch_Update3dCoords*>(batch[indexCommand]);
		if(lastindexCommand != -1)
		{
			CBatch_Update3dCoords* LastCommand = dynamic_cast<CBatch_Update3dCoords*>(batch[lastindexCommand]);

			if(LastCommand->SameAs(Ortho2D))
			{
				batch[indexCommand] = NULL;
				while(batch.size() && batch.back() == NULL)
					batch.pop_back();
				indexCommand = lastindexCommand;
				lastindexCommand = -1;
				return;
			}
		}
		Command->Ortho2D = Ortho2D;
	}
	else
#endif
	{
		lastindexCommand = indexCommand;
		indexCommand = batch.size();

		CBatch_Update3dCoords* pBatch = new (this) CBatch_Update3dCoords(Ortho2D);
		batch.push_back((CBatchBase*)pBatch);
	}
*/
#endif
	


}

int CD3DDisplay::GetRenderTarget()
{
	return renderTex;
}

void CD3DDisplay::Blit(float x, float y, int w, int h, float angle, D3DCOLOR filter, const RECTF* uv)
{
	// Use the rect blit.
	RECTF rc;
	SetRectF(&rc, x, y, x+w, y+h);
	Blit(rc, angle, filter, uv);
}

void CD3DDisplay::Blit(float x, float y, float xf, float yf, float angle, D3DCOLOR filter, const RECTF* uv)
{
	RECTF rc;
	SetRectF(&rc, x, y, x + TexInfo(curTexture)->imgwidth, y + TexInfo(curTexture)->imgheight);

	Quadrilateral q;

	if (angle == 0.0f) {
		if (xf == 1.0f && yf == 1.0f)
			q = rc;
		else
			q.Stretched(rc, xf, yf);
	}
	else {
		if (xf == 1.0f && yf == 1.0f)
			q.Rotated(rc, angle);
		else
			q.StretchRotated(rc, xf, yf, angle);
	}

	BlitTex(q, MixFilters(globalcolor, filter), uv);
}

void CD3DDisplay::Blit(float x, float y, int w, int h, float angle, D3DCOLOR filter, D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4, const RECTF* uv)
{
	// Use the rect blit.
	RECTF rc;
	SetRectF(&rc, x, y, x+w, y+h);
	Blit(rc, angle,filter, f1,f2,f3,f4, uv);
}

void CD3DDisplay::Blit(float x, float y, float xf, float yf, float angle, D3DCOLOR filter, D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4, const RECTF* uv)
{
	RECTF rc;
	SetRectF(&rc, x, y, x + TexInfo(curTexture)->imgwidth, y + TexInfo(curTexture)->imgheight);

	Quadrilateral q;

	if (angle == 0.0f) {
		if (xf == 1.0f && yf == 1.0f)
			q = rc;
		else
			q.Stretched(rc, xf, yf);
	}
	else {
		if (xf == 1.0f && yf == 1.0f)
			q.Rotated(rc, angle);
		else
			q.StretchRotated(rc, xf, yf, angle);
	}

	f1 = MixFilters(globalcolor, MixFilters(filter,f1));
	f2 = MixFilters(globalcolor, MixFilters(filter,f2));
	f3 = MixFilters(globalcolor, MixFilters(filter,f3));
	f4 = MixFilters(globalcolor, MixFilters(filter,f4));

	BlitTex(q,f1,f2,f3,f4, uv);
}

void CD3DDisplay::Blit(RECTF& rc, float angle, D3DCOLOR filter,D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4, const RECTF* uv, float skewX, float skewY)
{
	// Stretch the rect so real image size equals target image size
	Quadrilateral q;
	q.width = rc.right - rc.left;
	q.height = rc.bottom - rc.top;
	if (angle == 0.0f) {
		q = rc;
		q.Skew(skewX, skewY);
	}
	else
		q.Rotated(rc, angle, skewX, skewY);

	// Draw
	f1 = MixFilters(globalcolor, MixFilters(filter,f1));
	f2 = MixFilters(globalcolor, MixFilters(filter,f2));
	f3 = MixFilters(globalcolor, MixFilters(filter,f3));
	f4 = MixFilters(globalcolor, MixFilters(filter,f4));

	BlitTex(q, f1, f2, f3, f4, uv);
}

void CD3DDisplay::Blit(RECTF& rc, float angle, D3DCOLOR filter, const RECTF* uv)
{
	// Stretch the rect so real image size equals target image size
	Quadrilateral q;
	q.width = rc.right - rc.left;
	q.height = rc.bottom - rc.top;
	if (angle == 0.0f)
		q = rc;
	else
		q.Rotated(rc, angle);

	// Draw
	BlitTex(q, MixFilters(globalcolor, filter), uv);
}

inline void CD3DDisplay::Blit(Quadrilateral& quad, D3DCOLOR filter, const RECTF* uv)
{
	BlitTex(quad, MixFilters(globalcolor, filter), uv);
}

// Mix filters: e.g. if f1 is 50% red and f2 is 60% red, the result is a 30% red filter
D3DCOLOR CD3DDisplay::MixFilters(D3DCOLOR f1, D3DCOLOR f2)
{
	// If either of the filters does not affect, return the other filter
	if (f1 == 0xFFFFFFFF) return f2;
	if (f2 == 0xFFFFFFFF) return f1;

	// Else mix the filters
	float afilter = (float((f1 & 0xFF000000) >> 24) / 256.0f) * (float((f2 & 0xFF000000) >> 24) / 256.0f);
	afilter *= 256.0f;
	float rfilter = (float((f1 & 0x00FF0000) >> 16) / 256.0f) * (float((f2 & 0x00FF0000) >> 16) / 256.0f);
	rfilter *= 256.0f;
	float gfilter = (float((f1 & 0x0000FF00) >> 8) / 256.0f) * (float((f2 & 0x0000FF00) >> 8) / 256.0f);
	gfilter *= 256.0f;
	float bfilter = (float(f1 & 0x000000FF) / 256.0f) * (float(f2 & 0x000000FF) / 256.0f);
	bfilter *= 256.0f;

	D3DCOLOR mixedfilter = ((int)afilter << 24) + ((int)rfilter << 16) + ((int)gfilter << 8) + ((int)bfilter);
	return mixedfilter;
}

/*
inline void SetVertex(TLVERTEX* vertex, D3DCOLOR color, float x, float y, float u, float v)
{
	vertex->colour = color;
	vertex->x = x;
	vertex->y = y;
	vertex->z = 0.0f;
#ifndef USE_3D_CORDS
	vertex->rhw = 1.0f;
#endif
	vertex->u = u;
	vertex->v = v;
}
*/

void CD3DDisplay::AddVertex(D3DCOLOR color, float x, float y, float u, float v)
{
	verticesVec.push_back(TLVERTEX());
	TLVERTEX& vx = verticesVec.back();

	vx.colour = color;
	vx.x = x;
	vx.y = y;
	vx.z = 0.0f;
#ifndef USE_3D_CORDS
	vx.rhw = 1.0f;
#endif
	vx.u = u;
	vx.v = v;
}

void CD3DDisplay::AddVertex3d(D3DCOLOR color, float x, float y, float z, float u, float v)
{
	verticesVec.push_back(TLVERTEX());
	TLVERTEX& vx = verticesVec.back();

	vx.colour = color;
	vx.x = x;
	vx.y = y;
	vx.z = z;
#ifndef USE_3D_CORDS
	vx.rhw = 1.0f;
#endif
	vx.u = u;
	vx.v = v;
}

void CD3DDisplay::AddIndex(unsigned short i)
{
	indicesVec.push_back(i);
}

// Blit a texture-dimensioned image with image-wide filtering
inline void CD3DDisplay::BlitTex(Quadrilateral& q, D3DCOLOR filter, const RECTF* uv)
{
	BlitTex(q, filter, filter, filter, filter, uv);
}

void CD3DDisplay::BlitTex(Quadrilateral& q, D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4, const RECTF* uv)
{
	// Check for same rendertarget/texture (DX complains about this)
	if (GetTexture() != -1 && GetTexture() == GetRenderTarget())
		throw CD3DError("An invalid rendering operation was attempted.  This is a bug in the display engine, please report it to Scirra.");

	BeginBatch(BATCH_QUADS,0);

	// Adjust UV co-ords to pick out the image area
	RECTF u;
	u.left = 0.0f;
	u.top = 0.0f;
	float xf = 1.0f;
	float yf = 1.0f;

	if (curTexture > -1) {
		xf = TexInfo(curTexture)->xf;
		yf = TexInfo(curTexture)->yf;
		float xoffset = 0, yoffset = 0;

		u.right = xf;
		u.bottom = yf;
	}
	else {
		u.right = 0.0f;
		u.bottom = 0.0f;
	}

	//Setup vertices
	if (uv == NULL) {
		AddVertex(f1, q.p1.x, q.p1.y, u.left, u.top);
		AddVertex(f2, q.p2.x, q.p2.y, u.right,u.top);
		AddVertex(f3, q.p3.x, q.p3.y, u.left, u.bottom);
		AddVertex(f4, q.p4.x, q.p4.y, u.right, u.bottom);

		unsigned short index = batch.back()->batchCount;
		AddIndex(index);
		AddIndex(index + 1);
		AddIndex(index + 2);
		AddIndex(index + 3);
		AddIndex(index + 2);
		AddIndex(index + 1);
	}
	else {
		RECTF uv2 = *uv;
		uv2.left *= xf;
		uv2.right *= xf;
		uv2.top *= yf;
		uv2.bottom *= yf;
		AddVertex(f1, q.p1.x, q.p1.y, uv2.left, uv2.top);
		AddVertex(f2, q.p2.x, q.p2.y, uv2.right,uv2.top);
		AddVertex(f3, q.p3.x, q.p3.y, uv2.left, uv2.bottom);
		AddVertex(f4, q.p4.x, q.p4.y, uv2.right,uv2.bottom);

		unsigned short index = batch.back()->batchCount;
		AddIndex(index);
		AddIndex(index + 1);
		AddIndex(index + 2);
		AddIndex(index + 3);
		AddIndex(index + 2);
		AddIndex(index + 1);
	}

	batch.back()->batchCount += 4;
	batchIndex += 4;
	batch.back()->indexCount += 6;
}

// Set current texture for Blit operations
void CD3DDisplay::SetTexture(int index)
{
	if(curTexture == index)
		return;
	
	//EndBatch();

	IDirect3DBaseTexture9* texture = NULL;
	curTexture = index;

	if(index != -1)
		texture = GetTexture(index)->texture;

	int &indexCommand = iSetTexture;
	int &lastindexCommand = lastiSetTexture;

#ifdef BATCH_OPTIMISE
	if(indexCommand > batch_lastdraw)
	{
		CBatch_SetTexture* Command = dynamic_cast<CBatch_SetTexture*>(batch[indexCommand]);
		if(lastindexCommand != -1)
		{
			CBatch_SetTexture* LastCommand = dynamic_cast<CBatch_SetTexture*>(batch[lastindexCommand]);

			if(LastCommand->SameAs(texture))
			{
				batch[indexCommand] = NULL;
				while(batch.size() && batch.back() == NULL)
					batch.pop_back();
				indexCommand = lastindexCommand;
				lastindexCommand = -1;
				return;
			}
		}
		Command->texture = texture;
	}
	else
#endif
	{
		lastindexCommand = indexCommand;
		indexCommand = batch.size();
		CBatch_SetTexture* pBatch = new (this) CBatch_SetTexture(texture);
		batch.push_back((CBatchBase*)pBatch);
	}


	//CBatch_SetTexture* pBatch = new CBatch_SetTexture(texture);
	//batch.push_back((CBatchBase*)pBatch);

	//EndBatch();
}

int CD3DDisplay::GetTexture()
{
	return curTexture;
}

// Delete a texture and invalidate its reference
void CD3DDisplay::RemoveTexture(int iTexture)
{
	EndBatch();

	TextureInfo* texInfo = textures[iTexture];

	if (texInfo->rendertarget)
		vramtargetbytes -= texInfo->texwidth * texInfo->texheight * 4;
	else
		vramtexturebytes -= texInfo->texwidth * texInfo->texheight * 4;

	hr = texInfo->texture->Release();
	ASSERT(SUCCEEDED(hr));
	delete texInfo;
	textures.erase(iTexture);
}

// Decrement the reference count of a texture, and RemoveTexture() if it hits 0
void CD3DDisplay::DecrementTextureReference(int iTexture, bool deleteIfZero)
{
	EndBatch();

	TextureInfo* texInfo = textures[iTexture];

	texInfo->referenceCount--;

	if (texInfo->referenceCount == 0 && deleteIfZero)
		RemoveTexture(iTexture);
}

// Increment reference count
void CD3DDisplay::IncrementTextureReference(int iTexture, int references /* = 1 */)
{
	TexInfo(iTexture)->referenceCount += references;
}

// Shortcut to get TextureInfo pointer from index
TextureInfo* CD3DDisplay::TexInfo(int index)
{
	return GetTexture(index);
}

// Premultiply a texture
void CD3DDisplay::Premultiply(int& iTexture)
{
	EndBatch();

	int w = TexInfo(iTexture)->imgwidth;
	int h = TexInfo(iTexture)->imgheight;
	int oldRT = renderTex; // old rendertarget
	int oldTex = curTexture; // old texture

	int newTex = AddTextureRT(w,h);

	SetTexture(iTexture);
	SetRenderTarget(newTex);
	BeginScene();
	ClearRT();

	SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
	SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	Blit(0,0);

	SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA);
	SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	Blit(0,0);

	// Reset to std drawing
	SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
	SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (oldRT == iTexture)
		SetRenderTarget(newTex);
	else
		SetRenderTarget(oldRT);

	if (oldTex == iTexture)
		SetTexture(newTex);
	else
		SetTexture(oldTex);

	// Remove old texture and update parameter
	RemoveTexture(iTexture);
	iTexture = newTex;

	EndScene();
//	SetRenderTarget(-1);

}

void CD3DDisplay::ReplaceTexture(int oldTex, int newTex)
{
	EndBatch();

	TextureInfo* previous = GetTexture(oldTex);
	TextureInfo* next = GetTexture(newTex);

	// Release old texture
	previous->texture->Release();

	if (previous->rendertarget)
		vramtargetbytes -= previous->texwidth * previous->texheight * 4;
	else
		vramtexturebytes -= previous->texwidth * previous->texheight * 4;

	// Copy across texture info
	*previous = *next;

	// Delete newtex
	delete textures[newTex];
	textures.erase(newTex);
}

// Convert to RT
void CD3DDisplay::ConvertToRT(int& iTexture)
{
	EndBatch();

	int w = TexInfo(iTexture)->imgwidth;
	int h = TexInfo(iTexture)->imgheight;
	int oldRT = renderTex; // old rendertarget
	int oldTex = curTexture; // old texture

	int newTex = AddTextureRT(w,h);

	SetTexture(iTexture);
	SetRenderTarget(newTex);
	BeginScene();
	ClearRT();

	SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
	SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	Blit(0,0);

	// Reset to std drawing
	SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
	SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (oldRT == iTexture)
		SetRenderTarget(newTex);
	else
		SetRenderTarget(oldRT);

	if (oldTex == iTexture)
		SetTexture(newTex);
	else
		SetTexture(oldTex);

	// Remove old texture and update parameter
	RemoveTexture(iTexture);
	iTexture = newTex;

	EndScene();
//	SetRenderTarget(-1);

}


// Set global filter fx
void CD3DDisplay::SetGlobalFilter(D3DCOLOR color)
{
	EndBatch();
	globalcolor = color;
}

// Set global filter fx
void CD3DDisplay::SetGlobalFilter(unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue)
{
	EndBatch();
	globalcolor = (alpha << 24) + (red << 16) + (green << 8) + blue;
}

void CD3DDisplay::SetTextQuality(DWORD quality)
{
	textQuality = quality;
}

void CD3DDisplay::UpdateAllExistingFonts()
{
	vector<class CD3DDisplayFont*>::iterator i = fontRefs.begin();

	for ( ; i != fontRefs.end(); i++) {
		(*i)->d3dxfont->Release();

		D3DXCreateFont(d3dDevice, (*i)->pxSize, 0, (*i)->bold, 0, (*i)->italics,
				   DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, textQuality, DEFAULT_PITCH|FF_DONTCARE,
				   (*i)->fontface, &((*i)->d3dxfont));
	}
}

// CreateFont wrapper
void CD3DDisplay::CreateFont(const char* fontface, unsigned int pxSize, bool italics, bool bold, CD3DDisplayFont* pFont)
{
	EndBatch();
	/*
	D3DXCreateFont(d3dDevice, pxSize, 0, (bold ? FW_BOLD : FW_NORMAL), 0, italics,
				   DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE,
				   fontface, &(pFont->d3dxfont));*/
	D3DXCreateFont(d3dDevice, pxSize, 0, (bold ? FW_BOLD : FW_NORMAL), 0, italics,
				   DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, textQuality, DEFAULT_PITCH|FF_DONTCARE,
				   fontface, &(pFont->d3dxfont));

	pFont->pxSize = pxSize;
	pFont->bold = (bold ? FW_BOLD : FW_NORMAL);
	pFont->italics = italics;
	pFont->fontface = fontface;

	pFont->pDisplay = this;
	push_back_unique(fontRefs, pFont);
}

void CD3DDisplay::DrawText(CD3DDisplayFont& font, const char* text, RECT& rc, D3DCOLOR color, DWORD format)
{
	EndBatch();

	RECTF rcf;
	rcf.left = rc.left;
	rcf.top = rc.top;
	rcf.right = rc.right;
	rcf.bottom = rc.bottom;
	DrawText(font, text, rcf, color, format);
}

// Render text
void CD3DDisplay::DrawText(CD3DDisplayFont& font, const char* text, RECTF& rc, D3DCOLOR color, DWORD format)
{
	if (format & DT_CALCRECT) {
		RECT rect;
		SetRect(&rect, rc.left, rc.top, rc.right, rc.bottom);

		font.d3dxfont->DrawText(m_pD3DXSprite, text, -1, &rect, format, MixFilters(globalcolor, color));

		rc.left = rect.left;
		rc.top = rect.top;
		rc.right = rect.right;
		rc.bottom = rect.bottom;
	}
	else {
		// Floating point text rendering
		RECT rect;
		SetRect(&rect, rc.left, rc.top, rc.right, rc.bottom);

		EndBatch();

		m_pD3DXSprite->Begin(D3DXSPRITE_OBJECTSPACE|D3DXSPRITE_ALPHABLEND);

		// For floating point offsets
		D3DXMATRIX offset, scale;
		D3DXMatrixTranslation(&offset, rc.left - floor(rc.left), rc.top - floor(rc.top), 0.0);
		m_pD3DXSprite->SetTransform(&offset);

		
		font.d3dxfont->DrawText(m_pD3DXSprite, text, -1, &rect, format, MixFilters(globalcolor, color));
		m_pD3DXSprite->End();	
	
	}

}

// Fill rect
void CD3DDisplay::Fill(const RECTF& rc, D3DCOLOR color)
{
	// No texture for flood fill
	SetTexture(-1);

	Quadrilateral q;
	q = rc;

	BlitTex(q, MixFilters(color, globalcolor));
}

void CD3DDisplay::Fill(const RECT& rc, D3DCOLOR color)
{
	RECTF rcf;
	rcf.left = rc.left;
	rcf.top = rc.top;
	rcf.right = rc.right;
	rcf.bottom = rc.bottom;
	Fill(rcf, color);
}

// Fill rect overload
void CD3DDisplay::Fill(int x, int y, int w, int h, D3DCOLOR color)
{
	// No texture for flood fill
	SetTexture(-1);

	RECTF rc;
	SetRectF(&rc, x, y, x + w, y + h);

	Quadrilateral q;
	q = rc;

	BlitTex(q, MixFilters(color, globalcolor));
}

void CD3DDisplay::FillGradient(const RECT& rc, D3DCOLOR color1, D3DCOLOR color2, Direction direction)
{
	RECTF rcf;
	rcf.left = rc.left;
	rcf.top = rc.top;
	rcf.right = rc.right;
	rcf.bottom = rc.bottom;
	FillGradient(rcf, color1, color2, direction);
}

// Fill a rect with a gradient, from color1 to color2
void CD3DDisplay::FillGradient(const RECTF& rc, D3DCOLOR color1, D3DCOLOR color2, Direction direction)
{
	// No texture
	SetTexture(-1);

	color1 = MixFilters(color1, globalcolor);
	color2 = MixFilters(color2, globalcolor);

	Quadrilateral q;
	q = rc;

	switch(direction) {
	case DIR_UP:
		BlitTex(q, color2, color2, color1, color1);
		break;
	case DIR_DOWN:
		BlitTex(q, color1, color1, color2, color2);
		break;
	case DIR_LEFT:
		BlitTex(q, color2, color1, color2, color1);
		break;
	case DIR_RIGHT:
		BlitTex(q, color1, color2, color1, color2);
		break;
	}
}

// Fill gradient with x,y,w,h
void CD3DDisplay::FillGradient(int x, int y, int w, int h, D3DCOLOR color1, D3DCOLOR color2, Direction direction)
{
	RECTF rc;
	SetRectF(&rc, x, y, x + w, y + h);
	FillGradient(rc, color1, color2, direction);
}

// Set a RECTF
void SetRectF(RECTF* prc, float xLeft, float yTop, float xRight, float yBottom)
{
	if (prc != NULL) {
		prc->left = xLeft;
		prc->top = yTop;
		prc->right = xRight;
		prc->bottom = yBottom;
	}
}

// Draw a point
void CD3DDisplay::Point(float x, float y, D3DCOLOR color)
{
	BeginBatch(BATCH_POINTS,0);
	AddVertex(color, x, y, 0.0f, 0.0f);

	batch.back()->batchCount += 1;
	batchIndex += 1;
}

// Draw a line
void CD3DDisplay::Line(float x1, float y1, float x2, float y2, D3DCOLOR color1, D3DCOLOR color2)
{
	x1 += 0.5f;
	x2 += 0.5f;
	y1 += 0.5f;
	y2 += 0.5f;
	// I dont know why...but lines still seem to need the offset otherwise they appear blurred.

	BeginBatch(BATCH_LINES,0);
	AddVertex(color1, x1, y1, 0.0f, 0.0f);
	AddVertex(color2, x2, y2, 1.0f, 1.0f);

	batch.back()->batchCount += 2;
	batchIndex += 2;
}

void CD3DDisplay::SmoothLineBegin()
{
	// Can't batch smooth lines using ID3DXLine
	EndBatch();

	if (lineDrawer == NULL) {
		D3DXCreateLine(d3dDevice, &lineDrawer);
		lineDrawer->SetAntialias(TRUE);
	}

	lineDrawer->Begin();
}

void CD3DDisplay::SmoothLineEnd()
{
	if (lineDrawer)
		lineDrawer->End();
}

void CD3DDisplay::SmoothLine(float x1, float y1, float x2, float y2, D3DCOLOR color)
{
	// Added antialiased lines using D3DX.  Calling ID3DXLine::Begin / End will optimise many line draws, but
	// it's difficult to figure out how to batch that...
	D3DXVECTOR3 coords[2];
	coords[0].x = x1;
	coords[0].y = y1;
	coords[0].z = 0;
	coords[1].x = x2;
	coords[1].y = y2;
	coords[1].z = 0;
	lineDrawer->DrawTransform(coords, 2, &Ortho2D, color);
}

void CD3DDisplay::Line(float x1, float y1, float x2, float y2, D3DCOLOR color)
{
	Line(x1, y1, x2, y2, color, color);
}

// Draw a box of lines
void CD3DDisplay::Box(int x, int y, int w, int h, D3DCOLOR color)
{
	RECTF rc;
	SetRectF(&rc, x, y, x+w, y+h);
	Box(rc, color);
}

void CD3DDisplay::Box(RECT rc, D3DCOLOR color)
{
	RECTF rcf;
	rcf.left = rc.left;
	rcf.top = rc.top;
	rcf.right = rc.right;
	rcf.bottom = rc.bottom;
	Box(rcf, color);
}

// Rect overload
void CD3DDisplay::Box(RECTF rc, D3DCOLOR color)
{
	// DAVO CHANGES: A box of width 3 will have a width of 2 due
	// to tigs being a noob. Now Fixed
		// When batching, add some line batches for the box
		Line(rc.left,	 rc.top,      rc.right-1, rc.top,      color);
		Line(rc.right-1, rc.top,      rc.right-1, rc.bottom-1, color);
		Line(rc.right-1, rc.bottom-1, rc.left,    rc.bottom-1, color);
		Line(rc.left,    rc.bottom-1, rc.left,    rc.top,      color);
}

// Draw the outlines of a Quad
void CD3DDisplay::Quad(Quadrilateral q, D3DCOLOR color)
{
	// Batch lines instead
	Line(q.p1.x, q.p1.y, q.p2.x, q.p2.y, color);
	Line(q.p2.x, q.p2.y, q.p4.x, q.p4.y, color);
	Line(q.p4.x, q.p4.y, q.p3.x, q.p3.y, color);
	Line(q.p3.x, q.p3.y, q.p1.x, q.p1.y, color);
}

// Get private vars
D3DCOLOR CD3DDisplay::GetGlobalFilter()
{
	return globalcolor;
}

IDirect3DDevice9* CD3DDisplay::GetD3DDevice() const
{

	return d3dDevice;
}

D3DCAPS9* CD3DDisplay::GetD3DCaps()
{
	return &d3dCaps;
}

/*
HWND CD3DDisplay::GetHwnd()
{
	return hWnd;
}
*/

void CBatch_SetBackBuffer::Do(CD3DDisplay* pDisplay)
{
	if (pSurf == NULL)
		pDisplay->pBackBuffer = pDisplay->pDisplaySurface;
	else
		pDisplay->pBackBuffer = pSurf;

	pDisplay->changedBackBuffer = true;
}

void CD3DDisplay::SetBackBuffer(LPDIRECT3DSURFACE9 pSurf)
{
	//EndBatch();

	batch.push_back(new (this) CBatch_SetBackBuffer(pSurf));
}

void CD3DDisplay::SetAdditiveBlending()
{
	SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

// For premultiplied alpha
void CD3DDisplay::SetAlphaBlending()
{
	SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

unsigned long	CD3DDisplay::GetRenderState(D3DRENDERSTATETYPE rstype)
{
	//unsigned long value;
	//d3dDevice->GetRenderState(rstype,&value);
	if(rstype < 0 || rstype >= CD3D_NUM_RENDERSTATES)
		return NULL;
	return renderStates[rstype];
}
// States
void CD3DDisplay::SetRenderState(D3DRENDERSTATETYPE rstype, UINT value)
{
	//d3dDevice->SetRenderState(rstype, value);

	if(rstype < 0 || rstype >= CD3D_NUM_RENDERSTATES)
		return; // invalid type

	if(value == renderStates[rstype])
		return; // hasn't changed

	renderStates[rstype] = value;

	int &indexCommand = iRenderStates[rstype];
	int &lastindexCommand = lastiRenderStates[rstype];

#ifdef BATCH_OPTIMISE
	if(indexCommand > batch_lastdraw)
	{
		CBatch_SetRenderState* Command = dynamic_cast<CBatch_SetRenderState*>(batch[indexCommand]);
		if(lastindexCommand != -1)
		{
			CBatch_SetRenderState* LastCommand = dynamic_cast<CBatch_SetRenderState*>(batch[lastindexCommand]);

			if(LastCommand->SameAs(rstype, value))
			{
				batch[indexCommand] = NULL;
				while(batch.size() && batch.back() == NULL)
					batch.pop_back();
				indexCommand = lastindexCommand;
				lastindexCommand = -1;
				return;
			}
		}
		Command->rstype = rstype;
		Command->value = value;
	}
	else
#endif
	{
		lastindexCommand = indexCommand;
		indexCommand = batch.size();

		CBatch_SetRenderState* pBatch = new (this) CBatch_SetRenderState(rstype, value);
		batch.push_back((CBatchBase*)pBatch);
	}
	//EndBatch();
}

void CD3DDisplay::SetSamplerState(D3DSAMPLERSTATETYPE sstype, UINT value)
{
//	d3dDevice->SetSamplerState(0, sstype, value);


	if(sstype < 0 || sstype >= CD3D_NUM_SAMPLERSTATES)
		return; // invalid type

	if(value == samplerStates[sstype])
		return; // hasn't changed

	samplerStates[sstype] = value;

	int &indexCommand = iSamplerStates[sstype];
	int &lastindexCommand = lastiSamplerStates[sstype];

#ifdef BATCH_OPTIMISE
	if(indexCommand > batch_lastdraw)
	{
		CBatch_SetSamplerState* Command = dynamic_cast<CBatch_SetSamplerState*>(batch[indexCommand]);
		if(lastindexCommand != -1)
		{
			CBatch_SetSamplerState* LastCommand = dynamic_cast<CBatch_SetSamplerState*>(batch[lastindexCommand]);

			if(LastCommand->SameAs(sstype, value))
			{
				batch[indexCommand] = NULL;
				while(batch.size() && batch.back() == NULL)
					batch.pop_back();
				indexCommand = lastindexCommand;
				lastindexCommand = -1;
				return;
			}
		}
		Command->sstype = sstype;
		Command->value = value;
	}
	else
#endif
	{
		lastindexCommand = indexCommand;
		indexCommand = batch.size();

		CBatch_SetSamplerState* pBatch = new (this) CBatch_SetSamplerState(sstype, value);
		batch.push_back((CBatchBase*)pBatch);
	}
}

// States
void CD3DDisplay::SetTransform(D3DTRANSFORMSTATETYPE state, D3DXMATRIX* pMatrix)
{
	//d3dDevice->SetRenderState(rstype, value);

	if(state < 0 || state >= CD3D_NUM_TRANSFORMS)
		return; // invalid type

	if(*pMatrix == transforms[state])
		return; // hasn't changed

	transforms[state] = *pMatrix;

	int &indexCommand = iTransform[state];
	int &lastindexCommand = lastiTransform[state];

#ifdef BATCH_OPTIMISE
	if(indexCommand > batch_lastdraw)
	{
		CBatch_SetTransform* Command = dynamic_cast<CBatch_SetTransform*>(batch[indexCommand]);
		if(lastindexCommand != -1)
		{
			CBatch_SetTransform* LastCommand = dynamic_cast<CBatch_SetTransform*>(batch[lastindexCommand]);

			if(LastCommand->SameAs(state, pMatrix))
			{
				batch[indexCommand] = NULL;
				while(batch.size() && batch.back() == NULL)
					batch.pop_back();
				indexCommand = lastindexCommand;
				lastindexCommand = -1;
				return;
			}
		}
		Command->state = state;
		Command->matrix = *pMatrix;
	}
	else
#endif
	{
		lastindexCommand = indexCommand;
		indexCommand = batch.size();

		CBatch_SetTransform* pBatch = new (this) CBatch_SetTransform(state, pMatrix);
		batch.push_back((CBatchBase*)pBatch);
	}
	//EndBatch();
}

unsigned long CD3DDisplay::GetSamplerState(D3DSAMPLERSTATETYPE sstype)
{
	if(sstype < 0 || sstype >= CD3D_NUM_SAMPLERSTATES)
		return NULL;
	return samplerStates[sstype];

	//DWORD value;
	//d3dDevice->GetSamplerState(0, sstype, &value);
	//return value;
}
// Batching
// Begin batch: lock the batch buffer
void CD3DDisplay::ResetBatch()
{
	// Resize vectors back to zero (they keep their memory so this is still fast)
	verticesVec.resize(0);
	indicesVec.resize(0);

	batchIndex = 0;
	indexIndex = 0;

	iFxEndPass = -1;
	lastiFxEndPass = -1;
	
	iFxEnd = -1;
	lastiFxEnd = -1;
	
	iSetTexture = -1;
	lastiSetTexture = -1;

	iUpdate3dCoords = -1;
	lastiUpdate3dCoords = -1;

	iSetRenderTarget = -1;
	lastiSetRenderTarget = -1;



	for(int i = 0; i < CD3D_NUM_RENDERSTATES; i++)
		iRenderStates[i] = -1;
	for(int i = 0; i < CD3D_NUM_RENDERSTATES; i++)
		lastiRenderStates[i] = -1;

	for(int i = 0; i < CD3D_NUM_SAMPLERSTATES; i++)
		iSamplerStates[i] = -1;
	for(int i = 0; i < CD3D_NUM_SAMPLERSTATES; i++)
		lastiRenderStates[i] = -1;

	for(int i = 0; i < CD3D_NUM_TRANSFORMS; i++)
		iTransform[i] = -1;
	for(int i = 0; i < CD3D_NUM_TRANSFORMS; i++)
		lastiTransform[i] = -1;

}

void CD3DDisplay::BeginBatch(CD3DBATCH type, int num)
{
	// BeginBatch is now only called internally
	if(batchIndex >= CD3D_MAX_BATCH_SIZE-5)
		EndBatch();
	
	bool newbatch = false;

	if(batch.size() == 0)
		newbatch = true;
	else if (batch.back() == NULL)
		newbatch = true;
	else if (batch.back()->batchType != BATCH_NULL)
		newbatch = true;

	if(!batch.empty() && batch.back() != NULL && batch.back()->batchType == type)
		return;	

	if(newbatch)
	{
		CBatchBase* pBatch = new (this) CBatchBase();
		batch.push_back(pBatch);
	}
	CBatchBase* pBase = batch.back();
		
	batch_lastdraw = batch.size()-1;
	pBase->batchType = type;
}

void CD3DDisplay::ClearBatchData()
{
	// Call destructors for all batch memory objects
	vector<CBatchBase*>::iterator i = batch.begin();
	vector<CBatchBase*>::iterator end = batch.end();

	for ( ; i != end; i++)
		delete (*i);

	// Reset the custom allocator implicitly freeing all memory
	BatchMemoryReset();

	// Clear all commands
	batch.resize(0);

	indexIndex = 0;
	batchIndex = 0;
}

void CD3DDisplay::OptimiseBatchStack()
{
	// This function works by applying the following optimisations
	// 1 - If you end an operation, and begin the same one (and no vertex drawing has occured between) then remove the end and begin


	

	int count = 0;
	vector<CBatchBase*>::iterator i = batch.begin();
	for (vector<CBatchBase*>::iterator v = batch.begin(); v!= batch.end(); v++)
	{
		if(*v)
		{
			*i = *v;
			count ++;
			i++;
		}
	}
	batch.resize(count);



	return;
/*	
	CBatchBase**		pFxEndPass	= NULL;
	CBatch_Fx_EndPass*	iFxEndPass	= NULL;
	
	CBatchBase**		pFxEnd		= NULL;
	CBatch_Fx_End*		iFxEnd		= NULL;

	for(int i = 0; i < batch.size(); i++)
	{
		CBatchBase** ppBatchBase = &batch.at(i);
		CBatchBase* pBatchBase = batch.at(i);

		if(dynamic_cast<CBatch_Fx_EndPass*>(pBatchBase))
		{
			pFxEndPass = ppBatchBase;
			iFxEndPass = (CBatch_Fx_EndPass*)pBatchBase;
		}

		if(dynamic_cast<CBatch_Fx_End*>(pBatchBase))
		{
			pFxEnd = ppBatchBase;
			iFxEnd = (CBatch_Fx_End*)pBatchBase;
		}
			
		if(dynamic_cast<CBatch_Fx_StateEnd*>(pBatchBase))
		{
			pFxStateEnd = ppBatchBase;
			iFxStateEnd = (CBatch_Fx_StateEnd*)pBatchBase;
		}	
		if(CBatch_Fx_BeginPass* iFxBeginPass =  dynamic_cast<CBatch_Fx_BeginPass*>(pBatchBase))
		{
			if(iFxEndPass)
			{
				if( iFxBeginPass->curEff == iFxEndPass->curEff 
					&& iFxBeginPass->i == iFxEndPass->i )
				{	

					delete iFxEndPass;
					*pFxEndPass = NULL;
					pFxEndPass = NULL;
					iFxEndPass = NULL;

					CBatchBase* rep = new CBatchBase();
					rep->batchCount = pBatchBase->batchCount;
					rep->indexCount = pBatchBase->indexCount;
					rep->batchType = pBatchBase->batchType;
					delete pBatchBase;
					pBatchBase = rep;
						
					*ppBatchBase = pBatchBase;
				}
			}
		}
		else if(CBatch_Fx_Begin* iFxBegin =  dynamic_cast<CBatch_Fx_Begin*>(pBatchBase))
		{
			if(iFxEnd)
			{
				if( iFxBegin->curEff == iFxEnd->curEff)
				{		
					delete iFxEnd;
					*pFxEnd = NULL;
					pFxEnd = NULL;
					iFxEnd = NULL;

					CBatchBase* rep = new CBatchBase();
					rep->batchCount = pBatchBase->batchCount;
					rep->indexCount = pBatchBase->indexCount;
					rep->batchType = pBatchBase->batchType;
					delete pBatchBase;
					pBatchBase = rep;
						
					*ppBatchBase = pBatchBase;
				}
			}
		}
		else if(CBatch_Fx_StateBegin* iFxStateBegin =  dynamic_cast<CBatch_Fx_StateBegin*>(pBatchBase))
		{
			if(iFxStateEnd)
			{
				if( iFxStateBegin->SrcBlend == iFxStateEnd->SrcBlend
					&& iFxStateBegin->DestBlend == iFxStateEnd->DestBlend)
				{		
					delete iFxStateEnd;
					*pFxStateEnd = NULL;
					pFxStateEnd = NULL;
					iFxStateEnd = NULL;

					//CBatchBase* rep = new CBatchBase();
					//rep->batchCount = pBatchBase->batchCount;
					//rep->indexCount = pBatchBase->indexCount;
					//rep->batchType = pBatchBase->batchType;
					delete pBatchBase;
					pBatchBase = NULL; // rep;
						
					*ppBatchBase = pBatchBase;
				}
			}
		}

		if(pBatchBase->batchType != BATCH_NULL)
		{
			// If we draw something we must set all our pointers to null as we cant optimise anything before this point
			iFxEndPass = NULL;
			pFxEndPass = NULL;
			iFxEnd = NULL;
			pFxEnd = NULL;
			iFxStateEnd = NULL;
			pFxStateEnd = NULL;
		}

	}*/
}


// End batch
void CD3DDisplay::EndBatch()
{
	if(batch.size())
	{
		batchesPerPresent++;

		// Lock exactly the right number of indices and vertices
		TLVERTEX* vertexMem;
		unsigned short* indexMem;

		unsigned int vertexBytes = verticesVec.size() * sizeof(TLVERTEX);
		unsigned int indexBytes = indicesVec.size() * sizeof(unsigned short);

		// Update only when bytes are to be copied (D3D interprets 0 as 'lock entire buffer'!)
		if (vertexBytes != 0) {

			batchBuffer->Lock(0, vertexBytes, (void**)&vertexMem, D3DLOCK_DISCARD);
			memcpy(vertexMem, &(verticesVec.front()), vertexBytes);
			batchBuffer->Unlock();
		}
		if (indexBytes != 0) {

			indexBuffer->Lock(0, indexBytes, (void**)&indexMem, D3DLOCK_DISCARD);
			memcpy(indexMem, &(indicesVec.front()), indexBytes);
			indexBuffer->Unlock();
		}

		batchRunIndex = 0;
		indexRunIndex = 0;

		//OptimiseBatchStack(); // leave null items

		vector<CBatchBase*>::iterator i = batch.begin();
		vector<CBatchBase*>::iterator end = batch.end();

		for( ; i != end; i++)
		{
			if(*i)
			{
				(*i)->Do(this);
				DoBatch(*i);
			}
		}

		ClearBatchData();
		//DeleteBatchData();

		// Loop batching
		ResetBatch();
	}
}

//////////////////////////////////////////////
// CD3DError functions
CD3DError::CD3DError()
{
	hr = 0;
	isHr = false;
}

CD3DError::CD3DError(CString msg)
{
	hr = 0;
	isHr = false;
	errStr = msg;
}

CD3DError::CD3DError(HRESULT _hr)
{
	hr = _hr;
	isHr = true;
}

CString CD3DError::GetString()
{
	if (isHr) {
		switch (hr) {
		case D3D_OK:
			return "Universal Logic Fault: 1 = 0, please jump out your nearest wormhole";
		case D3DERR_DEVICELOST:
			return "The device has been lost";
		case D3DERR_DRIVERINTERNALERROR:
			return "There was a problem with your graphics drivers";
		case D3DERR_INVALIDCALL:
			return "An invalid call was made";
		case D3DXERR_INVALIDDATA:
			return "Invalid data";
		case D3DERR_OUTOFVIDEOMEMORY:
			{
				const float megabyte = 1024 * 1024;
				float targetMb = (float)CD3DDisplay::vramtargetbytes / megabyte;
				float texMb = (float)CD3DDisplay::vramtexturebytes / megabyte;

				CString msg;
				msg.Format("Out of video memory!\n\nConstruct has asked for the following amounts of VRAM:\n%.2fmb textures\n%.2fmb targets\n%.2fmb total", texMb, targetMb, targetMb + texMb);
				return msg;
			}
		case E_FAIL:
			return "Unspecified error";
		case E_OUTOFMEMORY:
			return "Out of memory";
		case 0x8876086a:
			return "3D not available; your graphics card does not support this application's drawing mode.";
		default: {
			CString s;
			s.Format("Unknown error %d", hr);
			return s;
			 }
		}
	}
	else
		return errStr;
}

//////////////////////////////////////////////
// QUADRILATERAL FUNCTIONS

// Create from stretched floating point rect
void Quadrilateral::Stretched(RECTF& rc, float xf, float yf)
{
	// Set from rc
	operator = (rc);

	// Width and height
	float w = rc.right - rc.left;
	float h = rc.bottom - rc.top;

	// p1 does not move

	// stretch p2 relative to p1
	p2.x = p1.x + (w * xf);

	// stretch p3 relative to p1
	p3.y = p1.y + (h * yf);

	// stretch p4 relative to p1
	p4.x = p2.x;
	p4.y = p3.y;

	
	// quick flip changes
	/*if( xf < 0)
	{
		p1.x -= (w * xf);
		p2.x -= (w * xf);
		p3.x -= (w * xf);
		p4.x -= (w * xf);
	}
	if( yf < 0)
	{
		p1.y -= (w * yf);
		p2.y -= (w * yf);
		p3.y -= (w * yf);
		p4.y -= (w * yf);
	}*/


}

// Create from rotated floating point rect
void Quadrilateral::Rotated(RECTF& rc, float angle)
{
	// Set from rc
	operator = (rc);

	if (angle == 0)
		return;

	// Covert mathematical angle to radians and flip (for bearing)
	angle = Rad(-angle);

	const float w = rc.right - rc.left;
	const float h = rc.bottom - rc.top;

	//const float sin_a_normal = sin(angle);
	//const float cos_a_normal = cos(angle);
	float sina, cosa;
	cr::sincosf(angle, &sina, &cosa);

	//const float weird = sin(-0.78539819);

	const float wcosa = w * cosa;
	const float wsina = w * sina;
	const float hsina = h * sina;
	const float hcosa = h * cosa;

	p2.x = p1.x + wcosa;
	p2.y = p1.y - wsina;

	p3.x = p1.x + hsina;
	p3.y = p1.y + hcosa;

	p4.x = p1.x + wcosa + hsina;
	p4.y = p1.y - wsina + hcosa;
}

void Quadrilateral::Skew(float skewX, float skewY)
{
	// Apply skew
	if (skewX != 0.0f) {
		p1.x += skewX;
		p2.x += skewX;
	}
	if (skewY != 0.0f) {
		p2.y += skewY;
		p4.y += skewY;
	}
}

// Create from rotated floating point rect
void Quadrilateral::Rotated(RECTF& rc, float angle, float skewX, float skewY)
{
	// No skew:  use faster overload
	//if (skewX == 0.0f && skewY == 0.0f) {
	//	Rotated(rc, angle);
	//	return;
	//}

	// Set from rc
	operator = (rc);

	float originX = rc.left;
	float originY = rc.top;

	// Apply skew before rotation
	Skew(skewX, skewY);

	// Covert mathematical angle to radians and flip (for bearing)
	angle = Rad(angle);

	// Rotate points individually - skew unaligns points
	p1.x -= originX;	p1.y -= originY;
	p2.x -= originX;	p2.y -= originY;
	p3.x -= originX;	p3.y -= originY;
	p4.x -= originX;	p4.y -= originY;

	RotatePoint(p1.x, p1.y, angle);
	RotatePoint(p2.x, p2.y, angle);
	RotatePoint(p3.x, p3.y, angle);
	RotatePoint(p4.x, p4.y, angle);

	p1.x += originX;	p1.y += originY;
	p2.x += originX;	p2.y += originY;
	p3.x += originX;	p3.y += originY;
	p4.x += originX;	p4.y += originY;
}

// Create from rotated floating point rect
void Quadrilateral::RotatedAll(RECTF& rc, float angle)
{
	// Set from rc
	operator = (rc);

	// Covert mathematical angle to radians and flip (for bearing)
	angle = Rad(angle);

	// Rotate points individually
	RotatePoint(p1.x, p1.y, angle);
	RotatePoint(p2.x, p2.y, angle);
	RotatePoint(p3.x, p3.y, angle);
	RotatePoint(p4.x, p4.y, angle);
}

// Create from stretched then rotated floating point rect
void Quadrilateral::StretchRotated(RECTF& rc, float xf, float yf, float angle)
{
	// First create from stretch
	Stretched(rc, xf, yf);

	// Make new rect
	RECTF stretched;
	stretched.left = p1.x;
	stretched.top = p1.y;
	stretched.right = p4.x;
	stretched.bottom = p4.y;

	// Create from rotated
	if (angle != 0)
		Rotated(stretched, angle);
}

// Create from stretched then rotated floating point rect
void Quadrilateral::StretchRotatedAll(RECTF& rc, float xf, float yf, float angle)
{
	// First create from stretch
	Stretched(rc, xf, yf);

	// Make new rect
	RECTF stretched;
	stretched.left = p1.x;
	stretched.top = p1.y;
	stretched.right = p4.x;
	stretched.bottom = p4.y;

	// Create from rotated
	if (angle != 0)
		RotatedAll(stretched, angle);	// Skew overload
}

void Quadrilateral::GetBoundingRect(RECTF& rc)
{
	rc.left = min(min(p1.x, p2.x), min(p3.x, p4.x));
	rc.right = max(max(p1.x, p2.x), max(p3.x, p4.x));
	rc.top = min(min(p1.y, p2.y), min(p3.y, p4.y));
	rc.bottom = max(max(p1.y, p2.y), max(p3.y, p4.y));
}

// Create from rectf
void Quadrilateral::operator =(RECTF rc)
{
	p1.x = rc.left;
	p1.y = rc.top;

	p2.x = rc.right;
	p2.y = rc.top;

	p3.x = rc.left;
	p3.y = rc.bottom;

	p4.x = rc.right;
	p4.y = rc.bottom;
}

// Create from rect
void Quadrilateral::operator =(RECT rc)
{
	p1.x = rc.left;
	p1.y = rc.top;

	p2.x = rc.right;
	p2.y = rc.top;

	p3.x = rc.left;
	p3.y = rc.bottom;

	p4.x = rc.right;
	p4.y = rc.bottom;
}

//////////////////////////////////////////////
// CD3DDISPLAYFONT FUNCTIONS
CD3DDisplayFont::CD3DDisplayFont()
{
	d3dxfont = NULL;
	pDisplay = NULL;
}

CD3DDisplayFont::~CD3DDisplayFont()
{
	if (d3dxfont)
		Destroy();
}

void CD3DDisplayFont::Destroy()
{
	if (d3dxfont) {
		d3dxfont->Release();
		d3dxfont = NULL;
	}

	if (pDisplay != NULL) {
		vector<CD3DDisplayFont*>::iterator f = find(pDisplay->fontRefs.begin(), pDisplay->fontRefs.end(), this);

		if (f != pDisplay->fontRefs.end())
			pDisplay->fontRefs.erase(f);
	}
}


void CD3DDisplay::DoBatch(CBatchBase* pBase)
{
	int& batchCount = pBase->batchCount;
	int& indexCount = pBase->indexCount;
	CD3DBATCH& batchType = pBase->batchType;

	// Smooth lines mode uses ID3DXLine to draw lines with antialiasing.
	/*
	if (smoothLines && batchType == BATCH_LINES) {

		if (lineDrawer == NULL) {
			D3DXCreateLine(d3dDevice, &line);
			lineDrawer->SetAntialias(TRUE);
		}

		lineDrawer->Begin();

		D3DXVECTOR2* coords = new D3DXVECTOR2[batchCount];
		coords[0].x = x1;
		coords[0].y = y1;
		coords[1].x = x2;
		coords[1].y = y2;
		line->Draw(coords, 2, color1);

		lineDrawer->End();
	}
	*/

	if(batchCount == 0)
		return; // Nothing to batch
	

	D3DPRIMITIVETYPE ptype;

	int primCount = pBase->batchCount;

	switch (batchType) 
	{

		case BATCH_POINTS:
			ptype = D3DPT_POINTLIST;
			break;
		case BATCH_LINESTRIP:
			ptype = D3DPT_LINESTRIP;
			primCount/=2;
			break;
		case BATCH_LINES:
		case BATCH_BOXES:
			primCount/=2;
			ptype = D3DPT_LINELIST;		
			break;
		case BATCH_QUADS:
			primCount=indexCount / 3;
			d3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, batchRunIndex, 0, batchCount, indexRunIndex, primCount);
			batchRunIndex += batchCount;
			indexRunIndex += indexCount;
			return; // 

		default:
			return;
	}

	d3dDevice->DrawPrimitive(ptype, batchRunIndex, primCount);
	batchRunIndex += batchCount;
	indexRunIndex += indexCount;
}

void CBatch_SetTexture::Do(CD3DDisplay* pDisplay)
{
	pDisplay->GetD3DDevice()->SetTexture(0, texture);
}

void CBatch_SetRenderState::Do(CD3DDisplay* pDisplay)
{
	pDisplay->GetD3DDevice()->SetRenderState(rstype, value);
}

void CBatch_SetSamplerState::Do(CD3DDisplay* pDisplay)
{
	pDisplay->GetD3DDevice()->SetSamplerState(0, sstype, value);
}

void CBatch_SetTransform::Do(CD3DDisplay* pDisplay)
{
	pDisplay->GetD3DDevice()->SetTransform(state, &matrix);
}
void CBatch_Fx_Begin::Do(CD3DDisplay* pDisplay)
{
	ID3DXEffect* pEffect = curEff->pEffect;

	if (curEff->vForegroundTexture)	pEffect->SetTexture(curEff->vForegroundTexture, params.foregroundTexture);
	if (curEff->vBackgroundTexture)	pEffect->SetTexture(curEff->vBackgroundTexture, params.backgroundTexture);
	if (curEff->vSourceTexture)		pEffect->SetTexture(curEff->vSourceTexture, params.sourceTexture);
	if (curEff->vPreviousTexture)	pEffect->SetTexture(curEff->vPreviousTexture, params.previousTexture);
	if (curEff->vFrameCounter)		pEffect->SetFloat(curEff->vFrameCounter, params.frameCounter);
	if (curEff->vBoxLeft)			pEffect->SetFloat(curEff->vBoxLeft, params.boxLeft);
	if (curEff->vBoxTop)			pEffect->SetFloat(curEff->vBoxTop, params.boxTop);
	if (curEff->vBoxRight)			pEffect->SetFloat(curEff->vBoxRight, params.boxRight);
	if (curEff->vBoxBottom)			pEffect->SetFloat(curEff->vBoxBottom, params.boxBottom);
	if (curEff->vBoxWidth)			pEffect->SetFloat(curEff->vBoxWidth, params.boxWidth);
	if (curEff->vBoxHeight)			pEffect->SetFloat(curEff->vBoxHeight, params.boxHeight);
	if (curEff->vHotspotX)			pEffect->SetFloat(curEff->vHotspotX, params.hotspotX);
	if (curEff->vHotspotY)			pEffect->SetFloat(curEff->vHotspotY, params.hotspotY);
	if (curEff->vPixelWidth)		pEffect->SetFloat(curEff->vPixelWidth, params.pixelWidth);
	if (curEff->vPixelHeight)		pEffect->SetFloat(curEff->vPixelHeight, params.pixelHeight);
	if (curEff->vBgStart)			pEffect->SetFloatArray(curEff->vBgStart, params.bgStart, 2);
	if (curEff->vBgEnd)				pEffect->SetFloatArray(curEff->vBgEnd, params.bgEnd, 2);

	vector<CustomParamBatch>::iterator i = params.customParams.begin();
	vector<CustomParamBatch>::iterator end = params.customParams.end();

	for ( ; i != end; i++)
		pEffect->SetFloat(i->handle, i->value);

	pEffect->Begin(NULL, 0);
}

void CBatch_Fx_BeginPass::Do(CD3DDisplay* pDisplay)
{
	curEff->BeginPass(i);
}

void CBatch_Fx_EndPass::Do(CD3DDisplay* pDisplay)
{
	curEff->EndPass();
}

void CBatch_Fx_End::Do(CD3DDisplay* pDisplay)
{
	curEff->End();
}

void CBatch_ClearRT::Do(CD3DDisplay* pDisplay)
{
	if (clearAll)
		pDisplay->GetD3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET, color, 0.0f, 0);
	else
		pDisplay->GetD3DDevice()->Clear(1, &clearrect, D3DCLEAR_TARGET, color, 0.0f, 0);
}

void CBatch_SetRenderTarget::Do(CD3DDisplay* pDisplay)
{
	if (tex == NULL)
		pDisplay->GetD3DDevice()->SetRenderTarget(0, pDisplay->pBackBuffer);
	else {
		IDirect3DSurface9* pSurf;
		tex->GetSurfaceLevel(0, &pSurf);
		pDisplay->GetD3DDevice()->SetRenderTarget(0, pSurf);
		pSurf->Release();
	}
}

void CBatch_Update3dCoords::Do(CD3DDisplay* pDisplay)
{
	pDisplay->GetD3DDevice()->SetTransform(D3DTS_PROJECTION, &Ortho2D);
}




#endif // APPRUNTIME


#endif // include once
