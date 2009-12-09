#if !defined PICTUREEDITOR_INCLUDED
#define PICTUREEDITOR_INCLUDED



#define COLOR_BLUE(a) (BYTE)a
#define COLOR_GREEN(a) (BYTE)(a>>8)
#define COLOR_RED(a) (BYTE)(a>>16)
#define COLOR_ALPHA(a) (BYTE)(a>>24)



#define MULTIPLYALPHA(a) D3DCOLOR_ARGB(COLOR_ALPHA(a),COLOR_RED(a)*COLOR_ALPHA(a)>>8,COLOR_GREEN(a)*COLOR_ALPHA(a)>>8,COLOR_BLUE(a)*COLOR_ALPHA(a)>>8)

#define DIVIDEALPHA(a) COLOR_ALPHA(a)==0?0:D3DCOLOR_ARGB(COLOR_ALPHA(a),COLOR_RED(a)>>8/COLOR_ALPHA(a),COLOR_GREEN(a)>>8/COLOR_ALPHA(a,COLOR_BLUE(a)>>8/COLOR_ALPHA(a))



class CFloatPoint
{
public:
	double x;
	double y;
	CFloatPoint(double _x,double  _y) {x =_x; y=_y;}
	CFloatPoint(){x=0; y=0;}
};



class CDragImage: public CObject
{
public:
	CDragImage() 
	{
		pImage = NULL;
	}
	virtual void Serialize( CArchive& ar )
	{
		if(ar.IsLoading())
		{
			long size;
			ar >> size;
			BYTE* buffer = new BYTE[size];
			ar.Read( buffer,size);
				
			pImage->Decode(buffer,size,CXIMAGE_FORMAT_PNG);
			delete[] buffer;
		}
		else
		{
			BYTE *buffer = 0;
			long size;
			pImage->Encode(buffer,size,CXIMAGE_FORMAT_PNG);
			ar << size;
			ar.Write(buffer,size);
			pImage->FreeMemory(buffer);
		}

	}
	
	CxImage* pImage;
};




// This structure is passed around to all tools
struct ToolProperties
{
	double Hardness;
	double Angle;
	double Thickness;
	double Size;
	double Step;
	double LineThickness;
	double Opacity;
	double Flow;
	double Alpha1;
	double Alpha2;

	CxImage src1;
	CxImage src2;

	int Brush_srcHeight;
	int Brush_srcWidth;
//	int Brush_Hardness;


	// Brush1 and Brush2 are generated images
	int Brush_Height; // actual
	int Brush_Width; // actual

	int Brush1;
	int Brush2;

	int Style;
	bool bSmooth;




};


class CPictureEditor;

////////////////////////////////////////////////
//      Tools
//----------------------------------------------



class CTool
{
public:
	
	CTool(){};

	virtual void Cancel();	// called if the user cancels

	virtual void PostRender(); //called after canvas is zoomed
	virtual void Render(); // for displaying on the canvas
	virtual void Initialise(ToolProperties* props); // called each time you switch to the tool
	
	virtual void OnMouseDown(CFloatPoint pt);
	virtual void OnMouseMove(CFloatPoint pt);
	virtual void OnMouseUp(CFloatPoint pt);


	virtual void Proportion();
	virtual void OnMouseDouble(CFloatPoint pt);
	// Special Drawing Operation

	float MouseSteps;
	float maxsteps;
	ToolProperties* m_pProp;

	CFloatPoint MouseCoords;
	CFloatPoint FirstMouseCoords;
	bool m_draw; // a mouse button is down
	CPictureEditor* PicEd;

	virtual void OverlayCallback();
	virtual void OverlayCallbackBegin() {};
	virtual void OverlayCallbackEnd() {};
	virtual void OnKeyDown(long key){};


	 int m_Brush;

