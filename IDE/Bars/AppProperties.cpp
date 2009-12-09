
#include "StdAfx.h"
#include "PropertiesBar.h"

ControlTableEntry controlTable[] = {
	{VK_LBUTTON,	"Left mouse button"},			// 0
	{VK_MBUTTON,	"Middle mouse button"},
	{VK_RBUTTON,	"Right mouse button"},
	{VK_TAB,		"Tab"},
	{VK_RETURN,		"Enter"},
	{VK_SHIFT,		"Shift"},
	{VK_LSHIFT,		"Shift (left)"},
	{VK_RSHIFT,		"Shift (right)"},
	{VK_CONTROL,	"Control"},
	{VK_LCONTROL,	"Control (left)"},
	{VK_RCONTROL,	"Control (right)"},				// 10
	{VK_MENU,		"Alt"},
	{VK_LMENU,		"Alt (left)"},
	{VK_RMENU,		"Alt (right)"},
	{VK_PAUSE,		"Pause"},
	{VK_ESCAPE,		"Escape"},
	{VK_SPACE,		"Space"},
	{VK_HOME,		"Home"},
	{VK_END,		"End"},
	{VK_UP,			"Up arrow"},
	{VK_DOWN,		"Down arrow"},					// 20
	{VK_LEFT,		"Left arrow"},
	{VK_RIGHT,		"Right arrow"},
	{VK_INSERT,		"Insert"},
	{VK_DELETE,		"Delete"},
	{'0',			"0"},
	{'1',			"1"},
	{'2',			"2"},
	{'3',			"3"},
	{'4',			"4"},
	{'5',			"5"},							// 30
	{'6',			"6"},
	{'7',			"7"},
	{'8',			"8"},
	{'9',			"9"},
	{'A',			"A"},
	{'B',			"B"},
	{'C',			"C"},
	{'D',			"D"},
	{'E',			"E"},
	{'F',			"F"},
	{'G',			"G"},
	{'H',			"H"},
	{'I',			"I"},
	{'J',			"J"},
	{'K',			"K"},
	{'L',			"L"},
	{'M',			"M"},
	{'N',			"N"},
	{'O',			"O"},
	{'P',			"P"},
	{'Q',			"Q"},
	{'R',			"R"},
	{'S',			"S"},
	{'T',			"T"},
	{'U',			"U"},
	{'V',			"V"},
	{'W',			"W"},
	{'X',			"X"},
	{'Y',			"Y"},
	{'Z',			"Z"},
	{VK_BACK,		"Backspace"},
	{VK_NUMPAD0,	"Num pad 0"},
	{VK_NUMPAD1,	"Num pad 1"},
	{VK_NUMPAD2,	"Num pad 2"},
	{VK_NUMPAD3,	"Num pad 3"},
	{VK_NUMPAD4,	"Num pad 4"},
	{VK_NUMPAD5,	"Num pad 5"},
	{VK_NUMPAD6,	"Num pad 6"},
	{VK_NUMPAD7,	"Num pad 7"},
	{VK_NUMPAD8,	"Num pad 8"},
	{VK_NUMPAD9,	"Num pad 9"},
	{VK_ADD,		"Num pad Add"},
	{VK_SUBTRACT,	"Num pad Subtract"},
	{VK_MULTIPLY,	"Num pad Mulitply"},
	{VK_DIVIDE,		"Num pad Divide"},
	{VK_DECIMAL,	"Num pad Decimal"},
	{VK_F1,			"F1"},
	{VK_F2,			"F2"},
	{VK_F3,			"F3"},
	{VK_F4,			"F4"},
	{VK_F5,			"F5"},
	{VK_F6,			"F6"},
	{VK_F7,			"F7"},
	{VK_F8,			"F8"},
	{VK_F9,			"F9"},
	{VK_F10,		"F10"},
	{VK_F11,		"F11"},
	{VK_F12,		"F12"},
	{-2,			"(Disabled)"},	// 'GetControlVK' used by plugins returns -1 when no control specified; use -2 as disabled
};


