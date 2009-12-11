// Include StdAfx.h
#include "StdAfx.h"

template <class _T>
_T lerp(_T a, _T b, double r)
{
	return a * (1-r) + b * r;
}

class Drawing{
public:
	virtual void Draw(float x, float y, float w, float h, float iw, float ih, float a, float hsx, float hsy, 
		float left, float top, float right, float bottom, cr::color c_tl, cr::color c_tr, cr::color c_bl, cr::color c_br)
	{
		// Calculate uv's
		float u[4];
		float v[4];
		u[0] = 0;
		u[1] = left / iw;
		u[2] = (iw - right) / iw;
		u[3] = 1;

		v[0] = 0;
		v[1] = top / ih;
		v[2] = (ih - bottom) / ih;
		v[3] = 1;

		// Get a quad of our entire shape

		cr::point tl(x, y);
		tl -= cr::point(hsx, hsy);

		// Calculate width/heights
		float ww[4];
		float hh[4];

		ww[0] = min(w/2, left);
		ww[2] = min(w/2, right);
		ww[1] = w - ww[0] - ww[2];

		hh[0] = min(h/2, top);
		hh[2] = min(h/2, bottom);
		hh[1] = h - hh[0] - hh[2];

		// Calculate x and y
		float xx[4];
		float yy[4];

		xx[0] = tl.x;
		xx[1] = xx[0] + ww[0];
		xx[2] = xx[1] + ww[1];
		xx[3] = xx[0] + w;

		yy[0] = tl.y;
		yy[1] = yy[0] + hh[0];
		yy[2] = yy[1] + hh[1];
		yy[3] = yy[0] + h;

		// Now loop...
		for(int ix = 0; ix < 3; ++ix)
		{
			for(int iy = 0; iy < 3; ++iy)
			{
				cr::color c[2][2];

				for(int iix = 0; iix < 2; iix ++)
				{
					for(int iiy = 0; iiy < 2; iiy++)
					{
						c[iix][iiy] = lerp(
								lerp(c_tl, c_tr, (xx[ix+iix]-xx[0]) / w),
								lerp(c_bl, c_br, (xx[ix+iix]-xx[0]) / w),
								(yy[iy+iiy]-yy[0]) / h);
					}
				}
				
				cr::point p = cr::point(xx[ix], yy[iy]);
				p.rotate(cr::to_radians(a), cr::point(x,y) );

				RECTF uv = { u[ix], v[iy], u[ix+1], v[iy+1]};
				RenderChunk(p.x, p.y, ww[ix], hh[iy], a, c[0][0], c[1][0], c[0][1], c[1][1], &uv);
			}
		}
	}
private:
	virtual void RenderChunk(float x, float y, float w, float h, float a, cr::color tl, cr::color tr, cr::color bl, cr::color br, RECTF* uv) = 0;

};



#ifdef RUN_ONLY

class RunDrawing : public Drawing
{
public:
	RunDrawing(IRenderer* ir, cr::point uvscale) : renderer(ir), uv_scale(uvscale){}

private:
	IRenderer* renderer;
	cr::point uv_scale;

	virtual void RenderChunk(float x, float y, float w, float h, float a, cr::color tl, cr::color tr, cr::color bl, cr::color br, RECTF* _uv)
	{
		cr::color colors[4] = {tl, tr, bl, br};
		cr::rect uv(_uv->left, _uv->top, _uv->right, _uv->bottom);
		uv *= uv_scale;

		renderer->Quad_xywh(x, y, w, h,cr::to_radians(a), cr::point(0,0), opaque_white, &uv, 0,0, colors, 0); 
	}
};


/////////////////////////////
// RUNTIME drawing
// Draw: Construct calls this when it wants you to draw your object.  You can
// leave it blank if your plugin doesn't draw anything.  This is not called when OF_NODRAW is set.
void ExtObject::Draw()
{
	double hsx = info.HotSpotX;
	double hsy = info.HotSpotY;
	

	float& x = info.x;
	float& y = info.y;
	float& w = info.w;
	float& h = info.h;
	float& a = info.angle;

	// Get original image size
	float iw = iTexture->image_widthf;
	float ih = iTexture->image_heightf;
	renderer->SetTexture(iTexture);



	cr::point uv_scale( iw / iTexture->surface_width, ih / iTexture->surface_height);
	RunDrawing draw(renderer, uv_scale);
	draw.Draw(x, y, w, h, iw, ih, a, ceil(hsx), ceil(hsy), 
		image_left_margin, image_top_margin, image_right_margin, image_bottom_margin,
		vertex_filter[0] * info.pInfo->filter, vertex_filter[1] * info.pInfo->filter, vertex_filter[2] * info.pInfo->filter, vertex_filter[3] * info.pInfo->filter);

}