	// Updating Properties
	 virtual void ChangeBrushSize(double size)
	 {
		(m_pProp->Size) = size;
	 }
	 virtual void ChangeHardness(double hard) 
	 {
		(m_pProp->Hardness) = hard/100.0;
	 }
	 virtual void ChangeThickness(long thick)
	 {
		(m_pProp->Thickness) = thick;
	 }
	 virtual void ChangeAngle(double angle)
	 {
	 	(m_pProp->Angle) = angle;
	 }
	 virtual void ChangeStep(double step)
	 {
		(m_pProp->Step) = step;
	 }
	 virtual void ChangeLineThickness(double thick)
	 {
		(m_pProp->LineThickness) = thick;
	 }
	 virtual void ChangeOpacity(double opacity)
	 {
		 (m_pProp->Opacity) = opacity;
	 }
	 virtual void ChangeFlow(double flow) 
	 {
		(m_pProp->Flow) = flow;
	 }
	 virtual void ChangeAlpha1(double alpha) 
	 {
		(m_pProp->Alpha1) = alpha;
	 }
	 virtual void ChangeAlpha2(double alpha) 
	 {
		(m_pProp->Alpha2) = alpha;
	 }

	 virtual void Modify()
	 {
	 }
	 virtual COLORREF GetColor();
	 virtual COLORREF GetColor2();
};


class CBrushTool : public CTool
{
// Functions
public:
	virtual void OnMouseMove(CFloatPoint pt);
	virtual void Brush(char* filename);
	virtual void ModifyBrush();

	virtual void Render();
	virtual void OnMouseDown(CFloatPoint pt);
	virtual void OnMouseUp(CFloatPoint pt);

	virtual void PenMark(float x, float y,float W,float H);
	virtual void DrawLine(float x, float y, float endx, float endy, float& MouseSteps,float& maxsteps);

	virtual void OverlayCallbackBegin();
	virtual void OverlayCallbackEnd();


	virtual void Modify()	 {		ModifyBrush();	 }

public:

	int m_BrushImage;
	bool m_Preview;
};


class CLineTool : public CBrushTool
{
public:
	virtual void OnMouseMove(CFloatPoint pt);
	virtual void Render();
};

class CErasorTool: public CBrushTool
{
	virtual void OverlayCallbackBegin();
	//virtual void PenMark(float x, float y,float W,float H);
};

class CPenTool : public CBrushTool
{
	virtual void ModifyBrush();
};

class CFillTool : public CTool
{
	virtual void OnMouseDown(CFloatPoint pt);


};

class CHotspotTool: public CTool
{
public:
	virtual void PostRender();
	virtual void Cancel();
	virtual void Initialise(ToolProperties* props);
	virtual void OnMouseUp(CFloatPoint pt);
	virtual void OnMouseDown(CFloatPoint pt);
	virtual void OnMouseMove(CFloatPoint pt);
	virtual void Modify();
	virtual void OnKeyDown(long key);
	CFloatPoint hotspot;

	virtual void UpdateDisplayOffset();
};

class CActionTool: public CHotspotTool
{
public:
	virtual void PostRender();
	virtual void OnMouseUp(CFloatPoint pt);
	virtual void Initialise(ToolProperties* props);
	virtual void Modify();
	virtual void UpdateDisplayOffset();

};

class CSelectFreeTool : public CTool
{
public:
	CSelectFreeTool()
	{
		m_resizeleft = false;
		m_resizeright = false;
		m_resizetop = false;
		m_resizebottom = false;
	}
	

	void OnMouseUpDrawingSelection(CFloatPoint pt);
	void OnMouseDownResize(CFloatPoint pt);
	void FinishResizing(CFloatPoint pt);
	void DrawDottedLine(int x, int y, int endx, int endy, int* count);
	void AddSelectionPoint(CFloatPoint pt);
	void ResizeMove(CFloatPoint pt);
	bool IsResizing();
	void DrawGrippers();
	virtual bool CreateSelectionForCxImage(CxImage& curImage);

public:
	void GetDragRects(CRect* Rect);


	virtual void OnMouseUp(CFloatPoint pt);
	virtual void OnMouseMove(CFloatPoint pt);
	virtual void OnMouseDown(CFloatPoint pt);
	virtual void PostRender();

public:
	vector<POINT> m_Sel;
	bool m_resizeleft;
	bool m_resizeright;
	bool m_resizetop;
	bool m_resizebottom;
};


