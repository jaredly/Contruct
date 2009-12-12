// CLayout.cpp
// All CLayout functions

#include "StdAfx.h"
#include "..\Structure.h"
#include "..\Dialogs\ImageEditorDlg.h"
#include "..\MainFrm.h"

extern CMainFrame* g_MainFrame;
bool g_SuspendFrameEditor;
extern BOOL g_bPasteClone;


inline void PremultiplyFilter(D3DCOLOR& filter)
{
	// Premultiply filter
	int a, r, g, b;
	float af;
	a = D3D_A(filter);
	af = (float)a / 255.0f;
	r = (float)D3D_R(filter) * af;
	g = (float)D3D_G(filter) * af;
	b = (float)D3D_B(filter) * af;

	filter = D3D_ARGB(a, r, g, b);
}

//
//  Important Note
//		This code was originally written for multiple frame editors having different CDisplays
//		However, this is not the case. Instead, m_LayoutEditor always contains the frame editor
//		which requested the operation - the others are set to null. The array should be modified
//		for clarity some time, but it does not harm atm
CLayout* g_pFrame;

CLayout::CLayout() :
	application_background(false),
	tree(NULL)
{
	Set(640, 480);
	SetColor(RGB(255,255,255));
	SetOpacity(100.0);

	// Layout title
	CString LayoutTitle;
	LayoutTitle.Format("%s 1", LAYOUT);
	m_Name = LayoutTitle;

	objects.InitHashTable(1423);

	m_unboundedScrolling = false;

	scroll_position = CPoint(0, 0);
	m_EventEditorScroll = CPoint(0, 0);

	m_LayoutEditor = NULL;
	application   = 0;
	m_Grid   = false;
	m_SnapMovements = false;
	m_SnapResize = false;
	m_GridWidth = 16;
	m_GridHeight = 16;
	m_ObjectFrameIsLockedTo = 0;
	m_ParentObjectFrameIsLockedTo = 0;

	m_oTemporyBehaviorInfo=0;
	m_pTempMovExt=0;

	m_oControlBehaviorInfo=0;	
	m_pControlBehaviorExt=0;
	m_pControlBehaviorData = 0;
	m_pControlBehaviorDataSize = 0;

	m_unboundedScrolling = false;
	m_bChanged = false;

	hdcTexture = -2;
	hdc = NULL;

	texture_loading = tl_use_app_setting;
}


void CLayout::Set( long w, long h )
{
    m_w = w;
    m_h = h;
}

long CLayout::GetW() const
{
    return m_w;
}

long CLayout::GetH() const
{
    return m_h;
}

CLayout::~CLayout() 
{
	CObj *o;
	POSITION pos = objects.GetStartPosition();
	long nKey;

	while (pos != NULL) 
	{ 
		// iterate through objects to choose one to select
		objects.GetNextAssoc( pos, nKey, o );
		delete o;		
	}

	objects.RemoveAll();

	CLayer *layer;
	pos = layers.GetHeadPosition();
	while (pos != NULL) 
	{ 
		layer = layers.GetNext(pos);
		delete layer;
	}

	layers.RemoveAll();
}

void CLayout::SetName(CString name_) 
{
	if (name_ == "")
		return;

	// check it's not in use by other layouts
	CLayout* layout;
	POSITION position = application->layouts.GetHeadPosition();

	for (int i = 0; i < application->layouts.GetCount(); i++)
	{ 
		layout = application->layouts.GetNext(position);
		
		if (layout->GetName() == name_)
			return;
	}

	m_Name = name_;
	m_WndName = name_;

	HWND ClientWindow;
	ClientWindow = ::GetWindow(g_MainFrame->m_hWndMDIClient, GW_CHILD);

	// Now find any layouts within this application
	while (ClientWindow != NULL)
	{
		CChildFrame* pWnd = (CChildFrame*)CWnd::FromHandle(ClientWindow);
		if(pWnd->layout == this)
		{
			// Rename
			CString Title;
			Title.Format("%s : %s", pWnd->application->file_information.name, m_Name);
			pWnd->SetWindowText(Title);
		}

		ClientWindow = :: GetWindow(ClientWindow, GW_HWNDNEXT);
	}

	// update project bar
	g_MainFrame->project_bar.tree.SetItemText(tree, name_);

	// Update windows displaying this potentially
	g_MainFrame->tabs.RedrawWindow();

}

void EventSheet::SetName(LPCSTR name_)
{
	// update tree
	g_MainFrame->project_bar.tree.SetItemText(second_tree, name);

	map<CLayout*, HTREEITEM>::iterator i = tree.begin();

	for ( ; i != tree.end(); i++)
		g_MainFrame->project_bar.tree.SetItemText(i->second, name);

	name = name_;
}

void CLayout::SetColor( COLORREF clr ) 
{
	m_clr = clr + (m_clr & 0xff000000);
}

void CLayout::SetOpacity( float op ) 
{
	m_clr = (((int)(255*(op/100.0))) << 24) + (m_clr & 0x00ffffff);
}

CString CLayout::GetName()
{
	return m_Name;
}

COLORREF CLayout::GetColor()
{
	if (application_background)
		return GetSysColor(COLOR_3DFACE) & 0x00ffffff;
	else
		return m_clr & 0x00ffffff;
}

float CLayout::GetOpacity()
{
	return ((m_clr & 0xff000000 )>> 24)   / 255.0 ;
}

