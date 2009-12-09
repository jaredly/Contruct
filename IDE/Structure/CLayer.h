#pragma once

// Layer type
typedef enum {
	LAYER_NORMAL = 0,
	LAYER_WINDOWCTRLS,
	LAYER_NONFRAME,
	LAYER_INCLUDE
} LAYER_TYPES;

// Layer state
typedef enum {
	LAYSTATE_VISIBLE_UNLOCKED = 0,
	LAYSTATE_HIDDEN_UNLOCKED,
	LAYSTATE_VISIBLE_LOCKED,
	LAYSTATE_HIDDEN_LOCKED
} LAYER_STATE;

#define ISLAYERVISIBLE(a) (!(a&1))
#define ISLAYERLOCKED(a) ((a&2)>>1)
typedef enum {
	LAYSTATE_VISIBLE = 0x00,
	LAYSTATE_HIDDEN = 0x01,
	LAYSTATE_LOCKED = 0x00,
	LAYSTATE_UNLOCKED = 0x10
} LAYER_MASK_STATE;

// CLayer class
class CLayer
{
public:
    // Constructor/destructor
	CLayer(CString szName, long layerType);
	~CLayer();

    // Attributes
    void SetName( CString name ) {
		m_name = name;
	}
	CString GetName() const {
		return m_name;
	}

    // Operations
	bool Serialize(CArchive& ar);

	void GetEveryObject(CObjList& o, CLayout* pFrame, long parent = -1);

	// To save which properties are expanded
	map<CString, bool> expandSave;

    // Implementation
private:
public:
	// Colour filter
	COLORREF color;

	// Scrolling coefficients
	float scrollXf, scrollYf;

	float scrollX, scrollY;

	// Zoom coefficients
	float zoomXf, zoomYf;

	float zoomX, zoomY;

	// Semitransparency
	float opacity;

	// Layer effects
	list<CEffect> effects;

	// Clear the background 
	bool clearBack;
	bool forceOwnTexture;
	COLORREF backColor;

	long m_layerID;
	long m_layerType;
	BOOL m_bDelImage;
	long m_state;
	CBitmap m_iThumbnail;
	CString m_name;
	CObjList m_zOrder;
	float m_angle;

	enum LayerSampler {
		sample_default,	// use application setting
		sample_point,
		sample_linear
	} sampler;

	bool enable3d;	// enable 3D layering (Z buffer)

	int m_inherrit_layerID; // used when you want a layer to inherit another layer
};