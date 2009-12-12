
// File includes
#include "StdAfx.h"
#include "..\Construct.h"
#include "..\Construct Doc.h"
#include "..\Bars\LayerBar.h"
#include "..\Editors\LayoutEditor.h"
#include "..\ChildFrame.h"
#include "..\Dialogs\Insert Object Dlg.h"
#include "..\Dialogs\ArrayPasteDlg.h"
#include "..\MainFrm.h"
#include "..\Dialogs\Used Plugins Dlg.h"
#include "..\Dialogs\New Behavior Dlg.h"
#include "..\Dialogs\NewTransitionDlg.h"
#include "..\Dialogs\NewHLSLDlg.h"
#include "..\Dialogs\ImageEditorDlg.h"
#include "..\Dialogs\AddVariableDlg.h"
#include "..\Dialogs\NewFamilyDlg.h"
#include "..\Dialogs\ManageFamiliesDlg.h"
#include "..\Dialogs\AttributesManagerDlg.h"
#include "..\Dialogs\ManageVariableDlg.h"
#include "..\Dialogs\Family Manager dialog\Family Manager Dlg.h"
#include "..\Dialogs\PythonLibs.h"
#include "..\UI Elements\XHTMLTree\XHTMLTree.h"
#include "..\Dialogs\ControlsDlg.h"
#include "..\Utilities\Helper.h"

extern CMainFrame* pMainWnd;

// Helper function for enabling/disabling other controls
void EnableItemByName(CExtPropertyItem* pParent, CString name, bool bEnable = true, DWORD addStyle = 0)
{
	CExtPropertyItem* pItem = pParent->ItemGetByName(name);

	if (bEnable)
		pItem->ValueActiveGet()->ModifyStyle(__EGCS_READ_ONLY | __EGCS_NO_INPLACE_CONTROL | addStyle);
	else
		pItem->ValueActiveGet()->ModifyStyle(0, __EGCS_READ_ONLY | __EGCS_NO_INPLACE_CONTROL | addStyle);
}

//////////////////////
// PROPERTY CHANGES //
// Get and store the value of a property that changed using dynamic casts.
void CPropValueMgr::GetPropertyValue()
{
	CExtGridCellString* pString;
	CExtGridCellComboBox* pCombo;
	CExtGridCellNumber* pNumber;
	CExtGridCellCheckBox* pCheck;
	CExtGridCellColor* pColor;
	CExtGridCellSlider* pSlider;
	CExtGridCellCurrency* pCurrency;

	if (pSlider = dynamic_cast<CExtGridCellSlider*>(pCell)) {
		value.i = ((CExtGridCellSlider*)pItem->ValueActiveGet())->GetPos();
		value.text.Format("%d", value.i);
	}  
	if (pCombo = dynamic_cast<CExtGridCellComboBox*>(pCell)) {
		value.i = ((CExtGridCellComboBox*)pItem->ValueActiveGet())->GetCurSel();
		value.text.Format("%d", value.i);
	}  
	else if (pNumber = dynamic_cast<CExtGridCellNumber*>(pCell)) {
		CExtSafeString str;
		pItem->ValueActiveGet()->TextGet(str);
		value.text = str;
		value.text.Replace(",", ".");		// Replace , with . for locale settings using , for decimals
		value.i = atoi(value.text);
		value.f = atof(value.text);
	}
	else if (pCurrency = dynamic_cast<CExtGridCellCurrency*>(pCell)) {
		CExtSafeString str;
		pItem->ValueActiveGet()->TextGet(str);
		value.text = str;
		value.text.Replace(",", ".");
		value.i = atoi(value.text);
		value.f = atof(value.text);
	}
	else if (pCheck = dynamic_cast<CExtGridCellCheckBox*>(pCell)) {
		value.i = ((CExtGridCellCheckBox*)pItem->ValueActiveGet())->GetCheck();
		value.text.Format("%d", value.i);
	}
	else if (pColor = dynamic_cast<CExtGridCellColor*>(pCell)) {
		value.i = ((CExtGridCellColor*)pItem->ValueActiveGet())->GetColor();
		value.text.Format("%d", value.i);
	}
	// Several controls inherit string, so check last.
	else if (pString = dynamic_cast<CExtGridCellString*>(pCell)) {

		CExtSafeString str;
		pItem->ValueActiveGet()->TextGet(str);
		value.text = str;

		CString textForNumbers = value.text;
		textForNumbers.Replace(",", ".");
		value.i = atoi(textForNumbers);
		value.f = atof(textForNumbers);
	}
}

int MotionBlurCombo[] = {
	0,	2,	3,	4,	5,	6,	7,	8,	9,	10,	15,	20
};