bool CLayout::Serialize(CArchive& ar)
{
	CString ExpectedName = "CLayout";
	int     Version      = 5;

	if (!SerializeClassHeader(ExpectedName, Version, ar))
		return false;

	g_pFrame = this;
	if(ar.IsLoading())
	{
		ar >> identifier >> m_w >> m_h >> m_Name >> m_clr >> m_unboundedScrolling >> application_background;
		
		m_ObjectFrameIsLockedTo = 0;
		m_oTemporyBehaviorInfo=0;	// used inbetwen creation of tempory Behaviors for modifying properties - I use it so if an edittime function is called, we can work out from what object it is if the identifier is -1
		m_pTempMovExt=0;	// used inbetwen creation of tempory Behaviors for modifying properties - I use it so if an edittime function is called, we can work out from what object it is if the identifier is -1
		m_oControlBehaviorInfo=0;	// If you tell a Behavior to be in control of the frame editor this is used
		m_pControlBehaviorExt=0;// If you tell a Behavior to be in control of the frame editor this is used
		m_pControlBehaviorData=0;	// If you tell a Behavior to be in control of the frame editor, this is used to serialize to once unlocked.
		m_pControlBehaviorDataSize=0;
		m_ParentObjectFrameIsLockedTo=0;

		objects.RemoveAll();
		long objCnt;

		ar >> objCnt;
		CObj *o;
		int i;
		for (i = 0; i < objCnt; i++) 
		{
			long nKey;
			ar >> nKey;
			o = new CObj();

			if (!o->Serialize(ar))
				return false;

			objects.SetAt(nKey, o);
		}

		ar >> objCnt;
		CLayer *layer;
		for (i = 0; i < objCnt; i++)
		{
			layer = new CLayer("", LAYER_NORMAL);

			if (!layer->Serialize(ar))
				return false;

			layers.AddTail(layer);
			if (i == 1)
				current_layer = layer;
		}

		if (Version < 4)
		{
			// add non-layout layer if one doesn't exist
			if (layers.GetHead()->m_layerType != LAYER_NONFRAME)
			{
				CString layer_name;
				layer_name.Format("Non-layout");
				CLayer* nonlayout_layer = new CLayer(layer_name, LAYER_NONFRAME);
				nonlayout_layer->m_state = LAYSTATE_HIDDEN;

				nonlayout_layer->m_layerID = application->m_layerID++;
				layers.AddHead(nonlayout_layer);
			}
		}

		ar >> temporary_event_sheet_id;

		// This changed in v2; make sure the function knows
		if (Version >= 2)
			layoutKeys.Serialize(ar, true);
		else
			layoutKeys.Serialize(ar, false);

		// V3: save grid details
		if (Version >= 3) {
			ar >> m_Grid >> m_SnapMovements >> m_SnapResize >> m_GridWidth >> m_GridHeight;
		}

		if (Version >= 5) {
			int temp;
			ar >> temp;
			texture_loading = (LayoutTextureLoading)temp;
		}
	}
	
	else
	{
		if(m_pControlBehaviorExt)
		{
			bin movData(g_allocator);
			movData.allocate(1024);
			movData.loading = false;
			m_oControlBehaviorInfo->ETSerialize(m_pControlBehaviorExt, movData);

			*m_pControlBehaviorData = (BYTE*)movData.ptr();
			*m_pControlBehaviorDataSize = movData.size();

			movData.detach();
		}

		ar << identifier << m_w << m_h << m_Name << m_clr << m_unboundedScrolling << application_background;
					
		ar << objects.GetCount();

		CObj *o;
		POSITION pos = objects.GetStartPosition();

		// Fix expanding bug
		for (int i = 0; i < objects.GetCount(); i++) 
		{
			long nKey = 0;
			objects.GetNextAssoc(pos, nKey, o);

			ar << nKey;
			o->Serialize(ar);
		}
			
		ar << layers.GetCount();
		pos = layers.GetHeadPosition();
		for (int i=0; i < layers.GetCount(); i++)
			layers.GetNext(pos)->Serialize(ar);

		ar << event_sheet->identifier;
		layoutKeys.Serialize(ar, true);

		// V3: Save grid settings
		ar << m_Grid << m_SnapMovements << m_SnapResize << m_GridWidth << m_GridHeight;

		// V5: Save texture controls
		ar << (int)texture_loading;

		SetChanged(false);
	}

	SerializeClassPadding(10, 15, ar);

	return true;
}

void CLayout::SetChanged(bool bChanged)
{
	m_bChanged = bChanged;

	g_MainFrame->tabs.Invalidate();
}

bool CLayout::GetChanged()
{
	return m_bChanged;
}

CObj* CLayout::GetObj(long instanceID)
{
	CObjMap* objMap = &objects;

	CObj* o;
	POSITION pos = objMap->GetStartPosition();
	long nKey;
	while (pos != NULL)
	{ 
		// iterate through objects to choose one to select
		objMap->GetNextAssoc(pos, nKey, o);

		if (instanceID == o->GetInstanceID())
			return o;
	}

	return NULL;
}


void CLayout::Fill(RECT rc, D3DCOLOR color)
{
	if (!m_LayoutEditor)
		return;

	PremultiplyFilter(color);

		// Offset fill
	rc.left += FRAME_MARGIN_W*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollX;
	rc.right += FRAME_MARGIN_W*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollX;
	rc.top += FRAME_MARGIN_H*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollY;
	rc.bottom += FRAME_MARGIN_H*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollY;

	(*(m_LayoutEditor->m_display))->Fill(rc, color);
}

void CLayout::Line(int x1, int y1, int x2, int y2, D3DCOLOR color) 
{
	CLayout::Line(x1, y1, x2, y2, color, color);
}

void CLayout::Line(int x1, int y1, int x2, int y2, D3DCOLOR color1, D3DCOLOR color2)
{
	if (!m_LayoutEditor)
		return;

	PremultiplyFilter(color1);
	PremultiplyFilter(color2);

	x1 += FRAME_MARGIN_W*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollX;
	x2 += FRAME_MARGIN_W*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollX;
	y1 += FRAME_MARGIN_H*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollY;
	y2 += FRAME_MARGIN_H*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollY;

	(*(m_LayoutEditor->m_display))->Line(x1, y1, x2, y2, color1, color2);
}
void CLayout::Point(int x, int y, D3DCOLOR color)
{
	if (!m_LayoutEditor)
		return;

	PremultiplyFilter(color);

	x += FRAME_MARGIN_W*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollX;
	y += FRAME_MARGIN_H*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollY;
	(*(m_LayoutEditor->m_display))->Point(x, y, color);
}

void CLayout::Box(RECT rc, D3DCOLOR color)
{
	if (!m_LayoutEditor)
		return;
	PremultiplyFilter(color);

	rc.left += FRAME_MARGIN_W*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollX;
	rc.right += FRAME_MARGIN_W*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollX;
	rc.top += FRAME_MARGIN_H*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollY;
	rc.bottom += FRAME_MARGIN_H*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollY;

	(*(m_LayoutEditor->m_display))->Box(rc, color);
}

void CLayout::SetTexture(int index)
{
	if (!m_LayoutEditor)
		return;
	(*(m_LayoutEditor->m_display))->SetTexture(index);
}

int CLayout::AddDefaultTexture()
{
	if (!m_LayoutEditor)
		return -1;

	int tex = (*(m_LayoutEditor->m_display))->AddTextureFromResource(NULL, MAKEINTRESOURCE(IDB_DEFAULTTEX));
	(*(m_LayoutEditor->m_display))->Premultiply(tex);

	return tex;
}

int CLayout::AddTextureFromFile(const char* file)
{
	if (!m_LayoutEditor)
		return -1;
	int tex = (*(m_LayoutEditor->m_display))->AddTextureFromFile(file);
	(*(m_LayoutEditor->m_display))->Premultiply(tex);
	
	return tex;
}

void CLayout::DeleteTexture(int handle)
{
	if (!m_LayoutEditor)
		return;

	(*(m_LayoutEditor->m_display))->RemoveTexture(handle);
}

