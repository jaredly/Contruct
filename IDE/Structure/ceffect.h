#pragma once





// Runtime effect classes for drawing active effects
struct CRunEffectParameter {
	int type;
	CString name;
	CString varname;
	CString value;
	D3DXHANDLE handle;
};

enum effectscriptcode
{
	CODE_BLIT = 0,
	CODE_COMPLETE,
	CODE_RENDERSTATE,

};

class CRunEffectScript{
public:
	effectscriptcode code;
	double param[10];
};

class CRunEffect {
public:
	ID3DXEffect*	pEffect;
	CString			name;

	vector<CRunEffectParameter> params;
	vector<CRunEffectScript> script;

	CRunEffect()
		: pEffect(NULL)
	{}

	~CRunEffect()
	{
		if (pEffect)
			pEffect->Release();
	}

	// Look for all standard script parameters eg ForegroundTexture
	void GetParameters();
	// Setup and initialise default values and handles for script defined parameters
	void SetupCustomParameters();
	// Clone an effect and initialise parameters & defaults
	void InitialiseFrom(CRuntime* pRuntime, CRunEffect* pEffect);

	// For effects such as Lens which may sample pixels it is simultaneously writing to.
	bool			crossSampling;
 
	// For effects such as Warp which commonly need to sample pixels outside the bounding box.
	bool			borderMode;

	// For effects which may need the entire foreground buffer clear.  Unusual; slow performance
	bool			fullScreenMode;

	// True if anything in the chain references SourceTexture.
	bool			usesSourceTexture;

	// Handles
	D3DXHANDLE vBackgroundTexture;
	D3DXHANDLE vForegroundTexture;
	D3DXHANDLE vSourceTexture;
	D3DXHANDLE vPreviousTexture;
	D3DXHANDLE vFrameCounter;
	D3DXHANDLE vBoxLeft, vBoxTop, vBoxRight, vBoxBottom;
	D3DXHANDLE vBoxWidth, vBoxHeight;
	D3DXHANDLE vHotspotX, vHotspotY;
	D3DXHANDLE vPixelWidth, vPixelHeight;
	D3DXHANDLE vBgStart, vBgEnd;
};


// An effect parameter
class EffectParameter {
public:
	int type;
	CString variableName;
	CString value;
	CString name;
	CString description;

	bool operator==(const CString& other) const {
		CString l = name;
		CString r = other;
		l.MakeLower();
		r.MakeLower();
		return l == r;
	}
};

enum DisableShaderWhen {
	SVA_NOSETTING,
	SVA_PS20UNAVAILABLE,
	SVA_PS20AVAILABLE,
	SVA_PS14UNAVAILABLE,
	SVA_PS14AVAILABLE,
	SVA_PS11UNAVAILABLE,
	SVA_PS11AVAILABLE,
};

class CEffect {
public:
	CString Text;
	CString effectFile;
	float version;

	int m_effectID;

	bool initialised;

	CEffect()
		: initialised(0)
	{ disableMode = SVA_NOSETTING; }

	~CEffect()
	{
	}

	// Effect parameter info
	vector<EffectParameter> params;

	// Runtime effect class for rendering effect in frame
	CRunEffect runEffect;

	ACESEntry* GetACESEntry(int type, int index);
	ACESEntry temp;

	DisableShaderWhen disableMode;

	// Initialise
	bool Init(CLayoutEditor* pFrame);

	bool Serialize(CArchive& ar)
	{
		CString ExpectedName = "CEffect";
		int     Version      = 1;
		
		if (!SerializeClassHeader(ExpectedName, Version, ar))
			return false;

		if (ar.IsLoading())
		{
			int count;
			ar >> m_effectID;
			ar >> effectFile >> Text >> version;

			CPath path;
			path.SetToCurrentDirectory();

			// Quick check that the effect is there!
			if (!FileExists(path.GetFullPath() + "Effects\\" + effectFile))
			{
				CString Error;

				Error.Format("The effect file '%s' is required to open this file. Please locate it and put it in your effects directory under your Construct installation.", effectFile);

				CErrorDlg Dlg;
				Dlg.Error("Effect missing", Error);
				return false;
			}

			int temp;
			ar >> temp;
			disableMode = (DisableShaderWhen)temp;
			ar >> count;
			params.clear();

			for (int i = 0; i < count; i++) {
				params.push_back(EffectParameter());
				EffectParameter& p = params.back();
				ar >> p.type >> p.variableName >> p.value >> p.name >> p.description;
			}
		}
		else {
			ar << m_effectID;
			ar << effectFile << Text << version << (int)disableMode;
			ar << params.size();

			for (int i = 0; i < params.size(); i++) {
				EffectParameter& p = params[i];
				ar << p.type << p.variableName << p.value << p.name << p.description;
			}
		}

		return true;
	}

	// If compared to string, use Text member (used when we are testing if we can name something)
	bool operator==(const CString other) const {
		CString l = Text;
		CString r = other;

		l.MakeLower();
		r.MakeLower();

		return l == r;
	}
};