class CSelectPolygonTool: public CSelectFreeTool
{
	virtual void OnMouseUp(CFloatPoint pt);
	virtual void OnMouseMove(CFloatPoint pt);
	virtual void OnMouseDown(CFloatPoint pt);
	virtual void PostRender();
	virtual void OnMouseDouble(CFloatPoint pt);
};
class CSelectRectangleTool: public CSelectFreeTool
{
	virtual void OnMouseUp(CFloatPoint pt);
	virtual void PostRender();
	bool CreateSelectionForCxImage(CxImage& curImage);
};
class CSelectEllipseTool: public CSelectRectangleTool
{
	//virtual void OnMouseUp(CFloatPoint pt);
	virtual void PostRender();
	bool CreateSelectionForCxImage(CxImage& curImage);
};
class CSelectColorTool: public CSelectFreeTool
{
	virtual void OnMouseUp(CFloatPoint pt);
	virtual void OnMouseMove(CFloatPoint pt);
	virtual void OnMouseDown(CFloatPoint pt);
	bool CreateSelectionForCxImage(CxImage& curImage);
};
class CSelectWandTool: public CSelectColorTool
{
	virtual bool CreateSelectionForCxImage(CxImage& curImage);
};
class CZoomTool: public CTool
{
	virtual void OnMouseDown(CFloatPoint pt);
};
class CPickTool: public CTool
{
public:
	int lasttool;
private:
	virtual void OnMouseDown(CFloatPoint pt);
};
class CCustomBrushTool: public CTool
{

};
class CTubeTool: public CTool
{

};
class CTextTool: public CTool
{

};
class CArrowTool: public CTool
{
	virtual void OnMouseDown(CFloatPoint pt);
};


class CPolygonTool: public CTool
{
public:
	virtual void Render();
	virtual void OnMouseDown(CFloatPoint pt);
	virtual void OnMouseUp(CFloatPoint pt);
	virtual void OnMouseMove(CFloatPoint pt);
	virtual void Cancel();
	virtual void OnMouseDouble(CFloatPoint pt);
	virtual void AddPoint(CFloatPoint pt);
	virtual void OnMouseUpDrawPolygon(CFloatPoint pt);

public:
	vector<POINT> m_CFloatPoint;
};

class CRectangleTool: public CTool
{
public:
	virtual void OnMouseMove(CFloatPoint pt);
	virtual void Render();

};


class CShapeTool : public CRectangleTool
{
	// The shape tool is used for rectangles and circles and other shapes
public:
	void Initialise(ToolProperties* props);
	virtual void Render();
	int m_shape;
};










class CDisplayState
{
public:
	unsigned long colourwrite;
	unsigned long srcblend;
	unsigned long destblend;
	int rendertarget;
	int currenttexture;

};
struct Line
{
	int x;
	int y;
	BYTE type;
	///type 
	//     _1_ 
	//  4 |x,y|2  
	//    |___|
    //      3 
};



class CPicUndo;
class CImageEditor;

CRect CropImage(CxImage& picture, bool border);

class CPictureEditor
{
public:
	D3DFORMAT GetTextureFormat();
	CPoint m_StatusCoordStart;
	CPoint m_StatusCoordEnd;
	void UpdateStatus();

	D3DTEXTUREFILTERTYPE GetBlendType();

	CImageEditor* m_pImageEditor;
	CPoint& GetHotspot();
	map<CString, CPoint>& GetAction();
	map<int, int> m_shapes;
	vector<CPicUndo*> m_undo;
	vector<CPicUndo*> m_redo;

	bool BackInvert;

	void Load(CxImage* loadfrom);
	bool m_modded;

	bool bSeamless;