void CLayout::GetTextureSize(int handle, SIZE* pSize)
{
	int done = -1;

	if (!m_LayoutEditor)
	{
		pSize->cx = -1;
		pSize->cy = -1;
		return;
	}
	TextureInfo* pTex = (*(m_LayoutEditor->m_display))->TexInfo(handle);
	pSize->cx = pTex->texwidth;
	pSize->cy = pTex->texheight;
}

void CLayout::TranslateFrameToScreenCoords(float& x, float& y)
{
	if (!m_LayoutEditor)
		return;

	x += FRAME_MARGIN_W*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollX;
	y += FRAME_MARGIN_H*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollY;
}

void CLayout::Blitrc(float x, float y, float w, float h, float angle, D3DCOLOR filter, const RECTF* uv)
{
	if (!m_LayoutEditor)
		return;

	RECTF rc;
	SetRectF(&rc, x, y, x+w, y+h);

	rc.left += FRAME_MARGIN_W*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollX;
	rc.right += FRAME_MARGIN_W*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollX;
	rc.top += FRAME_MARGIN_H*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollY;
	rc.bottom += FRAME_MARGIN_H*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollY;

		// Premultiply filter
	PremultiplyFilter(filter);

	(*(m_LayoutEditor->m_display))->Blit(rc, angle, filter, uv);
}
void CLayout::Blitrc(float x, float y, float w, float h, float angle, D3DCOLOR filter, D3DCOLOR f1, D3DCOLOR f2, D3DCOLOR f3, D3DCOLOR f4,  const RECTF* uv)
{
	if (!m_LayoutEditor)
		return;
	RECTF rc;
	SetRectF(&rc, x, y, x+w, y+h);

	rc.left += FRAME_MARGIN_W*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollX;
	rc.right += FRAME_MARGIN_W*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollX;
	rc.top += FRAME_MARGIN_H*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollY;
	rc.bottom += FRAME_MARGIN_H*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollY;

	// Premultiply filter
	PremultiplyFilter(filter);
	PremultiplyFilter(f1);
	PremultiplyFilter(f2);
	PremultiplyFilter(f3);
	PremultiplyFilter(f4);
	(*(m_LayoutEditor->m_display))->Blit(rc, angle, filter, f1,f2,f3,f4, uv);
}

void CLayout::BlitrcSkew(float x, float y, float w, float h, float angle, D3DCOLOR filter, const RECTF* uv, float skewX, float skewY)
{	
	if (!m_LayoutEditor)
		return;
	RECTF rc;
	SetRectF(&rc, x, y, x+w, y+h);

	rc.left += FRAME_MARGIN_W*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollX;
	rc.right += FRAME_MARGIN_W*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollX;
	rc.top += FRAME_MARGIN_H*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollY;
	rc.bottom += FRAME_MARGIN_H*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollY;

	// Premultiply filter
	PremultiplyFilter(filter);

	(*(m_LayoutEditor->m_display))->Blit(rc, angle, filter,
		0xFFFFFFFF,  0xFFFFFFFF,  0xFFFFFFFF,  0xFFFFFFFF, uv, skewX, skewY);
}

int CLayout::AddTextureFromResource(HMODULE hModule, LPCTSTR pResource)
{
	if (!m_LayoutEditor)
		return -1;
	int	tex = (*(m_LayoutEditor->m_display))->AddTextureFromResource(hModule, pResource);
	return tex;
}


int CLayout::CreateFont(const char* fontface, unsigned int pxSize, bool italics, bool bold)
{
	if (!m_LayoutEditor)
		return -1;

	CD3DDisplayFont f;
	Fonts.push_back(f);
	(*(m_LayoutEditor->m_display))->CreateFont(fontface, pxSize, italics, bold, &(Fonts.back()));
	int font = Fonts.size() - 1;
	return font;
}

void CLayout::DrawText(int iFont, const char* text, RECT& rc, D3DCOLOR color, DWORD format)
{
	if (!m_LayoutEditor)
		return;

	PremultiplyFilter(color);

	rc.left += FRAME_MARGIN_W*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollX;
	rc.right += FRAME_MARGIN_W*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollX;
	rc.top += FRAME_MARGIN_H*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollY;
	rc.bottom += FRAME_MARGIN_H*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollY;

	CD3DDisplay* pDisplay = (*(m_LayoutEditor->m_display));

	list<CD3DDisplayFont>::iterator f = Fonts.begin();
	advance(f, iFont);
	pDisplay->DrawText(*f, text, rc, color, format);
}

void CLayout::DestroyFont(int iFont)
{
	list<CD3DDisplayFont>::iterator f = Fonts.begin();
	advance(f, iFont);
	f->Destroy();
}

HWND CLayout::GetFrameHwnd()
{
	if (!m_LayoutEditor)
		return NULL;

	return m_LayoutEditor->GetSafeHwnd();
}

long CLayout::GetUnusedLayerID() 
{
	long ID = 0;
	BOOL bDone = FALSE, bFound = FALSE;
	CLayer *lyr;
	while (bDone == FALSE) 
	{
		POSITION pos = layers.GetHeadPosition();
		bFound = FALSE;

		for (int i = 0; i < layers.GetCount(); i++)
		{
			lyr = layers.GetNext(pos);
			if (lyr->m_layerID == ID) 
			{
				bFound = TRUE;
				break;
			}
		}

		if (!bFound) 
		{
			bDone = TRUE;
			break;
		}

		else
			ID++;
	}
	return ID;

}

bool CLayout::EditImage(int a)
{
	g_SuspendFrameEditor = true;

	CImageResource* it = application->resources.FindImageResourceFromNumber(a);

	if(!it)
		return 0;

	try
	{
		CImageEditorDlg MyDlg;

		MyDlg.m_Animation = false; // we are editing a single image...however we need to setup the picture editor regardless
		MyDlg.application = this->application;
		MyDlg.m_sourceImages.push_back(it);	// we give it one image
		
		MyDlg.CopyNeededDataFromImages();


		if(MyDlg.EditImage(&it->bitmap))
		{
			MyDlg.CopyNeededDataToImages();	// This function also handles the textures

			// Force the texture to update by erasing the image-to-texture map entry.
			CApplication* app = application;
			map<int,int>::iterator i = app->m_image_to_texture.find(a);
			if(i != app->m_image_to_texture.end()) {
				app->m_display->RemoveTexture(i->second);
				app->m_image_to_texture.erase(i);
			}

			// Image bank content has changed
			app->resources.images_changed = true;
		}
	}
	catch (CD3DError e) 
	{
		CErrorDlg err;
		err.Error("Image Editor Error", e.GetString());
	}

	g_SuspendFrameEditor = false;

	return 0;
}
bool CLayout::EditImageInAnimation(int aHandle, int index)
{
	g_SuspendFrameEditor = true;

	try
	{
		CImageEditorDlg MyDlg;
		MyDlg.application = application;
		MyDlg.EditAnimation(aHandle, index, application);
	}
	catch (CD3DError e) 
	{
		CErrorDlg err;
		err.Error("Image Editor Error", e.GetString());
	}
	catch (...) {
		CErrorDlg err;
		err.Error("Image Editor Error", "Error opening the image editor.  Please report this bug to Scirra.");
	}


	return 0;
}
int CLayout::AddImage(int iWidth, int iHeight)
{
	CApplication* pApplication = application;

	// Add image
	pApplication->resources.images.push_back(CImageResource());
	pApplication->resources.images_changed = true;
	CImageResource& Image = pApplication->resources.images.back();

	Image.m_Dimensions.Format("%d x %d", iWidth, iHeight);
	Image.m_Name.Format("ObjectImage%d", pApplication->m_ImageFixID - 1);
	Image.bitmap.Create(iWidth, iHeight, 32);
	Image.bitmap.Clear(0);
	Image.bitmap.AlphaCreate();
	Image.bitmap.AlphaInvert();
	Image.m_FixedID = pApplication->m_ImageFixID++;
	

	Image.m_Hotspot.x = iWidth / 2;
	Image.m_Hotspot.y = iHeight / 2;

	return Image.m_FixedID;
}

