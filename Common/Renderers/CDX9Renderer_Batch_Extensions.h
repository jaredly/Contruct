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

/////////////////////////////////////////////////////////////////////////////
// Batch extensions for CRuntime effects processing.  Not an integral part
// of the CDX9Renderer display engine.  Dependencies on CRuntime/Construct-specifics
#pragma once

#ifdef CDX9RENDERER_USE_BATCH_EXTENSIONS
class CRunEffect;	// forwards decl. outside cr namespace
#endif //CDX9RENDERER_USE_BATCH_EXTENSIONS

namespace cr {

	struct CustomParamBatch {
		D3DXHANDLE handle;
		float value;
	};

	struct EffectParameterBatch {
		IDirect3DBaseTexture9*	foregroundTexture,
							 *	backgroundTexture,
							 *	sourceTexture,
							 *	previousTexture;
		float	frameCounter,
				boxLeft,
				boxTop,
				boxRight,
				boxBottom,
				boxWidth,
				boxHeight,
				hotspotX,
				hotspotY,
				pixelWidth,
				pixelHeight;
		float bgStart[2];
		float bgEnd[2];

		std::vector<CustomParamBatch> customParams;
	};

	class CBatchX_FX_Begin : public CBatchBase {
	public:
		CBatchX_FX_Begin(CDX9Renderer* _renderer, CRunEffect* _eff)
			: CBatchBase(_renderer, batchop_fx_begin),
			  eff(_eff)
		{}
			  
		CRunEffect* eff;
		EffectParameterBatch params;

		void Do();
	};

	class CBatchX_FX_BeginPass : public CBatchBase {
	public:
		CBatchX_FX_BeginPass(CDX9Renderer* _renderer, ID3DXEffect* _fx, uint _pass)
			: CBatchBase(_renderer, batchop_fx_beginpass),
			  fx(_fx),
			  pass(_pass)
		{}

		ID3DXEffect* fx;
		uint pass;

		void Do();
	};

	class CBatchX_FX_EndPass : public CBatchBase {
	public:
		CBatchX_FX_EndPass(CDX9Renderer* _renderer, ID3DXEffect* _fx)
			: CBatchBase(_renderer, batchop_fx_endpass),
			  fx(_fx)
		{}

		ID3DXEffect* fx;

		void Do();
	};

	class CBatchX_FX_End : public CBatchBase {
	public:
		CBatchX_FX_End(CDX9Renderer* _renderer, ID3DXEffect* _fx)
			: CBatchBase(_renderer, batchop_fx_end),
			  fx(_fx)
		{}

		ID3DXEffect* fx;

		void Do();
	};

}