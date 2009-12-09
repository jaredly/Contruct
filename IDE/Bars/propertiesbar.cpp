// mybar.cpp : implementation file
//

#include "stdafx.h"
#include "PropertiesBar.h"
#include "../MainFrm.h"

IMPLEMENT_DYNCREATE(CExtGridCellHyperLinkEx, CExtGridCellHyperLink);
IMPLEMENT_DYNCREATE(CExtGridCellCurrencyEx, CExtGridCellCurrency);
IMPLEMENT_DYNCREATE(CExtGridCellNumberEx, CExtGridCellNumber);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern PropertiesBar* propertyWindow;
extern CImageList* m_treeImages;
extern CMainFrame* pMainWnd;

#include "..\ChildFrame.h"
#include "..\Construct.h"

// Image editor include
#include "..\Editors\ImageEditor.h"

extern CLayerBar* g_pLayerBar;
/////////////////////////////////////////////////////////////////////////////
// CProperties Bar
class CPerformanceTimer {
public:
	__int64 start;
	__int64 freq;

	map<CString, int> timers;

	CString curTimer;

	CPerformanceTimer() {
		QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	}

	void Start(CString name) {
		curTimer = name;
		QueryPerformanceCounter((LARGE_INTEGER*)&start);
	}

	void Stop() {
		__int64 c;
		QueryPerformanceCounter((LARGE_INTEGER*)&c);

		timers[curTimer] += c - start;
	}

	void QuickReport() {
		CString report = "Timers report:\n\n";
		
		map<CString, int>::iterator i = timers.begin();

		for ( ; i != timers.end(); i++) {
			CString line;
			
			float ms = ((float)(i->second) * 1000.0f) / (float)freq;

			line.Format("%s : %.2f ms\n", i->first, ms);
			report += line;
		}

		MessageBox(NULL, report, "Timers quickreport", MB_OK);
	}

};

#define IDC_PROPERTYTREE			11065



PropertiesBar::PropertiesBar()
{
	curExpandSave = NULL;
	m_pSelectedObjType = NULL;
	m_pSelectedObj = NULL;
}

PropertiesBar::~PropertiesBar()
{
	// Delete any allocated property stores
//	vector<CExtPropertyStore*>::iterator i = m_pMultiStore.begin();

	//for ( ; i != m_pMultiStore.end(); i++)
	//	delete *i;
}

BEGIN_MESSAGE_MAP(PropertiesBar, CExtControlBar)
	//{{AFX_MSG_MAP(PropertiesBar)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyBar message handlers

BOOL PropertiesBar::OnEraseBkgnd(CDC* pDC) 
{
      return TRUE;
}

void PropertiesBar::OnSize(UINT nType, int cx, int cy)
{
	CExtControlBar::OnSize(nType, cx, cy);
}

int PropertiesBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CExtControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pGrid->Create(this);

	return 0;
}

enum OPTION_CONTROL_TYPE {
	OPTION_CONTROL_EDIT,
	OPTION_CONTROL_STATIC,
	OPTION_CONTROL_BUTTON,
	OPTION_CONTROL_VALUE,
	OPTION_CONTROL_COMBO,
	OPTION_CONTROL_FLOAT,
	OPTION_CONTROL_COLOR,
	OPTION_CONTROL_CHECKBOX,
	OPTION_CONTROL_RADIO,
	OPTION_CONTROL_FORCE_DWORD = 0x7fffffff
};

// Refresh by repeating the last Update()
void PropertiesBar::Refresh()
{
	Update(m_view, m_type, &m_sel, m_CurrentLayout, m_objects, m_pCurrentApplication, 0, pLastLayer, NULL);
}

template <typename M, typename C>
bool PropertiesBar::SelectionMembersEqual(M C::* member)
{
	return ptrmem_members_equal(vSelectedObjects.begin(),
							    vSelectedObjects.end(),
								&CObj::editObject,
								member);
}

void PropertiesBar::ShowTip(std::string const tip)
{
	tips.Attach(m_pGrid->GetActiveGrid());

	tips.SetHighlightOnMouseOver();

	tips.SetWrapText();
	tips.SetText(tip.c_str());
}

void PropertiesBar::AddAnimationProperties()
{
	CExtPropertyItem* pRoot;
	CExtPropertyItem* pItem;
	CExtGridCellString* pString;
	CExtGridCellDropListComboBox* pCombo;
	CExtGridCellNumber* pNumber;
	CExtGridCellCheckBox* pCheck;

	m_pGrid->pCurView = m_view;
	curExpandSave = NULL;
	m_pGrid->pGridCategorized->expandSaves.resize(0);

	if (m_iIndex == -1)
	{
		pRoot = InsertCategory(AB_ANIMATION, AB_ANIMATIONPROPERTIES);

		if (m_pAnimation->m_Images.empty()) 
		{
			pItem = InsertAnimationItem(_T(AB_ANIMATIONNAME), AB_ANIMATIONNAMEDESC, m_pAnimation, -1, pRoot);
			pString = GetEdit(pItem);
			pString->TextSet(m_pAnimation->m_Name);
			pItem->ValueActiveFromDefault();

			pItem = InsertAnimationItem(AB_ANIMATIONTAG, AB_ANIMATIONTAGDESC, m_pAnimation, -1, pRoot);
			pCombo = GetCombo(pItem);
			pCombo->InsertString(NONE);
			pCombo->InsertString(AB_STOPPED);
			pCombo->InsertString(AB_WALKING);
			pCombo->InsertString(AB_JUMPING);
			pCombo->InsertString(AB_FALLING);
			pCombo->SetCurSel(m_pAnimation->tag);
			pItem->ValueActiveFromDefault();	
		} 
		
		else 
		{
			pItem = InsertAnimationItem(AB_ANIMATIONSPEED, AB_ANIMATIONSPEEDDESC, m_pAnimation, -1, pRoot);
			pNumber = GetInteger(pItem);
			pNumber->_VariantAssign(m_pAnimation->m_Speed);
			pItem->ValueActiveFromDefault();

			pItem = InsertAnimationItem(AB_ANIMATIONLOOP, AB_ANIMATIONLOOPDESC, m_pAnimation, -1, pRoot);
			pCheck = GetCheckBox(pItem);
			pCheck->SetCheck(m_pAnimation->m_bForever);
			pItem->ValueActiveFromDefault();

			pItem = InsertAnimationItem(AB_ANIMATIONREPEAT, AB_ANIMATIONREPEATDESC, m_pAnimation, -1, pRoot);
			pNumber = GetInteger(pItem);
			pNumber->_VariantAssign(m_pAnimation->m_Repeat);
			pItem->ValueActiveFromDefault();

			pItem = InsertAnimationItem(AB_ANIMATIONREPEATTO, AB_ANIMATIONREPEATTODESC, m_pAnimation, -1, pRoot);
			pNumber = GetInteger(pItem);
			pNumber->_VariantAssign(m_pAnimation->m_RepeatTo);
			pItem->ValueActiveFromDefault();

			pItem = InsertAnimationItem(AB_ANIMATIONPINGPONG, AB_ANIMATIONPINGPONGDESC, m_pAnimation, -1, pRoot);
			pCheck = GetCheckBox(pItem);
			pCheck->SetCheck(m_pAnimation->m_bPingPong);
			pItem->ValueActiveFromDefault();
		}
	}

	else
	{
		pRoot = InsertCategory(AB_FRAME, AB_FRAMEDESC);

		if(m_pAnimation->m_Images.size() == 0)
			return;
		pItem = InsertAnimationItem(AB_FRAMESPEED, AB_FRAMESPEEDDESC, m_pAnimation,  m_iIndex, pRoot);
		pNumber = GetFloat(pItem);
		pNumber->_VariantAssign(m_pAnimation->m_FrameTimes[m_iIndex]);
		pItem->ValueActiveFromDefault();
	}
	
}