int	CLayout::AddImageFromResource(HMODULE hModule, LPCTSTR pResource)
{
	CApplication* pApplication = application;

	// Add image
	pApplication->resources.images.push_back(CImageResource());
	pApplication->resources.images_changed = true;
	CImageResource& Image = pApplication->resources.images.back();
	
	Image.m_Name.Format("ObjectImage%d", pApplication->m_ImageFixID - 1);
	Image.bitmap.LoadResource(FindResource(hModule,pResource, "PNG"), 0, hModule);
	Image.bitmap.AlphaCreate();
	Image.Premultiply();
	Image.m_Dimensions.Format("%d x %d", Image.bitmap.GetWidth(), Image.bitmap.GetHeight());

	Image.m_FixedID = pApplication->m_ImageFixID++;

	Image.m_Hotspot.x = Image.bitmap.GetWidth() / 2;
	Image.m_Hotspot.y = Image.bitmap.GetHeight() / 2;
	
	return Image.m_FixedID;
}

void CLayout::FillGradient(const RECT& _rc, D3DCOLOR color1, D3DCOLOR color2, Direction direction)
{
	if (!m_LayoutEditor)
		return;

	PremultiplyFilter(color1);
	PremultiplyFilter(color2);

	RECT rc = _rc;
		
	// Offset fill
	rc.left += FRAME_MARGIN_W*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollX;
	rc.right += FRAME_MARGIN_W*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollX;
	rc.top += FRAME_MARGIN_H*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollY;
	rc.bottom += FRAME_MARGIN_H*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollY;

	(*(m_LayoutEditor->m_display))->FillGradient(rc, color1, color2, direction);
}

int CLayout::AddImageAsTexture(int a)
{
	CApplication* app = application;
	map<int,int>::iterator i = app->m_image_to_texture.find(a);
	if(i == app->m_image_to_texture.end())
	{
		CImageResource* img = app->resources.FindImageResourceFromNumber(a);
		if(img)
		{
			CxImage guttered;
			bool neededgutter = img->CopyToImageAndAddGutter(guttered);
			int image = (*m_LayoutEditor->m_display)->AddTextureFromCxImage(guttered);
			app->m_image_to_texture[a] = image;

			if(neededgutter)
				(*m_LayoutEditor->m_display)->ApplyTextureGutter(image);

			return image;
		}

	}
	else
	{
		return i->second;
	}
	return -2; 
}

bool CLayout::TextureExists(int iTexture)
{
	if (!m_LayoutEditor)
		return false;
	if (m_LayoutEditor->m_display == NULL) 
		return false;
	if (*(m_LayoutEditor->m_display) == NULL)
		return false;

	return (*m_LayoutEditor->m_display)->TextureExists(iTexture);
}


void CLayout::SerializeImage(int& image, bin& ar)
{
	CApplication* app = application;

	if(ar.loading)
	{
		ar >> image;
		if(app && app->m_drag_app_from && app->m_drag_app_from != app)
		{
			//If the application isn't already in the map, we create it...it doesn't matter
			map<int, int>& imagefromto = app->m_imagefrom_imageto_map[app->m_drag_app_from];

			// now the important part...we need to see if we've done this before
			if( imagefromto.find(image) == imagefromto.end())
			{
				//damn it hasn't been done before...we have work to do
				app->resources.images.push_back(CImageResource());
				app->resources.images_changed = true;

				if(app->resources.FindImageResourceFromNumber(image))
				{
					app->images_to_save[image] = 0; 
				}
				image = imagefromto[image] = app->m_ImageFixID++;
				app->resources.images.back().m_FixedID = image;
				// we use the imagefromto map later to put our bitmaps that we serialized at the end of CDragObjects into
				// our blank imageBank spots
			}
			else
			{
				// Yeah! we've been here and done it! 
				image = imagefromto[image];
			}
		}
		else if(g_bPasteClone)
		{
			image = app->CloneImage(image);
			app->resources.images_changed = true;
		}
	}
	else	//Saving
	{
		ar << image;
		if(app->resources.FindImageResourceFromNumber(image))
			app->images_to_save[image] = 0;	//todo: register serialization of CObj and add a pointer here when read so we can track what objects use what images!


	}
	
}
void	CLayout::SerializeAnimation(int& anim, bin& ar)
{
	CApplication* app = application;

	if(ar.loading)
	{
		ar >> anim;
		if(app && app->m_drag_app_from && app->m_drag_app_from != app)
		{
		//If the application isn't already in the map, we create it...it doesn't matter
			map<int, int>& animfromto = app->m_animfrom_animto_map[app->m_drag_app_from];

			// now the important part...we need to see if we've done this before
			if( animfromto.find(anim) == animfromto.end())
			{
				//damn it hasn't been done before...we have work to do
				app->resources.animations.push_back(CAnimation());
				
				app->animations_to_save[anim] = 0; 
				anim = animfromto[anim] = app->m_AnimationFixID++;
				app->resources.animations.back().m_FixedID = anim;
				// we use the imagefromto map later to put our bitmaps that we serialized at the end of CDragObjects into
				// our blank imageBank spots
			}
			else
			{
				// Yeah! we've been here and done it! 
				anim = animfromto[anim];
			}

		}
		else if(g_bPasteClone)
		{
			anim = app->CloneAnimation(anim);
			app->resources.images_changed = true;
		}
	}
	else
	{
		ar << anim;
		app->animations_to_save[anim] = 0;	//todo: register serialization of CObj
	}

}
int CLayout::AddAngle(int iWidth, int iHeight, int iFrameCount, float angle, int iAnimation)
{
	CApplication* pApplication = application;

	CAnimation Angle;
	Angle.m_IsAngle = true;
	Angle.m_FixedID = pApplication->m_AnimationFixID++;
	Angle.application = application;
	
	for(int i = 0; i < iFrameCount; i++)
	{
		Angle.m_Images.push_back(AddImage(iWidth, iHeight));
		Angle.m_FrameTimes.push_back(1.0);
	}

	CAnimation* parent = pApplication->resources.FindAnimationFromNumber(iAnimation);

	if(parent)
		parent->m_SubAnimations.push_back(Angle);
	else
		pApplication->resources.animations.push_back(Angle);

	return Angle.m_FixedID;
}
int CLayout::AddAnimation(int iWidth, int iHeight, int iFrameCount, LPCSTR szName, int iParentHandle)
{
	CApplication* pApplication = application;


	CAnimation* parent = pApplication->resources.FindAnimationFromNumber(iParentHandle);

	CAnimation* newanim = 0;
	if(parent)
	{
		parent->m_SubAnimations.push_back(CAnimation());
		newanim = &parent->m_SubAnimations.back();
	}
	else
	{
		pApplication->resources.animations.push_back(CAnimation());
		newanim = &pApplication->resources.animations.back();
	}


	if(newanim)
	{
		// Add animation

		newanim->m_Name    = szName;
		newanim->m_FixedID = pApplication->m_AnimationFixID++;
		newanim->m_Speed   = 50.0f;
		newanim->application = application;

		for (int i = 0; i < iFrameCount; i++)
		{
			newanim->m_Images.push_back(AddImage(iWidth, iHeight));
			newanim->m_FrameTimes.push_back(1.0);
		}

		return newanim->m_FixedID;	//return
	}
	return NULL;
}
void CLayout::RemoveAnimation(int iHandle)
{
	CAnimation* parent;
	if(  CAnimation* anim = application->resources.FindAnimationFromNumber(iHandle, &parent))
	{
		if(parent)
		{
			for(list<CAnimation>::iterator i = parent->m_SubAnimations.begin(); i!= parent->m_SubAnimations.end(); i++)
				if (i->m_FixedID == iHandle)
					parent->m_SubAnimations.erase(i);
		}
		else
		{
			for(list<CAnimation>::iterator i = application->resources.animations.begin(); i!= application->resources.animations.end(); i++)
				if (i->m_FixedID == iHandle)
					application->resources.animations.erase(i);
		}
	}
}

