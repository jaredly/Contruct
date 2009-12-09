 /////////////////////////////////////////////////////////////////////////////
 // CDX9Renderer															//
 // Copyright (c) 2009 Scirra												//
 // Written by Ashley Gullen												//
 /////////////////////////////////////////////////////////////////////////////
 /*
	This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include "stdafx.h"
#include "CDX9Renderer.h"

namespace cr {

	void CBatchX_FX_Begin::Do()
	{
		ID3DXEffect* pEffect = eff->pEffect;

		if (eff->vForegroundTexture)	pEffect->SetTexture(eff->vForegroundTexture, params.foregroundTexture);
		if (eff->vBackgroundTexture)	pEffect->SetTexture(eff->vBackgroundTexture, params.backgroundTexture);
		if (eff->vSourceTexture)		pEffect->SetTexture(eff->vSourceTexture, params.sourceTexture);
		if (eff->vPreviousTexture)		pEffect->SetTexture(eff->vPreviousTexture, params.previousTexture);
		if (eff->vFrameCounter)			pEffect->SetFloat(eff->vFrameCounter, params.frameCounter);
		if (eff->vBoxLeft)				pEffect->SetFloat(eff->vBoxLeft, params.boxLeft);
		if (eff->vBoxTop)				pEffect->SetFloat(eff->vBoxTop, params.boxTop);
		if (eff->vBoxRight)				pEffect->SetFloat(eff->vBoxRight, params.boxRight);
		if (eff->vBoxBottom)			pEffect->SetFloat(eff->vBoxBottom, params.boxBottom);
		if (eff->vBoxWidth)				pEffect->SetFloat(eff->vBoxWidth, params.boxWidth);
		if (eff->vBoxHeight)			pEffect->SetFloat(eff->vBoxHeight, params.boxHeight);
		if (eff->vHotspotX)				pEffect->SetFloat(eff->vHotspotX, params.hotspotX);
		if (eff->vHotspotY)				pEffect->SetFloat(eff->vHotspotY, params.hotspotY);
		if (eff->vPixelWidth)			pEffect->SetFloat(eff->vPixelWidth, params.pixelWidth);
		if (eff->vPixelHeight)			pEffect->SetFloat(eff->vPixelHeight, params.pixelHeight);
		if (eff->vBgStart)				pEffect->SetFloatArray(eff->vBgStart, params.bgStart, 2);
		if (eff->vBgEnd)				pEffect->SetFloatArray(eff->vBgEnd, params.bgEnd, 2);

		std::vector<CustomParamBatch>::iterator i = params.customParams.begin();
		std::vector<CustomParamBatch>::iterator end = params.customParams.end();

		for ( ; i != end; ++i)
			pEffect->SetFloat(i->handle, i->value);

		pEffect->Begin(NULL, 0);
	}

	void CBatchX_FX_BeginPass::Do()
	{
		fx->BeginPass(pass);
	}

	void CBatchX_FX_EndPass::Do()
	{
		fx->EndPass();
	}

	void CBatchX_FX_End::Do()
	{
		fx->End();
	}
}