void PropertiesBar::AddLayoutProperties(CLayout* layout)
{
	CExtPropertyItem* pRoot;
	CExtPropertyItem* pItem;
	CExtGridCellString* pString;
	CExtGridCellDropListComboBox* pCombo;
	CExtGridCellNumber* pNumber;
	CExtGridCellCheckBox* pCheck;
	CExtGridCellColor* pColor;
	CExtGridCellHyperLinkEx* pLink;

	m_CurrentLayout = layout;

	m_pGrid->pCurView = m_view;
	curExpandSave = &(layout->expandSave);
	set_one(m_pGrid->pGridCategorized->expandSaves, curExpandSave);

	m_pCurrentStore = m_pStore;
	
	pRoot = InsertCategory(LP_TITLE, LP_TITLE_DESC);

	pItem = InsertFrameItem(LP_NAME, LP_NAME_DESC, pRoot);
	pString = GetEdit(pItem);
	pString->TextSet(layout->GetName());
	pItem->ValueActiveFromDefault();

	pItem = InsertFrameItem(LP_WIDTH, LP_WIDTH_DESC, pRoot);
	pNumber = GetInteger(pItem);
	pNumber->_VariantAssign(layout->GetW());
	pItem->ValueActiveFromDefault();

	pItem = InsertFrameItem(LP_HEIGHT, LP_HEIGHT_DESC, pRoot);
	pNumber = GetInteger(pItem);
	pNumber->_VariantAssign(layout->GetH());
	pItem->ValueActiveFromDefault();

	pItem = InsertFrameItem(LP_APPLICATIONBACKGROUND, LP_APPLICATIONBACKGROUND_DESC, pRoot);
	pCheck = GetCheckBox(pItem);
	pCheck->SetCheck(layout->application_background);
	pItem->ValueActiveFromDefault();

	pItem = InsertFrameItem(LP_COLOUR, LP_COLOUR_DESC, pRoot);
	pColor = GetColor(pItem);
	pColor->SetColor(layout->GetColor());
	pItem->ValueActiveFromDefault();

	pItem = InsertFrameItem(LP_UNBOUNDEDSCROLLING, LP_UNBOUNDEDSCROLLING_DESC, pRoot);
	pCheck = GetCheckBox(pItem);
	pCheck->SetCheck(layout->m_unboundedScrolling);
	pItem->ValueActiveFromDefault();

	pItem = InsertFrameItem(LP_EVENT_SHEET, LP_EVENT_SHEET_DESC, pRoot);
	pCombo = GetCombo(pItem);
	CApplication* app = layout->application;
	int itemToSelect = 0;
	int index = 0;
	for(list<EventSheet*>::iterator i = app->event_sheets.begin(); i!= app->event_sheets.end(); i++, index++)
	{
		pCombo->InsertString((*i)->GetName());
		if((*i)->identifier == layout->event_sheet->identifier)
			itemToSelect = index;
	}

	pCombo->SetCurSel(itemToSelect);
	pItem->ValueActiveFromDefault();

	pItem = InsertFrameItem(_T("Load Textures"), _T("Override the application texture loading option for this layout."), pRoot);
	pCombo = GetCombo(pItem);
	pCombo->AddString(_T("(use application setting)"));
	pCombo->AddString(_T("On application startup"));
	pCombo->AddString(_T("On layout startup"));
	pCombo->SetCurSel((int)layout->texture_loading);
	pItem->ValueActiveFromDefault();

	pItem = InsertFrameItem(APPLICATION, LP_APPLICATION_DESC, pRoot);
	pLink = GetHyperLink(pItem);
	pLink->TextSet(PROPERTIES);
	pItem->ValueActiveFromDefault();
}