int	CLayout::QueryAnimationFrameCount(int iHandle)
{
	if(  CAnimation* anim = application->resources.FindAnimationFromNumber(iHandle))
		return anim->m_Images.size();
	
	return -1;
}
int CLayout::GetSubAnimation(int iHandle, int index)
{
	if(  CAnimation* anim = application->resources.FindAnimationFromNumber(iHandle))
	{
		list<CAnimation>::iterator i = anim->m_SubAnimations.begin();
		advance(i, index);
		return i->m_FixedID;
	}
	return -1;
}
int CLayout::GetSubAnimationCount(int iHandle)
{
	if(  CAnimation* anim = application->resources.FindAnimationFromNumber(iHandle))
	{
		return anim->m_SubAnimations.size();
	}
	return -1;
}
void CLayout::InsertFrameIntoAnimation(int iHandle, int iPosition, int iFrame)
{
	if(  CAnimation* anim = application->resources.FindAnimationFromNumber(iHandle))
	{
		anim->m_FrameTimes.push_back(1);
		anim->m_Images.push_back(iFrame);
	}
	//CErrorDlg dlg;
	//dlg.Error("Not yet implimented","Sorry! InsertFrameIntoAnimation has not yet been implimented. Please contact the scirra team alerting them of this error message!");
	//
}

void CLayout::RemoveFrameFromAnimation(int iHandle, int iPosition, int iCount)
{
	CErrorDlg dlg;
	dlg.Error("Not yet implimented","Sorry! RemoveFrameFromAnimation has not yet been implimented. Please contact the scirra team alerting them of this error message!");
}
int CLayout::GetAnimationFrame(int iHandle, int index)
{
	CAnimation* anim = application->resources.FindAnimationFromNumber(iHandle);
	if(!anim)
		return -1;
	if(index < 0)
		return -1;
	if(index >= anim->m_Images.size())
		return -1;
	
	return anim->m_Images.at(index);

}
bool CLayout::EditAnimation(int iHandle)
{
	return EditImageInAnimation(iHandle, 0);
}

void CLayout::GenerateIcon(int iObjectHandle, bool bSmallIcon, int iHandle)
{
	CApplication* app = application;
	for(list<CImageResource>::iterator i = app->resources.images.begin(); i!= app->resources.images.end(); i++)
	{
		if (i->m_FixedID == iHandle)
		{
			CObjType* oT;

			POSITION pos = app->object_types.GetStartPosition();
			long mapKey;
			CList<CObjType*, CObjType*&> otList;

			// Loop through and add the objects
			while (pos != NULL) 
			{
				app->object_types.GetNextAssoc(pos, mapKey, oT);

				if (oT->ObjectIdentifier == iObjectHandle)
				{
					CxImage Temp;
					Temp.Copy(i->bitmap, true, true, true);

					RGBQUAD canvas;
					canvas.rgbBlue = 255;
					canvas.rgbGreen = 255;
					canvas.rgbRed = 255;

					// Large icon
					if (!bSmallIcon)
					{
						CreatePremultipliedThumbnail(&Temp, 0, 32,32, 0xffffff, 0);
						//Temp.Thumbnail(32, 32, canvas);
						oT->m_Image.Copy(Temp, true, true, true);
					}

					// Small like Gullens penis
					// davo - :P    okay i made my own thumbnail function which allows for alpha channels that are premultiplied.
					// 
					else
					{
						CreatePremultipliedThumbnail(&Temp, 0, 16,16, 0xffffff, 0);

						//Temp.Thumbnail(16, 16, canvas);
						oT->small_image.Copy(Temp, true, true, true);
					}

					RefreshObjectList();

					break;
				}
			}
		}
	}
}


POINT CLayout::GetImageCenterPoint(int handle)
{
	CApplication* app = application;
	if(app)
	{
		CImageResource* img = app->resources.FindImageResourceFromNumber(handle);
		if(img)
			return img->m_Hotspot;	
	}
	
	POINT pt;
	pt.x = 0;
	pt.y = 0;
	return pt;

}

POINT CLayout::GetImagePoint(int handle, CString name)
{	
	CApplication* app = application;
	if(app)
	{
		CImageResource* img = app->resources.FindImageResourceFromNumber(handle);
		if(img)
		{
			return img->m_Action[name];
		}
	}

	POINT pt;
	pt.x = 0;
	pt.y = 0;
	return pt;
		
}

