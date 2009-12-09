
// Definitions for application runtime only
#define D3D_CONVERTCOLORREFALPHA(a) ((COLORREF)a)
//#include "..\Common\DXSnips.h"

typedef DWORD D3DCOLOR;

#include "..\Common\Renderers\RendererCommon.h"

class null_renderer {
public:
};

typedef null_renderer Renderer;
typedef void* TextureHandle;
typedef void* FontHandle;
typedef cr::point point;
typedef cr::point3d point3d;
typedef cr::rect rect;
typedef cr::quad quad;
const TextureHandle unallocated_texture((TextureHandle)-1);
typedef int D3DRENDERSTATETYPE;
typedef int D3DSAMPLERSTATETYPE;