void PropertiesBar::AddLayerProperties(CLayer* layer)
{
	CExtPropertyItem* pRoot;
	CExtPropertyItem* pItem;
	CExtGridCellString* pString;
	CExtGridCellCheckBox* pCheck;
	CExtGridCellComboBox* pCombo;
	CExtGridCellColor* pColor;
	CExtGridCellHyperLinkEx* pLink;
	CExtGridCellCurrency* pCurr;

	curExpandSave = &(layer->expandSave);
	set_one(m_pGrid->pGridCategorized->expandSaves, curExpandSave);
	m_pGrid->pCurView = m_view;
	pLastLayer = layer;

	// Simply output property items to the display.
	m_pCurrentStore = m_pStore;

	pRoot = InsertCategory(LAYER_PROPERTIES, LAYER_PROPERTIES_DESC);

	pItem = InsertLayerItem(LAYER_NAME, LAYER_NAME_DESC, pRoot);
	pString = GetEdit(pItem);
	pString->TextSet(layer->GetName());
	pItem->ValueActiveFromDefault();

	pItem = InsertLayerItem(LAYER_TYPE, LAYER_TYPE_DESC, pRoot);
	pString = GetEdit(pItem);
	pString->ModifyStyle(__EGCS_READ_ONLY | __EGCS_NO_INPLACE_CONTROL);
	switch(layer->m_layerType) {
	case LAYER_NORMAL:
		pString->TextSet(LAYER_DISPLAY); break;
	case LAYER_NONFRAME:
		pString->TextSet(LAYER_NONLAYOUT); break;
	default:
		pString->TextSet("???"); break;
	}
	pItem->ValueActiveFromDefault();

	if (layer->m_layerType == LAYER_NORMAL) 
	{
		pRoot = InsertCategory(LAYER_DISPLAY, LAYER_DISPLAY_DESC);

		pItem = InsertLayerItem(LAYER_SHOW, LAYER_SHOW_DESC, pRoot);
		pCheck = GetCheckBox(pItem);
		pCheck->SetCheck(ISLAYERVISIBLE(layer->m_state));
		pItem->ValueActiveFromDefault();

		pItem = InsertLayerItem(LAYER_TRANSPARENT, LAYER_TRANSPARENT_DESC, pRoot);
		pCheck = GetCheckBox(pItem);
		pCheck->SetCheck(!layer->clearBack);
		pItem->ValueActiveFromDefault();

		pItem = InsertLayerItem(_T("3D layering"), _T("Enable 3D Z sorting on this layer with objects changing Z elevation."), pRoot);
		pCheck = GetCheckBox(pItem);
		pCheck->SetCheck(layer->enable3d);
		pItem->ValueActiveFromDefault();

		pItem = InsertLayerItem(LAYER_BACKGROUND, LAYER_BACKGROUND_DESC, pRoot);
		pColor = GetColor(pItem);
		if (!layer->clearBack) pColor->ModifyStyle(__EGCS_READ_ONLY | __EGCS_NO_INPLACE_CONTROL);
		pColor->SetColor(layer->backColor);
		pItem->ValueActiveFromDefault();

		pItem = InsertLayerItem(LAYER_SCROLLX, LAYER_SCROLLX_DESC, pRoot);
		pCurr = GetPercent(pItem);
		pCurr->_VariantAssign(layer->scrollX * 100.0);
		pItem->ValueActiveFromDefault();

		pItem = InsertLayerItem(LAYER_SCROLLY, LAYER_SCROLLY_DESC, pRoot);
		pCurr = GetPercent(pItem);
		pCurr->_VariantAssign(layer->scrollY * 100.0);
		pItem->ValueActiveFromDefault();

		pItem = InsertLayerItem(LAYER_SCROLLX_RATE, LAYER_SCROLLX_RATE_DESC, pRoot);
		pCurr = GetPercent(pItem);
		pCurr->_VariantAssign(layer->scrollXf * 100.0);
		pItem->ValueActiveFromDefault();

		pItem = InsertLayerItem(LAYER_SCROLLY_RATE, LAYER_SCROLLY_RATE_DESC, pRoot);
		pCurr = GetPercent(pItem);
		pCurr->_VariantAssign(layer->scrollYf * 100.0);
		pItem->ValueActiveFromDefault();

		pItem = InsertLayerItem(LAYER_ZOOMX, LAYER_ZOOMX_DESC, pRoot);
		pCurr = GetPercent(pItem);
		pCurr->_VariantAssign(layer->zoomX * 100.0);
		pItem->ValueActiveFromDefault();

		pItem = InsertLayerItem(LAYER_ZOOMY, LAYER_ZOOMY_DESC, pRoot);
		pCurr = GetPercent(pItem);
		pCurr->_VariantAssign(layer->zoomY * 100.0);
		pItem->ValueActiveFromDefault();

		pItem = InsertLayerItem(LAYER_ZOOMX_RATE, LAYER_ZOOMX_RATE_DESC, pRoot);
		pCurr = GetPercent(pItem);
		pCurr->_VariantAssign(layer->zoomXf * 100.0);
		pItem->ValueActiveFromDefault();

		pItem = InsertLayerItem(LAYER_ZOOMY_RATE, LAYER_ZOOMY_RATE_DESC, pRoot);
		pCurr = GetPercent(pItem);
		pCurr->_VariantAssign(layer->zoomYf * 100.0);
		pItem->ValueActiveFromDefault();

		pItem = InsertLayerItem(LAYER_OPACITY, LAYER_OPACITY_DESC, pRoot);
		pCurr = GetPercent(pItem);
		pCurr->_VariantAssign(layer->opacity * 100.0);
		pItem->ValueActiveFromDefault();

		pItem = InsertLayerItem(LAYER_FILTER, LAYER_FILTER_DESC, pRoot);
		pColor = GetColor(pItem);
		pColor->SetColor(layer->color);
		pItem->ValueActiveFromDefault();

		pItem = InsertLayerItem(LAYER_FORCEOWNTEXTURE, LAYER_FORCEOWNTEXTURE_DESC, pRoot);
		pCheck = GetCheckBox(pItem);
		pCheck->SetCheck(layer->forceOwnTexture);
		pItem->ValueActiveFromDefault();

		pItem = InsertLayerItem(_T("Sampling"), _T("Override the sampling for this layer."), pRoot);
		pCombo = GetCombo(pItem);
		pCombo->InsertString("(use Application setting)");
		pCombo->InsertString("Point");
		pCombo->InsertString("Linear");
		pCombo->SetCurSel(layer->sampler);
		pItem->ValueActiveFromDefault();

		// inheritance
		pRoot = InsertCategory(LAYER_INHERITANCE, LAYER_INHERITANCE_DESC);

		pItem = InsertLayerItem(LAYER_INHERITANCELAYER, LAYER_INHERITANCELAYER_DESC, pRoot);
		CLayer* result = m_pCurrentApplication->FindLayerFromNumber(layer->m_inherrit_layerID, NULL);
		CString name = "";
		if(result)
			name = result->m_name;
		
		pString = GetEdit(pItem);
		pString->TextSet(name);
		pItem->ValueActiveFromDefault();	

		// effects
		CExtPropertyItem* pEffectsRoot = InsertCategory(LAYER_EFFECTS, LAYER_EFFECTS_DESC);

		// For each effect
		EffectIterator ef = layer->effects.begin();
		EffectConstIterator fx_end = layer->effects.end();

		for ( ; ef != fx_end; ef++) 
		{
			/*
			CExtPropertyItem* pEffectCat = InsertCategory(e->Text, "...", pEffectsRoot);
			
			// Add each of the effect's properties.
			vector<EffectParameter>::iterator i = e->params.begin();
			const vector<EffectParameter>::const_iterator params_end = e->params.end();

			for ( ; i != params_end; i++) 
			{
				// Make a fake set of information to represent this property and add it
				CPropItem item;
				item.Description = i->description;
				item.Label = i->name;
				item.Type = i->type;
				item.Text = "???";

				CString itemText = i->value;

				PropReturn itemValue;
				itemValue.fVal = atof(itemText);
				itemValue.val = atoi(itemText);
				itemValue.val64 = _atoi64(itemText);

				pItem = InsertEffectItem(item.Label, item.Description, &(*e), layer->effects, NULL, layer, pEffectCat);
				AddPropertyItemToTree(item, pItem, itemText, itemValue);
				pItem->ValueActiveFromDefault();
			}

			pItem = InsertNeutralItem(" ", "...", pEffectCat);
			m_pGrid->propValueMgrs[pItem] = new CEffectPropertyValueMgr(&(*e), layer->effects, NULL, layer, m_view, REMOVE);
			m_pGrid->propValueMgrs[pItem]->pItem = pItem;
			pLink = GetHyperLink(pItem);
			pLink->TextSet(LAYER_REMOVEEFFECT);
			pItem->ValueActiveFromDefault();*/

			CString caption;
			caption.Format("%s (PS %.1f)", ef->Text, ef->version);
			CExtPropertyItem* pEffectCat = InsertCategory(caption, PROPERTIES_EFFECT_DESC, pEffectsRoot);

			pItem = InsertEffectItem("Name", "The name of this effect", &(*ef), layer->effects, NULL, layer, pEffectCat);
			pString = GetEdit(pItem);
			pString->TextSet(ef->Text);
			pItem->ValueActiveFromDefault();		
			
			pItem = InsertEffectItem("Disable when:", "Use a pixel shader version to affect the activated state of this effect.", &(*ef), layer->effects, NULL, layer, pEffectCat);
			pCombo = GetCombo(pItem);
			pCombo->AddString("(no setting)");
			pCombo->AddString("PS 2.0 Unavailable");
			pCombo->AddString("PS 2.0 Available");
			pCombo->AddString("PS 1.4 Unavailable");
			pCombo->AddString("PS 1.4 Available");
			pCombo->AddString("PS 1.1 Unavailable");
			pCombo->AddString("PS 1.1 Available");
			pCombo->SetCurSel((int)ef->disableMode);
			pItem->ValueActiveFromDefault();
			
			// Add each of the effect's properties.
			vector<EffectParameter>::iterator i = ef->params.begin();
			const vector<EffectParameter>::const_iterator params_end = ef->params.end();

			for ( ; i != params_end; i++) {

				// Make a fake set of information to represent this property and add it
				CPropItem item;
				item.Description = i->description;
				item.Label = i->name;
				item.Type = i->type;
				item.Text = "???";

				CString itemText = i->value;

				PropReturn itemValue;
				itemValue.fVal = atof(itemText);
				itemValue.val = atoi(itemText);
				itemValue.val64 = _atoi64(itemText);

				pItem = InsertEffectItem(item.Label, item.Description, &(*ef), layer->effects, NULL, layer, pEffectCat);
				AddPropertyItemToTree(item, pItem, itemText, itemValue);
				pItem->ValueActiveFromDefault();
			}





			pItem = InsertNeutralItem(" ", PROPERTIES_DESC_REMOVEEFFECT, pEffectCat);
			m_pGrid->propValueMgrs[pItem] = new CEffectPropertyValueMgr(&(*ef), layer->effects,
												NULL, layer, m_view, PROPERTIES_REMOVE);
			m_pGrid->propValueMgrs[pItem]->pItem = pItem;
			pLink = GetHyperLink(pItem);
			pLink->TextSet(PROPERTIES_REMOVEEFFECT);
			pItem->ValueActiveFromDefault();


		}

		// "Add new effect" link
		pItem = InsertLayerItem(LAYER_ADDEFFECT, LAYER_ADDEFFECT_DESC, pEffectsRoot);
		pLink = GetHyperLink(pItem);
		pLink->TextSet(ADD);
		pItem->ValueActiveFromDefault();
	}		
}