void PropertiesBar::DoAppProperties()
{
	CExtPropertyItem* pRoot;
	CExtPropertyItem* pItem;
	CExtGridCellString* pString;
	CExtGridCellDropListComboBox* pCombo;
	CExtGridCellNumber* pNumber;
	CExtGridCellCheckBox* pCheck;
	CExtGridCellHyperLinkEx* pLink;

	CApplication* app = m_pCurrentApplication;

	//////////////////////////
	// INFORMATION CATEGORY ///////////////////////////////////////////////////
	pRoot = InsertCategory(PROPERTIES_INFORMATION, PROPERTIES_DESC_INFORMATION);

	pItem = InsertAppItem(PROPERTIES_CREATOR, PROPERTIES_DESC_CREATOR, pRoot);
	pString = GetEdit(pItem);
	pString->TextSet(app->file_information.creator);
	pItem->ValueActiveFromDefault();

	pItem = InsertAppItem(PROPERTIES_TITLE, PROPERTIES_DESC_TITLE, pRoot);
	pString = GetEdit(pItem);
	pString->TextSet(app->file_information.name);
	pItem->ValueActiveFromDefault();

	/////////////////////
	// WINDOW CATEGORY ///////////////////////////////////////////////////
	pRoot = InsertCategory(PROPERTIES_WINDOW, PROPERTIES_DESC_WINDOW);

	pItem = InsertAppItem("Menu", "Show a menu by default.", pRoot);
	pCheck = GetCheckBox(pItem);
	pCheck->SetCheck(app->window_information.show_menu);
	pItem->ValueActiveFromDefault();

	pItem = InsertAppItem("Caption", "Have a window titlebar and border.", pRoot);
	pCheck = GetCheckBox(pItem);
	pCheck->SetCheck(app->window_information.caption);
	pItem->ValueActiveFromDefault();

	pItem = InsertAppItem("Resizing", "Make the window resizable and decide what happens when the window is resized.", pRoot);
	pCombo = GetCombo(pItem);
		pCombo->InsertString("Disabled");
		pCombo->InsertString("Show more window content");

		// Stretch option only available to DX games
	if (m_pCurrentApplication->runtime == CApplication::rt_directx)
		pCombo->InsertString("Stretch window content");

		pCombo->SetCurSel(app->window_information.resizable);
	pItem->ValueActiveFromDefault();

	pItem = InsertAppItem("Minimize Box", "Show a minimize box on the application window.", pRoot);
	pCheck = GetCheckBox(pItem);
	pCheck->SetCheck(app->window_information.minimize_box);
	pItem->ValueActiveFromDefault();

	pItem = InsertAppItem("Maximize Box", "Show a maximize box on the application window.", pRoot);
	pCheck = GetCheckBox(pItem);
	pCheck->SetCheck(app->window_information.maximize_box);
	pItem->ValueActiveFromDefault();

	pItem = InsertAppItem(PROPERTIES_WINDOWWIDTH, PROPERTIES_DESC_WINDOWWIDTH, pRoot);
	pNumber = GetInteger(pItem);
	pNumber->_VariantAssign(app->window_information.window_width);
	pItem->ValueActiveFromDefault();

	pItem = InsertAppItem(PROPERTIES_WINDOWHEIGHT, PROPERTIES_DESC_WINDOWHEIGHT, pRoot);
	pNumber = GetInteger(pItem);
	pNumber->_VariantAssign(app->window_information.window_height);
	pItem->ValueActiveFromDefault();

	//////////////////////
	// RUNTIME CATEGORY ///////////////////////////////////////////////////
	pRoot = InsertCategory(PROPERTIES_CRUNTIME, PROPERTIES_DESC_CRUNTIME);

	pItem = InsertAppItem("Enable Scripting", "Enable Python scripting.  Requires Python26.dll redistribution.", pRoot);
	pCheck = GetCheckBox(pItem);
	pCheck->SetCheck(app->game_information.use_python);
	pItem->ValueActiveFromDefault();

	if (m_pCurrentApplication->runtime == CApplication::rt_directx)
	{
		pItem = InsertAppItem(PROPERTIES_FRAMERATEMODE, PROPERTIES_DESC_FRAMERATEMODE, pRoot);
		pCombo = GetCombo(pItem);
		pCombo->InsertString(PROPERTIES_VSYNCED);
		pCombo->InsertString(PROPERTIES_UNLIMITED);
		pCombo->InsertString(PROPERTIES_FIXED);
		pCombo->SetCurSel(app->game_information.fps_mode);
		pItem->ValueActiveFromDefault();

		pItem = InsertAppItem(PROPERTIES_FPS, PROPERTIES_DESC_FPS, pRoot);
		pNumber = GetInteger(pItem);
		pNumber->_VariantAssign(app->game_information.fps);
		pItem->ValueActiveFromDefault();

		pItem = InsertAppItem("FPS in caption", "Show the framerate in the window caption.", pRoot);
		pCheck = GetCheckBox(pItem);
		pCheck->SetCheck(app->game_information.fps_in_caption);
		pItem->ValueActiveFromDefault();

		pItem = InsertAppItem(PROPERTIES_FULLSCREEN, PROPERTIES_DESC_FULLSCREEN, pRoot);
		pCheck = GetCheckBox(pItem);
		pCheck->SetCheck(app->game_information.fullscreen);
		pItem->ValueActiveFromDefault();
	}

	pItem = InsertAppItem(PROPERTIES_RUNTIME, PROPERTIES_DESC_RUNTIME, pRoot);
	pCombo = GetCombo(pItem);
	pCombo->InsertString(PROPERTIES_APPLICATION);
	pCombo->InsertString(_T("Direct-X 9"));
	pCombo->SetCurSel(app->runtime);
	pItem->ValueActiveFromDefault();

	// SDL not yet implemented -AG
	/*
	if (m_pCurrentApplication->m_Runtime == RUNTIME_SDL) 
	{
		pItem = InsertAppItem(_T("Use hardware"), _T("SDL will use hardware acceleration where available."), pRoot);
		pCheck = GetCheckBox(pItem);
		pCheck->SetCheck(m_pCurrentApplication->m_SDLUseHardware);
		pItem->ValueActiveFromDefault();
	}
	*/

	if (m_pCurrentApplication->runtime == CApplication::rt_directx) 
	{
		pItem = InsertAppItem(_T("Load Textures"), _T("Specify when object textures are loaded in to VRAM."), pRoot);
		pCombo = GetCombo(pItem);
		pCombo->InsertString(_T("On application start"));
		pCombo->InsertString(_T("Per layout"));
		pCombo->SetCurSel((int)app->game_information.texture_loading);
		pItem->ValueActiveFromDefault();

		pItem = InsertAppItem(_T("Sampling"), _T("Specify point or linear sampling for rotations and scaling."), pRoot);
		pCombo = GetCombo(pItem);
		pCombo->InsertString(_T("Point"));
		pCombo->InsertString(_T("Linear"));
		pCombo->SetCurSel((int)app->game_information.sampler);
		pItem->ValueActiveFromDefault();

		pItem = InsertAppItem(_T("Text rendering"), _T("Specify the quality of the text rendering."), pRoot);
		pCombo = GetCombo(pItem);
		pCombo->InsertString(_T("Aliased"));
		pCombo->InsertString(_T("Antialiased"));
		pCombo->InsertString(_T("Cleartype"));
		pCombo->SetCurSel((int)app->game_information.text_rendering_mode);
		pItem->ValueActiveFromDefault();

		pItem = InsertAppItem(_T("Multisampling"), _T("Antialiasing of object edges"), pRoot);
		pCombo = GetCombo(pItem);
		pCombo->InsertString(_T("Off"));
		pCombo->InsertString(_T("2x"));
		pCombo->InsertString(_T("4x"));
		pCombo->InsertString(_T("8x"));
		pCombo->SetCurSel((int)app->game_information.multisamples);
		pItem->ValueActiveFromDefault();
	}

	// Advanced timedelta settings subcategory
	pRoot = InsertCategory("Advanced", "Advanced runtime settings", pRoot);

	pItem = InsertAppItem("Minimum FPS", "Minimum framerate.  Game slows down below this point to compensate.", pRoot);
	pNumber = GetFloat(pItem);
	pNumber->_VariantAssign(app->game_information.minimum_fps);
	pItem->ValueActiveFromDefault();

	pItem = InsertAppItem("Override TimeDelta", "Override the time delta such that a constant value is returned.", pRoot);
	pCheck = GetCheckBox(pItem);
	pCheck->SetCheck(app->game_information.override_timedelta);
	pItem->ValueActiveFromDefault();

	pItem = InsertAppItem("Timer FPS", "The intended framerate.  TimeDelta will evaluate to 1 divided by this value.", pRoot);
	pNumber = GetInteger(pItem);
	pNumber->_VariantAssign(app->game_information.time_delta_override);
	pItem->ValueActiveFromDefault();

	if (m_pCurrentApplication->runtime == CApplication::rt_directx)
	{
	
		pRoot = InsertCategory("Effects", "Settings and information about this application's pixel shaders and effects.");

		CString s;

		CD3DDisplay* pDisplay = m_pCurrentApplication->m_display;

		if (pDisplay != NULL) {

			// For version checking shaders
			UINT ps_major = D3DSHADER_VERSION_MAJOR(pDisplay->GetD3DCaps()->PixelShaderVersion);
			UINT ps_minor = D3DSHADER_VERSION_MINOR(pDisplay->GetD3DCaps()->PixelShaderVersion);
			s.Format("PS %d.%d", ps_major, ps_minor);
		}
		else
			s = "Unknown";

		pItem = InsertAppItem("Shader version", "Your computer's supported pixel shader version.", pRoot);
		pString = GetEdit(pItem);
		pString->ModifyStyle(__EGCS_READ_ONLY | __EGCS_NO_INPLACE_CONTROL);
		pString->TextSet(s);
		pItem->ValueActiveFromDefault();

		pItem = InsertAppItem(_T("Run without PS"), _T("Allow effects not supported by hardware to be disabled, and continue running game."), pRoot);
		pCheck = GetCheckBox(pItem);
		pCheck->SetCheck(m_pCurrentApplication->game_information.run_without_ps);
		pItem->ValueActiveFromDefault();

		pItem = InsertAppItem("Simulate shader", "Simulate limited pixel shader capability for testing.", pRoot);
		pCombo = GetCombo(pItem);
		pCombo->AddString("(no override)");
		pCombo->AddString("PS 1.4");
		pCombo->AddString("PS 1.1");
		pCombo->AddString("PS 0.0");
		pCombo->SetCurSel((int)m_pCurrentApplication->game_information.simulate_shaders);
		pItem->ValueActiveFromDefault();

		int MotionBlurCombo[] = {
			0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20
		};

		pItem = InsertAppItem("Motion Blur", "Enable a global motion blur effect at runtime.", pRoot);
		pCombo = GetCombo(pItem);
		pCombo->AddString("Off");
		pCombo->AddString("2x (lowest quality)");
		pCombo->AddString("3x");
		pCombo->AddString("4x");
		pCombo->AddString("5x (recommended)");
		pCombo->AddString("6x");
		pCombo->AddString("7x");
		pCombo->AddString("8x");
		pCombo->AddString("9x");
		pCombo->AddString("10x");
		pCombo->AddString("15x");
		pCombo->AddString("20x (highest quality)");

		int x;
		for (x = 0; x < 12; x++) {
			if (MotionBlurCombo[x] == m_pCurrentApplication->game_information.motion_blur_steps)
				break;
		}
		pCombo->SetCurSel(x);
		pItem->ValueActiveFromDefault();
	}
/*
	pRoot = InsertCategory(PROPERTIES_GLOBALVARS, PROPERTIES_DESC_GLOBALVARS);

		// Add global vars
		list<CApplication::GlobalVariable>::iterator g = m_pCurrentApplication->global_variables.begin();
		list<CApplication::GlobalVariable>::iterator globals_end = m_pCurrentApplication->global_variables.end();

		for ( ; g != globals_end; g++) {

			pItem = InsertGlobalItem(g->name, PROPERTIES_DESC_GLOBALVAR, pRoot);
			pString = GetEdit(pItem);
			pString->TextSet(g->value);
			pItem->ValueActiveFromDefault();
		}

		pItem = InsertAppItem(PROPERTIES_EDITGLOBALS, PROPERTIES_DESC_EDITGLOBALS , pRoot);
		pLink = GetHyperLink(pItem);
		pLink->TextSet(PROPERTIES_ADD_EDIT);
		pItem->ValueActiveFromDefault();*/

	CExtPropertyItem* pTransitionRoot = InsertCategory("Transitions", "A list of all transitions available to the application.");
	
		list<CTransition>::iterator t = m_pCurrentApplication->m_Transitions.begin();

		for ( ; t != m_pCurrentApplication->m_Transitions.end(); t++) {

			CExtPropertyItem* pTransitionCat = InsertCategory(t->m_Name, "", pTransitionRoot);

			pItem = InsertTransitionItem("Name", "", &(*t), pTransitionCat);
			pString = GetEdit(pItem);
			pString->TextSet(t->m_Name);
			pItem->ValueActiveFromDefault();

			// Add each of the transition's properties.
			vector<EffectParameter>::iterator i = t->params.begin();
			const vector<EffectParameter>::const_iterator params_end = t->params.end();

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

				pItem = InsertTransitionItem(item.Label, item.Description, &(*t), pTransitionCat);
				AddPropertyItemToTree(item, pItem, itemText, itemValue);
				pItem->ValueActiveFromDefault();
			}

			pItem = InsertTransitionItem("Remove", "", &(*t), pTransitionCat);
			pLink = GetHyperLink(pItem);
			pLink->TextSet("Remove");
			pItem->ValueActiveFromDefault();
		}

		pItem = InsertAppItem("Add transition", "Click to add a new transition.", pTransitionRoot);
		pLink = GetHyperLink(pItem);
		pLink->TextSet(PROPERTIES_ADD_EDIT);
		pItem->ValueActiveFromDefault();

		// Event lists
		/*CExtPropertyItem* pEventSheetRoot = InsertCategory("Event Sheets", "A list of all event sheets in the application.");
	
		list<EventSheet*>::iterator e = m_pCurrentApplication->event_sheets.begin();

		for ( ; e != m_pCurrentApplication->event_sheets.end(); e++) 
		{
			CExtPropertyItem* pEventSheet = InsertEventSheetItem((*e)->GetName(), "", (*e), pEventSheetRoot);
			pLink = GetHyperLink(pEventSheet);
			pLink->TextSet("Edit");
			pEventSheet->ValueActiveFromDefault();		
		}

		pItem = InsertAppItem("Manage Event Sheets", "Click to edit and delete all the application's event sheets.", pEventSheetRoot);
		pLink = GetHyperLink(pItem);
		pLink->TextSet(PROPERTIES_ADD_EDIT);
		pItem->ValueActiveFromDefault();*/

	pRoot = InsertCategory("Grouping", "Modify the grouping tools in this application.");

		pItem = InsertAppItem("Edit Attributes", "Click to edit which attributes are available in Construct.", pRoot);
		pLink = GetHyperLink(pItem);
		pLink->TextSet(PROPERTIES_ADD_EDIT);
		pItem->ValueActiveFromDefault();

		pItem = InsertAppItem("Edit Families", "Click to edit which families are available in Construct.", pRoot);
		pLink = GetHyperLink(pItem);
		pLink->TextSet(PROPERTIES_ADD_EDIT);
		pItem->ValueActiveFromDefault();

		pItem = InsertAppItem("Manage Families", "Click to edit which families are available in Construct.", pRoot);
		pLink = GetHyperLink(pItem);
		pLink->TextSet("Open");
		pItem->ValueActiveFromDefault();

	pRoot = InsertCategory("Controls", "Set the controls used for movements.");

		vector<BehaviorControl>::iterator c = app->m_Controls.begin();

		for ( ; c != app->m_Controls.end(); c++) {

			CString label;
			label.Format("%s (P%d)", c->name, c->player + 1);
			pItem = InsertControlItem(label, "A control key.", c->player, pRoot);
			pCombo = GetCombo(pItem);

			int numElems = sizeof(controlTable) / sizeof(ControlTableEntry);

			for (int i = 0; i < numElems; i++)
				pCombo->InsertString(controlTable[i].name);

			pCombo->SetCurSel(c->comboIndex);

			pItem->ValueActiveFromDefault();
		}

		pItem = InsertAppItem("Manage Controls", "Click to edit controls.", pRoot);
		pLink = GetHyperLink(pItem);
		pLink->TextSet(PROPERTIES_ADD_EDIT);
		pItem->ValueActiveFromDefault();

		/*
	pRoot = InsertCategory("Python", "Setting for python script");
		pItem = InsertAppItem("Manage Python", "Click to edit python distribution options.", pRoot);
		pLink = GetHyperLink(pItem);
		pLink->TextSet(PROPERTIES_ADD_EDIT);
		pItem->ValueActiveFromDefault();
		*/

	/*
		pItem = InsertAppItem(_T("Runjlkj without PS"), _T(""), pRoot);
		pCheck = GetCheckBox(pItem);
		pCheck->SetCheck(m_pCurrentApplication->m_runWithoutPs);
		pItem->ValueActiveFromDefault();
*/
}