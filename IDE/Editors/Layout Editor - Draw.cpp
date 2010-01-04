// FrameEditorDraw.cpp : implementation of the CLayoutEditor class
//

// File includes
#include "StdAfx.h"
#include "..\Construct.h"
#include "..\Construct Doc.h"
#include "..\Bars\LayerBar.h"
#include "LayoutEditor.h"
#include "..\ChildFrame.h"
#include "..\MainFrm.h"
#include "..\Utilities\Helper.h"
#include "..\Dialogs\ProgressDlg.h"

CPoint g_ptOffset;
extern CLayerBar* g_pLayerBar;
extern bool g_SuspendFrameEditor;

extern class CMainFrame* g_MainFrame;
extern CLayoutEditor* g_pLayoutView;

/////////////////////////////////////////////////////////////////////////////
// CLayoutEditor drawing

#define COLOR_TRANSPARENT RGB(8,8,8)
#define WHITE RGB(255,255,255)

void AllocateDisplaySurface(int& iTexture, CD3DDisplay& display)
{
	if (iTexture == -2)
		iTexture = display.AddTextureRT(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
}

void AllocateDisplaySurface(int* iTexture, CD3DDisplay* display)
{
	AllocateDisplaySurface(*iTexture, *display);
}

void AllocateDisplaySurface(int* iTexture, CD3DDisplay** display)
{
	AllocateDisplaySurface(*iTexture, **display);
}

inline void MakeUVFromDimensions(const D3DRECT& dim, int w, int h, RECTF& uv)
{
	uv.left = (float)dim.x1 / (float)w;
	uv.top = (float)dim.y1 / (float)h;
	uv.right = (float)dim.x2 / (float)w;
	uv.bottom = (float)dim.y2 / (float)h;
}

inline void ClipToWindow(D3DRECT& r, int w, int h)
{
	if (r.x1 < 0) r.x1 = 0;
	if (r.x2 > w) r.x2 = w;
	if (r.y1 < 0) r.y1 = 0;
	if (r.y2 > h) r.y2 = h;
}

// For drawing effect chains
bool CLayoutEditor::RenderEffectChain(list<CEffect*>& effects, int sourceTexture, int destTexture, D3DRECT& clearZone, D3DRECT& drawZone, RECTF& tbox, float hsX, float hsY)
{
	list<CEffect*>::iterator _e = effects.begin();
	list<CEffect*>::const_iterator fx_first = _e;
	list<CEffect*>::const_iterator fx_end = effects.end();
	list<CEffect*>::const_iterator fx_last = fx_end;
	fx_last--;

	int targetSwitch = 1;
	CD3DDisplay& Display = *(*m_display);

	int winWidth = GetSystemMetrics(SM_CXSCREEN);
	int winHeight = GetSystemMetrics(SM_CYSCREEN);

	// Object bounding box in texture coords, clipped.
	RECTF uv;
	MakeUVFromDimensions(drawZone, winWidth, winHeight, uv);

	AllocateDisplaySurface(iEffectTargets[0], Display);
	AllocateDisplaySurface(iEffectTargets[1], Display);

	// The next SetTexture source.
	int fxSource = sourceTexture;

	// For each effect
	while (_e != fx_end) {
		CEffect* e = (*_e);

		bool nonShaderEffect = e->version == 0.0f;
		int nextSource = -1;

		// When we've serialized, effects are not initialised.  Make sure initialised.
		if (!e->initialised) {
			try {
				e->Init(this);
			}
			catch (runtime_error err) {
				// Effect creation failed: notify user and close application
				CString msg;
				msg.Format("Failed to initialise the effect '%s' (%s):\n\n%s\n\nIf your computer does not support the pixel shaders used by this application, unsupported effects will not display.", e->effectFile, e->Text, err.what());
				//::MessageBox(NULL, msg, "Effects", MB_OK | MB_ICONHAND);
				//application->CloseSafely();
				// Re-throw
				throw runtime_error((const char*)msg);
			}
		}

		// Last effect draws to target. (either a layer tex, or the backbuffer)
		if (_e == fx_last && nonShaderEffect) {
			Display.SetRenderTarget(destTexture);
			targetSwitch = !targetSwitch;
		}
		// Else alternating between temp textures.
		else {
			Display.SetRenderTarget(iEffectTargets[targetSwitch]);
			nextSource = iEffectTargets[targetSwitch];
			targetSwitch = !targetSwitch;	// Toggle render target

			// Clear the render target area in preparation for the draw.
			//Display.EndBatch();
			//Display.GetD3DDevice()->Clear(1, &clearZone, D3DCLEAR_TARGET, 0x00000000, 0.0f, 0);
			Display.ClearRT(0x00000000, clearZone);
		}

		unsigned int nPasses;
		ID3DXEffect* curFx = e->runEffect.pEffect;

		// Update parameters
		if (e->runEffect.vForegroundTexture)	curFx->SetTexture(e->runEffect.vForegroundTexture, Display.TexInfo(iEffectTargets[targetSwitch])->texture);
		if (e->runEffect.vBackgroundTexture)	curFx->SetTexture(e->runEffect.vBackgroundTexture, Display.TexInfo(destTexture)->texture);
		if (e->runEffect.vSourceTexture)		curFx->SetTexture(e->runEffect.vSourceTexture, Display.TexInfo(sourceTexture)->texture);
		if (e->runEffect.vPreviousTexture)		curFx->SetTexture(e->runEffect.vPreviousTexture, Display.TexInfo(_e == fx_first ? (*iDisplayTexture) : iEffectTargets[targetSwitch])->texture);
		if (e->runEffect.vFrameCounter)			curFx->SetFloat(e->runEffect.vFrameCounter, 0.0f);	// Frame effects don't animate
		if (e->runEffect.vBoxLeft)				curFx->SetFloat(e->runEffect.vBoxLeft, tbox.left);
		if (e->runEffect.vBoxTop)				curFx->SetFloat(e->runEffect.vBoxTop, tbox.top);
		if (e->runEffect.vBoxRight)				curFx->SetFloat(e->runEffect.vBoxRight, tbox.right);
		if (e->runEffect.vBoxBottom)			curFx->SetFloat(e->runEffect.vBoxBottom, tbox.bottom);
		if (e->runEffect.vBoxWidth)				curFx->SetFloat(e->runEffect.vBoxWidth, tbox.right - tbox.left);
		if (e->runEffect.vBoxHeight)			curFx->SetFloat(e->runEffect.vBoxHeight, tbox.bottom - tbox.top);
		if (e->runEffect.vHotspotX)				curFx->SetFloat(e->runEffect.vHotspotX, tbox.left + (float)(hsX) / winWidth);
		if (e->runEffect.vHotspotY)				curFx->SetFloat(e->runEffect.vHotspotY, tbox.top + (float)(hsY) / winHeight);
		if (e->runEffect.vPixelWidth)			curFx->SetFloat(e->runEffect.vPixelWidth, m_Zoom / (float)winWidth);
		if (e->runEffect.vPixelHeight)			curFx->SetFloat(e->runEffect.vPixelHeight, m_Zoom / (float)winHeight);
		if (e->runEffect.vBgStart) {
			// IDE always does full texture renders
			float zeroes[2] = {0.0f, 0.0f};
			curFx->SetFloatArray(e->runEffect.vBgStart, zeroes, 2);
		}
		if (e->runEffect.vBgEnd) {
			float ones[2] = {1.0f, 1.0f};
			curFx->SetFloatArray(e->runEffect.vBgEnd, ones, 2);
		}

		Display.EndBatch();
		// Begin rendering the effect.
		curFx->Begin(&nPasses, 0);

		for (int i = 0; i < nPasses; i++) {

			curFx->BeginPass(i);

			// Use the last render texture as the draw texture.  May be SourceTexture.
			//if (!finishesWithCrossSampling)
				Display.SetTexture(fxSource);

			Display.Blit(drawZone.x1, drawZone.y1, drawZone.x2 - drawZone.x1, drawZone.y2 - drawZone.y1,
							0.0f, 0xFFFFFFFF, &uv);
			Display.EndBatch();

			curFx->EndPass();

		}//for each pass

		curFx->End();

		// Last effect used cross sampling - is still on a temp texture
		if (_e == fx_last && !nonShaderEffect) {

			Display.SetRenderTarget(destTexture);
			Display.SetTexture(iEffectTargets[!targetSwitch]);
			Display.Blit(drawZone.x1, drawZone.y1, drawZone.x2 - drawZone.x1, drawZone.y2 - drawZone.y1,
							0.0f, 0xFFFFFFFF, &uv);
			Display.EndBatch();
		}

		fxSource = nextSource;
		_e++;

	}//for each effect

	return true;
}

void CLayoutEditor::DrawObject(CObj *o, CPoint pOffset, bool unmoved, bool bOutline)
{
	// Zoom in..this could be done better but it works and i dont have time
	// to make a better solution!

	int objectDisplayTarget = (*m_display)->GetRenderTarget();
	int winWidth = GetSystemMetrics(SM_CXSCREEN);
	int winHeight = GetSystemMetrics(SM_CYSCREEN);
	CObjType* pType = o->GetObjectType(application);

	if(!pType)
		return;

	double oX = o->editObject->objectX;
	double oY = o->editObject->objectY;
	double oWidth  = o->editObject->objectWidth;
	double oHeight = o->editObject->objectHeight;
	double oA = o->editObject->objectAngle;

	if(o->m_bIsGroup)
		o->UpdateGroupInfo(FALSE);

	o->editObject->objectX = oX 				* m_Zoom;
	o->editObject->objectY = oY 				* m_Zoom;
	//o->editObject->objectX = floor(o->editObject->objectX+0.5);
	//o->editObject->objectY = floor(o->editObject->objectY+0.5);

	o->editObject->objectWidth = (oX+oWidth) * m_Zoom;
	o->editObject->objectHeight = (oY+oHeight) * m_Zoom;
	//o->editObject->objectWidth = floor(o->editObject->objectWidth+0.5);
	//o->editObject->objectHeight = floor(o->editObject->objectHeight+0.5);

	o->editObject->objectWidth -= o->editObject->objectX;
	o->editObject->objectHeight -= o->editObject->objectY;


	// For version checking shaders
	UINT ps_major = D3DSHADER_VERSION_MAJOR((*m_display)->GetD3DCaps()->PixelShaderVersion);
	UINT ps_minor = D3DSHADER_VERSION_MINOR((*m_display)->GetD3DCaps()->PixelShaderVersion);
	float ps_version = ps_major;
	ps_version += (float)ps_version / 10.0f;

	// Get oInfo pointer
	OINFO *oInfo;
	DWORD oid = o->GetDLLIdentifier();

	g_dlls->Lookup(oid, oInfo);

	o->editObject->objectX += pOffset.x;
	o->editObject->objectY += pOffset.y;

	// if an object is being copied, we need to show the original one too
	if(g_bDuplicate)	// hack to make dragging from layer bar work
	{
		m_de |= DROPEFFECT_COPY;
		m_bMovingObject = true;
	}

	if (!o->m_bIsGroup) 
	{
		/*if (m_bMovingObject && (m_de | DROPEFFECT_COPY) == m_de) {
			
				// Little code to work out if object is changing
				bool IsChanging = true;
			
				long objInstance = o->GetInstanceID();
				if(!m_sel.Find(objInstance))
					IsChanging = false;

				if (IsChanging) {
						
					// Blending for RGBA -> RGBA for surface tex
					(*m_display)->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE);
					(*m_display)->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);

					oInfo->ETDrawObject(o->editObject);
				}

		}*/

	}

	// if object is being sized or moved, change rect temporarily
	long objInstance = o->GetInstanceID();

	//CRect rc(o->GetX(), o->GetY(), o->GetX() + o->GetW(), o->GetY() + o->GetH());
	bool temporychange = false;

	if(!unmoved)
	{
		CObj* po = o;
		while(!temporychange && po)
		{
			long id = po->GetInstanceID();
			if(m_sel.Find(id))
				temporychange = true;
			else
			{
				po = po->m_group;
			}
		}
	}

	if (!o->m_bIsGroup) 
	{
		if (temporychange) 
		{
			// Restore after zoom
			o->editObject->objectX = oX;
			o->editObject->objectY = oY;
			o->editObject->objectWidth = oWidth;
			o->editObject->objectHeight = oHeight;
			o->editObject->objectAngle = oA;

			// Our new values will reflect the zoom as well
			CObjectRectangle rc2 = GetTempChangedRect(o);
			o->SetPos(rc2.x, rc2.y);
			o->SetSize(rc2.w, rc2.h);
			o->SetAngle(rc2.a);
			//TODO: other properties like angle potentially
		}

		// Get window size
		RECT clientRec;
		GetClientRect(&clientRec);
		
		// If visible and in client area
		CRect bounding = o->GetObjectRect(this).GetBoundingRect();
		if(o->editObject->BoundingRect)
		{
			RECTF* pBox = o->editObject->BoundingRect;
			bounding.left   = pBox->left*m_Zoom ;
			bounding.right  = pBox->right*m_Zoom ;
			bounding.top    = pBox->top*m_Zoom ;
			bounding.bottom = pBox->bottom*m_Zoom ;
		}

		if (o->GetVisible() //&& o->m_Draw 
			&& bounding.left   <= scrollX - FRAME_MARGIN_W*m_Zoom + clientRec.right 
			&& bounding.top    <= scrollY - FRAME_MARGIN_H*m_Zoom + clientRec.bottom
			&& bounding.right  >= scrollX - FRAME_MARGIN_W*m_Zoom
			&& bounding.bottom >= scrollY - FRAME_MARGIN_H*m_Zoom
			)
		{
			// We are drawing this object
			//LAYERBOXES
			if (bOutline && m_bLayerBoxes)
			{
				CRect OutlineBox = bounding;
				OutlineBox.InflateRect(1, 1, 1, 1);
				FrameBox(2, OutlineBox, D3D_CONVERTCOLORREF(RGB(255, 0, 0)));
			}

			// Blending for RGBA -> RGBA for surface tex
			(*m_display)->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE);
			(*m_display)->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);
		//	(*m_display)->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);

			// Blit again, for alpha
		//	(*m_display)->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE);
		//	(*m_display)->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);
		//	(*m_display)->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA);
			if(oInfo)
			{
				// Create a list of the actual effects to run based on the disablemode setting
				list<CEffect*> effectsToRun;

				list<CEffect>::iterator ef = pType->effects.begin();
				list<CEffect>::iterator effects_end = pType->effects.end();

				for ( ; ef != effects_end; ef++) {
					bool canAdd = true;

						if (!ef->initialised) {
						try {
							ef->Init(this);
						}
						catch (runtime_error err) {
							// Effect creation failed: notify user and close application
							CString msg;
							msg.Format("Failed to initialise the effect '%s' (%s):\n\n%s\n\nIf your computer does not support the pixel shaders used by this application, unsupported effects will not display.", ef->effectFile, ef->Text, err.what());
							//::MessageBox(NULL, msg, "Effects", MB_OK | MB_ICONHAND);
							//application->CloseSafely();
							// Re-throw
							throw runtime_error((const char*)msg);
						}
					}

					switch (ef->disableMode) {
						case SVA_NOSETTING:	break;
						case SVA_PS20UNAVAILABLE:
							if (ps_version < 2.0f)
								canAdd = false;
							break;
						case SVA_PS20AVAILABLE:
							if (ps_version >= 2.0f)
								canAdd = false;
							break;
						case SVA_PS14UNAVAILABLE:
							if (ps_version < 1.4f)
								canAdd = false;
							break;
						case SVA_PS14AVAILABLE:
							if (ps_version >= 1.4f)
								canAdd = false;
							break;
						case SVA_PS11UNAVAILABLE:
							if (ps_version < 1.1f)
								canAdd = false;
							break;
						case SVA_PS11AVAILABLE:
							if (ps_version >= 1.1f)
								canAdd = false;
							break;
					}
					if (canAdd)
						effectsToRun.push_back(&*ef);
					
				}

				// Object has no effects - draw straight to target
				if (effectsToRun.empty())
				{
					(*m_display)->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE);
					(*m_display)->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);
					oInfo->ETDrawObject(o->editObject);
		
					//REQUIRED FOR TAG
					float oldzoom = m_Zoom;
					m_Zoom = 1.0;
					o->GetObjectRect(this,true); //update zoom values etc (we have already applied zoom to the values...so we make 
												//zoom 1 so the values arn't multiplied
					m_Zoom = oldzoom;
					// END REQUIRE
				}

				else {
					// Set up an effect chain and run!
					D3DRECT drawZone;
					CRect rect = o->GetObjectRect(this).GetBoundingRect();
					drawZone.x1 = rect.left + (FRAME_MARGIN_W *m_Zoom- scrollX);		// round down
					drawZone.y1 = rect.top + (FRAME_MARGIN_H *m_Zoom- scrollY);			// round down
					drawZone.x2 = rect.right + (FRAME_MARGIN_W *m_Zoom- scrollX);		// round up
					drawZone.y2 = rect.bottom + (FRAME_MARGIN_H *m_Zoom- scrollY);		// round up
					D3DRECT clearZone = drawZone;

					int hotX = o->GetW() / 2;
					int hotY = o->GetH() / 2;

					// Object bounding box in texture coords
					RECTF tbox;
					MakeUVFromDimensions(drawZone, winWidth, winHeight, tbox);

					// Clip to window area
					ClipToWindow(drawZone, winWidth, winHeight);

					if (pType->effects.front().runEffect.borderMode) {
						int xAdjust = max((drawZone.x2 - drawZone.x1) / 2, 32);
						int yAdjust = max((drawZone.y2 - drawZone.y1) / 2, 32);
						float xUvAdjust = ((float)xAdjust / (float)winWidth) / 2.0f;
						float yUvAdjust = ((float)yAdjust / (float)winHeight) / 2.0f;
						clearZone.x1 -= xAdjust;
						clearZone.x2 += xAdjust;
						clearZone.y1 -= yAdjust;
						clearZone.y2 += yAdjust;
						drawZone.x1 -= xAdjust / 2;
						drawZone.x2 += xAdjust / 2;
						drawZone.y1 -= yAdjust / 2;
						drawZone.y2 += yAdjust / 2;
						// Clip to window area
						ClipToWindow(clearZone, winWidth, winHeight);
						ClipToWindow(drawZone, winWidth, winHeight);
					}

					// Draw to original texture, then blit an effect chain
					int sourceTextureTarget;

					if (pType->effects.front().runEffect.usesSourceTexture) {
						AllocateDisplaySurface(iOriginalTexture, m_display);
						sourceTextureTarget = (*iOriginalTexture);
					}
					else {
						AllocateDisplaySurface(iEffectTargets[0], **m_display);
						sourceTextureTarget = iEffectTargets[0];
					}

					(*m_display)->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE);
					(*m_display)->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);
					(*m_display)->SetRenderTarget(sourceTextureTarget);
					//(*m_display)->EndBatch();
					//(*m_display)->GetD3DDevice()->Clear(1, &clearZone, D3DCLEAR_TARGET, 0x00000000, 0.0f, 0);
					(*m_display)->ClearRT(0x00000000, clearZone);
					oInfo->ETDrawObject(o->editObject);
				
					//REQUIRED FOR TAG
					float oldzoom = m_Zoom;
					m_Zoom = 1.0;
					o->GetObjectRect(this,true); //update zoom values etc (we have already applied zoom to the values...so we make 
												//zoom 1 so the values arn't multiplied
					m_Zoom = oldzoom;
					// END REQUIRE

					(*m_display)->EndBatch();

					// Render the effect chain
					try {
						RenderEffectChain(effectsToRun, sourceTextureTarget, objectDisplayTarget, clearZone,
										drawZone, tbox, hotX, hotY);
					}
					catch (runtime_error err) {
						if (!application->warnedAboutShaders) {
							::MessageBox(NULL, err.what(), "Error", MB_OK | MB_ICONHAND);
							application->warnedAboutShaders = true;
						}
					}

					(*m_display)->EndBatch();
					(*m_display)->SetRenderTarget(objectDisplayTarget);
				}
			}
		}
	}
	else 
	{
		// loop through objects in group and recursively call this function
		if (o->m_bInGroup && o->m_bIsGroup) { // cannot have open groups in closed groups
			if (o->m_bGroupOpen && !o->m_group->m_bGroupOpen)
				o->m_bGroupOpen = FALSE;
		}

		if (o->m_bGroupOpen)
		{
			CObjectRectangle tmp = o->GetObjectRect(this, true);
			
			//rc.OffsetRect(pOffset);

			// if object is being sized or moved, change rect
			long id = o->GetInstanceID();
			
			CObj* po = o;
			bool temporychange = false;
			while(!temporychange && po)
			{
				long id = po->GetInstanceID();
				if(m_sel.Find(id))
					temporychange = true;
				else
				{
					po = po->m_group;
				}
			}
		
		
			if (temporychange)
				tmp = GetTempChangedRect(o);

			CRect rc2 = tmp.GetBoundingRect();

			FrameFill(rc2, 0x20FF0000);
			FrameBox(rc2, 0xFFFF0000);
		}

		o->GetObjectRect(this, true);	// this forces the group to update its temp co-ordinates internally

		POSITION pos = o->m_groupObjects.GetHeadPosition();
		CObj *o2;
		for (int k = 0; k < o->m_groupObjects.GetCount(); k++) {
			long groupitemid = o->m_groupObjects.GetNext(pos);
			o2 = GetObject(groupitemid);
			(*m_display)->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE);
			(*m_display)->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);
			o2->m_group = o;
			DrawObject(o2, pOffset, unmoved, false);
		}
				
		o->UpdateGroupInfo(false, m_Zoom);
		GetTempChangedRect(o);
	}

	// Restore after zoom
	o->editObject->objectX = oX;
	o->editObject->objectY = oY;
	o->editObject->objectWidth = oWidth;
	o->editObject->objectHeight = oHeight;
	o->editObject->objectAngle = oA;
}