void PropertiesBar::AddResourceProperties(ApplicationResource* resource, CApplication* application, list<ApplicationResource>* stored)
{
	CExtPropertyItem* pRoot;
	CExtPropertyItem* pItem;
	CExtGridCellString* pString;
	CExtGridCellCheckBox* pCheck;
	CExtGridCellHyperLinkEx* pLink;

	m_pCurrentStore = m_pStore;
	
	pRoot = InsertCategory(PROPERTIES_RESOURCE, PROPERTIES_DESC_RESOURCE);

	pItem = InsertResourceItem(_T("Resource name"), PROPERTIES_DESC_RESNAME, application, stored, resource, pRoot);
	pString = GetEdit(pItem);
	pString->TextSet(resource->GetName());
	pItem->ValueActiveFromDefault();

	pItem = InsertAppItem(PROPERTIES_RESTYPE, PROPERTIES_DESC_RESTYPE, pRoot);
	pString = GetEdit(pItem);
	
	if (resource->GetType() == resource_binary)	pString->TextSet(RBT_BINARY);
	else if (resource->GetType() == resource_menu) pString->TextSet(RBT_MENU); 
	else if (resource->GetType() == resource_icon) pString->TextSet(RBT_ICON); 

	pString->ModifyStyle(__EGCS_READ_ONLY | __EGCS_NO_INPLACE_CONTROL);
	pItem->ValueActiveFromDefault();

	// if it's a binary resource, we'll add some extra goodies in
	if (resource->GetType() == resource_binary)	
	{
		// file location
		pItem = InsertResourceItem(_T("File location"), "Location of the file resource", application, stored, resource, pRoot);
		pString = GetEdit(pItem);
		pString->TextSet(((BinaryResource*)resource)->path);
		pItem->ValueActiveFromDefault();
 
		// check the extension
		int begin = ((BinaryResource*)resource)->path.ReverseFind('.');
		CString extension = ((BinaryResource*)resource)->path.Right(((BinaryResource*)resource)->path.GetLength() - begin);

		if (extension == ".wav" || extension == ".mp3" || extension == ".ogg" || extension == ".wma")
		{
			pItem = InsertResourceItem(_T("Preview"), "Preview this file", application, stored, resource, pRoot);
			pLink = GetHyperLink(pItem);
			pLink->TextSet("Preview");
			pItem->ValueActiveFromDefault();
		}
	}
}