//////////////////////////
// APP PROPERTY CHANGES //
// Write application properties back to the CApp when they change.
int CAppPropertyValueMgr::OnChange()
{
	GetPropertyValue();

	if (propName == PROPERTIES_TITLE)
	{
		application->ChangeName(value.text);

		CXHtmlTree* tree = &pMainWnd->project_bar.tree;

		tree->SetItemText(application->tree_items.application, application->file_information.name);
	}
	else if (propName == PROPERTIES_CREATOR)
		application->file_information.creator = value.text;
	else if (propName == "Menu")
		application->window_information.show_menu = value.i ? true : false;
	else if (propName == "Runtime")
	{
		switch (value.i)
		{
		case 0:
			application->runtime = CApplication::rt_application;
			break;

		case 1:
			application->runtime = CApplication::rt_directx;
			break;
		}

		return 1;
	}
	else if (propName == "Window Width")
		application->window_information.window_width = value.i;
	else if (propName == "Window Height") 
		application->window_information.window_height = value.i;
	else if (propName == "Eye Distance (3d)") 
		application->window_information.eye_distance = value.i;
	else if (propName == "Caption")
		application->window_information.caption = value.i!=0;
	else if (propName == "Minimize Box")
		application->window_information.minimize_box = value.i!=0;
	else if (propName == "Maximize Box")
		application->window_information.maximize_box = value.i!=0;
	else if (propName == "Resizing")
		application->window_information.resizable = (CApplication::WindowInformation::ResizeSetting)value.i;
	else if (propName == "Fullscreen")
		application->game_information.fullscreen = value.i!=0;
	else if (propName == "Framerate Mode")
	{
		int lol = value.i;

		switch (value.i)
		{
		case 0:
			application->game_information.fps_mode = CApplication::GameInformation::fps_vsync; 
			break;

		case 1:
			application->game_information.fps_mode = CApplication::GameInformation::fps_unlimited; 
			break;

		case 2:
			application->game_information.fps_mode = CApplication::GameInformation::fps_fixed; 
			break;
		}

		if (application->game_information.fps_mode == 2)
			pMainWnd->m_PropertiesBar.ShowTip("Using a fixed framerate reduces the display quality of Direct-X games.  V-Synced is recommended.");
	}
	else if (propName == "FPS")
		application->game_information.fps = value.i;
	else if (propName == "Sampling") 
	{
		switch (value.i)
		{
		case 0:
			application->game_information.sampler = CApplication::GameInformation::sample_point;
			break;

		case 1:
			application->game_information.sampler = CApplication::GameInformation::sample_linear;
			break;
		}

		if (application->m_display != NULL) {
			if (application->game_information.sampler == CApplication::GameInformation::sample_point) {
				application->m_display->SetSamplerState(D3DSAMP_MAGFILTER, D3DTEXF_POINT);
				application->m_display->SetSamplerState(D3DSAMP_MINFILTER, D3DTEXF_POINT);
			}
			else if (application->game_information.sampler == CApplication::GameInformation::sample_linear) {
				application->m_display->SetSamplerState(D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
				application->m_display->SetSamplerState(D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			}
		}
	}
	else if (propName == "Text rendering") {
		application->game_information.text_rendering_mode = (CApplication::GameInformation::TextRenderingMode)value.i;

		DWORD qualityTable[] = 
		{
			NONANTIALIASED_QUALITY,
			ANTIALIASED_QUALITY,
			CLEARTYPE_QUALITY
		};

		if (application->m_display != NULL) 
		{
			application->m_display->SetTextQuality(qualityTable[value.i]);
			application->m_display->UpdateAllExistingFonts();
		}
	}
	else if (propName == "Multisampling") {
		application->game_information.multisamples = value.i;
	}
	else if (propName == "Override TimeDelta") {

		if (value.i) {
			// Warn because this usually isnt a good idea
			pMainWnd->m_PropertiesBar.ShowTip("Please note: Enabling this option should only be done in specific circumstances and you risk reducing the quality of your game if you do not fully understand its effect.");
		}

		application->game_information.override_timedelta = value.i;
	}
	else if (propName == "Timer FPS") {
		application->game_information.time_delta_override = value.f;
	}
	else if (propName == "Minimum FPS") {
		application->game_information.minimum_fps = value.f;
	}
	else if (propName == "Install Settings") {
		// TODO
	}
	else if (propName == "Edit Attributes") 
	{
		CAddNewTraitDlg Dlg;
		Dlg.application = application;
		
		Dlg.DoModal();

		// refresh grid
		return 1;
	}
	else if (propName == "Edit Globals") {

		CManageVariableDlg Dlg;
		Dlg.pType = NULL;
		Dlg.layout = NULL;
		Dlg.application = application;
		Dlg.globalsMode = true;
		Dlg.DoModal();

		// Refresh grid
		return 1;
	}
	else if (propName == "Manage Event Sheets") {

		/*CNewEventSheetDlg Dlg;
		Dlg.pType = NULL;
		Dlg.layout = NULL;
		Dlg.application = application;
		Dlg.globalsMode = true;
		Dlg.DoModal();

		// Refresh project bar for this application
		pMainWnd->project_bar.Update(application);

		// Refresh grid*/
		return 1;
	}
	else if (propName == "Add transition") 
	{
		CNewTransitionDlg Dlg;
		if (Dlg.DoModal() == IDOK)
		{
			// Add to application
			CTransition& tr = Dlg.m_Transition;

			CString attemptName = tr.m_Name;
			int counter = 2;

			// Check unique name
			while (exists_element(application->m_Transitions.begin(), application->m_Transitions.end(), attemptName))
				attemptName.Format("%s %d", tr.m_Name, counter++);

			tr.m_Name = attemptName;

			tr.Init();

			application->m_Transitions.push_back(Dlg.m_Transition);
		}

		// Refresh grid
		return 1;
	}
	else if (propName == "Manage Controls")
	{
		CControlsDlg Dlg;
		Dlg.application = application;
		Dlg.DoModal();

		// Refresh grid
		return 1;
	}
	else if (propName == "Manage Python")
	{
		CPythonLibs Dlg;
		Dlg.application = application;
		Dlg.DoModal();

		// Refresh grid
		return 1;
	}
/*
		pItem = InsertAppItem("Manage Python", "Click to edit python distribution options.", pRoot);
		pLink = GetHyperLink(pItem);
		pLink->TextSet(PROPERTIES_ADD_EDIT);
		pItem->ValueActiveFromDefault();
*/
	else if (propName == "Run without PS")
		application->game_information.run_without_ps = value.i;
	else if (propName == "Enable Scripting")
		application->game_information.use_python = value.i;
	else if (propName == "Disable Windows Key")
		application->window_information.disable_windows_key = value.i;
	else if (propName == "Edit Families")
	{
		CManageFamiliesDlg Dlg;
		Dlg.application = application;
		Dlg.DoModal();
	}
	else if (propName == "Simulate shader")
		application->game_information.simulate_shaders = (CApplication::GameInformation::SimulateShaders)value.i;
	else if (propName == "Motion Blur") {
		int setting = MotionBlurCombo[value.i];

		// Off
		if (setting == 0) {
			application->game_information.use_motion_blur = false;
			application->game_information.motion_blur_steps = 0;
		}
		// On
		else {
			application->game_information.use_motion_blur = true;
			application->game_information.motion_blur_steps = setting;

		}
	}
	else if (propName == "FPS in caption")
		application->game_information.fps_in_caption = value.i;
	else if (propName == "Manage Families") 
	{
		FamilyManagerDialog dialog(application);
		dialog.DoModal();
		return 1;
	}
	else if (propName == "Load Textures") {
		application->game_information.texture_loading = (CApplication::GameInformation::TextureLoading)value.i;
	}
	else
		CExtMsgBox::DoMsgBox(NULL, CString("Error: broken property '") + propName + "' - please notify Scirra of this bug!", "Error", MB_OK | MB_ICONHAND);
	return 0;
}

////////////////////////////
// LAYOUT PROPERTY CHANGES //
// Write frame property changes back to the CLayout
// oh i also modded it so it updated the project bar
int CLayoutPropertyValueMgr::OnChange()
{
	GetPropertyValue();

	if (propName == LP_NAME)
	{
		if (value.text == "")
			return 2;
		layout->SetName(value.text);
		pMainWnd->project_bar.UpdateApplication(layout->application);
	}

	else if (propName == LP_WIDTH)
	{
		layout->Set(value.i, layout->GetH());
	}

	else if (propName == LP_HEIGHT)
	{
		layout->Set(layout->GetW(),value.i);
	}

	else if (propName == LP_APPLICATIONBACKGROUND)
	{
		layout->application_background = value.i;
		return 4;
	}

	else if (propName == LP_COLOUR)
	{
		layout->SetColor(value.i);
	}

	else if (propName == LP_EVENT_SHEET)
	{
		// Combo goes by event sheet index, get the identifier at that index
		list<EventSheet*>::iterator i = layout->application->event_sheets.begin();
		advance(i, value.i);

		// and remove it
		pMainWnd->project_bar.tree.DeleteItem(layout->event_sheet->tree[layout]);
		layout->event_sheet->tree.erase(layout);

		layout->event_sheet = (*i);

		// add linked event sheet
		layout->event_sheet->tree[layout] = pMainWnd->project_bar.tree.InsertItem(layout->event_sheet->GetName(), image_event_sheet, image_event_sheet, layout->tree);
		pMainWnd->project_bar.tree.SetItemData(layout->event_sheet->tree[layout], (LPARAM)layout->event_sheet);

		layout->application->ChangeModifiedStatus();

		pMainWnd->project_bar.UpdateApplication(layout->application);
	}

	else if (propName == LP_UNBOUNDEDSCROLLING)
	{
		layout->m_unboundedScrolling = value.i;
	}

	else if( propName == APPLICATION)
	{
		pMainWnd->m_PropertiesBar->Update(NULL, TYPE_APP, NULL, NULL, NULL, layout->application);
	}
	else if (propName == "Load Textures")
	{
		layout->texture_loading = (CLayout::LayoutTextureLoading)value.i;
	}
	else
		CExtMsgBox::DoMsgBox(NULL, CString("Error: broken property '") + propName + "' - please notify Scirra of this bug!", "Error", MB_OK | MB_ICONHAND);

	// Return and refresh frame
	return 2;
}

int ApplicationResourcePropertyValueMgr::OnChange()
{
	GetPropertyValue();

	if (propName == "Resource name")
	{
		if (!application->resources.ResourceNameExists(stored, value.text))
			resource->SetName(value.text);
	}
	else if (propName == "File location")
		((BinaryResource*)resource)->path = value.text;
	else if (propName == "Preview")
		((BinaryResource*)resource)->Preview();
	else
		CExtMsgBox::DoMsgBox(NULL, CString("Error: broken property '") + propName + "' - please notify Scirra of this bug!", "Error", MB_OK | MB_ICONHAND);
	
	// Refresh resource bar
	return 3;
}

////////////////////////////
// LAYER PROPERTY CHANGES //
// Write layer property changes back to the CLayer
int CLayerPropertyValueMgr::OnChange()
{
	CAction *action;
	CArchive *ar;
	long ID;

	GetPropertyValue();
	
	// START UNDO INFO //
	action = new CAction();
	action->m_type = ACTION_CHANGELAYER;
	ar = action->Prepare1();
	ID = pLayer->m_layerID;
	*ar << ID;
	pLayer->Serialize(*ar);
	action->Prepare2();
	layout->m_undo->AddAction(action);
	// END UNDO INFO //

	if (propName == LAYER_NAME)
		pLayer->SetName(value.text);
	else if (propName == LAYER_SCROLLX_RATE)
		pLayer->scrollXf = value.f / 100.0;
	else if (propName == LAYER_SCROLLY_RATE)
		pLayer->scrollYf = value.f / 100.0;
	else if (propName == LAYER_ZOOMX_RATE)
		pLayer->zoomXf = value.f / 100.0;
	else if (propName == LAYER_ZOOMY_RATE)
		pLayer->zoomYf = value.f / 100.0;
	else if (propName == LAYER_ZOOMX)
		pLayer->zoomX = value.f / 100.0;
	else if (propName == LAYER_ZOOMY)
		pLayer->zoomY = value.f / 100.0;
	else if (propName == LAYER_SCROLLX)
		pLayer->scrollX = value.f / 100.0;
	else if (propName == LAYER_SCROLLY)
		pLayer->scrollY = value.f / 100.0;
	else if (propName == LAYER_OPACITY)
		pLayer->opacity = value.f / 100.0;
	else if (propName == LAYER_FILTER)
		pLayer->color = value.i;
	else if (propName == LAYER_TRANSPARENT) {
		pLayer->clearBack = !value.i;
		EnableItemByName(pItem->ItemParentGet(), LAYER_BACKGROUND, !pLayer->clearBack, __EGCS_BUTTON_DROPDOWN);
	}
	else if (propName == LAYER_BACKGROUND)
		pLayer->backColor = value.i;
	else if (propName == LAYER_SHOW) {
		pLayer->m_state = ISLAYERVISIBLE(value.i);
		return 6;	// Refresh layer bar
	}
	else if (propName == LAYER_INHERITANCELAYER)
	{
		CLayer* result = layout->application->FindLayerByName(value.text);
		pLayer->m_inherrit_layerID = -1;
		if(result)
			pLayer->m_inherrit_layerID = result->m_layerID;
	}
	else if (propName == LAYER_ANGLE)
		pLayer->m_angle = value.f;
	else if (propName == LAYER_ADDEFFECT) 
	{
		// Check there isn't a PS0.0 effect already at the end
		if (!pLayer->effects.empty() && pLayer->effects.back().version == 0.0f) {
			CString msg;
			msg.Format("There is a non-pixel shader effect as the last effect (%s).  Non-pixel shader effects must be the last effect.  Remove this effect to add other effects.",
				pLayer->effects.back().Text);
			CExtMsgBox::DoMsgBox(NULL, msg, "Add effect", MB_OK | MB_ICONEXCLAMATION);
			return 0;
		}

		// Insert new effect
		CNewHLSLDlg fxDlg;
		if (fxDlg.DoModal() == IDOK) {
			pLayer->effects.push_back(CEffect());
			CEffect& fx = pLayer->effects.back();
			fx.m_effectID = layout->application->m_objID++;
			fx.Text = fxDlg.m_Text;
			fx.effectFile = fxDlg.m_Filename;
			fx.version = fxDlg.m_Version;

			try {
				fx.Init(layout);
			}
			catch (runtime_error e) {
				// Effect creation failed: remove and notify user
				pLayer->effects.pop_back();
				CString msg;
				msg.Format("Failed to add the effect '%s' (%s):\n\n%s", fx.effectFile, fx.Text, e.what());
				CExtMsgBox::DoMsgBox(NULL, msg, "Effects", MB_OK | MB_ICONHAND);
				layout->Invalidate();
				return 1;
			}

			// Bordermode must affect first effect
			if (fx.runEffect.borderMode)
				pLayer->effects.front().runEffect.borderMode = true;
			if (fx.runEffect.vSourceTexture)
				pLayer->effects.front().runEffect.usesSourceTexture = true;

			// TODO: make more intelligent surface allocation.
			// For now allocate all temp surfaces.
			/*
			if (frame->iOriginalTexture == -1)
				frame->iOriginalTexture = frame->m_display->AddTextureRT(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
			
			if (frame->iEffectTargets[0] == -1)
				frame->iEffectTargets[0] = frame->m_display->AddTextureRT(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
			
			if (frame->iEffectTargets[1] == -1)
				frame->iEffectTargets[1] = frame->m_display->AddTextureRT(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
			
			if (frame->iDisplayTexture == -1)
				frame->iDisplayTexture = frame->m_display->AddTextureRT(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
			*/

			// Redraw for effect
			layout->Invalidate();			
		}

		// Refresh property bar
		return 1;
	}

	else if (propName == LAYER_TYPE) {
		; // Do nothing, sometimes triggers even for read only props!
	}
	else if (propName == LAYER_FORCEOWNTEXTURE) {
		pLayer->forceOwnTexture = value.i;
		layout->Invalidate();
	}
	else if (propName == "Sampling") {
		pLayer->sampler = (CLayer::LayerSampler)value.i;
		layout->Invalidate();
	}
	else if (propName == "3D layering") {
		pLayer->enable3d = (value.i != 0);
	}
	else
		CExtMsgBox::DoMsgBox(NULL, CString("Error: broken property '") + propName + "' - please notify Scirra of this bug!", "Error", MB_OK | MB_ICONHAND);
	
	// Refresh frame
	return 2;
}

CString disallowedObjectNames[] = {
	"sin", "cos", "tan", "sqrt", "str", "int", "float", "and", "or", "asin", "acos", "atan", "abs", "exp",
	"log", "ln", "log10", "floor", "ceil", "round", "random", "len", "at", "self", "system", "_varindex",
};

//////////////////////////////////////
// OBJECT STANDARD PROPERTY CHANGES //
// Write standard property changes back to the CObj
int CPluginStdPropertyValueMgr::OnChange()
{
	GetPropertyValue();

	int ret = 2;

	CAction *action;
	CArchive *ar;
	long ID;

	if (propName == "Name") 
	{
		// START UNDO INFO //
		action = new CAction();
		action->m_type = ACTION_CHANGETYPE;
		ar = action->Prepare1();
		ID = pType->ObjectIdentifier;
		*ar << ID;
		pType->Serialize(*ar);
		action->Prepare2();
		frame->m_undo->AddAction(action);
		// END UNDO INFO //

		POSITION pos = frame->application->object_types.GetStartPosition();

		bool bFound = false;

		value.text.Trim();
		CString filteredName, lwrName;

		for (int i = 0; i < value.text.GetLength(); i++) {

			// Allowed characters: a-z, A-Z, 0-9, _
			char c = value.text[i];

			if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_')
				filteredName += value.text[i];
		}

		value.text = filteredName;
		if (value.text == "") {
			CExtMsgBox::DoMsgBox(NULL, "The object name you entered is invalid.  Object names can consist only of letters, numbers and underscore.", "Object name", MB_OK | MB_ICONEXCLAMATION);
			return 1;
		}
		lwrName = filteredName;
		lwrName.MakeLower();

		// Check it hasn't been given a system expression name
		CExpression nameChecker;
		if (nameChecker.SystemExpressionExists(lwrName)) 
		{
			CString msg;
			msg.Format("You cannot name this object '%s' because it is the name of a system expression.  Please choose another name for your object.", lwrName);
			CExtMsgBox::DoMsgBox(NULL, msg, "Renaming object", MB_OK | MB_ICONEXCLAMATION);
			return 1;
		}

		// Check for disallowed names
		int numDisallowedNames = sizeof(disallowedObjectNames) / sizeof(disallowedObjectNames[0]);
		CString* disallowed_end = disallowedObjectNames + numDisallowedNames;

		if (find(disallowedObjectNames, disallowed_end, lwrName) != disallowed_end) {
			CString msg;
			msg.Format("The object name '%s' is not allowed because it is a system expression or built-in name.  Please choose another name for your object.", lwrName);
			CExtMsgBox::DoMsgBox(NULL, msg, "Renaming object", MB_OK | MB_ICONEXCLAMATION);
			return 1;
		}

		// Check the user isn't using the name of a family in the Families directory
		char curDir[MAX_PATH+9];
		GetModuleFileName(NULL, curDir, MAX_PATH+9);
		PathRemoveFileSpec(curDir);
		strcat(curDir, "\\Families\\*");

		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = INVALID_HANDLE_VALUE;
		hFind = FindFirstFile(curDir, &FindFileData);

		if (hFind != INVALID_HANDLE_VALUE) 
		{
			while (FindNextFile(hFind, &FindFileData) != 0) 
			{
				CString file = FindFileData.cFileName;
				file.MakeLower();
				CString family = file.Left(file.GetLength() - 4);
				CString ext = file.Right(4);

				if (ext == ".bmp" && lwrName == family) {
					CString msg;
					msg.Format("'%s' is the name of a Family and cannot be used as an object name.  Please choose another name for your object.", value.text);
					CExtMsgBox::DoMsgBox(NULL, msg, "Renaming object", MB_OK | MB_ICONEXCLAMATION);
					return 1;
				}
			}//while
		}

		vector<CString>::iterator a = frame->application->traits.begin();
		vector<CString>::iterator traits_end = frame->application->traits.end();

		for ( ; a != traits_end; a++) {
			CString curName = *a;
			curName.MakeLower();

			if (curName == lwrName) {
				CString msg;
				msg.Format("'%s' is the name of an attribute.  You cannot name your object the same as an attribute.  Please choose another name for your object.",
					*a);
				CExtMsgBox::DoMsgBox(NULL, msg, "Renaming object", MB_OK | MB_ICONEXCLAMATION);
				return 1;
			}
		}


		CObjType* oT;
		long nKey = 0;
		int objectnumber = 0;
		while (pos != NULL) 
		{	
			frame->application->object_types.GetNextAssoc(pos, nKey, oT);
			if(oT && !oT->m_bIsGroupType && oT != pType) {
				CString r = oT->GetName();
				r.MakeLower();
				if (lwrName == r)
					bFound = true;
			}
		}

		if (!bFound)
			pType->SetName(value.text);
		else {
			CExtMsgBox::DoMsgBox(NULL, "An object with this name already exists.  Please choose another name.", "Renaming object", MB_OK | MB_ICONEXCLAMATION);
			return 1;
		}

		CChildFrame* pChildFrame = (CChildFrame*)frame->GetParentFrame();
		if (pChildFrame)
			pChildFrame->object_bar.Refresh();
	}

	else if (propName == "Icon") {
			// START UNDO INFO //
			action = new CAction();
			action->m_type = ACTION_CHANGETYPE;
			ar = action->Prepare1();
			ID = pType->ObjectIdentifier;
			*ar << ID;
			pType->Serialize(*ar);
			action->Prepare2();
			frame->m_undo->AddAction(action);
			// END UNDO INFO //
	
			try
			{
				CImageEditorDlg imageEditor;
				imageEditor.m_Icon = true;
				imageEditor.application = this->frame->application;
				imageEditor.EditImage(&(pType->m_Image));
			}
			catch (CD3DError e) 
			{
				CErrorDlg err;
				err.Error("Image Editor Error", e.GetString());
			}
			catch (...)
			{
				CErrorDlg err;
				err.Error("Image Editor Error", "Error opening the image editor.  Please report this bug to Scirra.");
			}

			pMainWnd->m_PropertiesBar->Refresh();

	}
	else if (propName == "Small Icon") {
			// START UNDO INFO //
			action = new CAction();
			action->m_type = ACTION_CHANGETYPE;
			ar = action->Prepare1();
			ID = pType->ObjectIdentifier;
			*ar << ID;
			pType->Serialize(*ar);
			action->Prepare2();
			frame->m_undo->AddAction(action);
			// END UNDO INFO //

			try
			{
				CImageEditorDlg imageEditor;
				imageEditor.m_Icon = true;
				imageEditor.application = this->frame->application;
				imageEditor.EditImage(&(pType->small_image));
			}
			catch (CD3DError e) 
			{
				CErrorDlg err;
				err.Error("Image Editor Error", e.GetString());
			}
			catch (...)
			{
				CErrorDlg err;
				err.Error("Image Editor Error", "Error opening the image editor.  Please report this bug to Scirra.");
			}

		pMainWnd->m_PropertiesBar->Refresh();
	}
	else if (propName == "New Behavior") {
		CNewBehaviorDlg movDlg;
		if (movDlg.DoModal() == IDOK) {
			// START UNDO INFO //
			action = new CAction();
			action->m_type = ACTION_CHANGETYPE;
			ar = action->Prepare1();
			ID = pType->ObjectIdentifier;
			*ar << ID;
			pType->Serialize(*ar);
			action->Prepare2();
			frame->m_undo->AddAction(action);
			// END UNDO INFO //

			CString behaviorName = pType->GetUniqueBehaviorName(movDlg.Text);

			pType->behaviors.push_back(CBehavior());
			CBehavior& Mv = pType->behaviors.back();
			Mv.BehaviorDLLIndex = movDlg.ID;

			// TODO: need a CLayout* for CreateDefault
			Mv.CreateDefault(frame->layout);

			Mv.Text = behaviorName;
			Mv.id = frame->application->m_objID++;

			ret = 1; // refresh property bar
		}
	}
	else if (propName == "New Effect") {

		// Check there isn't a PS0.0 effect already at the end
		if (!pType->effects.empty() && pType->effects.back().version == 0.0f) {
			CString msg;
			msg.Format("There is a non-pixel shader effect as the last effect (%s).  Non-pixel shader effects must be the last effect.  Remove this effect to add other effects.",
				pType->effects.back().Text);
			CExtMsgBox::DoMsgBox(NULL, msg, "Add effect", MB_OK | MB_ICONEXCLAMATION);
			return 0;
		}

		// Insert new effect
		CNewHLSLDlg fxDlg;
		if (fxDlg.DoModal() == IDOK) {
			// START UNDO INFO //
			action = new CAction();
			action->m_type = ACTION_CHANGETYPE;
			ar = action->Prepare1();
			ID = pType->ObjectIdentifier;
			*ar << ID;
			pType->Serialize(*ar);
			action->Prepare2();
			frame->m_undo->AddAction(action);
			// END UNDO INFO //

			fxDlg.m_Text = pType->GetUniqueEffectName(fxDlg.m_Text);
			pType->effects.push_back(CEffect());
			CEffect& fx = pType->effects.back();
			fx.m_effectID = frame->application->m_objID++;
			fx.Text = fxDlg.m_Text;
			fx.effectFile = fxDlg.m_Filename;
			fx.version = fxDlg.m_Version;

			try {
				fx.Init(frame);
			}
			catch (runtime_error e) {
				// Effect creation failed: remove and notify user
				pType->effects.pop_back();
				CString msg;
				msg.Format("Failed to add the effect '%s' (%s):\n\n%s", fx.effectFile, fx.Text, e.what());
				CExtMsgBox::DoMsgBox(NULL, msg, "Effects", MB_OK | MB_ICONHAND);
				frame->Invalidate();
				ret = 1;
			}

			// Bordermode must affect first effect
			if (fx.runEffect.borderMode)
				pType->effects.front().runEffect.borderMode = true;
			if (fx.runEffect.vSourceTexture)
				pType->effects.front().runEffect.usesSourceTexture = true;

			// TODO: make more intelligent surface allocation.
			// For now allocate all temp surfaces.
			/*
			if (frame->iOriginalTexture == -1)
				frame->iOriginalTexture = frame->m_display->AddTextureRT(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
			
			if (frame->iEffectTargets[0] == -1)
				frame->iEffectTargets[0] = frame->m_display->AddTextureRT(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
			
			if (frame->iEffectTargets[1] == -1)
				frame->iEffectTargets[1] = frame->m_display->AddTextureRT(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
			
			if (frame->iDisplayTexture == -1)
				frame->iDisplayTexture = frame->m_display->AddTextureRT(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
			*/
			// Redraw for effect
			frame->Invalidate();
		}
		// Refresh property bar
		ret = 1;

	}
	else if (propName == "Edit Variables") {

		// Undo information doesn't work here - if you undo the deletion or addition of a private variable,
		// it changes m_PrivateVariables in CObjType.  However, it does not affect the m_values vector in the
		// respective CObj instances, which contains the initial values.  Since the initial values are missing
		// you get an out-of-bounds array access when the property grid tries to look up the initial value.
		// - AG

		/*
		// START UNDO INFO //
		action = new CAction();
		action->m_type = ACTION_CHANGETYPE;
		ar = action->Prepare1();
		ID = pType->ObjectIdentifier;
		*ar << ID;
		pType->Serialize(*ar);
		action->Prepare2();
		frame->m_undo->AddAction(action);
		// END UNDO INFO //
		*/

		CManageVariableDlg Dlg;
		Dlg.pType = pType;
		Dlg.application = frame->application;
		Dlg.layout = frame->layout;
		Dlg.DoModal();

		ret = 1;
	}
	else if (propName == "New Family") 
	{
		CNewFamilyDlg Dlg;
		Dlg.application = frame->application;

		if (Dlg.DoModal() == IDOK)
		{
			CApplication* application = frame->application;

			// Add the family to all selected types
			vector<CObjType*>::iterator t = g_MainFrame->m_PropertiesBar.vSelectedTypes.begin();
			vector<CObjType*>::iterator end = g_MainFrame->m_PropertiesBar.vSelectedTypes.end();

			if (t == end) return 1;

			// Check no dupe
			list<Family>::iterator f = find(application->families.begin(), application->families.end(), Dlg.m_Name);
			//if(!f->is_in_use) f = application->families.end();
			// If the family is no longer in use, but we've just added something which uses that family, then put it back into use
			f->is_in_use = true;

			// check the name isn't used by any object types
			// fix, 0.99
			if (application->ObjectTypeNameExists(Dlg.m_Name))
			{
				CString msg = "This family cannot be added, as an object with this family name already exists.";
				CExtMsgBox::DoMsgBox(g_MainFrame->m_hWnd, msg, "Cannot add family", MB_OK | MB_ICONEXCLAMATION);

				return 1;
			}

			Family* pFamilyToAdd;

			// Not found: insert
			if (f == application->families.end())
			{
				application->families.push_back(Family());
				Family& c = application->families.back();
				c.application = application;
				c.name = Dlg.m_Name;
				c.image = Dlg.m_Image;
				c.small_image = Dlg.small_image;
				c.DLLIndex  = (*t)->DLLIndex;		// The Add Family button is only shown when one plugin type is selected
				c.identifier = application->m_objID++;
				pFamilyToAdd = &c;
			}
			else
				pFamilyToAdd = &(*f);

			for ( ; t != end; t++)
				application->AddObjectTypeToFamily(*t, pFamilyToAdd);

			pMainWnd->project_bar.UpdateApplication(application);
		}

		return 1;
	}
	else if (propName == "Container") 
	{
		ObjectSelectorDialog ObjectSelector(frame->application, frame->layout, "", -1, false, false, pType->ObjectIdentifier);

		if (ObjectSelector.DoModal() == IDOK)
		{
			CApplication* application = frame->application;

			// No container present? Create one.
			if (pType->m_pContainer == NULL)
			{
				application->m_Containers.push_back(CContainer());
				CContainer& c = application->m_Containers.back();
				c.application = application;

				// Add my OID to the IDs list
				c.m_IDs.push_back(pType->ObjectIdentifier);

				// Add the chosen OID to the IDs list
				c.m_IDs.push_back(ObjectSelector.selected.type->ObjectIdentifier);

				// Update my pointer
				pType->m_pContainer = &c;

				// Update the added object to point to this container
				long addID = ObjectSelector.selected.type->ObjectIdentifier;
				CObjType* pAddedType = application->object_types[addID];
				pAddedType->m_pContainer = &c;
			}
			else 
			{
				// Add this object to existing container
				pType->m_pContainer->m_IDs.push_back(ObjectSelector.selected.type->ObjectIdentifier);

				// Update the added object to point to this container
				long addID = ObjectSelector.selected.type->ObjectIdentifier;
				CObjType* pAddedType = application->object_types[addID];

				if (!pAddedType->m_pContainer)
					pAddedType->m_pContainer = pType->m_pContainer;
			}

		}

		return 1;
	}
	else if (propName == "Manage") {
		// START UNDO INFO //
		action = new CAction();
		action->m_type = ACTION_CHANGETYPE;
		ar = action->Prepare1();
		ID = pType->ObjectIdentifier;
		*ar << ID;
		pType->Serialize(*ar);
		action->Prepare2();
		frame->m_undo->AddAction(action);
		// END UNDO INFO //

		CManageVariableDlg Dlg;
		Dlg.pType = pType;
		Dlg.application = frame->application;
		Dlg.layout = frame->layout;
		Dlg.DoModal();

		return 1;
	}
	else if (propName == "X") {
		// START UNDO INFO //
		action = new CAction();
		action->m_type = ACTION_MOVESIZE;
		ar = action->Prepare1();
		ID = o->GetInstanceID();
		*ar << ID;
		o->Serialize(*ar);
		action->Prepare2();
		frame->m_undo->AddAction(action);
		// END UNDO INFO //
		o->SetPos(value.i, o->GetY());
	}
	else if (propName == "Y") {
		// START UNDO INFO //
		action = new CAction();
		action->m_type = ACTION_MOVESIZE;
		ar = action->Prepare1();
		ID = o->GetInstanceID();
		*ar << ID;
		o->Serialize(*ar);
		action->Prepare2();
		frame->m_undo->AddAction(action);
		// END UNDO INFO //
		o->SetPos(o->GetX(), value.i);
	}
	else if (propName == "Angle") {
		// START UNDO INFO //
		action = new CAction();
		action->m_type = ACTION_MOVESIZE;
		ar = action->Prepare1();
		ID = o->GetInstanceID();
		*ar << ID;
		o->Serialize(*ar);
		action->Prepare2();
		frame->m_undo->AddAction(action);
		// END UNDO INFO //
		o->editObject->objectAngle = value.f;
	}
	else if (propName == "Opacity"){
			// START UNDO INFO //
		action = new CAction();
		action->m_type = ACTION_MOVESIZE;
		ar = action->Prepare1();
		ID = o->GetInstanceID();
		*ar << ID;
		o->Serialize(*ar);
		action->Prepare2();
		frame->m_undo->AddAction(action);
		// END UNDO INFO //

		o->editObject->filter &= 0x00FFFFFF;
		int opacity = value.f * 2.55;
		Clamp(opacity, 0, 255);
		o->editObject->filter |= opacity << 24;	
	}
	else if (propName == "Filter"){
		// START UNDO INFO //
		action = new CAction();
		action->m_type = ACTION_MOVESIZE;
		ar = action->Prepare1();
		ID = o->GetInstanceID();
		*ar << ID;
		o->Serialize(*ar);
		action->Prepare2();
		frame->m_undo->AddAction(action);
		// END UNDO INFO //

		o->editObject->filter &= 0xFF000000;
		o->editObject->filter |= D3D_ARGB(0, GetRValue(value.i), GetGValue(value.i), GetBValue(value.i));
	}
	else if (propName == "Width") {
		// START UNDO INFO //
		action = new CAction();
		action->m_type = ACTION_MOVESIZE;
		ar = action->Prepare1();
		ID = o->GetInstanceID();
		*ar << ID;
		o->Serialize(*ar);
		action->Prepare2();
		frame->m_undo->AddAction(action);
		// END UNDO INFO //
		o->SetSize(value.i, o->GetH());
	}
	else if (propName == "Height") {
		// START UNDO INFO //
		action = new CAction();
		action->m_type = ACTION_MOVESIZE;
		ar = action->Prepare1();
		ID = o->GetInstanceID();
		*ar << ID;
		o->Serialize(*ar);
		action->Prepare2();
		frame->m_undo->AddAction(action);
		// END UNDO INFO //
		o->SetSize(o->GetW(), value.i);
	}

	else if (propName == "Global") {
		// START UNDO INFO //
		action = new CAction();
		action->m_type = ACTION_CHANGETYPE;
		ar = action->Prepare1();
		ID = pType->ObjectIdentifier;
		*ar << ID;
		pType->Serialize(*ar);
		action->Prepare2();
		frame->m_undo->AddAction(action);
		// END UNDO INFO //

		pType->m_bGlobal = value.i != 0;
	}

	else if (propName == "Show in editors") {
		// START UNDO INFO //
		action = new CAction();
		action->m_type = ACTION_CHANGETYPE;
		ar = action->Prepare1();
		ID = pType->ObjectIdentifier;
		*ar << ID;
		pType->Serialize(*ar);
		action->Prepare2();
		frame->m_undo->AddAction(action);
		// END UNDO INFO //

		pType->m_bShowInEventSheetEditor = value.i != 0;
	}

	else if (propName == "Object folder") 
	{
		pType->SetFolder(frame->application->object_folders.at(value.i).name);
	}

	else if (propName == "Destroy when:") {
		// START UNDO INFO //
		action = new CAction();
		action->m_type = ACTION_CHANGETYPE;
		ar = action->Prepare1();
		ID = pType->ObjectIdentifier;
		*ar << ID;
		pType->Serialize(*ar);
		action->Prepare2();
		frame->m_undo->AddAction(action);
		// END UNDO INFO //

		pType->destroyWhen = (DisableShaderWhen)value.i;
	}
	else
		CExtMsgBox::DoMsgBox(NULL, CString("Error: broken property '") + propName + "' - please notify Scirra of this bug!", "Error", MB_OK | MB_ICONHAND);
	// Refresh screen
	return ret;
}

int FamilyPropertyMgr::OnChange()
{
	GetPropertyValue();

	CAction* action;
	CArchive* ar;
	long ID;

		// START UNDO INFO //
		action = new CAction();
		action->m_type = ACTION_CHANGETYPE;
		ar = action->Prepare1();
		ID = pType->ObjectIdentifier;
		*ar << ID;
		pType->Serialize(*ar);
		action->Prepare2();
		frame->m_undo->AddAction(action);
		// END UNDO INFO //

	// Find and remove this family
	vector<Family*>::iterator i = pType->families.begin();
	vector<Family*>::iterator end = pType->families.end();

	for ( ; i != end; i++)
		if ((**i) == name)
			break;

	// Not found - already deleted somewhere else - refresh properties
	if (i == pType->families.end())
		return 1;

	CApplication* application = (*i)->application;

	// Check if last in family
	CApplication::ObjTypeFamilyStatus status = application->PromptIfTypeLastInFamily(pType, *i);

	switch (status) {

		// Delete family
	case CApplication::user_clicked_yes_delete_family:

		pType->families.erase(i);
		application->DeleteFamily(*i);

		break;

	case CApplication::user_clicked_no:

		break;	// user cancelled operation

	case CApplication::not_last_type_in_family:

		// Remove this type from the family
		pType->families.erase(i);

		break;
	}

	// Update grid
	return 1;
}

int CGlobalPropertyMgr::OnChange()
{
	GetPropertyValue();

	list<CApplication::GlobalVariable>::iterator i = find(application->global_variables.begin(), application->global_variables.end(), name);

	if (i == application->global_variables.end()) 
	{
		CExtMsgBox::DoMsgBox(NULL, "Error editing global value!  Please report this bug to Scirra.", "Error", MB_OK | MB_ICONHAND);
		return 0;
	}

	i->value = value.text;

	return 0;
}

int CAnimationPropertyValueMgr::OnChange()
{
	GetPropertyValue();

	bool bChange = false;

	// Normal animation
	if (frameIndex == -1)
	{
		if (propName == AB_ANIMATIONNAME) 
		{
			if (pAnim->m_Name != value.text)
				bChange = true;

			pAnim->m_Name = value.text;
		}
		if (propName == AB_ANIMATIONTAG) 
		{
			pAnim->tag = value.i;
		}
		if (propName == AB_ANIMATIONSPEED) {
			pAnim->m_Speed = value.f;
		}
		if (propName == AB_ANIMATIONLOOP) {
			pAnim->m_bForever = value.i;
		}
		if (propName == AB_ANIMATIONREPEAT) {
			pAnim->m_Repeat = value.i;
		}
		if (propName == AB_ANIMATIONREPEATTO) {
			pAnim->m_RepeatTo = value.i;
		}
		if (propName == AB_ANIMATIONPINGPONG) {
			pAnim->m_bPingPong = value.i;
		}
	}

	else
	{
		if (propName == "Frame speed") {
			if(value.f > 0)
				pAnim->m_FrameTimes[frameIndex] = value.f;
		}
	}

	if (bChange)
		return 5;
	else
		return 0;
}

int CPluginPrivateVarMgr::OnChange()
{
	GetPropertyValue();

	CAction* action;
	CArchive* ar;
	long ID;

		// START UNDO INFO //
		action = new CAction();
		action->m_type = ACTION_MOVESIZE;
		ar = action->Prepare1();
		ID = o->GetInstanceID();
		*ar << ID;
		o->Serialize(*ar);
		action->Prepare2();
		frame->m_undo->AddAction(action);
		// END UNDO INFO //

	o->m_values[valueIndex].value = value.text;

	// Don't redraw/refresh
	return 0;
}

CPluginPropertyValueMgr::CPluginPropertyValueMgr(CObj* _obj, editInfo* _data, CObjType* _type, OINFO* _oInfo, CLayoutEditor* _frame, const CString& _propName)
: obj(_obj),
  type(_type),
  oInfo(_oInfo),
  propName(_propName),
  frame(_frame),
  data(_data)
{
	
	if(_obj)
		data = _obj->editObject;

	// Find the property entry by looping and matching the label to propName
	vector<CPropItem>::iterator j = oInfo->Properties.begin();
	const vector<CPropItem>::const_iterator props_end = oInfo->Properties.end();

	for ( ; j != props_end; j++) {
		if (j->Label == propName)
			break;
	}

	// Not found - bail out
	if (j == props_end)
		throw;
	else
		// Save index to properties array
		propIndex = j - oInfo->Properties.begin();
}

int CPluginPropertyValueMgr::OnChange()
{
	CAction *action;
	CArchive *ar;
	long ID;

	if (type) {
		// START UNDO INFO //
		action = new CAction();
		action->m_type = ACTION_CHANGETYPE;
		ar = action->Prepare1();
		ID = type->ObjectIdentifier;
		*ar << ID;
		type->Serialize(*ar);
		action->Prepare2();
		frame->m_undo->AddAction(action);

		if (obj) {
			action = new CAction();
			action->m_type = ACTION_CHANGEOBJECTPROP; // Changed for 0.96.2
			action->m_bGroup = FALSE;
			ar = action->Prepare1();
			ID = obj->GetInstanceID();
			*ar << ID;
			obj->Serialize(*ar);
			action->Prepare2();
			frame->m_undo->AddAction(action);
		}
		// END UNDO INFO //
	}

	// A warning for future versions, after ETOnPropertyChanged it is possible that all
	// member variables of this class could be invalid. At this time we dont have any functions below
	// the ETOnPropertyChanged but its worth remembering. To fix, see solution in CBehaviorPropertyValueMgr
	GetPropertyValue();

	PropReturn propReturn;
	CString tempStr;			// String has to be on Construct-owned heap
	// Use propIndex (found in the constructor) to locate my CPropItem
	CPropItem& p = oInfo->Properties[propIndex];

	switch (p.Type)
	{
		// Edit
	case PROPTYPE_EDIT:
		// Static (won't change but may as well use same code as edit)
	case PROPTYPE_STATIC:
		tempStr = value.text;
		propReturn.chstr = tempStr;
		oInfo->ETOnPropertyChanged(data, propName, 1, NULL, &propReturn, NULL);
		break;

		// Button - notify of click
	case PROPTYPE_BUTTON:
		oInfo->ETOnPropertyChanged(data, propName, 1, NULL, &propReturn, NULL);
		break;

		// Value - send integer value
	case PROPTYPE_VALUE:
		propReturn.val = value.i;
		oInfo->ETOnPropertyChanged(data, propName, 1, NULL, &propReturn, NULL);
		break;

		// Combo - send integer value
	case PROPTYPE_COMBO:
		propReturn.val = value.i;
		oInfo->ETOnPropertyChanged(data, propName, 1, NULL, &propReturn, NULL);
		break;

		// Float - send double value
	case PROPTYPE_FLOAT:
		propReturn.fVal = value.f;
		oInfo->ETOnPropertyChanged(data, propName, 1, NULL, &propReturn, NULL);
		break;

		// Color
	case PROPTYPE_COLOR:
		propReturn.val = value.i;
		oInfo->ETOnPropertyChanged(data, propName, 1, NULL, &propReturn, NULL);
		break;

		// Checkbox
	case PROPTYPE_CHECKBOX:
		propReturn.val = value.i;
		oInfo->ETOnPropertyChanged(data, propName, 1, NULL, &propReturn, NULL);
		break;

		// font
	case PROPTYPE_FONT:
		propReturn.val = value.i;
		oInfo->ETOnPropertyChanged(data, propName, 1, NULL, &propReturn, NULL);
		break;

		// Percentage
	case PROPTYPE_PERCENT:
		propReturn.fVal = value.f / 100.0;
		oInfo->ETOnPropertyChanged(data, propName, 1, NULL, &propReturn, NULL);
		break;

		// Slider
	case PROPTYPE_SLIDER:
		propReturn.val = value.i;
		oInfo->ETOnPropertyChanged(data, propName, 1, NULL, &propReturn, NULL);
		break;

		// Frame
	case PROPTYPE_FRAME:
		propReturn.val = value.i;
		oInfo->ETOnPropertyChanged(data, propName, 1, NULL, &propReturn, NULL);
		break;
	}


	// Refresh screen
	return 2;
}

CEffectPropertyValueMgr::CEffectPropertyValueMgr(CEffect* _pEffect, list<CEffect>& _effects, CObjType* _pType,
		CLayer* _pLayer, CLayoutEditor *_frame, const CString& _propName)
: pEffect(_pEffect),
  effects(_effects),
  pType(_pType),
  pLayer(_pLayer),
  frame(_frame),
  propName(_propName)
{
	propIndex = -1;

	vector<EffectParameter>::iterator i = pEffect->params.begin();
	const vector<EffectParameter>::const_iterator end = pEffect->params.end();

	for ( ; i != end; i++) {
		if (i->name == propName) {
			propIndex = i - pEffect->params.begin();
			break;
		}
	}
}

int CEffectPropertyValueMgr::OnChange()
{
	CAction* action;
	CArchive* ar;
	long ID;

	if (pType) {
		// START UNDO INFO //
		action = new CAction();
		action->m_type = ACTION_CHANGETYPE;
		ar = action->Prepare1();
		ID = pType->ObjectIdentifier;
		*ar << ID;
		pType->Serialize(*ar);
		action->Prepare2();
		frame->m_undo->AddAction(action);
		// END UNDO INFO //
	}
	else {
		// START UNDO INFO //
		action = new CAction();
		action->m_type = ACTION_CHANGELAYER;
		ar = action->Prepare1();
		ID = pLayer->m_layerID;
		*ar << ID;
		pLayer->Serialize(*ar);
		action->Prepare2();
		frame->m_undo->AddAction(action);
		// END UNDO INFO //
	}

	if (propName == "Name")
	{
		// Renaming a Behavior
		GetPropertyValue();

		// Check name does not exist
		if (!exists_element(effects.begin(), effects.end(), value.text))
			pEffect->Text = value.text;

		return 0;
	}
	// If deleting an effect - remove me from owner
	if (propName == "Remove") {
		
		// Find the effect that matches the address of pEffect, and erase it
		EffectIterator i = effects.begin();
		EffectConstIterator fx_end = effects.end();

		for ( ; i != fx_end; i++) {
			// Match addresses of CEffects
			if (&(*i) == pEffect) {
				effects.erase(i);
				return 4;		// Refresh property bar and layout
			}
		}

		// Ought to be unreachable
		CExtMsgBox::DoMsgBox(NULL, "Remove effect broken - please contact Scirra about this bug!", "Error", MB_OK | MB_ICONHAND);
		return 0;
	}

	else if (propName == "Disable when:") {

		GetPropertyValue();

		// Update the disable mode
		pEffect->disableMode = (DisableShaderWhen)value.i;

		return 0;

	}

	// All other properties: simply read/write the text of the property
	else {
		GetPropertyValue();

		EffectParameter& ep = pEffect->params[propIndex];

		// I fully acknowledge there has possibly been some bad variable naming here
		if(!pEffect->initialised)
			pEffect->Init(frame);
		ID3DXEffect* pDXEffect = pEffect->runEffect.pEffect;
		CRunEffectParameter& curRunParam = pEffect->runEffect.params[propIndex];

		switch (ep.type) {
			// Float
			case 5:
				pEffect->params[propIndex].value = value.text;
				break;
			// Percent
			case 9:
				pEffect->params[propIndex].value = FloatToString(atof(value.text) / 100.0);
				break;
		}

		pDXEffect->SetFloat(curRunParam.handle, atof(pEffect->params[propIndex].value));
	}

	return 0;
}

BehaviorPropertyValueMgr::BehaviorPropertyValueMgr(CBehavior* _pMov, CObjType* _pOwner, CLayout* _level,
													 CLayoutEditor* _frame, const CString& _propName)
: pMov(_pMov),
  pOwner(_pOwner),
  propName(_propName),
  level(_level),
  frame(_frame)
{
	// Find the property entry by looping and matching the label to propName
	vector<CPropItem>::iterator j = pMov->oInfo->Properties.begin();
	const vector<CPropItem>::const_iterator props_end = pMov->oInfo->Properties.end();

	for ( ; j != props_end; j++) {
		if (j->Label == propName)
			break;
	}

	// Save index to properties array
	propIndex = j - pMov->oInfo->Properties.begin();
}

int BehaviorPropertyValueMgr::OnChange()
{
	// It is possible that on a change, we lose the properties, thus the memory held by the class could change!
	// So we duplicate all the variables at a new scope level
	CBehavior* temp_mov = pMov;
	CObjType* temp_owner = pOwner;
	CLayout* temp_level = level;

	CBehavior* pMov = temp_mov;
	CObjType* pOwner = temp_owner;
	CLayout* level = temp_level;

	CAction *action;
	CArchive *ar;
	long ID;

	// START UNDO INFO //
	action = new CAction();
	action->m_type = ACTION_CHANGETYPE;
	ar = action->Prepare1();
	ID = pOwner->ObjectIdentifier;
	*ar << ID;
	pOwner->Serialize(*ar);
	action->Prepare2();
	frame->m_undo->AddAction(action);
	// END UNDO INFO //

	// If deleting a Behavior - remove me from owner
	if (propName == "Remove") {
		
		// Find the CBehavior that matches the address of the pMov, and erase it
		BehaviorIterator i = pOwner->behaviors.begin();
		BehaviorConstIterator movs_end = pOwner->behaviors.end();

		for ( ; i != movs_end; i++) {
			// Match addresses of CBehaviors
			if (&(*i) == pMov) {
				pOwner->behaviors.erase(i);
				return 1;
			}
		}

		// Ought to be unreachable
		CExtMsgBox::DoMsgBox(NULL, "Remove Behavior broken - please contact Scirra about this bug!", "Error", MB_OK | MB_ICONHAND);
		return 0;
	}

	// Renaming a Behavior
	else if (propName == "Name") {
		GetPropertyValue();

		// Check name does not exist
		if (!exists_element(pOwner->behaviors.begin(), pOwner->behaviors.end(), value.text))
			pMov->Text = value.text;

		return 0;
	}

	GetPropertyValue();

	OINFO* oInfo = pMov->oInfo;

	PropReturn propReturn;
	CString tempStr;			// String has to be on Construct-owned heap
	// Use propIndex (found in the constructor) to locate my CPropItem
	CPropItem& p = oInfo->Properties[propIndex];

	// Create a temporary Behavior to save to
	editInfo* tempMovExt = new editInfo;
	tempMovExt->DLLIndex = pMov->BehaviorDLLIndex;
	tempMovExt->peditInfo = level;
	tempMovExt->instanceID = -1;
	
	oInfo->ETCreateObject(tempMovExt, level);

	level->m_pTempMovExt = tempMovExt;
	level->m_oTemporyBehaviorInfo = oInfo;
	level->m_pControlBehaviorData = &pMov->eData;
	level->m_pControlBehaviorDataSize = &pMov->eSize;
	

	// Load from existing data
	bin movDataLoad(g_allocator);
	movDataLoad.attach(pMov->eData, pMov->eSize);
	movDataLoad.loading = true;
	oInfo->ETSerialize(tempMovExt, movDataLoad);
	movDataLoad.detach();

	// Now tell the temp which property changed
	switch (p.Type)
	{
		// Edit
	case PROPTYPE_EDIT:
		// Static (won't change but may as well use same code as edit)
	case PROPTYPE_STATIC:
		tempStr = value.text;
		propReturn.chstr = tempStr;
		oInfo->ETOnPropertyChanged(tempMovExt, propName, 1, NULL, &propReturn, NULL);
		break;

		// Button - notify of click
	case PROPTYPE_BUTTON:
		oInfo->ETOnPropertyChanged(tempMovExt, propName, 1, NULL, &propReturn, NULL);
		break;

		// Value - send integer value
	case PROPTYPE_VALUE:
		propReturn.val = value.i;
		oInfo->ETOnPropertyChanged(tempMovExt, propName, 1, NULL, &propReturn, NULL);
		break;

		// Combo - send integer value
	case PROPTYPE_COMBO:
		// Color
	case PROPTYPE_COLOR:
		// Checkbox
	case PROPTYPE_CHECKBOX:
		// font
	case PROPTYPE_FONT:

	case PROPTYPE_SLIDER:

	case PROPTYPE_FRAME:

		propReturn.val = value.i;
		oInfo->ETOnPropertyChanged(tempMovExt, propName, 1, NULL, &propReturn, NULL);
		break;
		
		// Float - send double value
	case PROPTYPE_FLOAT:
		propReturn.fVal = value.f;
		oInfo->ETOnPropertyChanged(tempMovExt, propName, 1, NULL, &propReturn, NULL);
		break;

		// Percentage
	case PROPTYPE_PERCENT:
		propReturn.fVal = value.f / 100.0;
		oInfo->ETOnPropertyChanged(tempMovExt, propName, 1, NULL, &propReturn, NULL);
		break;
	}

	// Free old data
	if (pMov->eData)
		delete[] pMov->eData;

	// Now the temp object has been told of the change, serialize back again and replace old data
	bin movDataSave(g_allocator);
	movDataSave.loading = false;
	movDataSave.allocate(1024);
	oInfo->ETSerialize(tempMovExt, movDataSave);

	pMov->eData = (BYTE*)movDataSave.ptr();
	pMov->eSize = movDataSave.size();

	// Detach the bin to prevent it deallocating the data
	movDataSave.detach();

	if(level->m_ObjectFrameIsLockedTo == -1)
	{
		// the tempory Behavior has been locked...so we dont want to free it.
	}
	else
	{
		// Deallocate the temp object
		oInfo->ETDestroyObject(tempMovExt);

		level->m_pTempMovExt = 0;
		level->m_oTemporyBehaviorInfo = 0;
		level->m_pControlBehaviorData = 0;
		level->m_pControlBehaviorDataSize = 0;
		delete tempMovExt;
	}

	return 0;
}

int CContainerPropertyMgr::OnChange()
{
	// Delete this obj from container
	vector<int>::iterator i = find(pContainer->m_IDs.begin(), pContainer->m_IDs.end(), oid);

	// Remove this object from container
	if (i != pContainer->m_IDs.end()) {
		long lngID = *i;
		CObjType* pType = pContainer->application->object_types[lngID];
		pContainer->m_IDs.erase(i);
		pType->m_pContainer = NULL;
	}

	// Last in container?  delete.
	if (pContainer->m_IDs.size() == 1)
		pContainer->application->DeleteContainer(pContainer);

	return 1;	// refresh bar
}

int CTransitionPropertyMgr::OnChange()
{
	if (propName == "Remove") {

		// Find this transition and remove it
		list<CTransition>::iterator i = application->m_Transitions.begin();
		
		for ( ; i != application->m_Transitions.end(); i++) {
			if (i->m_Name == m_pTransition->m_Name) {
				application->m_Transitions.erase(i);
				return 1;
			}
		}
	}

	else if (propName == "Name") {

		GetPropertyValue();

		// Check new name doesn't exist
		if (!exists_element(application->m_Transitions.begin(), application->m_Transitions.end(), value.text))
			m_pTransition->m_Name = value.text;

		return 0;
	}

	// All other properties: simply read/write the text of the property
	else {
		GetPropertyValue();

		EffectParameter& ep = *find(m_pTransition->params.begin(), m_pTransition->params.end(), propName);

		switch (ep.type) {
			// Float
			case 5:
				ep.value = value.text;
				break;
			// Percent
			case 9:
				ep.value = FloatToString(atof(value.text) / 100.0);
				break;
		}
	}

	return 0;
}
extern CMultiDocTemplate* pDocTemplate;
extern CMainFrame* pMainWnd;
int EventSheetPropertyMgr::OnChange()
{
	// This class is allocated for each item, and the only item for each event sheet is 'edit'. 
	CChildFrame *childFrm = new CChildFrame(application, NULL, m_pEventSheet);


	CCreateContext context;
	context.m_pCurrentFrame = childFrm;
	context.m_pNewViewClass = RUNTIME_CLASS(EventSheetEditor);
	context.m_pNewDocTemplate = pDocTemplate;
	context.m_pCurrentDoc = pDocTemplate->CreateNewDocument();
	CRect rc(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT);

	CString EventSheetTitle;
	EventSheetTitle.Format("Event Sheet: %s", m_pEventSheet->GetName());
	BOOL ret = childFrm->Create(NULL, 
		EventSheetTitle, 
					 WS_CHILD | WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
					 rc, pMainWnd, &context);

	// Maximize
	childFrm->ShowWindow(SW_SHOWMAXIMIZED);

	CWnd* view = childFrm->splitter.GetWindow(GW_CHILD);
	if(view)
	for(int a = 0; a < 10; a ++)
	{
		if(!dynamic_cast<EventSheetEditor*>(view))
		{
			view = view->GetWindow(GW_HWNDNEXT);
			if(!view)
				a = 10;

		}
	}
	
	((EventSheetEditor*)view)->m_pEventList = &m_pEventSheet->m_EventList;
	((EventSheetEditor*)view)->m_pHeader = &m_pEventSheet->m_Header;
	((EventSheetEditor*)view)->OnInitialUpdate();

	// now remove the tab
	view = childFrm->splitter.GetWindow(GW_CHILD);
	for(int a = 0; a < 10; a ++)
	{
		if(!dynamic_cast<CExtTabWnd*>(view))
		{
			view = view->GetWindow(GW_HWNDNEXT);
			if(!view)
				a = 10;

		}
	}

	if(view)
		((CExtTabWnd*)view)->ItemRemove(0,1,true);

	childFrm->RecalcLayout();
	childFrm->splitter.RecalcLayout();

	return 0;
}

///////////////////////////
// Traits

CTraitPropertyValueMgr::CTraitPropertyValueMgr(CString _Trait, CObjType* _pOwner, CLayoutEditor* _frame, const CString& _propName)
: Trait(_Trait),
  pOwner(_pOwner),
  propName(_propName),
  frame(_frame)
{
	// Find the property entry by looping and matching the label to propName
	// vector<CPropItem>::iterator j = pMov->oInfo->Properties.begin();
	// const vector<CPropItem>::const_iterator props_end = pMov->oInfo->Properties.end();

	//for ( ; j != props_end; j++) {
	//	if (j->Label == propName)
	//		break;
	//}

	// Save index to properties array
	//propIndex = j - pMov->oInfo->Properties.begin();
}

int CControlPropertyValueMgr::OnChange()
{
	GetPropertyValue();

	CString controlName = propName;
	controlName.Delete(controlName.Find('(') - 1, controlName.GetLength());

	// Find this control
	//vector<BehaviorControl>::iterator i = find(application->m_Controls.begin(), application->m_Controls.end(), propName);
	vector<BehaviorControl>::iterator i = application->m_Controls.begin();

	for ( ; i != application->m_Controls.end(); ++i) {
		if (i->name == controlName && i->player == player)
			break;
	}

	if (i != application->m_Controls.end()) {
		i->comboIndex = value.i;
		i->vk = controlTable[value.i].vk;
	}

	return 0;
}

int CTraitPropertyValueMgr::OnChange()
{
	GetPropertyValue();

	// Adding a trait
	if (value.i) {
		// If not already in the traits list
		if (find(pOwner->traits.begin(), pOwner->traits.end(), Trait) == pOwner->traits.end())
			// Add to traits list
			pOwner->traits.push_back(Trait);
	}
	// Removing a trait
	else {
		// Find the trait to remove if it's there (due to multiselection)
		vector<CString>::iterator t = find(pOwner->traits.begin(), pOwner->traits.end(), Trait);

		if (t != pOwner->traits.end())
			pOwner->traits.erase(t);
	}
	/*
	vector<CString>::iterator i = pOwner->traits.begin();
	vector<CString>::iterator traits_end = pOwner->traits.end();

	for ( ; i != traits_end; i++) {
		// Match addresses of CBehaviors
		if ((*i) == Trait) {
			pOwner->traits.erase(i);
			return 1;
		}
	}

	// Ought to be unreachable
	MessageBox(NULL, "Remove trait broken - please contact Scirra about this bug!", "Error", MB_OK | MB_ICONHAND);
	*/
	return 0;
}