SIZE CLayout::GetImageSize(int handle)
{
	CApplication* app = application;
	if(app)
	{
		CImageResource* img = app->resources.FindImageResourceFromNumber(handle);
		if(img)
		{
			SIZE pt;
			pt.cx = img->bitmap.GetWidth();
			pt.cy = img->bitmap.GetHeight();
			return pt;
		}
	}
	
	SIZE pt;
	pt.cx = 0;
	pt.cy = 0;
	return pt;

}

void CLayout::FindContainerObjects(CObj* o, vector<CObj*>& object_list)		// not a virtual function of VRuntime
{
	object_list.clear();

	CObjType* selectObjectType = o->GetObjectType(application);
	if(!selectObjectType)
		return;
	CContainer* pContainer = selectObjectType->m_pContainer;

	if(!pContainer)	// no container..oh well
		return ;	


	POSITION layerpos = layers.GetHeadPosition();
	int index_select = -1;
	while(layerpos)
	{
		CLayer* layer = layers.GetNext(layerpos);

		POSITION zorderpos = layer->m_zOrder.GetHeadPosition();

		while(zorderpos)
		{
			long id = layer->m_zOrder.GetNext(zorderpos);
			CObj* temp_o;
			objects.Lookup(id, temp_o);
			if(temp_o->GetObjectType(application) == selectObjectType)
			{
				index_select++;
			}
			if(temp_o == o)
			{
				// break out of our while loops
				zorderpos = 0;
				layerpos = 0;
			}
		}
	}

	if(index_select == -1)	//we couldn't find our object in all the layers :S
		return ;


	for(vector<int>::iterator objtypenumber = pContainer->m_IDs.begin(); objtypenumber != pContainer->m_IDs.end(); objtypenumber ++)
	{
		CObjType* pType = application->FindObjTypeFromNumber(*objtypenumber);
		if(pType == selectObjectType)
			continue;

		POSITION layerpos = layers.GetHeadPosition();
		int count_down = index_select;
		while(layerpos)
		{
			CLayer* layer = layers.GetNext(layerpos);

			POSITION zorderpos = layer->m_zOrder.GetHeadPosition();

			while(zorderpos)
			{
				long id = layer->m_zOrder.GetNext(zorderpos);
				CObj* temp_o;
				objects.Lookup(id, temp_o);
				if(temp_o->GetObjectType(application) == pType)
				{
					count_down--;
				}
				if(count_down == -1)
				{
					// Woot! we have found the item we are after at long last!
					// now go onto the next for loop iteration for the next item in the container :/
					object_list.push_back(temp_o);
					zorderpos = 0;
					layerpos = 0;
				}
			}
		}
	}
}

const char* CLayout::GetName(int iObjectIdentifier)
{
	if(CObjType* type = application->FindObjTypeFromNumber(iObjectIdentifier))
		return type->GetName();

	return 0;
}

void CLayout::CaptureFrameEditor(int iObjectIdentifier)
{
	if(m_ObjectFrameIsLockedTo != 0)
	{
		return;
	}
	
	m_ObjectFrameIsLockedTo = iObjectIdentifier;
	m_ParentObjectFrameIsLockedTo = 0;
	if(iObjectIdentifier == -1)
	{
		m_ObjectFrameIsLockedTo = iObjectIdentifier;
		if(m_LayoutEditor)
			m_ParentObjectFrameIsLockedTo = m_LayoutEditor->m_sel.GetHead();

		// this will happen if a Behavior is locking the frame editor.
		// Because all the Behaviors will have the identifier as -1, we have a work around
		// where when creating a tempory Behavior we store in CApplication the information about it
		// so we can create a new Behavior. 
		m_oControlBehaviorInfo = m_oTemporyBehaviorInfo;

		if(m_pControlBehaviorExt)
		{
			m_oControlBehaviorInfo->ETRemoveObject(m_pControlBehaviorExt);
			delete m_pControlBehaviorExt;
			m_pControlBehaviorExt = 0;
		}

		m_pControlBehaviorExt = m_pTempMovExt;
			
	}

	if(m_LayoutEditor)
	{
		m_LayoutEditor->Invalidate();

	}

	RefreshProperties();
}

void	CLayout::ReleaseFrameEditor(int iObjectIdentifier)
{
	if(m_ObjectFrameIsLockedTo == iObjectIdentifier)
	{
			m_ObjectFrameIsLockedTo = 0;
			m_ParentObjectFrameIsLockedTo = 0;

			if(m_pControlBehaviorExt)
			{

				bin movData(g_allocator);
				movData.allocate(1024);
				movData.loading = false;
				m_oControlBehaviorInfo->ETSerialize(m_pControlBehaviorExt, movData);

				*m_pControlBehaviorData = (BYTE*)movData.ptr();
				*m_pControlBehaviorDataSize = movData.size();

				movData.detach();

				m_oControlBehaviorInfo->ETRemoveObject(m_pControlBehaviorExt);
				delete m_pControlBehaviorExt;
				m_pControlBehaviorExt = 0;

			}
	}
	RefreshProperties();
}

CObj* CLayout::FindObjFromNumber(int fixed)
{
	long id = fixed;
	CObj* o =  0;
	if (objects.Lookup(id, o)) 
	{
		return o;
	}
	else
	{
		return 0;
	}
}

	 void CLayout::SetAnimationModificationSettings(int iHandle,	
		 bool AllowRemove, bool AllowSubanimations, bool AllowAddRemoveFrame, bool AllowRenameFrame, bool AutoGenerateButton)
	 {
		 CAnimation* anim = application->resources.FindAnimationFromNumber(iHandle);
		 if(anim)
		 {
			 anim->m_AllowRemove = AllowRemove;
			 anim->m_AllowSubanimations = AllowSubanimations;
			 anim->m_AllowRenameFrame = AllowRenameFrame;
			 anim->m_AllowAddRemoveFrame = AllowAddRemoveFrame;
			 anim->m_AllowAutoButton = AutoGenerateButton;
		 }
		
	 }
	 void CLayout::SetImageName(int iHandle, LPCSTR szName)
	 {

		 CImageResource* img = application->resources.FindImageResourceFromNumber(iHandle);
		if(img)
			img->m_Name = szName;
	 }



void CLayout::Message(int message)
{
	if(m_ObjectFrameIsLockedTo != -1)
	{
		CObj* obj = FindObjFromNumber(m_ObjectFrameIsLockedTo);
		CObjType* type = obj->GetObjectType(application);
		
		// Get oInfo pointer
		OINFO *oInfo;
		DWORD oid = obj->GetDLLIdentifier();

		g_dlls->Lookup(oid, oInfo);

		if(oInfo->ETOnMessage)
		{
			oInfo->ETOnMessage(obj->editObject, message);
		}
		
	}
	else
	{
		//Behavior
		if(m_oControlBehaviorInfo)
		{
			m_oControlBehaviorInfo->ETOnMessage(m_pControlBehaviorExt, message);
		}
	}






}