void PropertiesBar::Update(CLayoutEditor* view, long type, CObjList* sel, CLayout* pLayout, CObjMap *objects, CApplication* app, long expand, CLayer* layer, ApplicationResource* resource, list<ApplicationResource>* stored) 
{
	if(app) m_pCurrentApplication = app;
	CPerformanceTimer timer;

	// Force the properties bar to show only the locked object if the layout editor is locked
	if(pLayout)
	{
		if(pLayout->m_ObjectFrameIsLockedTo != 0)
		{
			type = TYPE_OBJECT_CAPTURED;
		}
	}


	timer.Start("STARTUP");

	// Update type and if necessary application
	m_type = type;
	m_view = view;
	m_objects = objects;

	// Create a vector of selected objects, and a vector of the selected types.
	vSelectedObjects.resize(0);
	vSelectedTypes.resize(0);
	vSelectedOINFOs.resize(0);

	if (type == TYPE_OBJECT) {
		POSITION selPos;

		// Copy a cache of the passed selection.  If refreshing from the cache (Refresh()) don't clear.
		if (sel != &m_sel)
			m_sel.RemoveAll();

		selPos = sel->GetHeadPosition();
		while (selPos)
		{
			long curID = sel->GetNext(selPos);

			if (sel != &m_sel)
				m_sel.AddTail(curID);

			CObj* curObj = 0;
			objects->Lookup(curID, curObj);
			if(!curObj)
				continue;
			vSelectedObjects.push_back(curObj);

			CObjType* curType = curObj->GetObjectType(app);
			if(!curType)
				continue;
			

			// Add this type if it isn't already in the vector.
			if(curType)
			{
				push_back_unique(vSelectedTypes, curType);

				// Add this OINFO if it isn't already.
				OINFO* oInfo;
				DWORD id = curObj->GetDLLIdentifier();
				g_dlls->Lookup(id, oInfo);

				push_back_unique(vSelectedOINFOs, oInfo);
			}
		}
	}

	multiSelect = vSelectedObjects.size() > 1;
	bool oneTypeSelected = vSelectedTypes.size() == 1;
	bool onePluginSelected = vSelectedOINFOs.size() == 1;
	for(int g = 0; g < vSelectedObjects.size(); g++)
	{
		if(vSelectedObjects[g]->m_bIsGroup)
		{
			oneTypeSelected = false;
			onePluginSelected = false;
			vSelectedOINFOs.clear();


		}
	}

	CExtPropertyItem* pRoot;
	CExtPropertyItem* pItem;
	CExtGridCellString* pString;
	CExtGridCellDropListComboBox* pCombo;
	CExtGridCellNumber* pNumber;
	CExtGridCellCurrency* pCurr;
	CExtGridCellCheckBox* pCheck;
	CExtGridCellColor* pColor;
	CExtGridCellHyperLinkEx* pLink;

	CString str;

//	char *cValue;
//	long *lValue;
//	COLORREF *clrValue;
//	BOOL *bValue;

	//CObj* object;

	// Clear all items
	m_pStore->ItemRemove();
	m_pGrid->Reset();
	m_pGrid->pPropBar = this;
	hyperLinks.clear();

	vector<CExtPropertyStore*>::iterator p = m_pMultiStore.begin();

	for ( ; p != m_pMultiStore.end(); p++)
		delete *p;

	m_pMultiStore.resize(0);

	// Remove icons
	m_pGrid->pGridCategorized->GridIconRemove();

	timer.Stop();	// STARTUP

	switch (type) 
	{
	case TYPE_ANIMATION:

		AddAnimationProperties();
		break;

	case TYPE_RESOURCE:
		
		AddResourceProperties(resource, app, stored);
		break;

		// If application, insert application properties
	case TYPE_APP:

		////////////////////////////
		// APPLICATION PROPERTIES //
		////////////////////////////
		m_pCurrentApplication = app;

		// Simply output property items to the Adisplay.
		m_pCurrentStore = m_pStore;

		curExpandSave = &(m_pCurrentApplication->property_expand_state);
		set_one(m_pGrid->pGridCategorized->expandSaves, curExpandSave);

		DoAppProperties();
		
		break;

	case TYPE_LAYOUT:

		AddLayoutProperties(pLayout);
		break;

	case TYPE_LAYER:

		AddLayerProperties(layer);
		break;

	case TYPE_OBJECT:
	{
		///////////////////////
		// OBJECT PROPERTIES //
		///////////////////////
		
		if(vSelectedOINFOs.size() &&  !view->InputLocked())
		{	
			timer.Start("OBJ SETUP");

			OINFO* oInfo = vSelectedOINFOs.front();

			CObj* frontObj = vSelectedObjects.front();

			// Transfer expandsaves to the categorized grid, so any expands/collapsed are written to all selected
			// object's expandsaves.
			curExpandSave = &(frontObj->expandSave);
			m_pGrid->pGridCategorized->expandSaves.resize(0);

			vector<CObj*>::iterator e = vSelectedObjects.begin();
			int index = 0;

			// Iterate every selected object, creating a property store of properties, and combining in to display.
			for ( ; e != vSelectedObjects.end(); e++, index++) {

				CObjType* pObjType = (*e)->GetObjectType(app);

				if (pObjType == NULL) {
					CExtMsgBox::DoMsgBox(NULL, "Error listing properties for selection - NULL object type returned!  Please report this bug to Scirra.", "Error");
					continue;
				}

				// Check private variables havent messed up!
				if ((*e)->m_values.size() != pObjType->m_PrivateValues.size()) {
					CString msg;
					msg.Format("Error in Private Variables!  The selected object type has %d variables, but the selected instance has %d.  Please report this bug to Scirra!",
						pObjType->m_PrivateValues.size(), (*e)->m_values.size());
					CExtMsgBox::DoMsgBox(NULL, msg, "Error");
					continue;
				}

				// Every object will save expandsaves on the multiselection
				m_pGrid->pGridCategorized->expandSaves.push_back(&((*e)->expandSave));

				// One object selected: out to display
				if (vSelectedObjects.size() == 1)
					m_pCurrentStore = m_pStore;

				// If no property store exists for me, create one
				else if (m_pMultiStore.size() <= index) {
					// Allocate a new property store and use
					m_pCurrentStore = new CExtPropertyStore;
					m_pMultiStore.push_back(m_pCurrentStore);
				}
				// Property store already allocated, simply use it to place these properties in.
				else
					m_pCurrentStore = m_pMultiStore[index];

				timer.Stop(); // OBJ SETUP

				timer.Start("COMMON");
				
				///////////////////////
				// COMMON PROPERTIES //
				///////////////////////		

				pRoot = InsertCategory(PROPERTIES_COMMON, PROPERTIES_DESC_COMMON);

				// 'Name' is not valid when multiple types are selected - it must be unique!
					//??? TODO - review purpose of GlobalID being -1
					//if (object->GetGlobalID() == -1) str = object->m_tempName;
					//else str = object->GetObjectType(m_view->m_app)->GetName();

				pItem = InsertStdObjectItem(PROPERTIES_OBJECTNAME, PROPERTIES_DESC_OBJECTNAME, *e, pObjType, pRoot);
				pString = GetEdit(pItem);

				if (oneTypeSelected)
					pString->TextSet(vSelectedTypes.front()->GetName());
				else {
					pString->ModifyStyle(__EGCS_READ_ONLY | __EGCS_NO_INPLACE_CONTROL);
					
					CString multiNames;

					for (vector<CObjType*>::iterator t = vSelectedTypes.begin(); t != vSelectedTypes.end(); t++) {
						multiNames += (*t)->GetName();

						if (t != vSelectedTypes.end() - 1)
							multiNames += ", ";
					}
					pString->TextSet(multiNames);
				}

				pItem->ValueActiveFromDefault();
				
				// 0.99, add a type field
				if (oneTypeSelected) 
				{
					pItem = InsertStdObjectItem("Plugin", "The plugin this object belongs to", *e, pObjType, pRoot);
					pString = GetEdit(pItem);
					pString->ModifyStyle(__EGCS_READ_ONLY | __EGCS_NO_INPLACE_CONTROL);
					pString->TextSet(GetOINFO(pObjType->DLLIndex)->extName);
					pItem->ValueActiveFromDefault();
				}
					
				if (!(GetOINFO(pObjType->DLLIndex)->ideFlags & OF_NODRAW))
				{
					pItem = InsertStdObjectItem("X", PROPERTIES_DESC_X, *e, pObjType, pRoot);
					pNumber = GetInteger(pItem);
					pNumber->_VariantAssign((*e)->GetX());
					pItem->ValueActiveFromDefault();

					pItem = InsertStdObjectItem("Y", PROPERTIES_DESC_Y, *e, pObjType, pRoot);
					pNumber = GetInteger(pItem);
					pNumber->_VariantAssign((*e)->GetY());
					pItem->ValueActiveFromDefault();

					if((*e)->m_AllowSize)
					{
						pItem = InsertStdObjectItem(PROPERTIES_OBJECTWIDTH, PROPERTIES_DESC_OBJECTWIDTH, *e, pObjType, pRoot);
						pNumber = GetInteger(pItem);
						pNumber->_VariantAssign((*e)->GetW());
						pItem->ValueActiveFromDefault();

						pItem = InsertStdObjectItem(PROPERTIES_OBJECTHEIGHT, PROPERTIES_DESC_OBJECTHEIGHT, *e, pObjType, pRoot);
						pNumber = GetInteger(pItem);
						pNumber->_VariantAssign((*e)->GetH());
						pItem->ValueActiveFromDefault();
					}
				}

				if ((*e)->m_ShowAngles) 
				{
					pItem = InsertStdObjectItem(PROPERTIES_OBJECTANGLE, PROPERTIES_DESC_OBJECTANGLE, *e, pObjType, pRoot);
					pNumber = GetFloat(pItem);
					pNumber->_VariantAssign((*e)->editObject->objectAngle);
					pItem->ValueActiveFromDefault();
				}

				if ((GetOINFO(pObjType->DLLIndex)->ideFlags & OF_EFFECTS))
				{
					pItem = InsertStdObjectItem("Opacity", "Semitransparency of the object.", *e, pObjType, pRoot);
					pCurr = GetPercent(pItem);
					int opacity = (D3D_A((*e)->editObject->filter) * 100) / 255.0 + 0.5;
					pCurr->_VariantAssign(opacity);
					pItem->ValueActiveFromDefault();

					pItem = InsertStdObjectItem("Filter", "Tint the object a diferent color, white gives original colors", *e, pObjType, pRoot);
					pColor = GetColor(pItem);
					COLORREF col = COLORREF_CONVERTD3D((*e)->editObject->filter);
					pColor->SetColor(col);
					pItem->ValueActiveFromDefault();
				}

				if (oneTypeSelected) 
				{
					if (m_pCurrentApplication->runtime == CApplication::rt_directx)
					{
						pItem = InsertStdObjectItem("Destroy when:", "Only allow object to exist based on pixel shader support.", *e, pObjType, pRoot);
						pCombo = GetCombo(pItem);
						pCombo->AddString("(no setting)");
						pCombo->AddString("PS 2.0 Unavailable");
						pCombo->AddString("PS 2.0 Available");
						pCombo->AddString("PS 1.4 Unavailable");
						pCombo->AddString("PS 1.4 Available");
						pCombo->AddString("PS 1.1 Unavailable");
						pCombo->AddString("PS 1.1 Available");
						pCombo->SetCurSel((int)pObjType->destroyWhen);
						pItem->ValueActiveFromDefault();
					}

					pItem = InsertStdObjectItem(PROPERTIES_OBJECTICON, PROPERTIES_DESC_OBJECTICON, *e, pObjType, pRoot);
					pLink = GetHyperLink(pItem);
					pLink->TextSet(PROPERTIES_EDIT);
					pItem->ValueActiveFromDefault();

					// Resize the 32x32 icon
					if (!multiSelect) {
						CxImage img = pObjType->m_Image;
						img.Resample2(16, 16);
						SetGridCellIcon(pItem->ValueActiveGet(), img.MakeBitmap());
					}

					pItem = InsertStdObjectItem(PROPERTIES_SMALLOBJECTICON, PROPERTIES_DESC_SMALLOBJECTICON, *e, pObjType, pRoot);
					pLink = GetHyperLink(pItem);
					pLink->TextSet(PROPERTIES_EDIT);
					pItem->ValueActiveFromDefault();

					if (!multiSelect)
						SetGridCellIcon(pItem->ValueActiveGet(), pObjType->small_image.MakeBitmap());

					pItem = InsertStdObjectItem("Global", "Make this object global", *e, pObjType, pRoot);
					pCheck = GetCheckBox(pItem);
					pCheck->SetCheck(pObjType->m_bGlobal);
					pItem->ValueActiveFromDefault();

					pItem = InsertStdObjectItem("Show in editors", "Show this object in the event sheet editor", *e, pObjType, pRoot);
					pCheck = GetCheckBox(pItem);
					pCheck->SetCheck(pObjType->m_bShowInEventSheetEditor);
					pItem->ValueActiveFromDefault();

					// object folder
					pItem = InsertStdObjectItem("Object folder", "Folder this object will appear under in event wizard and project bar", *e, pObjType, pRoot);
					pCombo = GetCombo(pItem);

					vector<CApplication::ObjectFolder>::iterator folder = app->object_folders.begin();

					for ( ; folder != app->object_folders.end(); folder++)
						pCombo->AddString(folder->name);
		
					pCombo->SetCurSel(pCombo->FindStringExact(pObjType->GetFolder()));

					pItem->ValueActiveFromDefault();
				}

				timer.Stop();	// COMMON

				timer.Start("EFFECTS");


				////////////////////
				// OBJECT EFFECTS //
				////////////////////

				// If object allows effects
				if (oneTypeSelected && oInfo->ideFlags & OF_EFFECTS)
				{
					CExtPropertyItem* pEffectsRoot = InsertCategory(PROPERTIES_OBJECTEFFECTS, PROPERTIES_DESC_OBJECTEFFECTS);

					// For each effect
					EffectIterator ef = pObjType->effects.begin();
					EffectConstIterator fx_end = pObjType->effects.end();

					for ( ; ef != fx_end; ef++) 
					{
						CString caption;
						caption.Format("%s (PS %.1f)", ef->Text, ef->version);
						CExtPropertyItem* pEffectCat = InsertCategory(caption, PROPERTIES_EFFECT_DESC, pEffectsRoot);
	
						pItem = InsertEffectItem("Name", "The name of this effect", &(*ef), pObjType->effects, pObjType, NULL, pEffectCat);
						pString = GetEdit(pItem);
						pString->TextSet(ef->Text);
						pItem->ValueActiveFromDefault();		
						
						pItem = InsertEffectItem("Disable when:", "Use a pixel shader version to affect the activated state of this effect.", &(*ef), pObjType->effects, pObjType, NULL, pEffectCat);
						pCombo = GetCombo(pItem);
						pCombo->AddString("(no setting)");
						pCombo->AddString("PS 2.0 Unavailable");
						pCombo->AddString("PS 2.0 Available");
						pCombo->AddString("PS 1.4 Unavailable");
						pCombo->AddString("PS 1.4 Available");
						pCombo->AddString("PS 1.1 Unavailable");
						pCombo->AddString("PS 1.1 Available");
						pCombo->SetCurSel((int)ef->disableMode);
						pItem->ValueActiveFromDefault();
						
						// Add each of the effect's properties.
						vector<EffectParameter>::iterator i = ef->params.begin();
						const vector<EffectParameter>::const_iterator params_end = ef->params.end();

						for ( ; i != params_end; i++) {

							// Make a fake set of information to represent this property and add it
							CPropItem item;
							item.Description = i->description;
							item.Label = i->name;
							item.Type = i->type;
							item.Text = "???";

							CString itemText = i->value;

							PropReturn itemValue;
							itemValue.fVal = atof(itemText);
							itemValue.val = atoi(itemText);
							itemValue.val64 = _atoi64(itemText);

							pItem = InsertEffectItem(item.Label, item.Description, &(*ef), pObjType->effects, pObjType, NULL, pEffectCat);
							AddPropertyItemToTree(item, pItem, itemText, itemValue);
							pItem->ValueActiveFromDefault();
						}





						pItem = InsertNeutralItem(" ", PROPERTIES_DESC_REMOVEEFFECT, pEffectCat);
						m_pGrid->propValueMgrs[pItem] = new CEffectPropertyValueMgr(&(*ef), pObjType->effects,
															pObjType, NULL, m_view, PROPERTIES_REMOVE);
						m_pGrid->propValueMgrs[pItem]->pItem = pItem;
						pLink = GetHyperLink(pItem);
						pLink->TextSet(PROPERTIES_REMOVEEFFECT);
						pItem->ValueActiveFromDefault();



					}

					// "Add new effect" link
					pItem = InsertStdObjectItem(PROPERTIES_NEWEFFECT, PROPERTIES_DESC_NEWEFFECT, *e, pObjType, pEffectsRoot);
					pLink = GetHyperLink(pItem);
					pLink->TextSet(PROPERTIES_ADD);
					pItem->ValueActiveFromDefault();
				}

				timer.Stop();	// EFFECTS

				timer.Start("MOVEMENTS");

				//////////////////////
				// OBJECT MOVEMENTS //
				//////////////////////

				// If object allows movements
				m_pSelectedObjType = pObjType;
				m_pSelectedObj = frontObj;
				if (oneTypeSelected && oInfo->ideFlags & OF_MOVEMENTS)
				{
					int BehaviorCount = pObjType->behaviors.size();

					// Display either "Behaviors", "X Behavior" or "Behaviors (n)"
					CString movementsTitle, movementsDesc;
					if (BehaviorCount == 0)		movementsTitle = PROPERTIES_MOVEMENTS;
					else if(BehaviorCount == 1)	movementsTitle = pObjType->behaviors.front().Text + " " + PROPERTIES_MOVEMENT;
					else						movementsTitle.Format(_T("%s (%d)"), PROPERTIES_MOVEMENTS, BehaviorCount);

					// Description depending on number of movements showing
					if (BehaviorCount == 1)		movementsDesc = PROPERTIES_DESC_MOVEMENT;
					else						movementsDesc = PROPERTIES_DESC_MOVEMENTS;

					CExtPropertyItem* pBehaviorsRoot = InsertCategory(movementsTitle, movementsDesc);

					if (BehaviorCount == 1) 
					{
						// Add a Remove button for a single movement
						pItem = InsertNeutralItem(PROPERTIES_THISMOVEMENT, PROPERTIES_DESC_THISMOVEMENT, pBehaviorsRoot);
						// The "Remove" button is handled by the Behavior property manager.
						m_pGrid->propValueMgrs[pItem] = new BehaviorPropertyValueMgr(&(pObjType->behaviors.front()), pObjType, pLayout, m_view, "Remove");
						m_pGrid->propValueMgrs[pItem]->pItem = pItem;
						pLink = GetHyperLink(pItem);
						pLink->TextSet(PROPERTIES_REMOVE);
						pItem->ValueActiveFromDefault();

						if (!multiSelect)
							SetGridCellIcon(pItem->ValueActiveGet(), pObjType->behaviors.front().oInfo->hSmallIcon);
					}

					/////////////////////////////
					// ADD MOVEMENT PROPERTIES //
					/////////////////////////////

					// For each movement
					BehaviorIterator m = pObjType->behaviors.begin();
					BehaviorConstIterator mvt_end = pObjType->behaviors.end();

					for ( ; m != mvt_end; m++)
					{
						// Add a top-pLayout tree item for this movement.
						// If only one movement, use the existing movements group.
						CExtPropertyItem* pBehaviorCat;

						if (BehaviorCount > 1)
							pBehaviorCat = InsertCategory(m->Text, PROPERTIES_DESC_MOVEMENT, pBehaviorsRoot);
						else
							// One movement: put properties under existing header
							pBehaviorCat = pBehaviorsRoot;

						pItem = InsertBehaviorItem("Name", "The name of this movement.", &(*m), pObjType, pLayout, m_view, pBehaviorCat);
						pString = GetEdit(pItem);
						pString->TextSet(m->Text);
						pItem->ValueActiveFromDefault();

						// Create a temporary edittime movement instance
						// TODO: this could probably have a lifetime of CBehavior rather than
						// just a temporary property scope, but hey...
						editInfo* tempMovExt = new editInfo;
						pLayout->m_pTempMovExt = tempMovExt;
						pLayout->m_oTemporyBehaviorInfo = m->oInfo;
						pLayout->m_pControlBehaviorData = &m->eData;
						pLayout->m_pControlBehaviorDataSize = &m->eSize;

						tempMovExt->DLLIndex = m->BehaviorDLLIndex;
						tempMovExt->instanceID = -1;
						tempMovExt->peditInfo = pLayout;
						m->oInfo->ETCreateObject(tempMovExt, pLayout);

#ifndef STATICPROPERTIES
						PropertiesBar::Update_OINFO_Properties(tempMovExt);
#endif

						// Use an attached bin to tell the temporary to load from this movement's data
						bin movData(g_allocator);
						movData.attach(m->eData, m->eSize);
						movData.loading = true;
						m->oInfo->ETSerialize(tempMovExt, movData);
						movData.detach();

						// For each movement property
						vector<CPropItem>::iterator i = m->oInfo->Properties.begin();
						const vector<CPropItem>::const_iterator props_end = m->oInfo->Properties.end();

						for ( ; i != props_end; i++)
						{
							if (i->Type != -1) {
								// Add this property to the property tree, reading from the temp movement
								PropReturn Return;
								CString retStr;
								Return.CSPtr = &retStr;
								MicroPropertyVector mpv;	// For SDK-Construct communication

								// Get this property's value from the SDK
								m->oInfo->ETOnPropertyChanged(tempMovExt, i->Label, 2, NULL, &Return, &mpv);

								// Don't show behavior link properties when multiple objects selected
								if (i->Type == 2 && vSelectedObjects.size() > 1)
									continue;

								// Add the item control
								pItem = InsertBehaviorItem(i->Label, i->Description, &(*m), pObjType, pLayout, m_view, pBehaviorCat);
								AddPropertyItemToTree(*i, pItem, retStr, Return);
								pItem->ValueActiveFromDefault();
							}
						}
						if(pLayout->m_ObjectFrameIsLockedTo == -1)
						{
							// the tempory movement has been locked...so we dont want to free it.
						}
						else
						{
							// Now destroy the temporary
							m->oInfo->ETDestroyObject(tempMovExt);
							pLayout->m_pTempMovExt = 0;
							pLayout->m_oTemporyBehaviorInfo = 0;
							pLayout->m_pControlBehaviorData = 0;
							pLayout->m_pControlBehaviorDataSize = 0;
							delete tempMovExt;
						}


						// Add a Remove button for multiple movements
						if (BehaviorCount > 1) {
							pItem = InsertNeutralItem(" ", PROPERTIES_DESC_THISMOVEMENT, pBehaviorCat);
							m_pGrid->propValueMgrs[pItem] = new BehaviorPropertyValueMgr(&(*m), pObjType, pLayout, m_view, PROPERTIES_REMOVE);
							m_pGrid->propValueMgrs[pItem]->pItem = pItem;
							pLink = GetHyperLink(pItem);
							pLink->TextSet(PROPERTIES_REMOVE);
							pItem->ValueActiveFromDefault();

							if (!multiSelect)
								SetGridCellIcon(pItem->ValueActiveGet(), m->oInfo->hSmallIcon);
						}
					}

					// Add "Add new behavior" button
					pItem = InsertStdObjectItem(PROPERTIES_NEWMOVEMENT, PROPERTIES_DESC_NEWMOVEMENT, *e, pObjType, pBehaviorsRoot);
					pLink = GetHyperLink(pItem);
					pLink->TextSet(PROPERTIES_ADD);
					pItem->ValueActiveFromDefault();
				}

				timer.Stop();

				timer.Start("PRIVATE VARS");

				///////////////////////////
				// OBJECT PRIVATE VALUES //
				///////////////////////////
				if (oneTypeSelected && oInfo->ideFlags & OF_PRIVATEVARIABLES) {

					CExtPropertyItem* pValuesRoot = InsertCategory(PROPERTIES_PRIVATEVARS, PROPERTIES_DESC_PRIVATEVARS);

					// For each value
					vector<PrivateValue>::iterator v = pObjType->m_PrivateValues.begin();
					vector<PrivateValue>::iterator values_end = pObjType->m_PrivateValues.end();
					int index = 0;

					for ( ; v != values_end; v++, index++) {
						
						pItem = InsertPrivateVarItem(v->name, PROPERTIES_DESC_PRIVATEVAR, *e, pObjType, index, pValuesRoot);
						pString = GetEdit(pItem);
						pString->TextSet((*e)->m_values[index].value);
						pItem->ValueActiveFromDefault();

					}
					
					pItem = InsertStdObjectItem(PROPERTIES_EDITVARS, PROPERTIES_DESC_EDITVARS, *e, pObjType, pValuesRoot);
					pLink = GetHyperLink(pItem);
					pLink->TextSet(PROPERTIES_ADD_EDIT);
					pItem->ValueActiveFromDefault();
				}

				timer.Stop();	// PRIVATE VARS

				timer.Start("GROUPS");

				////////////
				// GROUPS //
				////////////

				pRoot = InsertCategory(PROPERTIES_GROUPS, PROPERTIES_DESC_GROUPS);

				//////////////////////
				// OBJECT TRAITS    //
				//////////////////////
				//if (GetOINFO(pObjType->DLLIndex)->ideFlags & OF_???TRAIT???)
				//if (oneTypeSelected)
				{
					int numTraits = pObjType->traits.size();

					CString TraitsTitle, TraitsDesc;
					TraitsTitle = PROPERTIES_ATTRIBUTES;
					TraitsDesc = PROPERTIES_DESC_ATTRIBUTES;
				
					CExtPropertyItem* pTraitsRoot = InsertCategory(TraitsTitle, TraitsDesc, pRoot);

					vector<CString>::iterator t = m_pCurrentApplication->traits.begin();
					vector<CString>::const_iterator traits_end = m_pCurrentApplication->traits.end();

					for ( ; t != traits_end; t++) {

						CString traitDesc;
						CString traitName = *t;

						if (traitName == ATTRIBS_SOLID)
							traitDesc = ATTRIBS_DESC_SOLID;
						else if (traitName == ATTRIBS_LADDER)
							traitDesc = ATTRIBS_DESC_LADDER;
						else if (traitName == ATTRIBS_PLATFORM)
							traitDesc = ATTRIBS_DESC_PLATFORM;
						else if (traitName == ATTRIBS_NOSERIALIZE)
							traitDesc = ATTRIBS_DESC_NOSERIALIZE;
						else if (traitName == ATTRIBS_HIFRIC)
							traitDesc = ATTRIBS_DESC_HIFRIC;
						else if (traitName == ATTRIBS_LOFRIC)
							traitDesc = ATTRIBS_DESC_LOFRIC;
						else if (traitName == ATTRIBS_CENTERVIEW)
							traitDesc = ATTRIBS_DESC_CENTER;
						else if (traitName == ATTRIBS_DESTROYSTART)
							traitDesc = ATTRIBS_DESC_DESTROYSTART;
						else if (traitName == ATTRIBS_DESTROYIFFAR)
							traitDesc = ATTRIBS_DESC_DESTROYIFFAR;
						else if (traitName == ATTRIBS_DESTROYIFNOTONSCREEN)
							traitDesc = ATTRIBS_DESC_DESTROYIFNOTONSCREEN;
						else
							traitDesc = ATTRIBS_DESC_USER;

						pItem = InsertTraitItem(traitName, traitDesc, traitName, pObjType, pTraitsRoot);
						pCheck = GetCheckBox(pItem);
						pCheck->SetCheck(find(pObjType->traits.begin(), pObjType->traits.end(), traitName) != pObjType->traits.end());
						pItem->ValueActiveFromDefault();
					}

					pItem = InsertAppItem(PROPERTIES_EDITATTRIBUTES, PROPERTIES_DESC_EDITATTRIBUTES, pTraitsRoot);
					pLink = GetHyperLink(pItem);
					pLink->TextSet(PROPERTIES_ADD_EDIT);
					pItem->ValueActiveFromDefault();
				}
				//////////////////////
				// OBJECT FAMILIES  //
				//////////////////////
				CExtPropertyItem* pFamiliesRoot = InsertCategory(PROPERTIES_FAMILIES, PROPERTIES_DESC_FAMILIES, pRoot);

				if (oneTypeSelected) {
					// Add families
					for (int i = 0; i < pObjType->families.size(); i++) {

  						pItem = InsertFamilyItem(pObjType->families[i]->name, PROPERTIES_DESC_FAMILY, pObjType, pFamiliesRoot);
						pLink = GetHyperLink(pItem);
						pLink->TextSet(PROPERTIES_REMOVE);
						pItem->ValueActiveFromDefault();

						if (!multiSelect)
							SetGridCellIcon(pItem->ValueActiveGet(), pObjType->families[i]->small_image.MakeBitmap());
					}
				}

				if (onePluginSelected) {
					pItem = InsertStdObjectItem(PROPERTIES_NEWFAMILY, PROPERTIES_DESC_NEWFAMILY, *e, pObjType, pFamiliesRoot);
					pLink = GetHyperLink(pItem);
					pLink->TextSet(PROPERTIES_ADD);
					pItem->ValueActiveFromDefault();
				}

				//////////////////////
				// OBJECT CONTAINER //
				//////////////////////
				{
					CExtPropertyItem* pContainerRoot = InsertCategory(PROPERTIES_CONTAINER, PROPERTIES_DESC_CONTAINER, pRoot);

					// Am I only object in container for some reason?  If so, delete the container.
					if (pObjType->m_pContainer) {
						if (pObjType->m_pContainer->m_IDs.size() == 1) {
							m_pCurrentApplication->DeleteContainer(pObjType->m_pContainer);
							pObjType->m_pContainer = NULL;
						}					
					}

					// Container is present?
					if (pObjType->m_pContainer) {

						vector<int>::iterator i = pObjType->m_pContainer->m_IDs.begin();
						vector<int>::iterator ids_end = pObjType->m_pContainer->m_IDs.end();

						for ( ; i != ids_end; i++) {

							// Same object type
							if (*i == pObjType->ObjectIdentifier) {
								pItem = InsertContainerItem(pObjType->GetName(), "This object.", pObjType->m_pContainer, -1, pContainerRoot);
								pString = GetEdit(pItem);
								pString->ModifyStyle(__EGCS_READ_ONLY | __EGCS_NO_INPLACE_CONTROL);
								pString->TextSet("(this object)");
								pItem->ValueActiveFromDefault();

								if (!multiSelect)
									SetGridCellIcon(pItem->ValueActiveGet(), pObjType->small_image.MakeBitmap());

								continue;
							}

							long lngID = *i;
							CObjType* pCurType = m_pCurrentApplication->object_types[lngID];

							pItem = InsertContainerItem(pCurType->GetName(), "Another object in the container.", pObjType->m_pContainer, pCurType->ObjectIdentifier, pContainerRoot);
							pLink = GetHyperLink(pItem);
							pLink->TextSet(PROPERTIES_REMOVE);
							pItem->ValueActiveFromDefault();

							if (!multiSelect)
								SetGridCellIcon(pItem->ValueActiveGet(), pCurType->small_image.MakeBitmap());
						}			
					}
					// No container
					else {
						pItem = InsertFamilyItem(pObjType->GetName(), "This object.", pObjType, pContainerRoot);
						pString = GetEdit(pItem);
						pString->ModifyStyle(__EGCS_READ_ONLY | __EGCS_NO_INPLACE_CONTROL);
						pString->TextSet("(this object)");
						pItem->ValueActiveFromDefault();

						if (!multiSelect)
							SetGridCellIcon(pItem->ValueActiveGet(), pObjType->small_image.MakeBitmap());
					}

					// Add objects to container link
					pItem = InsertStdObjectItem(PROPERTIES_NEWCONTAINER, PROPERTIES_DESC_NEWCONTAINER, *e, pObjType, pContainerRoot);
					pLink = GetHyperLink(pItem);
					pLink->TextSet("Add object");
					pItem->ValueActiveFromDefault();

				}

				timer.Stop();	// GROUPS

				///////////////////////////////////
				// ADD PLUGIN DEFINED PROPERTIES //
				///////////////////////////////////

				timer.Start("PLUGIN PROPS");

				// Only if one plugin type selected!
				if (onePluginSelected) {

#ifndef STATICPROPERTIES
					//TODO: what if 2 objects with the plugins have different properties...this wont have any effect..
					PropertiesBar::Update_OINFO_Properties(vSelectedObjects.front()->editObject);
#endif
					// Create a root item (root items should always be COptionTreeItem object since they
					// can not have properties
					pRoot = InsertCategory(PROPERTIES, PROPERTIES_DESC_OBJECT);

					// Get OINFO (come on guys, you can do this without 6 pLayouts of nesting too!)
					if ((*e)->GetGlobalID() == -1)
						throw;

					OINFO *oinfo;
					DWORD id = (*e)->GetDLLIdentifier();

					if (!g_dlls->Lookup(id, oinfo))
						throw;

					// For every property, read the saved value
					vector<CPropItem>::iterator i = oinfo->Properties.begin();
					const vector<CPropItem>::const_iterator props_end = oinfo->Properties.end();

					for ( ; i != props_end; i++)
					{
						// New category
						if (i->Type == 20) {

							pRoot = InsertCategory(i->Label, i->Description);

						}
						// Ordinary props.  Don't show link (type 2) props for multi selections.
						else if (i->Type != -1 && !(i->Type == 2 && vSelectedObjects.size() > 1)) {
							
							PropReturn Return;
							CString retStr;
							Return.CSPtr = &retStr;
							MicroPropertyVector mpv;	// For SDK-Construct communication

							// Get this property's value from the SDK
							oinfo->ETOnPropertyChanged((*e)->editObject, i->Label, 2, NULL, &Return, &mpv);

							// Add the property control
							pItem = InsertObjectItem(i->Label, i->Description, *e, NULL, pObjType, oinfo, pRoot);
							AddPropertyItemToTree(*i, pItem, retStr, Return);
							pItem->ValueActiveFromDefault();
						}
					}

				}

				timer.Stop();	// EXT PROPS

				timer.Start("COMBINE");

				if (vSelectedObjects.size() > 1)
					m_pStore->Combine(m_pCurrentStore);

				timer.Stop();	// COMBINE

			}
		}

		}
		break;

		case TYPE_OBJECT_CAPTURED:
		{
				timer.Start("EXT PROPS");

				editInfo* data = 0;
				OINFO *oinfo = 0;

				if(pLayout->m_ObjectFrameIsLockedTo == -1)
				{
					data = pLayout->m_pControlBehaviorExt;
					oinfo = pLayout->m_oControlBehaviorInfo;
				}
				else
				{
					CObj* lockedto = pLayout->FindObjFromNumber(pLayout->m_ObjectFrameIsLockedTo);
					if(!lockedto)
						throw;
					data = lockedto->editObject;
	
					// Get OINFO	
					DWORD id = lockedto->GetDLLIdentifier();

					if (!g_dlls->Lookup(id, oinfo))
						throw;
				}
		
#ifndef STATICPROPERTIES
				PropertiesBar::Update_OINFO_Properties(data);
#endif
				
				//if (onePluginSelected)
				{

					// Create a root item (root items should always be COptionTreeItem object since they
					// can not have properties
					pRoot = 0;

					// For every property, read the saved value
					vector<CPropItem>::iterator i = oinfo->Properties.begin();
					const vector<CPropItem>::const_iterator props_end = oinfo->Properties.end();

					for ( ; i != props_end; i++)
					{
						// New category
						if (i->Type == 20) {

							pRoot = InsertCategory(i->Label, i->Description);

						}
						// Ordinary props.  Don't show link (type 2) props for multi selections.
						else if (i->Type != -1) 
						{
							
							PropReturn Return;
							CString retStr;
							Return.CSPtr = &retStr;
							MicroPropertyVector mpv;	// For SDK-Construct communication

							// Get this property's value from the SDK
							oinfo->ETOnPropertyChanged(data, i->Label, 2, NULL, &Return, &mpv);

							// Add the property control
							pItem = InsertObjectItem(i->Label, i->Description, NULL, data, NULL, oinfo, pRoot);
							AddPropertyItemToTree(*i, pItem, retStr, Return);
							pItem->ValueActiveFromDefault();
						}
					}

				}

				timer.Stop();	// EXT PROPS


		}
		break;
	}

	timer.Start("SYNC");

	// Refresh grid
	m_pGrid->PropertyStoreSynchronize();
	m_pGrid->RecalcLayout();

	timer.Stop(); // SYNC

	CExtPropertyGridWnd* pGridWnd = m_pGrid->GetActiveGrid();
	pGridWnd->SetScrollPos(SB_VERT, 0);

	//if (type == TYPE_OBJECT && vSelectedObjects.size() > 20)
	//	timer.QuickReport();
}


void PropertiesBar::Update_OINFO_Properties(editInfo* info)
{
	CPropItem item;

	// We use a Virtual Property Item to abstract writing to CStrings
	// by taking their addresses.
	CVirtualPropItem vitem;
	vitem.Type = 0;
	vitem.Description = &(item.Description);
	vitem.Label = &(item.Label);
	vitem.Text = &(item.Text);


	OINFO* oInfo = GetOINFO(info->DLLIndex);
	
	oInfo->Properties.clear();
	// Micro vector tool abstracts the process of adding an element to a vector from the SDK
	MicroPropertyVector mpvt(&(oInfo->Properties), &item);

	// Call - davo : why is this called on loadup ?
	oInfo->ETOnPropertyChanged(info, NULL, 0, &vitem, NULL, &mpvt);

}