	void Blit(float x, float y, float w, float h, float angle = 0.0f, D3DCOLOR filter = 0xFFFFFFFF, const RECTF* uv = NULL);

public:
	CPictureEditor()
	{
		m_Width = 0;
		m_Height = 0;
		CurrentTool = 0;
		m_CanvasWidth = 0;
		m_CanvasHeight = 0;
		m_TempCanvasWidth = 0;
		m_TempCanvasHeight = 0;
		m_DisplayStep = 0;
		m_DisplayStepWidth = 0;
		m_DisplayStepHeight = 0;
		m_modded = false;

		RightMouseDown = false;
		LeftMouseDown = false;
		m_TempCanvas = -2;

		BackInvert = false;
		bSeamless = false;

		m_Mask = -2;
		m_Props.Brush1 = -2;
		m_Props.Brush2 = -2;
		m_TempCanvas = -2;
		m_DisplayStep = -2;
		m_Background = -2;
		m_CanvasPromoted = -2;

		displayOffsetX = 0;
		displayOffsetY = 0;
		displayOffsetEnabled = true;
		

		m_editingCollision = false;

		grid = false;
		m_Zoom = 2.0;
	}

	void UpdateTool();
	~CPictureEditor();

public:
	int m_MouseX;
	int m_MouseY;
public:
	D3DCOLOR Colour1;
	D3DCOLOR Colour2;
	void Render();
	void PostRender();
	CSize GetPositionOffset();

public: //Textures	
	void RestoreDisplayState();
	void SaveDisplayState();
	void Promote(CRect rc);
	void DeleteSelection();
	void MakeDottedSelectionFromMask();

	vector<int> layers;
	vector<Line> m_Sel2;
	int* m_pLayer;


	void ClearTemp();

	int m_Mask; // The mask is applied over m_TempCanvas ( eg. for selections );
	bool m_hasMask;
	int m_Width;
	int m_Height;

	int m_CanvasPromoted; // Used for paste...selections...etc  
	
	int m_DisplayStep;
	int m_DisplayStepWidth;
	int m_DisplayStepHeight;

	int* m_pCanvas; // CFloatPoint to the surface index that we will actually draw to
	int m_CanvasWidth; // width of the canvas
	int m_CanvasHeight; // height of the canvas

	
	int m_TempCanvas; // The inbetween step
	int m_TempCanvasWidth; // The width of the tempory canvas
	int m_TempCanvasHeight; // The height of the tempory canvas


	int m_Background; // Checker background
	CRect m_RectTempToCanvas; // How to convert m_TempCanvas;

	bool m_editingCollision;


	float m_Zoom;

	int m_ScrollX;
	int m_ScrollY;

	bool grid;


	CDragDropMgr ddm;

	float displayOffsetX; // used for hotspot tool and image point tool
	float displayOffsetY;  // used for hotspot tool and image point tool
	bool displayOffsetEnabled;




public:
	void Initialise(CD3DDisplay* mydisplay,CxImage* MyImage);

public: // Some simple conversions

	void ScreentoCanvas(CFloatPoint& pt);
	void CanvastoScreen(CFloatPoint& pt);

public:
	int timer;
	void Save(CxImage* SaveTo);
	bool LeftMouseDown;
	bool RightMouseDown;
public:
	void Rotate(int Angle);
	void Flip();
	void Mirror();
	void OnEditCrop();
public:
	CTool* CurrentTool;
	CD3DDisplay* display;
	vector<CDisplayState> m_displayState;
	ToolProperties m_Props;
public:
	CRect CreateSelectionFromCxImage(CxImage& image);

	void DropSelection();
	void DrawDottedLine(int x, int y, int endx, int endy);
	void DrawSelection();
	void CheckerBackground();
	void Undo();
	void Redo();
	void CreateUndo(CString reason = "");
	void OnLMouseDown(CFloatPoint pt);
	void OnRMouseDown(CFloatPoint pt);
	void OnLMouseDouble(CFloatPoint pt);
	void OnMouseMove(CFloatPoint pt);
	void OnLMouseUp(CFloatPoint pt);
	void OnRMouseUp(CFloatPoint pt);
	void OnRMouseDouble(CFloatPoint pt);
	void TempToCanvas(bool replace = true);
	void CanvasToTemp(bool replace = true);
	void OnKeyDown(long key);