void CLayoutEditor::UpdateStatus()
{
	CString zoom;
	zoom.Format("%d%%", (int)(m_Zoom*100));
	g_MainFrame->m_wndStatusBar.SetPaneText(1, zoom);
	
	CPoint pos;
	GetCursorPos(&pos);
	ScreenToClient(&pos);
	pos += CPoint(-FRAME_MARGIN_W*m_Zoom, -FRAME_MARGIN_H*m_Zoom);
	pos += GetScrollPosition();	
	pos.x /= m_Zoom;
	pos.y /= m_Zoom;
	CString mousetext;
	mousetext.Format("%d,%d", (int)pos.x, (int)pos.y);
	g_MainFrame->m_wndStatusBar.SetPaneText(2, mousetext);
}

// Lost device handling
class ObjTypeOnLostDevice : public ObjectTypeFunctor {
public:
	void operator() (CObjType* pType)
	{
		list<CEffect>::iterator i = pType->effects.begin();

		for ( ; i != pType->effects.end(); i++) {
			if (i->initialised)
				i->runEffect.pEffect->OnLostDevice();
		}
	}
};

class ObjTypeOnResetDevice : public ObjectTypeFunctor {
public:
	void operator() (CObjType* pType)
	{
		list<CEffect>::iterator i = pType->effects.begin();

		for ( ; i != pType->effects.end(); i++) {
			if (i->initialised)
				i->runEffect.pEffect->OnResetDevice();
		}
	}
};