float	CLayout::GetZoom()
{
	if(m_LayoutEditor)
	{
		return m_LayoutEditor->m_Zoom;
	}
	else
		return 1;

}
editInfo* CLayout::GetObjectBelongingToThisBehavior()
{
	CObjType* pType = g_MainFrame->m_PropertiesBar->m_pSelectedObjType;
	CObj* pObj = g_MainFrame->m_PropertiesBar->m_pSelectedObj;
	if(pObj)
		return pObj->editObject;
	if(pType)
	{
		POSITION pos = objects.GetStartPosition();
		long nKey;

		while (pos != NULL) 
		{ 
			CObj* o;

			// Iterate through objects to choose one to select
			objects.GetNextAssoc(pos, nKey, o);
			if(o->GetObjectType(application) == pType)	
				return o->editObject;
		}
	}

	return 0;
}
POINT CLayout::GetCursor()
{
	if(m_LayoutEditor)
	{
		CLayoutEditor* ed = m_LayoutEditor;

		CPoint pos;

		GetCursorPos(&pos);

		ed->ScreenToClient(&pos);

		pos += CPoint(-FRAME_MARGIN_W * GetZoom()+ed->ScrollPos.x, -FRAME_MARGIN_H * GetZoom()+ed->ScrollPos.y);

		return pos;
	}
	return CPoint(0,0);
}

void CLayout::RefreshProperties()
{
	if(m_LayoutEditor)
	{
		CLayoutEditor* ed = m_LayoutEditor;
		
		if(ed->InputLocked())
		{
			g_MainFrame->m_PropertiesBar->Update(ed, TYPE_OBJECT_CAPTURED, NULL, this, NULL, application); // show object props
		}
		else
		{
			g_MainFrame->m_PropertiesBar->Update(ed, TYPE_OBJECT, &ed->m_sel, this, &objects, application); // show objec
		}
	}
}

void CLayout::RefreshObjectList()
{
	if(m_LayoutEditor)
	{
		CChildFrame* pChild = dynamic_cast<CChildFrame*>(m_LayoutEditor->GetParent());
		CWnd* clientWindow = g_MainFrame->GetActiveFrame();//GetWindow(GW_CHILD);
		while (clientWindow != NULL)
		{
			CChildFrame* frame = dynamic_cast<CChildFrame*>(clientWindow);
			if(frame)
				frame->object_bar.Refresh();

			clientWindow = clientWindow->GetWindow(GW_HWNDNEXT);
		}
	}
}

void* CLayout::GetDisplayDevice()
{
	for (int i = 0; i < 4; i++) {
		if (!m_LayoutEditor)
			continue;
		return (*(m_LayoutEditor->m_display))->GetD3DDevice();
	}

	return NULL;
}

void CLayout::EndBatch()
{
	for (int i = 0; i < 4; i++) {
		if (!m_LayoutEditor)
			continue;
		(*(m_LayoutEditor->m_display))->EndBatch();
		return;
	}
}

void CLayout::Restore2DMode()
{
	for (int i = 0; i < 4; i++) {
		if (!m_LayoutEditor)
			continue;
		(*(m_LayoutEditor->m_display))->Setup2DMode();
		return;
	}
}


unsigned int CLayout::GetFrameCount()
{
	for (int i = 0; i < 4; i++) {
		if (!m_LayoutEditor)
			continue;
		return m_LayoutEditor->m_drawCount;
		
	}
	return 0;
}

int KeyCollection::GetSavedKeyCount()
{
	map<CString, KeyData>::iterator i = keys.begin();
	int count = 0;

	for ( ; i != keys.end(); i++) {
		if (i->second.bSave)
			count++;
	}

	return count;
}

void KeyCollection::Serialize(CArchive& ar, bool bNew)
{
	if (ar.IsLoading()) 
	{
		keys.clear();

		int count;
		ar >> count;

		// For each key
		for (int i = 0; i < count; i++)
		{
			CString name;
			int theData;
			KeyData k;
			int iType = 0;

			ar >> name;

			if (bNew)
				ar >> iType;

			if (iType == 0)
			{
				ar >> theData;
				k.pData = (void*)theData;
			}

			else
			{
				k.pData = NULL;
				CString String;
				ar >> String;
				k.String = String;
			}
				
			k.iType = iType;
			k.bSave = true;
			keys[name] = k;
		}
	}

	else 
	{
		ar << GetSavedKeyCount();

		map<CString, KeyData>::iterator i = keys.begin();

		for ( ; i != keys.end(); i++) 
		{
			if (i->second.bSave) 
			{
				ar << i->first;
				ar << i->second.iType;

				if (i->second.iType == 0)
					ar << (int)i->second.pData;
				else
					ar << i->second.String;
			}
		}
	}
}

void CLayout::SetGlobalKey(const char* name, void* param, bool save)
{
	KeyData k;
	k.iType = 0;
	k.pData = param;
	k.bSave = save;
	application->globalKeys.keys[name] = k;
}

void* CLayout::GetGlobalKey(const char* name)
{
	map<CString, KeyData>::iterator i = application->globalKeys.keys.find(name);

	if (i == application->globalKeys.keys.end())
		return NULL;
	else
		return i->second.pData;
}

bool CLayout::GlobalKeyExists(const char* name)
{
	return application->globalKeys.keys.find(name) != application->globalKeys.keys.end();
}

void CLayout::SetLayoutKey(const char* name, void* param, bool save)
{
	KeyData k;
	k.iType = 0;
	k.pData = param;
	k.bSave = save;
	layoutKeys.keys[name] = k;
}

void* CLayout::GetLayoutKey(const char* name)
{
	map<CString, KeyData>::iterator i = layoutKeys.keys.find(name);

	if (i == layoutKeys.keys.end())
		return NULL;
	else
		return i->second.pData;
}

void CLayout::SetLayoutKeyString(const char* name, const char* string, bool save)
{
	KeyData k;
	k.iType = 1;
	k.String.Format("%s", string);
	k.bSave = save;
	layoutKeys.keys[name] = k;
}

const char*	CLayout::GetLayoutKeyString(const char* name)
{
	map<CString, KeyData>::iterator i = layoutKeys.keys.find(name);

	if (i == layoutKeys.keys.end())
		return "";
	else
		return i->second.String;
}

void CLayout::SetGlobalKeyString(const char* name, const char* string, bool save)
{
	KeyData k;
	k.iType = 1;
	k.String = string;
	k.bSave = save;
	application->globalKeys.keys[name] = k;
}

const char*	CLayout::GetGlobalKeyString(const char* name)
{
	map<CString, KeyData>::iterator i = application->globalKeys.keys.find(name);

	if (i == application->globalKeys.keys.end())
		return "";
	else
		return i->second.String;
}