#else // RUN_ONLY

/////////////////////////////
// EDITTIME drawing
// Draw your object in the layout editor.

cr::color MixColorAndAlphaWithFilter(COLORREF c, float o, D3DCOLOR f)
{
	D3DCOLOR d = D3D_CONVERTCOLORREF(c);
	cr::color filter = cr::color(D3D_A(f) / 255.0, D3D_R(f) / 255.0, D3D_G(f) / 255.0, D3D_B(f) / 255.0);
	cr::color other = cr::color(c);
	other.a = o;

	return filter * other;
}

D3DCOLOR D3dColor(cr::color c)
{
	return D3D_ARGB((int)(c.a*255), (int)(c.r*255), (int)(c.g*255), (int)(c.b*255));
}

void GetHotspotPos(hotspot_position pos, double& hsx, double& hsy)
{
	switch (pos) 
	{
		case hs_topleft:
			hsx = 0.0;
			hsy = 0.0;
			break;
		case hs_top:
			hsx = 0.5;
			hsy = 0.0;
			break;
		case hs_topright:
			hsx = 1.0;
			hsy = 0.0;
			break;
		case hs_left:
			hsx = 0.0;
			hsy = 0.5;
			break;
		case hs_center:
			hsx = 0.5;
			hsy = 0.5;
			break;
		case hs_right:
			hsx = 1.0;
			hsy = 0.5;
			break;
		case hs_bottomleft:
			hsx = 0.0;
			hsy = 1.0;
			break;
		case hs_bottom:
			hsx = 0.5;
			hsy = 1.0;
			break;
		case hs_bottomright:
			hsx = 1.0;
			hsy = 1.0;
			break;
	}
}

class EditDrawing : public Drawing
{
public:
	EditDrawing(VEditTime* et) : pEditTime(et) {}

private:
	class VEditTime* pEditTime;

	virtual void RenderChunk(float x, float y, float w, float h, float a, cr::color tl, cr::color tr, cr::color bl, cr::color br, RECTF* uv)
	{
		pEditTime->Blitrc(x, y, w, h, a, -1, D3dColor(tl), D3dColor(tr), D3dColor(bl), D3dColor(br), uv);  
	}
};

void EditExt::Draw()
{
	int iTexture = pEditTime->AddImageAsTexture(imgTexture); // This gets looked up in a map

	// Calculate hotspot
	double hsx, hsy;
	GetHotspotPos((hotspot_position)hotspot_pos, hsx, hsy);
	
	// Tell the info about this...
	pInfo->originX = hsx;
	pInfo->originY = hsy;

	// Get our corner colours
	cr::color vertex_filter[4];
	for(int i = 0; i < 4; i++)
		vertex_filter[i] = MixColorAndAlphaWithFilter(vertex[i], vertex_opacity[i], pInfo->filter);

	// Now to draw our box....
	pEditTime->SetTexture(iTexture);

	// Get easy variables
	float x = pInfo->objectX;
	float y = pInfo->objectY;
	float w = pInfo->objectWidth;
	float h = pInfo->objectHeight;
	float a = pInfo->objectAngle;

	// Get original image size
	SIZE texturesize = pEditTime->GetImageSize(imgTexture);
	float iw = texturesize.cx;
	float ih = texturesize.cy;

	EditDrawing draw(pEditTime);
	draw.Draw(x, y, w, h, iw, ih, a, ceil(hsx * w), ceil(hsy * h), 
		image_left_margin, image_top_margin, image_right_margin, image_bottom_margin,
		vertex_filter[0], vertex_filter[1], vertex_filter[2], vertex_filter[3]);

}

#endif // RUN_ONLY