	void OnEditNew();
	void OnEditCut();
	void OnEditCopy();	
	void OnEditPaste();
	void OnEditSelectAll();

};

	


class CPicUndo
{
public:
	LPD3DXBUFFER ppImageBuf;
	CString m_text;
	int* pCanvas;	
	CxImage src;
	LPD3DXBUFFER ppMaskBuf;
	bool bMask;
	CRect RectToTemp;
	vector<Line> m_Sel2;

	

	void Save(CPictureEditor* Ed, CString message)
	{
		Ed->display->EndBatch();
		D3DXSaveTextureToFileInMemory(
			 &ppImageBuf,
			 D3DXIFF_PNG,
			Ed->display->TextureAt(*Ed->m_pCanvas),
			NULL);
		pCanvas = Ed->m_pCanvas;
		src.Decode((BYTE*)ppImageBuf->GetBufferPointer()
				   ,ppImageBuf->GetBufferSize(),CXIMAGE_FORMAT_PNG);

		bMask = false;
		if(Ed->m_hasMask)
		{
			bMask = true;
			D3DXSaveTextureToFileInMemory(
			 &ppMaskBuf,
			 D3DXIFF_PNG,
			 Ed->display->TextureAt(Ed->m_Mask),
			NULL);
		}
		RectToTemp = Ed->m_RectTempToCanvas;
		m_Sel2 = Ed->m_Sel2;

		
		m_text = message;
	}
	void Release()
	{
		ppImageBuf->Release();
	}
	void Restore(CPictureEditor* Ed)
	{
		bool IsSelection = true;
		if(*pCanvas == *Ed->m_pLayer)
		{
			IsSelection = false;
		}
		// Canvas or selection
		if(*pCanvas && *pCanvas != -2) //if its the selection, and the selection has been nulled, then theres no selection image to remove
			Ed->display->RemoveTexture(*pCanvas);
		Ed->display->SetTexture(-1);
		Ed->display->SetRenderTarget(-1);
		int texture = Ed->display->AddTextureFromBuffer((BYTE*)ppImageBuf->GetBufferPointer(), ppImageBuf->GetBufferSize());
		Ed->display->ConvertToRT(texture);
		
		//sizes
		Ed->m_CanvasWidth  =  Ed->display->GetTexture(texture)->imgwidth;
		Ed->m_CanvasHeight =  Ed->display->GetTexture(texture)->imgheight;
		if(!IsSelection)
		{
			Ed->m_Width = Ed->m_CanvasWidth;
			Ed->m_Height = Ed->m_CanvasHeight;
		}

		Ed->m_pCanvas = pCanvas;	
		*Ed->m_pCanvas = texture;


		if(bMask)
		{
			if(Ed->m_hasMask)
			{
				if(Ed->m_Mask)
					Ed->display->RemoveTexture(Ed->m_Mask);
			}
			Ed->m_Mask = Ed->display->AddTextureFromBuffer((BYTE*)ppMaskBuf->GetBufferPointer(), ppMaskBuf->GetBufferSize());
			Ed->display->ConvertToRT(Ed->m_Mask);
		}
		else
		{
			if(Ed->m_hasMask)
			{
				if(Ed->m_Mask)
					Ed->display->RemoveTexture(Ed->m_Mask);
			}
			Ed->m_Mask = -2;


		}
		Ed->m_hasMask = bMask;

		
	/**	CxImage temp;
		temp.Decode((BYTE*)ppImageBuf->GetBufferPointer()
				   ,ppImageBuf->GetBufferSize(),CXIMAGE_FORMAT_PNG);
		
		temp.Save("C:/test.png", CXIMAGE_FORMAT_PNG);*/

		Ed->m_RectTempToCanvas = RectToTemp;
		Ed->ClearTemp();
		Ed->m_Sel2 = m_Sel2;

		// note that as the current selected layer is a pointer to the layer, that value should be updated as well
	}


};



#endif