class LayerOnLostDevice : public LayerFunctor {
public:
	void operator() (CLayer* pLayer)
	{
		list<CEffect>::iterator i = pLayer->effects.begin();

		for ( ; i != pLayer->effects.end(); i++) {
			if (i->initialised)
				i->runEffect.pEffect->OnLostDevice();
		}
	}
};

class LayerOnResetDevice : public LayerFunctor {
public:
	void operator() (CLayer* pLayer)
	{
		list<CEffect>::iterator i = pLayer->effects.begin();

		for ( ; i != pLayer->effects.end(); i++) {
			if (i->initialised)
				i->runEffect.pEffect->OnResetDevice();
		}
	}
};

void CLayoutEditor::OnDraw( CDC* pDC )
{
	try
	{
		if(m_display == 0)
		{
			CreateDirectXFrame();
		}
		
		UpdateStatus();

		BOOL bUpdatePreview = FALSE;

		if ((m_UpdatePreview && !(GetKeyState(VK_LBUTTON) >> 4)) || m_bUpdateFrame) {
			CChildFrame *pCF = (CChildFrame*)GetParentFrame();
			pCF->UpdateAllEditors(this);
			m_bUpdateFrame = FALSE;
			m_UpdatePreview = FALSE;
			bUpdatePreview = TRUE;
		}

		if(!layout)
			return;

		//Tell the CLayout that it is our turn to draw - davo pwns
		layout->m_LayoutEditor = this;


		int screenW = GetSystemMetrics(SM_CXSCREEN);
		int screenH = GetSystemMetrics(SM_CYSCREEN);

		if (*m_display == NULL)
			return;

		
		(*m_display)->SetRenderTarget(-1);
		(*m_display)->SetTexture(-1);
		(*m_display)->BeginScene();
		(*m_display)->ClearRT();
		// If effects are active, draw to the temp display target.
		int displayTarget = -1;

		if ((*iDisplayTexture) > -1) {
			displayTarget = (*iDisplayTexture);
			(*m_display)->SetRenderTarget((*iDisplayTexture));
		}
		else
			(*m_display)->SetRenderTarget(-1);

		(*m_display)->ClearRT();

		if (doMove)
		{
			UpdateScrollbars();
			SetRedraw(false);
			
			// this centers the view first time
			doMove = false;
			CRect rc;
			GetClientRect(&rc);

			if (layout->scroll_position.x != 0 || layout->scroll_position.y != 0)
				ScrollToPosition(layout->scroll_position);
			else
				ScrollToPosition(CPoint(layout->GetW()/2, layout->GetH()/2) + CPoint(-(rc.right / 2), -(rc.bottom / 2)) + CPoint(m_Zoom*FRAME_MARGIN_W, m_Zoom*FRAME_MARGIN_H));
		
			SetRedraw(true);
		}
		else
			UpdateScrollbars();

		scrollX = GetScrollPosition().x;
		scrollY = GetScrollPosition().y;

		/* DRAW FRAME BACKGROUND */

		// Draw grey backing
		(*m_display)->FillGradient(CRect(0, 0, screenW, screenH*0.6), 0xffdddddd,D3D_SYSCOLOR(COLOR_BTNFACE),DIR_DOWN);
		(*m_display)->FillGradient(CRect(0, screenH*0.6, screenW, screenH), 0xffeeeeee,D3D_SYSCOLOR(COLOR_BTNFACE),DIR_UP);
		if((*iPicture))
		{
			(*m_display)->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			(*m_display)->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			(*m_display)->SetTexture((*iPicture));
			(*m_display)->Blit(0,0,(float)1,(float)1,0,0xffffffff);
			(*m_display)->EndBatch();
			(*m_display)->SetTexture(-1);
			(*m_display)->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			(*m_display)->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}

		// draw frame outlines
		FrameFill(CRect(0, 0, layout->GetW()*m_Zoom, layout->GetH()*m_Zoom), D3D_CONVERTCOLORREF(layout->GetColor()+0xff000000));

		FrameBox(CRect(-2, -2, layout->GetW()*m_Zoom + 2, layout->GetH()*m_Zoom + 2), D3D_SYSCOLOR(COLOR_BTNTEXT));

		// application size border
		if (application->window_information.window_width != layout->GetW() || application->window_information.window_height != layout->GetH())
		{
			(*m_display)->SetTexture(-1);
			FrameBox(CRect(-1, -1, (application->window_information.window_width + 1) * m_Zoom, (application->window_information.window_height + 1) * m_Zoom), D3D_SYSCOLOR(COLOR_BTNSHADOW));
		}

		FrameBox(CRect(-1,-1, layout->GetW()*m_Zoom+1, layout->GetH()*m_Zoom+1), D3D_SYSCOLOR(COLOR_BTNTEXT));

		// Get window size
		RECT clientRec;
		GetClientRect(&clientRec);

		// RANDOM INIT
		CObj* o;
		CLayer* lyr;
		POSITION pos, pos2;
		pos2 = layout->layers.GetHeadPosition();
		CLayerListBox *layerListBox = &g_pLayerBar->m_layerDlg.m_layerListBox;

		if(layout->m_pControlBehaviorExt)
		{
			layout->Message(WM_PAINT);
			//layout->m_oControlBehaviorInfo->ETDrawObject(layout->m_pControlBehaviorExt);
		}


		/////////////////////////
		// Loop layers

		bool copydrag = g_bDuplicate || (m_bMovingObject && (m_de & DROPEFFECT_COPY));


		//g_pLayerBar->m_layerDlg.images.clear();

		for (int j=0; j < (layout->layers.GetCount()); j++) 
		{
			lyr = layout->layers.GetNext(pos2);
			if (lyr->m_layerType != LAYER_NONFRAME) 
			{
				// Layers always draw to their own texture (since 0.89)
				int layerTarget;

				if (lyr->clearBack || lyr->color != RGB(255,255,255) || lyr->opacity != 1.0f
					|| !lyr->effects.empty() || lyr->forceOwnTexture)
				{
					AllocateDisplaySurface(iLayerTexture, m_display);
					layerTarget = *iLayerTexture;
				}
				else
					layerTarget = displayTarget;

				for(int doitTwice = 0; doitTwice < 2; doitTwice++)
				{
					if((doitTwice == 0 && ISLAYERVISIBLE(lyr->m_state)) || doitTwice == 1)
					{
						(*m_display)->SetRenderTarget(layerTarget);

						// Clear the layer
						if (layerTarget == (*iLayerTexture))
							(*m_display)->ClearRT();

						if (lyr->clearBack)
							FrameFill(CRect(0, 0, layout->GetW()*m_Zoom, layout->GetH()*m_Zoom), D3D_CONVERTCOLORREF(lyr->backColor));

						// Set the appropriate sampling
						switch (lyr->sampler) {

						// Use app setting
						case CLayer::sample_default:
							if (application->game_information.sampler == CApplication::GameInformation::sample_point) {
								(*m_display)->SetSamplerState(D3DSAMP_MINFILTER, D3DTEXF_POINT);
								(*m_display)->SetSamplerState(D3DSAMP_MAGFILTER, D3DTEXF_POINT);
							}
							else {
								(*m_display)->SetSamplerState(D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
								(*m_display)->SetSamplerState(D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
							}
							break;
						case CLayer::sample_point:
							(*m_display)->SetSamplerState(D3DSAMP_MINFILTER, D3DTEXF_POINT);
							(*m_display)->SetSamplerState(D3DSAMP_MAGFILTER, D3DTEXF_POINT);
							break;
						case CLayer::sample_linear:
							(*m_display)->SetSamplerState(D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
							(*m_display)->SetSamplerState(D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
							break;
						}



						// Tell all layer objects to draw to this surface
						pos = lyr->m_zOrder.GetHeadPosition();


						CLayout* pLayout = layout;
						CLayer* pDerived = application->FindLayerFromNumber(lyr->m_inherrit_layerID, &pLayout);


						

						int start = 0;
						POSITION pos2 = NULL;
						if(pDerived)
						{
							start = pDerived->m_zOrder.GetCount();
							pos2 = pDerived->m_zOrder.GetHeadPosition();

							pLayout->m_LayoutEditor = this;
						}

						for (int i = -start; i < lyr->m_zOrder.GetCount(); i++)
						{
							// Get current object
							long id = NULL;
							CObjMap* objMap = NULL;
							if(i >= 0)
							{
								id = lyr->m_zOrder.GetNext(pos);
								objMap = &layout->objects;
							}
							else
							{
								id = pDerived->m_zOrder.GetNext(pos2);
								objMap = &pLayout->objects;
							}


							objMap->Lookup(id, o);

							// If visible and in client area
							CRect bounding = o->GetObjectRect(this, true).GetBoundingRect();
							if(o->editObject->BoundingRect)
							{
								RECTF* pBox = o->editObject->BoundingRect;
								bounding.left   = pBox->left*m_Zoom ;
								bounding.right  = pBox->right*m_Zoom ;
								bounding.top    = pBox->top*m_Zoom ;
								bounding.bottom = pBox->bottom*m_Zoom ;
							}

							if ((o->GetVisible() //&& o->m_Draw 
								&& bounding.left   <= scrollX - FRAME_MARGIN_W*m_Zoom + clientRec.right 
								&& bounding.top    <= scrollY - FRAME_MARGIN_H*m_Zoom + clientRec.bottom
								&& bounding.right  >= scrollX - FRAME_MARGIN_W*m_Zoom
								&& bounding.bottom >= scrollY - FRAME_MARGIN_H*m_Zoom) || m_bMovingObject || m_bSizingObject) 
							{
								if (layout->current_layer == lyr)
									DrawObject(o, CPoint(0,0), copydrag, true);
								else
									DrawObject(o, CPoint(0,0), copydrag, false);
							}
						} // For each object on layer

						if(copydrag)
						{
							CLayer* pCurLayer = this->layout->current_layer;
							if(pCurLayer == lyr)
							{
								POSITION pos = m_sel.GetHeadPosition();
								for (int i=0; i < m_sel.GetCount(); i++) 
								{
									long object = m_sel.GetNext(pos);

									if (!layout->objects.Lookup(object, o)) 
										continue;

									if (layout->current_layer == lyr)
										DrawObject(o, CPoint(0,0), false, true);
									else
										DrawObject(o, CPoint(0,0), false, false);
								}
							}
						}

						// Layer blending
						(*m_display)->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
						(*m_display)->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

						// Blit this layer
						/*if (layerTarget == (*iLayerTexture))
						{
							(*m_display)->SetTexture((*iLayerTexture));
							(*m_display)->Blit(0, 0);
						}*/

						// Blit this layer to display if not direct
						if (layerTarget == (*iLayerTexture) && doitTwice == 0) {

							if (!lyr->effects.empty()) {
								AllocateDisplaySurface(iOriginalTexture, m_display);
								(*m_display)->SetRenderTarget((*iOriginalTexture));
								(*m_display)->ClearRT();
							}
							else
								(*m_display)->SetRenderTarget(displayTarget);

							float layerR = (float)GetRValue(lyr->color) * lyr->opacity;
							float layerG = (float)GetGValue(lyr->color) * lyr->opacity;
							float layerB = (float)GetBValue(lyr->color) * lyr->opacity;
							int layerA = lyr->opacity * 255.0;
							D3DCOLOR layerFilter = D3D_ARGB(layerA, (int)layerR, (int)layerG, (int)layerB);
							(*m_display)->SetTexture((*iLayerTexture));
							(*m_display)->Blit(0, 0, 1.0f, 1.0f, 0.0f, layerFilter);
							(*m_display)->EndBatch();

							// Render effect chain
							if (!lyr->effects.empty()) {
								D3DRECT drawZone;
								drawZone.x1 = 0;
								drawZone.x2 = screenW;
								drawZone.y1 = 0;
								drawZone.y2 = screenH;
								D3DRECT clearZone = drawZone;
								RECTF tbox;
								MakeUVFromDimensions(drawZone, screenW, screenH, tbox);
								list<CEffect*> EffectsToRender;
								for(list<CEffect>::iterator i = lyr->effects.begin(); i!=lyr->effects.end(); i++)
								{
									EffectsToRender.push_back(&*i);
								}

								// Render the effect chain
								try {
									RenderEffectChain(EffectsToRender,(*iOriginalTexture), displayTarget, clearZone, drawZone, tbox, screenW / 2, screenH / 2);
								}
								catch (runtime_error err) {
									if (!application->warnedAboutShaders) {
										::MessageBox(NULL, err.what(), "Error", MB_OK | MB_ICONHAND);
										application->warnedAboutShaders = true;
									}
								}

								//RenderEffectChain(lyr->effects,(*iOriginalTexture), displayTarget, clearZone, drawZone, tbox, screenW / 2, screenH / 2);
							}
						}
					}

					//TODO: if a thumbnail doesn't need to be updated, set doitTwice to 2!
					if(layerTarget == displayTarget && bUpdatePreview) {
						AllocateDisplaySurface(iLayerTexture, m_display);
						layerTarget = (*iLayerTexture);
					}
					else
						doitTwice = 2;
				}

				// Blit to the thumbnail texture, if it exists and is time to be updated
				if ((*iThumbnail > -1)&& (bUpdatePreview))
				{
					float scale = 0.08f;
					
					
					(*m_display)->SetRenderTarget(*iThumbnail);
					(*m_display)->ClearRT(-1);

					D3DCOLOR bg = D3D_CONVERTCOLORREF(layout->GetColor());

					CRect frame;
					frame.left =  (m_Zoom*FRAME_MARGIN_W - scrollX) * scale;
					frame.top =  (m_Zoom*FRAME_MARGIN_H - scrollY) * scale;
					frame.right = frame.left + (layout->GetW()*m_Zoom)*scale;
					frame.bottom = frame.top + (layout->GetH()*m_Zoom)*scale;

					(*m_display)->SetTexture(*iDisplayTexture);
					(*m_display)->Fill(frame, bg);
					(*m_display)->Box(frame, 0xff000000);

					RECTF blitRec;
					SetRectF(&blitRec, 0, 0, LYR_THUMB_W, LYR_THUMB_H);

					// Calculate the portion of the layer texture viewable, and set the texture
					// coordinates to use this region only in the thumbnail
					RECTF uv;
					uv.left = 0;
					uv.top = 0;
					uv.right = float(clientRec.right) / float(screenW);
					uv.bottom = float(clientRec.bottom) / float(screenH);

					// Blit the layer texture to the thumbnail
					(*m_display)->SetTexture(*iLayerTexture);
					(*m_display)->Blit(0,0,scale,scale,0,-1,0);

					ID3DXBuffer* pFile;

					// Save the thumbnail to memory
					(*m_display)->EndBatch();
					D3DXSaveTextureToFileInMemory(&pFile, D3DXIFF_BMP, (*m_display)->TexInfo(*iThumbnail)->texture, NULL);

					// Get the memory as an HBITMAP
					CxMemFile memfile((BYTE*)pFile->GetBufferPointer(),pFile->GetBufferSize());
					CxImage image(&memfile, CXIMAGE_FORMAT_BMP);

					PreviewImage im;
					im.Init();
					*im.bitmap=image;//copy
					g_pLayerBar->m_layerDlg.images[lyr].Release();
					g_pLayerBar->m_layerDlg.images[lyr] = im;

					// Send the bitmap to the layer bar
				//	::DeleteObject(g_pLayerBar->m_layerDlg.images[j].bitmap);
				//g_pLayerBar->m_layerDlg.images[j].bitmap = image.MakeBitmap();

					g_pLayerBar->m_layerDlg.m_layerListBox.Invalidate();

					// Free
					pFile->Release();
					
					m_UpdatePreview = false;
				}
			}
		}
		{

		m_bUpdateFrame = FALSE;

		// Restore standard alpha blending
		(*m_display)->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		(*m_display)->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		pos = layout->layers.GetHeadPosition();

		// Draw selections and sizing handles
		// Batch these commands
		//(*m_display)->BeginBatch(BATCH_BOXES, m_sel.GetCount() * 9);

		(*m_display)->SetRenderTarget(displayTarget);

		POSITION pos = m_sel.GetHeadPosition();
		for (int i=0; i < m_sel.GetCount(); i++) 
		{
			long object = m_sel.GetNext(pos);

			if (!layout->objects.Lookup(object, o)) 
				continue;

			


			// if object is being sized or moved, change rect
			//CRect rc = GetTempChangedRect(o).GetBoundingRect();

	#define GETPOINT88(A,B,C) (ObjRect.GetPoint(A,B,C) + CPoint((m_Zoom*FRAME_MARGIN_W - scrollX), (m_Zoom*FRAME_MARGIN_H - scrollY)))
	#define DRAWLINE88(A,B,C,D)	(*m_display)->SmoothLine(0.5+((double)GETPOINT88(A,B,0).x), 0.5+((double)GETPOINT88(A,B,0).y), 0.5+((double)GETPOINT88(C,D,0).x), 0.5+((double)GETPOINT88(C,D,0).y),0xFF0000AA)
	#define DRAWLINE89(A,B,C,D)	(*m_display)->SmoothLine(0.5+((double)GETPOINT88(A,B,0).x), 0.5+((double)GETPOINT88(A,B,0).y), 0.5+((double)GETPOINT88(C,D,0).x), 0.5+((double)GETPOINT88(C,D,0).y),0xFFAAAA00)

			vector<CObj*> containeditems;

			layout->FindContainerObjects(o, containeditems);
			double x,y,w,h,a;
			//if(containeditems.size() != 0) // i cant believe i even have to write this...but if i dont groups crash :S
			for(vector<CObj*>::iterator i = containeditems.begin(); i != containeditems.end(); i++)
			{
				//First this is a little measure we take to prevent rendering the containers if the item is already selected
				bool renderthis = true;
				POSITION pos2 = m_sel.GetHeadPosition();
				while(pos2) 
				{
					long object = m_sel.GetNext(pos2);
					CObj* temp;
					if (layout->objects.Lookup(object, temp)) 
						if(temp == *i)
						{
							renderthis = false;
							pos2 = 0;
						}
				}
				if(renderthis)
				{
					CObjectRectangle ObjRect = (*i)->GetObjectRect(this);
					//////////////////////////
					ObjRect.CalculateBlitValues(x,y,w,h,a);
					(*m_display)->SetTexture(-1);

					x += (m_Zoom*FRAME_MARGIN_W - scrollX);
					y += (m_Zoom*FRAME_MARGIN_H - scrollY);
					(*m_display)->Blit(x,y,(int)w,(int)h,a,0x20202000);

					(*m_display)->SmoothLineBegin();
					DRAWLINE89(0,0,0,1);
					DRAWLINE89(0,1,1,1);
					DRAWLINE89(1,1,1,0);
					DRAWLINE89(1,0,0,0);
					(*m_display)->SmoothLineEnd();
				}
				////////////////////////

			}

			bool nonlayout = (GetOINFO(o->GetObjectType(application)->DLLIndex)->ideFlags & OF_NODRAW);

			CObjectRectangle ObjRect = o->GetObjectRect(this);
			//////////////////////////
			ObjRect.CalculateBlitValues(x,y,w,h,a);
			(*m_display)->SetTexture(-1);

			x += (m_Zoom*FRAME_MARGIN_W - scrollX);
			y += (m_Zoom*FRAME_MARGIN_H - scrollY);

			if(!InputLocked() & !nonlayout)
			{
				(*m_display)->Blit(x,y,(int)w,(int)h,a,0x20000020);
				(*m_display)->SmoothLineBegin();
				DRAWLINE88(0,0,0,1);
				DRAWLINE88(0,1,1,1);
				DRAWLINE88(1,1,1,0);
				DRAWLINE88(1,0,0,0);
				(*m_display)->SmoothLineEnd();
			}
			////////////////////////


//			rc.OffsetRect(-FRAME_MARGIN_W*m_Zoom, -FRAME_MARGIN_H*m_Zoom);

			if (o->m_AllowSize && !o->m_bIsGroup && !InputLocked() & !nonlayout)
			if (m_sel.GetCount() == 1)
			{
				// Draw sizing grips
				// Top left
				{
					CPoint pt;
					pt = ObjRect.GetPoint(0,0,8);
					CRect sz(pt,pt);
					sz.InflateRect(3,3);
					FrameFill(sz, 0xFFFFFFFF);
					FrameBox(sz, 0xFF0000AA);
				}

				// Top middle
				{
					CPoint pt;
					pt =  ObjRect.GetPoint(0.5,0,8);
					CRect sz(pt,pt);
					sz.InflateRect(2,2);
					FrameFill(sz, 0xFFFFFFFF);
					FrameBox(sz, 0xFF0000AA);
				}

				// Top right
				{
					CPoint pt;
					pt =  ObjRect.GetPoint(1,0,8);
					CRect sz(pt,pt);
					sz.InflateRect(3,3);
					FrameFill(sz, 0xFFFFFFFF);
					FrameBox(sz, 0xFF0000AA);
				}

				// Middle left
				{
					CPoint pt;
					pt =  ObjRect.GetPoint(0,0.5,8);
					CRect sz(pt,pt);
					sz.InflateRect(2,2);
					FrameFill(sz, 0xFFFFFFFF);
					FrameBox(sz, 0xFF0000AA);
				}

				// Middle right
				{
					CPoint pt;
					pt =  ObjRect.GetPoint(1,0.5,8);
					CRect sz(pt,pt);
					sz.InflateRect(2,2);
					FrameFill(sz, 0xFFFFFFFF);
					FrameBox(sz, 0xFF0000AA);
				}

				// Bottom left
				{
					CPoint pt;
					pt =  ObjRect.GetPoint(0,1,8);
					CRect sz(pt,pt);
					sz.InflateRect(3,3);
					FrameFill(sz, 0xFFFFFFFF);
					FrameBox(sz, 0xFF0000AA);
				}
				// Bottom middle
				{
					CPoint pt;
					pt =  ObjRect.GetPoint(0.5,1,8);
					CRect sz(pt,pt);
					sz.InflateRect(2,2);
					FrameFill(sz, 0xFFFFFFFF);
					FrameBox(sz, 0xFF0000AA);
				}

				// Bottom right
				{
					CPoint pt;
					pt =  ObjRect.GetPoint(1,1,8);
					CRect sz(pt,pt);
					sz.InflateRect(3,3);
					FrameFill(sz, 0xFFFFFFFF);
					FrameBox(sz, 0xFF0000AA);
				}
			}
			if (m_sel.GetCount() == 1)
			if(o->m_ShowAngles && !InputLocked())
			{				
				CPoint pt;
				pt = CPoint(ObjRect.x,ObjRect.y);
				CRect sz(pt,pt);
				sz.InflateRect(2,2,3,3);
				//FrameFill(sz, 0xFFFFFFFF);
				FrameBox(sz, 0xFF0000AA);

				int distance = 20;
				CPoint pt2;

				float sin_a, cos_a;
				cr::sincosf(Rad(ObjRect.a), &sin_a, &cos_a);
				pt2.x = pt.x + cos_a*distance;
				pt2.y = pt.y + sin_a*distance;

				sz = CRect(pt2,pt2);
				sz.InflateRect(3,3,4,4);
				FrameFill(sz, 0xFFFFFFFF);
				FrameBox(sz, 0xFF0000AA);

				//draw a line
				pt2 += CPoint((m_Zoom*FRAME_MARGIN_W - scrollX), (m_Zoom*FRAME_MARGIN_H - scrollY));
				pt += CPoint((m_Zoom*FRAME_MARGIN_W - scrollX), (m_Zoom*FRAME_MARGIN_H - scrollY));

				(*m_display)->SmoothLineBegin();
				(*m_display)->SmoothLine(pt.x + 0.5, pt.y+ 0.5, pt2.x+ 0.5, pt2.y+ 0.5, 0xFF0000AA);
				(*m_display)->SmoothLineEnd();

			}



		}
	}


	(*m_display)->SetTexture(-1);
	// Flush the lines to display
	//(*m_display)->EndBatch();

	if (m_bDraggingRect) 
	{
		CRect rcSel(min(m_oldPt.x, m_curPt.x), min(m_oldPt.y, m_curPt.y),
					max(m_oldPt.x, m_curPt.x), max(m_oldPt.y, m_curPt.y));

		FrameFill(rcSel, 0x20000020);
		FrameBox(rcSel, 0xFF0000AA);
	}

	// If we are modifying a Behavior, call its draw function

	(*m_display)->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE);
	(*m_display)->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);

	if(layout->m_pControlBehaviorExt)
	{
		

		layout->m_oControlBehaviorInfo->ETDrawObject(layout->m_pControlBehaviorExt);
	}

	// Draw frame outline
	if(!InputLocked())
		FrameBox(CRect(-2, -2, layout->GetW()*m_Zoom + 2, layout->GetH()*m_Zoom + 2),0x55000000);
	

	// Now draw the grid.// todo: make a function
	if(!InputLocked())
	if (layout->m_Grid)
	{
			CPoint offset(floor(FRAME_MARGIN_W*g_pLayoutView->m_Zoom), floor(FRAME_MARGIN_H*g_pLayoutView->m_Zoom));
			offset -= g_pLayoutView->GetScrollPosition();
			int xdist = (floor(offset.x / (GridWidth()*m_Zoom)) * (GridWidth()*m_Zoom));
			int ydist = (floor(offset.y / (GridHeight()*m_Zoom)) * (GridHeight()*m_Zoom));
			
			offset.x -= xdist;
			offset.y -= ydist;

			CRect client;
			GetClientRect(client);

			(*m_display)->SetTexture(-1);

			if(m_Zoom*GridWidth() > 4 && m_Zoom*GridHeight() > 4)
			{
				for(int x = offset.x; x < client.right; x+= GridWidth()*m_Zoom)
					for(int y = offset.y; y < client.bottom; y += GridHeight()*m_Zoom)
					{
						(*m_display)->Point(x,y,0x77000000);
						(*m_display)->Point(x-1,y-1,0x77ffffff);
				}
			}
		}
	
		// If we draw a bar, do so!
		if (m_bPanel)
		{
			CD3DDisplayFont Font;
			(*m_display)->CreateFont("Arial", 14, false, false, &Font);

			(*m_display)->FillGradient(CRect(0, 0, 1000, 30), D3D_CONVERTCOLORREF(RGB(255, 177, 140)), D3D_CONVERTCOLORREF(RGB(255, 255, 210)), DIR_UP);

			CString Text;
			Text.Format("'%s' added to layout", m_AddedObject);
			Text.Replace(".csx", "");
			(*m_display)->DrawText(Font, Text, CRect(7, 7, 190, 23), D3D_CONVERTCOLORREF(RGB(1,1,1)), 0);
		}

		// If the temp display target was used, blit that to display now.
		if (displayTarget > -1) {
			(*m_display)->SetTexture(displayTarget);
			(*m_display)->SetRenderTarget(-1);
			(*m_display)->Blit(0, 0);
		}


		(*m_display)->EndBatch();

		(*m_display)->EndScene();
	
		// Present just the visible zone
	
		//note: we need to do it differently using the current window as the devices can be shared
	
		(*m_display)->Present(&clientRec, m_hWnd);
		//(*m_display)->d3dDevice->Present(&clientRec, NULL,m_hWnd, NULL);
	}

	catch (CD3DError e) {

		// Device lost: save all effects in layers and types
		if (e.hr == D3DERR_DEVICELOST) {
			ObjTypeOnLostDevice onTypesLostDevice;
			LayerOnLostDevice onLayersLostDevice;
			ForEachObjectType(application, onTypesLostDevice);
			ForEachLayer(application, onLayersLostDevice);
			(*m_display)->OnLostDevice();
		}

		// Device lost
		HRESULT hr = (*m_display)->GetD3DDevice()->TestCooperativeLevel();

		// Ready to reset?
		if (hr == D3DERR_DEVICENOTRESET) {

			CProgressDlg ProgressDlg(_T("Re-loading textures"));
			ProgressDlg.Start();
			ProgressDlg.SetProgress(0);

			SetWindowPos(NULL, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_NOMOVE);
			CD3DDisplay& Display = **m_display;

			// Recreate all DirectX resources
			Display.ClearTextures();

			// Z buffer exists?
			if (layout->LayoutKeyExists("zbuffer")) {
				IDirect3DSurface9* pZBuffer = (IDirect3DSurface9*)layout->GetLayoutKey("zbuffer");
				pZBuffer->Release();
				layout->SetLayoutKey("zbuffer", NULL, false);
			}

			Display.Reset();

			ProgressDlg.SetProgress(10);

			// All our textures have bombed out
			application->iLayerTexture = -2;
			application->iOriginalTexture = -2;
			application->iEffectTargets[0] = -2;
			application->iEffectTargets[1] = -2;
			application->iDisplayTexture = -2;
			application->iPicture = -2;
			application->iThumbnail = -2;
			layout->hdcTexture = -2;
			CreateDirectXFrame();

			ProgressDlg.SetProgress(20);

			// Set sampler back to what it used to be
			if (application->game_information.sampler == CApplication::GameInformation::sample_linear) {
				(*m_display)->SetSamplerState(D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
				(*m_display)->SetSamplerState(D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			}

			// Clear the images map
			application->m_image_to_texture.clear();

			// Reinitialise objects
			CObj *o;
			OINFO *oInfo;
			DWORD nKey2;
			CObjMap* objMap = &(layout->objects);
			POSITION pos = objMap->GetStartPosition();
			long nKey;

			for (int i = 0; i < objMap->GetCount(); i++)
			{
				// get object and dll info
				objMap->GetNextAssoc(pos, nKey, o);

				POSITION Find = g_dlls->GetStartPosition();
				for (int k = 0; k < g_dlls->GetCount(); k++)
				{	
					g_dlls->GetNextAssoc(Find, nKey2, oInfo);
					if (oInfo->oID == o->GetDLLIdentifier())
						break;
				}

				if (oInfo->oID != o->GetDLLIdentifier())
					continue;

				// call object creation function
				oInfo->ETInitializeObject(o->editObject, layout);
			}

			ProgressDlg.SetProgress(90);

			// Recreate effects
			ObjTypeOnResetDevice onTypesResetDevice;
			LayerOnResetDevice onLayersResetDevice;
			ForEachObjectType(application, onTypesResetDevice);
			ForEachLayer(application, onLayersResetDevice);
			Display.OnResetDevice();


			CChildFrame* pChildFrame = dynamic_cast<CChildFrame*>(this->GetParent()->GetParent());
			pChildFrame->RecalcLayout();
			pChildFrame->splitter.RecalcLayout();

			ProgressDlg.SetProgress(100);

			ProgressDlg.Finish();


			// Draw once more
			OnDraw(NULL);

		}
	}

	m_drawCount++;
	HasCreated = true;
}

float directionalAddHorizontal(float value, float dist, int dir)
{
	switch(dir)
	{
	case 1:
	case 4:
	case 7:
		return value - dist;
	case 2:
	case 5:
	case 8:
		return value;
	case 3:
	case 6:
	case 9:
		return value + dist;
	}
	return 0;
}
float directionalSubtractHorizontal(float value, float dist, int dir)
{
	switch(dir)
	{
	case 1:
	case 4:
	case 7:
		return dist - value;
	case 2:
	case 5:
	case 8:
		return dist - value;
	case 3:
	case 6:
	case 9:
		return value - dist;
	}
	return 0;
}
float directionalAddVertical(float value, float dist, int dir)
{
	switch(dir)
	{
	case 1:
	case 2:
	case 3:
		return value - dist;
	case 4:
	case 5:
	case 6:
		return value;
	case 7:
	case 8:
	case 9:
		return value + dist;
	}
	return 0;
}
float directionalSubtractVertical(float value, float dist, int dir)
{
	switch(dir)
	{
	case 1:
	case 2:
	case 3:
		return dist - value;
	case 4:
	case 5:
	case 6:
		return dist - value;
	case 7:
	case 8:
	case 9:
		return value - dist;
	}
	return 0;
}

bool ScalingShouldBeHorizontal(float w, float h, int type)
{
	switch(type)
	{
	case 1:
	case 4:
	case 7:
		return true;
	case 2:
	case 5:
	case 8:
		return false;
	case 3:
	case 6:
	case 9:
		return true;
	}
	return false;
}

// I made this quick class to deal with the CPoint rounding shit
class pointf
{
public:
	pointf()	{
		x = 0;
		y = 0;
	}
	pointf(CPoint p){
		x = p.x;
		y = p.y;
	}

	float x;
	float y;
};

void round_float(float& x)
{
	x = floor(x + 0.5);
}

void round_double(double& x)
{
	x = floor(x + 0.5);
}

bool CLayoutEditor::inPrecisionMode()
{
	return (GetKeyState(VK_MENU) >> 4) ? true : false;
}


CObjectRectangle CLayoutEditor::GetTempChangedRect(CObj *o)
{
	double x = o->GetX();
	double y = o->GetY();
	double a = o->GetA();
	double w = o->GetW();
	double h = o->GetH();
	double ox = o->GetOriginX();
	double oy = o->GetOriginY();
	bool proportional = GetKeyState(VK_SHIFT) >> 4;

	// We temporarily give the CObj a new editInfo
	editInfo& info = *(o->editObject);
	editInfo old = info;
	//*(o->editObject) = &info;

	info.objectX = x;
	info.objectY = y;
	info.objectAngle = a;
	info.objectWidth = w;
	info.objectHeight = h;
	info.originX = ox;
	info.originY = oy;

	double diffXl = (m_curPt.x - m_oldPt.x) / m_Zoom;
	double diffYl = (m_curPt.y - m_oldPt.y) / m_Zoom;

	// We need to work out the distance...but along the normal of the object angle.
	float sin_a, cos_a;
	cr::sincosf(Rad(a), &sin_a, &cos_a);
	double diffX = cos_a * diffXl + sin_a * diffYl;
	double diffY = cos_a * diffYl - sin_a * diffXl;

	if(proportional)
	{
		double ratio = h / w;
		if(ScalingShouldBeHorizontal(w + diffX, h + diffY, m_sizeType))
		{
			diffY = directionalSubtractVertical(directionalAddHorizontal(w,diffX, m_sizeType) * ratio , h, m_sizeType);
		}
		else
		{
			diffX = directionalSubtractHorizontal(directionalAddVertical(h,diffY, m_sizeType) / ratio, w, m_sizeType);
		}
	}


	if(m_bSizingObject || m_bMovingObject)
	{
		switch(m_sizeType)
		{
		case 1:
		case 4:
		case 7:	// left
			{
				if(diffX >= w)	// prevent negative width :(
					diffX = w-1;

				CObjectRectangle oldrect = o->GetObjectRect(this, true); //oldrect.a = 0;
				info.objectWidth = w - diffX;
				CObjectRectangle newrect = o->GetObjectRect(this, true); //newrect.a = 0;
				pointf offset = oldrect.GetPoint(1,1) - newrect.GetPoint(1,1);
				info.objectX += offset.x / m_Zoom;
				info.objectY += offset.y / m_Zoom;

				if(!inPrecisionMode())
				{
					round_float(info.objectX);
					round_float(info.objectY);
					round_float(info.objectWidth);
				}

				break;
			}
			break;
		case 2:
		case 5:
		case 8:
			{
				if(proportional)
					info.objectWidth = w - diffX;
				if(!inPrecisionMode())
				{
					round_float(info.objectWidth);
				}
			}
			break;
		case 3:
		case 6:
		case 9:	// right
			{
				if(diffX <= -w)	// prevent negative width :(
					diffX = -w+1;
				CObjectRectangle oldrect = o->GetObjectRect(this, true); //oldrect.a = 0;
				info.objectWidth = w + diffX;
				CObjectRectangle newrect = o->GetObjectRect(this, true); //newrect.a = 0;
				pointf offset = oldrect.GetPoint(0,0) - newrect.GetPoint(0,0);
				info.objectX += offset.x / m_Zoom;
				info.objectY += offset.y / m_Zoom;


				if(!inPrecisionMode())
				{
					round_float(info.objectX);
					round_float(info.objectY);
					round_float(info.objectWidth);
				}

				break;
			}
			break;
		}

		// work out verticle
		switch(m_sizeType)
		{
		case 1:
		case 2:
		case 3:	// top
			{
			if(diffY >= h)	// prevent negative width :(
					diffY = h-1;

				CObjectRectangle oldrect = o->GetObjectRect(this, true); //oldrect.a = 0;
				info.objectHeight = (h - diffY);
				CObjectRectangle newrect = o->GetObjectRect(this, true); //newrect.a = 0;
				pointf offset = oldrect.GetPoint(1,1) - newrect.GetPoint(1,1);
				info.objectX += offset.x / m_Zoom;
				info.objectY += offset.y / m_Zoom;

				if(!inPrecisionMode())
				{
					round_float(info.objectX);
					round_float(info.objectY);
					round_float(info.objectHeight);
				}


				break;
			}
			break;
		case 4:
		case 5:
		case 6:
			{
				if(proportional)
					info.objectHeight = (h - diffY);
				if(!inPrecisionMode())
				{
					round_float(info.objectHeight);
				}

			}
			break;
		case 7:
		case 8:
		case 9:	// bottom
			{
				if(diffY <= -h)	// prevent negative width :(
					diffY = -h+1;
				CObjectRectangle oldrect = o->GetObjectRect(this, true); //oldrect.a = 0;
				info.objectHeight = h + diffY;
				CObjectRectangle newrect = o->GetObjectRect(this, true); //newrect.a = 0;
				pointf offset = oldrect.GetPoint(0,0) - newrect.GetPoint(0,0);
				info.objectX += offset.x / m_Zoom;
				info.objectY += offset.y / m_Zoom;

				if( inPrecisionMode())
				{
					round_float(info.objectX);
					round_float(info.objectY);
					round_float(info.objectHeight);
				}
				break;
			}
			break;
		case 10: //rotating handle
			{
				info.objectAngle = Deg(atan2((float)(m_curPt.y - y* m_Zoom),(float)( m_curPt.x - x* m_Zoom)));

				if(proportional)
				{
					info.objectAngle = 5 * (int)(info.objectAngle / 5.0f + 0.5f);
				}

			}
		}
	}

	// now round it
	//info.objectX = floor(info.objectX+0.51);
	//info.objectY = floor(info.objectY+0.51);

	// moving


	if (m_bMovingObject) {
			CPoint pt;
			GetCursorPos(&pt);
			ScreenToClient(&pt);
			pt = pt - g_ptOffset;
			info.objectX += (pt.x - m_oldPt.x - FRAME_MARGIN_W * m_Zoom + GetScrollPosition().x) / m_Zoom;
			info.objectY += (pt.y - m_oldPt.y - FRAME_MARGIN_H * m_Zoom + GetScrollPosition().y) / m_Zoom;
		}



	CObjectRectangle rect = o->GetObjectRect(this, true);
	info = old;

	return rect;
	
/*	CPoint tl = rc2.TopLeft();
	CPoint br = rc2.BottomRight();

	if ((m_bSizingObject) && (o->m_AllowSize)) {
		CPoint pt;
		CPoint pt2;
		switch (m_sizeType) 
		{
		case 1: // top left
			pt = tl + (m_curPt - m_oldPt);
			pt.x = min(pt.x, br.x - 2);
			pt.y = min(pt.y, br.y - 2);
			rc2 = CRect(pt, br);
			break;

		case 2: // top
			pt = tl + (m_curPt - m_oldPt);
			pt.y = min(pt.y, br.y - 2);
			rc2 = CRect(tl.x, pt.y, br.x, br.y);
			break;

		case 3: // top right
			pt = tl + (m_curPt - m_oldPt);
			pt2 = br + (m_curPt - m_oldPt);
			pt.y = min(pt.y, br.y - 2);
			pt2.x = max(pt2.x, tl.x + 2);
			rc2 = CRect(tl.x, pt.y, pt2.x, br.y);
			break;

		case 4: // left
			pt = tl + (m_curPt - m_oldPt);
			pt.x = min(pt.x, br.x - 2);
			rc2 = CRect(pt.x, tl.y, br.x, br.y);
			break;

		case 6: // right
			pt2 = br + (m_curPt - m_oldPt);
			pt2.x = max(pt2.x, tl.x + 2);
			rc2 = CRect(tl.x, tl.y, pt2.x, br.y);
			break;

		case 7: // bottom left
			pt = tl + (m_curPt - m_oldPt);
			pt2 = br + (m_curPt - m_oldPt);
			pt.x = min(pt.x, br.x - 2);
			pt2.y = max(pt2.y, tl.y + 2);
			rc2 = CRect(pt.x, tl.y, br.x, pt2.y);
			break;

		case 8: // bottom
			pt2 = br + (m_curPt - m_oldPt);
			pt2.y = max(pt2.y, tl.y + 2);
			rc2 = CRect(tl.x, tl.y, br.x, pt2.y);
			break;

		case 9: // bottom right
			pt2 = br + (m_curPt - m_oldPt);
			pt2.x = max(pt2.x, tl.x + 2);
			pt2.y = max(pt2.y, tl.y + 2);
			rc2 = CRect(tl, pt2);
			break;
		}
	}

	if (m_bMovingObject) {
		CPoint pt;
		GetCursorPos(&pt);
		ScreenToClient(&pt);
		pt = pt - g_ptOffset;
		rc2.OffsetRect(pt - m_oldPt - CPoint(FRAME_MARGIN_W*m_Zoom,FRAME_MARGIN_H*m_Zoom) + GetScrollPosition());
	}

	return rc2;*/
}

BOOL CLayoutEditor::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CLayoutEditor printing

BOOL CLayoutEditor::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLayoutEditor::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CLayoutEditor::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

static void CalcSizeTotalAreaWnd( CDC* pDC, const CLayoutEditor* pView,
                                  CRect& totalAreaWnd ) {
	if ( pDC->IsPrinting() ){
	
	// if we are printing we get the total area from the clip box. 
	// Perhaps there is a better way to get this information

		pDC->GetClipBox(&totalAreaWnd);
	
	} else {

	// if we don't print we just take the client area of the window

		pView->GetClientRect( totalAreaWnd );
	}
}