bool CLayout::LayoutKeyExists(const char* name)
{
	return layoutKeys.keys.find(name) != layoutKeys.keys.end();
}

HDC CLayout::BeginHDCDraw()
{
	// Cannot get second HDC
	if (hdc != NULL)
		return NULL;

	CD3DDisplay* pDisplay;
	for (int i = 0; i < 4; i++) {
		if (!m_LayoutEditor)
			continue;
		pDisplay = (*(m_LayoutEditor->m_display));
		break;
	}

	CD3DDisplay& display = *pDisplay;

	hdcOldTarget = display.GetRenderTarget();
	hdcOldTexture = display.GetTexture();
	display.EndBatch();

	// Use an X8R8G8B8 texture - A8R8G8B8 is not supported by GetDC
	if (hdcTexture == -2)
		hdcTexture = display.AddTextureRT(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), D3DFMT_X8R8G8B8, D3DUSAGE_DYNAMIC);
	
	//display.SetRenderTarget(hdcTexture);

	//COLORREF cr = GetSysColor(COLOR_3DFACE);

	//display.ClearRT(D3D_CONVERTCOLORREF(cr));
	//display.EndBatch();

	display.GetTexture(hdcTexture)->texture->GetSurfaceLevel(0, &pHdcSurface);
	pHdcSurface->GetDC(&hdc);

	return hdc;
}

void CLayout::EndHDCDraw(RECT drawzone)
{
	CD3DDisplay* pDisplay;
	int i;
	for (i = 0; i < 4; i++) {
		if (!m_LayoutEditor)
			continue;
		pDisplay = (*(m_LayoutEditor->m_display));
		break;
	}

	CD3DDisplay& display = *pDisplay;

	pHdcSurface->ReleaseDC(hdc);
	pHdcSurface->Release();

	float w = GetSystemMetrics(SM_CXSCREEN);
	float h = GetSystemMetrics(SM_CYSCREEN);

	drawzone.left += FRAME_MARGIN_W*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollX;
	drawzone.right += FRAME_MARGIN_W*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollX;
	drawzone.top += FRAME_MARGIN_H*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollY;
	drawzone.bottom += FRAME_MARGIN_H*m_LayoutEditor->m_Zoom - m_LayoutEditor->scrollY;

	// Transfer hdcTexture to display
	RECTF uv;
	uv.left = 0;
	uv.right = (float)(drawzone.right - drawzone.left) / w;
	uv.top = 0;
	uv.bottom = (float)(drawzone.bottom - drawzone.top) / h;

	display.EndBatch();

	display.SetRenderTarget(hdcOldTarget);
	display.SetTexture(hdcTexture);
	display.Blit(drawzone.left, drawzone.top, (drawzone.right - drawzone.left), (drawzone.bottom - drawzone.top), 0.0f, 0xFFFFFFFF, &uv);
	display.EndBatch();

	display.SetTexture(hdcOldTexture);
	display.EndBatch();

	hdc = NULL;

	display.SetTexture(-1);
	display.Blit(0, 0, 32, 32, 0.0f, 0x00000000);
	display.EndBatch();

	display.SetTexture(hdcOldTexture);
	display.EndBatch();

}

editInfo* CLayout::GetObjectAt(POINT point)
{
	CObjMap* objMap = &objects;
	POSITION pos = layers.GetHeadPosition();
	CObj* o;
	//for (int j = 0; j < layers.GetCount(); j++) 
	while(pos)
	{
		CLayer *lyr = layers.GetNext(pos);
		if (lyr->m_layerType != LAYER_NONFRAME && lyr->m_state == LAYSTATE_VISIBLE) 
		{
			POSITION pos2 = lyr->m_zOrder.GetTailPosition();
			for (int i = 0; i < lyr->m_zOrder.GetCount(); i++) 
			{
				long id = lyr->m_zOrder.GetPrev(pos2);
				objMap->Lookup(id, o);

				if (o->GetVisible() && !o->GetLocked()) 
				{
					if (o->GetObjectRect(m_LayoutEditor).PtInRect(point))
					{
						if(!o->m_bInGroup && !o->m_bIsGroup)
							return o->editObject;
					}
				}
			}
		}
	}

	return NULL;
}

editInfo* CLayout::GetObjectByID(int InstanceID)
{
	CObj* pObj = GetObj(InstanceID);
	if(pObj)
		return pObj->editObject;
	else
		return NULL;
}

// The purpose of this function is to find an instance of an object type in the current layout. 
// It tries to pick one of the same instance, but if it cant it will pick one of the other instances.
editInfo* CLayout::FindObjectByAnID(int instanceID, int ObjectID)
{
	CObj* pObj = NULL;

	CObjMap* objMap = &objects;

	CObj* o;
	POSITION pos = objMap->GetStartPosition();
	long nKey;
	while (pos != NULL)
	{ 
		// iterate through objects to choose one to select
		objMap->GetNextAssoc(pos, nKey, o);

		if (instanceID == o->GetInstanceID())
			return o->editObject;
		if(ObjectID == o->GetGlobalID())
			pObj = o;

	}
	if(pObj)
		return pObj->editObject;
	else
		return NULL;
}

void CLayout::ApplyThemingToDialog(HWND hWnd)
{
	::SubclassChildControls(hWnd);
}

void CLayout::GetLocalObjectTypes(vector<CObjType*>& out)
{
	// Find layout objects
	POSITION pos = objects.GetStartPosition();

	CObjType* oT;
	long mapKey;
	CObj* curobj;

	// Iterate every instance on this layout
	while (pos != NULL) 
	{
		objects.GetNextAssoc(pos, mapKey, curobj);

		// Get this instance's object type
		oT = curobj->GetObjectType(application);

		// Add it if unique
		push_back_unique(out, oT);
	}

	// Get global objects too
	pos = application->object_types.GetStartPosition();

	while (pos != NULL) {
		application->object_types.GetNextAssoc(pos, mapKey, oT);

		if (oT->m_bGlobal)
			push_back_unique(out, oT);			
	}
}

int CLayout::GetControlCount()
{
	return application->m_Controls.size();
}
const char* CLayout::GetControlName(int index)
{
	if(index < 0 || index >= GetControlCount())
		return 0;
	else
		return application->m_Controls[index].name;
}
int CLayout::GetControlPlayer(int index)
{
	if(index < 0 || index >= GetControlCount())
		return 0;
	else
		return application->m_Controls[index].player;
}


const char* CLayout::GetAnimationName(int iHandle)
{
	CAnimation* anim = application->resources.FindAnimationFromNumber(iHandle);
	if(!anim)
		return NULL;
	return anim->m_Name;

}

float CLayout::GetEyeDistance3d()
{
	return application->window_information.eye_distance;
}