// SystemObject.cpp

#include "StdAfx.h"
#include "..\Structure.h"
#include "SystemObject.h"

// Get ACE via ID
void GetSystemAction(int ID, ACESEntry2*& Out)
{
	Out = &system_actions[ID];
	Out->params.clear();
	Out->aceDisplayText = "";
	Out->aceListName = "";
	Out->retrn = 0;
	Out->resourceID = ID;
	Out->aceCategory = "default";
	switch (ID)
	{
	case 0:		SO_FlushClipboard(*Out); break;
	case 1:		SO_SetClipboardText(*Out); break;
	case 2:		SO_WriteINI(*Out); break;
	case 3:		SO_PlaySound(*Out); break;
	case 4:		SO_ScrollX(*Out); break;
	case 5:		SO_ScrollY(*Out); break;
	case 6:		SO_SetDisplayAngle(*Out); break;
	//case 7:		SO_SetGlobalFilter(*Out); break;
	case 8:		SO_StartLoop(*Out); break;
	case 9:		SO_StopLoop(*Out); break;
	case 10:	SO_Break(*Out); break;
	case 11:	SO_MessageBox(*Out); break;
	case 12:	SO_CreateObject(*Out); break;
	case 13:	SO_CreateObjectByName(*Out); break;
	case 14:	SO_NextFrame(*Out); break;
	case 15:	SO_PreviousFrame(*Out); break;
	case 16:	SO_GoToFrame(*Out); break;
	case 17:	SO_Quit(*Out); break;
	case 18:	SO_CancelClose(*Out); break;
	case 19:	SO_SetFPS(*Out); break;
	case 20:	SO_SetGlobal(*Out); break;
	case 21:	SO_AddGlobal(*Out); break;
	case 22:	SO_SubGlobal(*Out); break;
	case 23:	SO_EnableGroup(*Out); break;
	case 24:	SO_ScrollToObject(*Out); break;
	case 25:	SO_Serialize(*Out); break;
	case 26:	SO_EndModalFrame(*Out); break;
	case 27:	SO_CreateRelativePP(*Out); break;
	case 28:	SO_CreateRelativeIP(*Out); break;

	case 29:	SO_SetLayerXScrollRatio(*Out); break;
	case 30:	SO_SetLayerYScrollRatio(*Out); break;
	case 31:	SO_SetLayerZoomOffset(*Out); break;

	case 32:	SO_SetLayerXScrollOffset(*Out); break;
	case 33:	SO_SetLayerYScrollOffset(*Out); break;
	case 34:	SO_SetLayerZoomRate(*Out); break;

	case 35:	SO_SetLayerVisible(*Out); break;
	case 36:	SO_SetLayerOpacity(*Out); break;
	case 37:	SO_SetLayerFilter(*Out); break;
	case 38:	SO_SetZoom(*Out); break;
	case 39:	SO_SetLayoutSize(*Out); break;
	case 40:	SO_SetResolution(*Out); break;
	case 41:	SO_Quicksave(*Out);		break;

	// 42 : FxActivate
	// 43 : FxDeactivate
	// 44 : FxSetParam

	case 45:	SO_RunScript(*Out);	break;
	case 46:	SO_SetMotionBlur(*Out); break;
	case 47:	SO_SetFullscreen(*Out); break;
	case 48:	{

					Out->aceDisplayText = "Set time scale to %0";
					Out->scriptName = "SetTimeScale";
					Out->aceListName = "Set time scale";
					Out->aceCategory = "Time";
					Out->retrn = 0;

					PARAM2 param;
					param.desc = "Time scale of the runtime.  0.5 is half-speed motion, 2.0 double speed, etc.";
					param.name = "Time scale";
					param.initStr = "1.0";
					param.type = 1;
					Out->params.push_back(param);

				} break;
	case 49:	{

					Out->aceDisplayText = "Set layer %0 angle to %1";
					Out->scriptName = "SetLayerAngle";
					Out->aceListName = "Set layer angle";
					Out->aceCategory = "Layers";
					Out->retrn = 0;

					PARAM2 param;
					param.desc = "Layer that is affected";
					param.name = "Layer name or number";
					param.initStr = "1";
					param.type = 1;
					Out->params.push_back(param);

					param.desc = "Angle of the layer, in degrees.";
					param.name = "Angle";
					param.initStr = "0";
					param.type = 1;
					Out->params.push_back(param);

				} break;
	case 50:	{

					Out->aceDisplayText = "Toggle group %0 enabled";
					Out->scriptName = "ToggleGroup";
					Out->aceListName = "Toggle group enabled";
					Out->aceCategory = "Event groups";
					Out->retrn = 0;

					PARAM2 param;
					param.desc = "Name of event group to toggle";
					param.name = "Group";
					param.initStr = "";
					param.type = EDITORPARAM_STRING;
					Out->params.push_back(param);

				} break;
	case 51:	{
					Out->aceDisplayText = "%0 textures for layout %1";
					Out->scriptName = "LoadTextures";
					Out->aceListName = "Load/unload layout textures";
					Out->aceCategory = "Application";
					Out->retrn = 0;

					PARAM2 param;
					param.desc = "Select whether to load or release texture memory for the given layout.";
					param.name = "Textures";
					param.initStr = "Unload|Load";
					param.type = EDITORPARAM_COMBOBOX;
					Out->params.push_back(param);

					PARAM2 param2;
					param2.desc = "Layout to affect.";
					param2.name = "Layout";
					param2.initStr = "1";
					param2.type = EDITORPARAM_STRING;
					Out->params.push_back(param2);

				} break;
	case 52:	{
					Out->aceDisplayText = "%0 : %1 %2"; // displays as: Sprite: Remove attribute 'Solid'
					Out->scriptName = "AddToAttribute";
					Out->aceListName = "Add/remove attribute from object";
					Out->aceCategory = "Attributes";
					Out->retrn = 0;

					PARAM2 param;
					param.desc = "Object to affect";
					param.name = OBJECTS;
					param.type = 6;
					Out->params.push_back(param);

					PARAM2 param2;
					param2.desc = "Select whether to load or release texture memory for the given layout.";
					param2.name = "Modification";
					param2.initStr = "Remove attribute|Add attribute";
					param2.type = EDITORPARAM_COMBOBOX;
					Out->params.push_back(param2);

					PARAM2 param3;
					param3.desc = "Attribute";
					param3.name = OBJECTS;
					param3.type = 6;
					Out->params.push_back(param3);
				} break;
	case 53:	{
					Out->aceDisplayText = "%0 : %1 %2"; // displays as: Sprite: Remove attribute 'Solid'
					Out->scriptName = "EnableLayerEffect";
					Out->aceListName = "Enable or disable layer effect";
					Out->aceCategory = "Layers";
					Out->retrn = 0;

					PARAM2 param;
					param.desc = "Layer that is affected";
					param.name = "Layer name or number";
					param.initStr = "1";
					param.type = 1;
					Out->params.push_back(param);

					PARAM2 param2;
					param2.desc = "Enable or disable effect";
					param2.name = "Effect";
					param2.initStr = "Disable|Enable";
					param2.type = EDITORPARAM_COMBOBOX;
					Out->params.push_back(param2);

					PARAM2 param3;
					param3.desc = "Effect name";
					param3.name = "Effect name";
					param3.type = 2;
					Out->params.push_back(param3);
				} break;

	case 54:	{
					Out->aceDisplayText = "%0 : %1 %2"; // displays as: Sprite: Remove attribute 'Solid'
					Out->scriptName = "SetLayerEffectParam";
					Out->aceListName = "Set layer effect param";
					Out->aceCategory = "Layers";
					Out->retrn = 0;

					PARAM2 param;
					param.desc = "Layer that is affected";
					param.name = "Layer name or number";
					param.initStr = "1";
					param.type = 1;
					Out->params.push_back(param);

					PARAM2 param2;
					param2.desc = "Effect name";
					param2.name = "Effect name";
					param2.type = 2;
					Out->params.push_back(param2);
	
					PARAM2 param3;
					param3.desc = "Param name";
					param3.name = "Param name";
					param3.type = 2;
					Out->params.push_back(param3);
	
					PARAM2 param4;
					param4.desc = "Param value";
					param4.name = "Param value";
					param4.type = 1;
					Out->params.push_back(param4);

				} break;

		/*
		// Couldn't make this work at runtime -AG
		case 51:	{

					Out->aceDisplayText = "Set multisampling to %0";
					Out->scriptName = "SetMultisampling";
					Out->aceListName = "Set multisampling";
					Out->aceCategory = "Display";
					Out->retrn = 0;

					PARAM2 param;
					param.desc = "Multisamples to render with.";
					param.name = "Multisampling";
					param.initStr = "Off|2x|3x|4x|5x|6x|7x|8x";
					param.type = EDITORPARAM_COMBOBOX;
					Out->params.push_back(param);

				} break;
				*/

#define LASTACTION 55
	}
}

// Get condition via ID
void GetSystemCondition(int ID, ACESEntry2*& Out)
{
	Out = &system_conditions[ID];
	Out->resourceID = ID;
	Out->params.clear();
	Out->aceDisplayText = "";
	Out->aceListName = "";
	Out->retrn = 0;
	Out->aceCategory = "System";
	switch (ID)
	{
	case 1: 
		{
			Out->aceDisplayText = "Start of layout";
			Out->aceListName = "*Start of layout";
			Out->aceCategory = "Start & End";
			Out->retrn = 0x0008;
		} break;
	case 2:
		{
			Out->aceDisplayText = "End of layout";
			Out->aceListName = "End of layout";
			Out->aceCategory = "Start & End";
			Out->retrn = 0x0008;
		} break;
		/*
	case 3:
		{
			Out->aceDisplayText = "Start of application";
			Out->aceListName = "Start of application";
			Out->aceCategory = "Start & End";
			Out->retrn = 0x0008;
		} break;
		*/
	case 4:
		{
			Out->aceDisplayText = "End of application";
			Out->aceListName = "End of application";
			Out->aceCategory = "Start & End";
			Out->retrn = 0x0008;
		} break;
	case 5:
		{
			Out->aceDisplayText = "<b><font color=""DarkBlue"">Always</font></b> (every tick)";
			Out->aceListName = "*Always";
			Out->retrn = 0;
		} break;
	case 6:
		{
			PARAM2 param1;
			param1.desc = "Value 1";
			param1.name = "Value 1";
			param1.type = 1; // ??is it 1 or 0 ffs! - davo

			PARAM2 param2;
			param2.initStr = "Equal to|Different to|Lower than|Lower or equal|Greater than|Greater or equal";
			param2.name = "Comparison";
			param2.desc = "Comparison to make";
			param2.type = 9;

			PARAM2 param3;
			param3.desc = "Value 2";
			param3.name = "Value 2";
			param3.type = 1; // ??is it 1 or 0 ffs! - davo

			Out->aceDisplayText = "<img>0</img> %0 %1 %2";
			Out->aceListName = "*Compare";
			Out->aceCategory = "Values";

			Out->params.push_back(param1);
			Out->params.push_back(param2);
			Out->params.push_back(param3);
			Out->retrn = 0;
		} break;
	case 7:
		{
			PARAM2 param;
			param.desc = "Run if expression evaluates as true";
			param.name = "Evaluate";
			param.type = 1; // ??is it 1 or 0 ffs! - davo

			Out->aceDisplayText = "<img>0</img> <i>%0</i>";
			Out->aceListName = "Evaluate";
			Out->aceCategory = "Values";
			Out->params.push_back(param);
			Out->retrn = 0x0000;
		} break;

		case 8:		SO_OnLoop(*Out); break;
		case 9:		SO_While(*Out); break;
		case 10:	SO_For(*Out); break;
		case 11:	SO_Repeat(*Out); break;
		case 12:	SO_ForEach(*Out); break;
		case 13:	SO_CompareGlobalVariable(*Out); break;
		case 14:	SO_EveryXMilliseconds(*Out); break;
		case 15:	SO_EveryXTicks(*Out); break;
		case 16:	SO_CompareTime(*Out); break;
		case 17:		// ELSE
			{
			Out->aceDisplayText = "<b><font color=""DarkBlue"">  Else</font></b>";
			Out->aceListName = "Else";
			Out->aceCategory = "Special conditions";
			Out->retrn = 0;
			}
			break;
		case 18:		// OR
			{
			Out->aceDisplayText = "<b><font color=""DarkBlue"">  OR</font></b>";
			Out->aceListName = "OR";
			Out->aceCategory = "Special conditions";
			Out->retrn = 0;
			} break;
		case 19:		// Object Overlaps points
			{
			PARAM2 param;
			param.desc = "Object to overlap";
			param.name = "Object";
			param.type = 6; // object

			PARAM2 param2;
			param2.desc = "Point X";
			param2.name = "Point X";
			param2.type = 1; //value

			PARAM2 param3;
			param3.desc = "Point Y";
			param3.name = "Point Y";
			param3.type = 1; //value

			Out->aceDisplayText = "<img>0</img> %0 at (%1,%2)";
			Out->aceListName = "Object overlaps point";
			Out->aceCategory = "Collisions";
			Out->params.push_back(param);
			Out->params.push_back(param2);
			Out->params.push_back(param3);
			Out->retrn = 0x0000;
			}
			break;
		case 20:		// Object collides with point
			{
			PARAM2 param;
			param.desc = "Object to collide";
			param.name = "Object";
			param.type = 6; // object

			PARAM2 param2;
			param2.desc = "Point X";
			param2.name = "Point X";
			param2.type = 1; //value

			PARAM2 param3;
			param3.desc = "Point Y";
			param3.name = "Point Y";
			param3.type = 1; //value

			Out->aceDisplayText = "<img>0</img> %0 collides with (%1,%2)";
			Out->aceListName = "Object collides with point";
			Out->aceCategory = "Collisions";
			Out->params.push_back(param);
			Out->params.push_back(param2);
			Out->params.push_back(param3);
			Out->retrn = 0x0008;
			}
			break;
		case 21:		// Make trigger
			{
			Out->aceDisplayText = "<b><font color=""DarkBlue"">  Trigger once</font></b>";
			Out->aceListName = "Trigger once while true";
			Out->aceCategory = "Special conditions";
			Out->retrn = 0;
			} break;
		case 22:		// Value is type
			{
			PARAM2 param1;
			param1.desc = "Value to test the type of.";
			param1.name = "Value";
			param1.type = 1; //value

			PARAM2 param2;
			param2.initStr = "Integer|Float|String|Array";
			param2.name = "Type";
			param2.desc = "The type to check.";
			param2.type = 9; // combo

			Out->aceDisplayText = "%0 is of type %1";
			Out->aceListName = "Value is of type";
			Out->aceCategory = "Values";
			Out->params.push_back(param1);
			Out->params.push_back(param2);
			Out->retrn = 0;
			} break;
		case 23:		// Menu item clicked (by text)
			{
			PARAM2 param1;
			param1.desc = "Text of the menu item to test if clicked.";
			param1.name = "Menu item text";
			param1.type = EDITORPARAM_STRING;

			Out->aceDisplayText = "<img>0</img> Menu item %0 clicked";
			Out->aceListName = "Menu item clicked (by text)";
			Out->aceCategory = "Menu";
			Out->params.push_back(param1);
			Out->retrn = 0x0008;	// triggered
			} break;
		case 24:		// Menu item clicked (by id)
			{
			PARAM2 param1;
			param1.desc = "ID of the menu item to test if clicked.";
			param1.name = "Menu item ID";
			param1.type = EDITORPARAM_INTEGER;

			Out->aceDisplayText = "<img>0</img> Menu item %0 clicked";
			Out->aceListName = "Menu item clicked (by ID)";
			Out->aceCategory = "Menu";
			Out->params.push_back(param1);
			Out->retrn = 0x0008;	// triggered
			} break;
		case 25:
			{

				PARAM2 param1;
   				param1.desc = "Python Statement String";
   				param1.name = "Python Statement String";
   				param1.type = EDITORPARAM_STRING; // ??is it 1 or 0 ffs! - davo
	   
   				PARAM2 param2;
   				param2.initStr = "Equal to|Different to|Lower than|Lower or equal|Greater than|Greater or equal";
   				param2.name = "Comparison";
   				param2.desc = "Comparison to make";
   				param2.type = 9;
	   
   				PARAM2 param3;
   				param3.desc = "Value 2";
   				param3.name = "Value 2";
   				param3.type = EDITORPARAM_UNDEFINED;
	   
   				Out->aceDisplayText = "<img>0</img> Python(%0) %1 %2";
   				Out->aceListName = "Python Compare";
   				Out->aceCategory = "Script";
	   
   				Out->params.push_back(param1);
   				Out->params.push_back(param2);
   				Out->params.push_back(param3);
   				Out->retrn = 0;
			} break;
		case 26:
			{

			Out->aceDisplayText = "Motion blur is supported";
			Out->aceListName = "Motion blur supported";
			Out->aceCategory = "Display";

			Out->retrn = 0;

			} break;
		case 27: SO_ForEachOrdered(*Out); break;
		case 28:	// on collision (advanced)
			{
				PARAM2 param1;
   				param1.desc = "Pick the first object involved in the collision.";
   				param1.name = "Object A";
   				param1.type = EDITORPARAM_OBJECT;
	   
   				PARAM2 param2;
   				param2.initStr = "Pick|Don't pick";
   				param2.name = "Pick object A?";
   				param2.desc = "Choose whether to pick object A for the event actions.";
   				param2.type = EDITORPARAM_COMBOBOX;

				PARAM2 param3;
   				param3.desc = "Pick the second object involved in the collision.";
   				param3.name = "Object B";
   				param3.type = EDITORPARAM_OBJECT;
	   
   				PARAM2 param4;
   				param4.initStr = "Pick|Don't pick";
   				param4.name = "Pick object B?";
   				param4.desc = "Choose whether to pick object B for the event actions.";
   				param4.type = EDITORPARAM_COMBOBOX;
	   
   				Out->params.push_back(param1);
   				Out->params.push_back(param2);
   				Out->params.push_back(param3);
				Out->params.push_back(param4);

				Out->aceDisplayText = "On collision between %0 (%1) and %2 (%3)";
				Out->aceListName = "On collision (advanced)";
				Out->aceCategory = "Collisions";
   				Out->retrn = 0;
			} break;
		case 29:	// is overlapping (advanced)
			{
				PARAM2 param1;
   				param1.desc = "Pick the first object involved in the overlap.";
   				param1.name = "Object A";
   				param1.type = EDITORPARAM_OBJECT;
	   
   				PARAM2 param2;
   				param2.initStr = "Pick|Don't pick";
   				param2.name = "Pick object A?";
   				param2.desc = "Choose whether to pick object A for the event actions.";
   				param2.type = EDITORPARAM_COMBOBOX;

				PARAM2 param3;
   				param3.desc = "Pick the second object involved in the overlap.";
   				param3.name = "Object B";
   				param3.type = EDITORPARAM_OBJECT;

   				PARAM2 param4;
   				param4.initStr = "Pick|Don't pick";
   				param4.name = "Pick object B?";
   				param4.desc = "Choose whether to pick object B for the event actions.";
   				param4.type = EDITORPARAM_COMBOBOX;
	   
   				Out->params.push_back(param1);
   				Out->params.push_back(param2);
   				Out->params.push_back(param3);
				Out->params.push_back(param4);

				Out->aceDisplayText = "%0 (%1) and %2 (%3) are overlapping";
				Out->aceListName = "Is overlapping (advanced)";
				Out->aceCategory = "Collisions";
   				Out->retrn = 0;
			} break;
		case 30:
			{
				PARAM2 param1;
				param1.name = "Group name";
   				param1.desc = "Enter the name of the event group to test.";
   				param1.type = EDITORPARAM_STRING;
	   
   				Out->aceDisplayText = "Group %0 is activated";
   				Out->aceListName = "Is group activated?";
   				Out->aceCategory = "Event groups";
	   
   				Out->params.push_back(param1);
   				Out->retrn = 0;
			} break;
		case 31:
			{
				PARAM2 param1;
				param1.desc = "Value to test";
				param1.name = "Value";
				param1.type = 1;

				PARAM2 param2;
				param2.desc = "Test if the value is greater or equal to this number.";
				param2.name = "Lowest number";
				param2.type = 1;

				PARAM2 param3;
				param3.desc = "Test if the value is also lower or equal to this number.";
				param3.name = "Highest number";
				param3.type = 1;

				Out->aceDisplayText = "<img>0</img> %0 is between %1 and %2";
				Out->aceListName = "Number is between...";
				Out->aceCategory = "Values";

				Out->params.push_back(param1);
				Out->params.push_back(param2);
				Out->params.push_back(param3);
				Out->retrn = 0;
			} break;
		case 32:
			{
				PARAM2 param1;
				param1.desc = "First angle to compare.";
				param1.name = "First angle";
				param1.type = 1;

				PARAM2 param2;
   				param2.initStr = "Clockwise|Anticlockwise";
   				param2.name = "Test";
   				param2.desc = "Choose whether to test if the first angle is clockwise or anticlockwise of the second angle.";
   				param2.type = EDITORPARAM_COMBOBOX;

				PARAM2 param3;
				param3.desc = "Second angle to compare.";
				param3.name = "Second angle";
				param3.type = 1;

				Out->aceDisplayText = "<img>0</img> Angle %0 is %1 from %2";
				Out->aceListName = "Angle is clockwise of...";
				Out->aceCategory = "Values";

				Out->params.push_back(param1);
				Out->params.push_back(param2);
				Out->params.push_back(param3);
				Out->retrn = 0;
			} break;
		case 33:
			Out->aceDisplayText = "On load";
			Out->aceListName = "On load";
			Out->aceCategory = "System";
			Out->retrn = 0x0008;
			break;
		case 34:
			Out->aceDisplayText = "On device reset";
			Out->aceListName = "On device reset";
			Out->aceCategory = "System";
			Out->retrn = 0x0008;
			break;
		case 35:
			Out->aceDisplayText = "<img>0</img> Layer %0 is visible";
			Out->aceListName = "Is layer visible?";
			Out->aceCategory = "Layers";
			Out->retrn = 0;

			PARAM2 param;
			param.desc = "Layer to test if visible";
			param.name = "Layer name or number";
			param.initStr = "1";
			param.type = 1;

			Out->params.push_back(param);
			break;


#define LASTCONDITION 36
	}
}

struct SystemExpressionEntry {
	CString expname;
	int numParams;
	CString listName;
	CString category;
	CString parametersToInsert;
};

// Probably a temporary solution because it does not incorporate intellisense.
// davo: the result u get when u ask tigs to do something ;p
SystemExpressionEntry SystemExpressionsTable[] = {
	{"MouseX",					0,		"Get mouse X",				"Mouse",			""},
	{"MouseY",					0,		"Get mouse Y",				"Mouse",			""},
	{"LoopIndex",				1,		"Get loop index",			"Loops",			"Loop name"},
	{"LoopLength",				1,		"Get loop length",			"Loops",			"Loop name"},
	{"max",						2,		"Get maximum value",		"Math",				"Value 1, Value 2"},
	{"min",						2,		"Get minimum value",		"Math",				"Value 1, Value 2"},
	{"TotalObjects",			0,		"Get object count",			"System",				""},
	{"global",					1,		"Get global variable",		"System",			"'Variable name'"},
	{"FormatDecimal",			2,		"Format decimal number",	"Text",				"Value, # of decimal places"},
	{"ZeroPad",					2,		"Pad number with zeroes",	"Text",				"Value, # of digits"},
	{"NewLine",					0,		"New line",					"Text",				""},
	{"left",					2,		"Get left substring",		"Text",				"String, Count"},
	{"right",					2,		"Get right substring",		"Text",				"String, Count"},
	{"mid",						3,		"Get middle substring",		"Text",				"String, Start, Count"},
	{"Uppercase",				1,		"Convert to uppercase",		"Text",				"String"},
	{"Lowercase",				1,		"Convert to lowercase",		"Text",				"String"},
	{"find",					2,		"Find string",				"Text",				"String, String to find"},
	{"remove",					2,		"Remove string",			"Text",				"String, String to remove"},
	{"replace",					2,		"Replace string",			"Text",				"String, Find, Replace"},
	{"trim",					1,		"Trim",						"Text",				"String"},
	{"reverse",					1,		"Reverse",					"Text",				"String"},
	{"NumTokens",				2,		"Get token count",			"Text",				"String, Separator"},
	{"GetToken",				3,		"Get token",				"Text",				"String, Index, Separator"},
	{"distance",				4,		"Distance between points",	"Math",				"x1, y1, x2, y2"},
	{"angle",					4,		"Angle between points",		"Math",				"x1, y1, x2, y2"},
	{"CountMatching",			1,		"Count objects matching event",	"System",		"Name or OID"},
	{"ScrollX",					0,		"Get scroll X",				"Display",			""},
	{"ScrollY",					0,		"Get scroll Y",				"Display",			""},
	{"DisplayWidth",			0,		"Get display width",		"Display",			""},
	{"DisplayHeight",			0,		"Get display height",		"Display",			""},
	{"PS_Version",				0,		"Get pixel shader version",	"Display",			""},
	{"RGB",						3,		"Get RGB value",			"Colours",			"Red, Green, Blue"},
	{"GetRed",					1,		"Get red value from RGB",	"Colours",			"RGB"},
	{"GetGreen",				1,		"Get green value from RGB",	"Colours",			"RGB"},
	{"GetBlue",					1,		"Get blue value from RGB",	"Colours",			"RGB"},
	{"TickCount",				0,		"Get tick count",			"Time",				""},
	{"Timer",					0,		"Get timer",				"Time",				""},
	{"sin",						1,		"Sin",						"Math",				"0"},
	{"cos",						1,		"Cos",						"Math",				"0"},
	{"tan",						1,		"Tan",						"Math",				"0"},
	{"sqrt",					1,		"Sqrt",						"Math",				"0"},
	{"str",						1,		"Convert value to string",	"Convert",			"Value"},
	{"int",						1,		"Convert value to integer",	"Convert",			"Value"},
	{"float",					1,		"Convert value to float",	"Convert",			"Value"},
	{"asin",					1,		"Asin",						"Math",				"0"},
	{"acos",					1,		"Acos",						"Math",				"0"},
	{"atan",					1,		"Atan",						"Math",				"0"},
	{"abs",						1,		"Absolute value",			"Math",				"0"},
	{"exp",						1,		"Exponent",					"Math",				"0"},
	{"log",						1,		"Log",						"Math",				"0"},
	{"log10",					1,		"Log10",					"Math",				"0"},
	{"floor",					1,		"Round down",				"Math",				"0"},
	{"ceil",					1,		"Round up",					"Math",				"0"},
	{"random",					1,		"Generate random number",	"Math",				"1.0"},
	{"len",						1,		"Get string length",		"Text",				"String"},
	{"round",					1,		"Round",					"Math",				"0"},
	{"LayerZoomX",				1,		"Get zoom X",				"Layers",			"Layer"},
	{"LayerZoomY",				1,		"Get zoom Y",				"Layers",			"Layer"},
	{"LayerScrollRateX",		1,		"Get scroll X rate",		"Layers",			"Layer"},
	{"LayerScrollRateY",		1,		"Get scroll Y rate",		"Layers",			"Layer"},
	{"LayerOpacity",			1,		"Get opacity",				"Layers",			"Layer"},
	{"LayerName",				1,		"Get name",					"Layers",			"Layer"},
	{"LayerCount",				0,		"Get layer count",			"Layers",			""},
	{"LoopIndex",				0,		"Get current loop index",	"Loops",			""},
	{"ZoomX",					0,		"Get zoom X",				"Display",			""},
	{"ZoomY",					0,		"Get zoom Y",				"Display",			""},
	{"TimeDelta",				0,		"Get time delta",			"Time",				""},
	{"ScrollXLeft",				0,		"Get X left of visible screen",		"Display",			""},
	{"ScrollXRight",			0,		"Get X right of visible screen",	"Display",			""},
	{"ScrollYTop",				0,		"Get Y top of visible screen",		"Display",			""},
	{"ScrollYBottom",			0,		"Get Y bottom of visible screen",	"Display",			""},
	{"DisplayAngle",			0,		"Get display angle",				"Display",			""},
	{"Clamp",					3,		"Clamp value",				"Math",				"Value, Lower bound, Upper bound"},
	{"AppPath",					0,		"Get application directory", "System",			""},
	{"LayoutWidth",				0,		"Get layout width",			"System",		""},
	{"LayoutHeight",			0,		"Get layout height",		"System",		""},
	{"Python",					1,      "Evaluate a python command",     "Python",       "Line"},
	{"lerp",					3,		"Linear interpolatation",	"Math",			"a, b, x"},
	{"VRAM",					0,		"Get remaining VRAM",		"Display",		""},
	{"DisplayDevice",			0,		"Display device name",		"Display",		""},
	{"TimeScale",				0,		"Get time scale",			"Time",			""},
	{"LayerAngle",				1,		"Get angle",				"Layers",		"Layer"},
	{"LinearAim",				7,		"Linear aim",				"Math",			"Bullet X, Bullet Y, Bullet speed, Target X, Target Y, Target Speed, Target Angle"},
	{"LayoutNumber",			0,		"Get layout number",		"System",		""},
	{"LayoutName",				0,		"Get layout name",			"System",		""},
	{"Sign",					0,		"Sign",						"Math",			"x"},
	{"Qarp",					0,		"Quadratic interpolation",	"Math",			"a, b, c, x"},
	{"Cosp",					3,		"Cosine interpolation",		"Math",			"a, b, x"},
	{"AngleDiff",				2,		"Get angle difference",		"Math",			"a, b"},
	{"RotateAngle",				3,		"Rotate angle",				"Math",			"start, end, step"},
	{"NormalRandom",			2,		"Normal distribution random", "Math",		"mean, sigma"},
	{"Cubic",					5,		"Cubic interpolation",		"Math",			"a, b, c, d, x"},
	{"Multisamples",			0,		"Get multisampling",		"Display",		""},
};

int GetSystemTableCount(int type)
{
	switch(type)
	{
	case ACTION:
		return LASTACTION;
	case CONDITION:
		return LASTCONDITION;
	case EXPRESSION:
		return sizeof(SystemExpressionsTable) / sizeof(SystemExpressionsTable[0]);
	default:
		return NULL;
	}
}

void GetSystemExpression(int ID, ACESEntry2*& Out)
{
	Out = &system_expressions[ID];
	Out->resourceID = ID + 1;		// Menu IDs off-by-one
	Out->params.clear();
	Out->aceDisplayText = "";
	Out->aceListName = "";
	Out->retrn = 0;

	SystemExpressionEntry& entry = SystemExpressionsTable[ID];
	Out->aceListName = entry.listName;
	Out->aceCategory = entry.category;
	Out->aceDisplayText = entry.expname;
	Out->scriptName = entry.expname;
	Out->auxStr = entry.parametersToInsert;
}

// ACEs for System Object
void SO_FlushClipboard(ACESEntry2& Out)
{
	Out.aceDisplayText =	SOBJ_FLUSHCLIPBOARD_D;
	Out.scriptName =		SOBJ_FLUSHCLIPBOARD_S;
	Out.aceListName = "Flush clipboard";
	Out.aceCategory = "System";
}

void SO_SetClipboardText(ACESEntry2& Out)
{
	PARAM2 param;
	param.desc = SOBJ_SETCLIPBOARDTEXT_P1_D;
	param.name = SOBJ_SETCLIPBOARDTEXT_P1_N;
	param.type = 2; 

	Out.aceDisplayText =	SOBJ_SETCLIPBOARDTEXT_D;
	Out.scriptName =		SOBJ_SETCLIPBOARDTEXT_S;
	Out.aceListName = "Set clipboard text";
	Out.aceCategory = "System";

	Out.params.push_back(param);
}

void SO_WriteINI(ACESEntry2& Out)
{
	PARAM2 param;
	param.desc = SOBJ_WRITEINI_P1_D;
	param.name = SOBJ_WRITEINI_P1_N;
	param.type = 2;

	PARAM2 param2;
	param2.desc = SOBJ_WRITEINI_P2_D;
	param2.name = SOBJ_WRITEINI_P2_N;
	param2.type = 2;

	PARAM2 param3;
	param3.desc = SOBJ_WRITEINI_P3_D;
	param3.name = SOBJ_WRITEINI_P3_N;
	param3.type = 2;

	PARAM2 param4;
	param4.desc = SOBJ_WRITEINI_P4_D;
	param4.name = SOBJ_WRITEINI_P4_N;
	param4.type = 2;

	Out.aceDisplayText =	SOBJ_WRITEINI_D;
	Out.scriptName =			SOBJ_WRITEINI_S;
	Out.aceListName = "Write to INI file";
	Out.aceCategory = "System";
	Out.params.push_back(param);
	Out.params.push_back(param2);
	Out.params.push_back(param3);
	Out.params.push_back(param4);
}

void SO_PlaySound(ACESEntry2& Out)
{
	PARAM2 param;
	param.desc = SOBJ_PLAYSOUND_P1_D;
	param.name = SOBJ_PLAYSOUND_P1_N;
	param.type = 2;

	PARAM2 param2;
	param2.desc = SOBJ_PLAYSOUND_P2_D;
	param2.name = SOBJ_PLAYSOUND_P2_N;
	param2.type = 2;

	Out.aceDisplayText =	SOBJ_PLAYSOUND_D;
	Out.scriptName =			SOBJ_PLAYSOUND_S;
	Out.aceListName = "Play sound (for apps)";
	Out.aceCategory = "System";
	Out.params.push_back(param);
	Out.params.push_back(param2);	
}

void SO_MessageBox(ACESEntry2& Out)
{
	PARAM2 param;
	param.desc = SOBJ_MESSAGEBOX_P2_D;
	param.name = SOBJ_MESSAGEBOX_P2_N;
	param.type = 2; 

	PARAM2 param2;
	param2.desc = SOBJ_MESSAGEBOX_P1_D;
	param2.name = SOBJ_MESSAGEBOX_P1_N;
	param2.type = 2;

	PARAM2 param3;
	param3.desc = SOBJ_MESSAGEBOX_P3_D;
	param3.name = SOBJ_MESSAGEBOX_P3_N;
	param3.initStr = SOBJ_MESSAGEBOX_P3_I;
	param3.type = 9; 

	PARAM2 param4;
	param4.desc = SOBJ_MESSAGEBOX_P4_D;
	param4.name = SOBJ_MESSAGEBOX_P4_N;
	param4.initStr = SOBJ_MESSAGEBOX_P4_I;
	param4.type = 9; 

	Out.aceDisplayText = SOBJ_MESSAGEBOX_D;
	Out.scriptName = SOBJ_MESSAGEBOX_S;
	Out.aceCategory = "System";
	Out.aceListName = "Message box";
	Out.params.push_back(param);
	Out.params.push_back(param2);	
	Out.params.push_back(param3);	
	Out.params.push_back(param4);	
}

void SO_ScrollX(ACESEntry2& Out)
{
	PARAM2 param;
	param.desc = SOBJ_SCROLLX_P1_D;
	param.name = SOBJ_SCROLLX_P1_N;
	param.type = 1;

	Out.aceDisplayText =	SOBJ_SCROLLX_D;
	Out.scriptName =			SOBJ_SCROLLX_S;
	Out.aceListName = "Scroll to X";
	Out.aceCategory = "Display";
	Out.params.push_back(param);
}

void SO_ScrollY(ACESEntry2& Out)
{
	PARAM2 param;
	param.desc = SOBJ_SCROLLY_P1_D;
	param.name = SOBJ_SCROLLY_P1_N;
	param.type = 1;

	Out.aceDisplayText =	SOBJ_SCROLLY_D;
	Out.scriptName =			SOBJ_SCROLLY_S;
	Out.aceListName = "Scroll to Y";
	Out.aceCategory = "Display";
	Out.params.push_back(param);
}

void SO_SetDisplayAngle(ACESEntry2& Out)
{
	PARAM2 param;
	param.desc = SOBJ_SETDISPLAYANGLE_P1_D;
	param.name = SOBJ_SETDISPLAYANGLE_P1_N;
	param.type = 1;

	Out.aceDisplayText =	SOBJ_SETDISPLAYANGLE_D;
	Out.scriptName =			SOBJ_SETDISPLAYANGLE_S;
	Out.aceListName = "Set display angle";
	Out.aceCategory = "Display";
	Out.params.push_back(param);
}

void SO_SetGlobalFilter(ACESEntry2& Out)
{
	PARAM2 param;
	param.desc = SOBJ_SETGLOBALFILTER_P1_D;
	param.name = SOBJ_SETGLOBALFILTER_P1_N;
	param.type = 3;

	Out.aceDisplayText =	SOBJ_SETGLOBALFILTER_D;
	Out.scriptName =			SOBJ_SETGLOBALFILTER_S;
	Out.aceListName = "Set global filter";
	Out.aceCategory = "Display";
	Out.params.push_back(param);
}

void SO_StartLoop(ACESEntry2& Out)
{
	PARAM2 param;
	param.desc = SOBJ_STARTLOOP_P1_D;
	param.name = SOBJ_STARTLOOP_P1_N;
	param.type = 2;

	PARAM2 param2;
	param2.desc = SOBJ_STARTLOOP_P2_D;
	param2.name = SOBJ_STARTLOOP_P2_N;
	param2.type = 1;

	Out.aceDisplayText =	SOBJ_STARTLOOP_D;
	Out.scriptName =			SOBJ_STARTLOOP_S;
	Out.aceListName = "Start loop";
	Out.aceCategory = "Loops";
	Out.params.push_back(param);
	Out.params.push_back(param2);
}

void SO_StopLoop(ACESEntry2& Out)
{
	PARAM2 param;
	param.desc = SOBJ_STARTLOOP_P1_D;
	param.name = SOBJ_STARTLOOP_P1_N;
	param.type = 2;

	Out.aceDisplayText =	SOBJ_STOPLOOP_D;
	Out.scriptName =			SOBJ_STOPLOOP_S;
	Out.aceListName = "Stop loop";
	Out.aceCategory = "Loops";
	Out.params.push_back(param);
}

void SO_Break(ACESEntry2& Out)
{
	Out.aceDisplayText =	SOBJ_BREAK_D;
	Out.scriptName =			SOBJ_BREAK_S;
	Out.aceListName = "Break";
	Out.aceCategory = "Loops";
}

void SO_CreateObject(ACESEntry2& Out)
{
	PARAM2 param;
	param.desc = SOBJ_CREATEOBJECT_P1_D;
	param.name = OBJECTS;
	param.type = 6;

	Out.aceDisplayText = "Create object %0 on layer %1 at (%2, %3)";
	Out.scriptName = "Create";	//"CreateObject";
	Out.aceListName = "*Create object";
	Out.aceCategory = "Create";
	Out.params.push_back(param);
	Out.retrn = SOL_MODIFIER;

	param.desc = "Layer to create on to";
	param.name = "Layer name or number";
	param.initStr = "1";
	param.type = 1;
	Out.params.push_back(param);

	param.desc = "X position to create at, in pixels";
	param.name = "X co-ordinate";
	param.initStr = "0";
	param.type = 1;
	Out.params.push_back(param);

	param.desc = "Y position to create at, in pixels";
	param.name = "Y co-ordinate";
	param.initStr = "0";
	param.type = 1;
	Out.params.push_back(param);
}

void SO_CreateRelativePP(ACESEntry2& Out)
{
	PARAM2 param;
	param.desc = SOBJ_CREATEOBJECT_P1_D;
	param.name = OBJECTS;
	param.type = 6;

	Out.aceDisplayText = "Create object %0 on layer %1 at (%2, %3) from %4's pivot point";
	Out.scriptName = "CreateRelObjPP"; //"CreateObjectAtObject";
	Out.aceListName = "Create object relative to object";
	Out.aceCategory = "Create";
	Out.params.push_back(param);
	Out.retrn = SOL_MODIFIER;

	param.desc = "Layer to create on to";
	param.name = "Layer name or number";
	param.initStr = "1";
	param.type = 1;
	Out.params.push_back(param);

	param.desc = "X position to adjust, in pixels";
	param.name = "X offset";
	param.initStr = "0";
	param.type = 1;
	Out.params.push_back(param);

	param.desc = "Y position to adjust, in pixels";
	param.name = "Y offset";
	param.initStr = "0";
	param.type = 1;
	Out.params.push_back(param);

	param.desc = "Select the object to create relative to.";
	param.name = "Object to position to";
	param.type = 6;
	Out.params.push_back(param);
}

void SO_CreateRelativeIP(ACESEntry2& Out)
{
	PARAM2 param;
	param.desc = SOBJ_CREATEOBJECT_P1_D;
	param.name = OBJECTS;
	param.type = 6;

	Out.aceDisplayText = "Create object %0 on layer %1 at (%2, %3) from %4's image point %5";
	Out.scriptName = "CreateRelObjIP";	//"CreateObjectIP";
	Out.aceListName = "Create object at image point";
	Out.aceCategory = "Create";
	Out.params.push_back(param);
	Out.retrn = SOL_MODIFIER;

	param.desc = "Layer to create on to";
	param.name = "Layer name or number";
	param.initStr = "1";
	param.type = 1;
	Out.params.push_back(param);

	param.desc = "X position to adjust, in pixels";
	param.name = "X offset";
	param.initStr = "0";
	param.type = 1;
	Out.params.push_back(param);

	param.desc = "Y position to adjust, in pixels";
	param.name = "Y offset";
	param.initStr = "0";
	param.type = 1;
	Out.params.push_back(param);

	param.desc = "Select the object to create relative to.";
	param.name = "Object to position to";
	param.type = 6;
	Out.params.push_back(param);

	param.desc = "Enter a string of the image point name, or its number, starting from 1.";
	param.name = "Image point name or number";
	param.initStr = "1";
	param.type = 10;		// PARAM_UNDEFINED
	Out.params.push_back(param);
}

void SO_CreateObjectByName(ACESEntry2& Out)
{
	PARAM2 param;
	param.desc = "Object name to create from";
	param.name = "Name";
	param.type = 2;

	Out.aceDisplayText = "Create object %0 on layer %1 at (%2, %3)";
	Out.scriptName = "CreateByName";	//"CreateObjectByName";
	Out.aceListName = "Create object by name";
	Out.aceCategory = "Create";
	Out.params.push_back(param);
	Out.retrn = SOL_MODIFIER;

	param.desc = "Layer to create on to";
	param.name = "Layer name or number";
	param.initStr = "1";
	param.type = 1;
	Out.params.push_back(param);

	param.desc = "X position to create at, in pixels";
	param.name = "X co-ordinate";
	param.initStr = "0";
	param.type = 1;
	Out.params.push_back(param);

	param.desc = "Y position to create at, in pixels";
	param.name = "Y co-ordinate";
	param.initStr = "0";
	param.type = 1;
	Out.params.push_back(param);
}

void SO_NextFrame(ACESEntry2& Out)
{
	PARAM2 param;
	param.desc = "Transition to use";
	param.name = "Transition";
	param.type = 17; // transition

	PARAM2 param2;
	param2.desc = "Duration (ms)";
	param2.name = "Duration of transition if picked, in milliseconds";
	param2.type = 1; // value

	Out.aceDisplayText = "Go to next layout with transition <i>%0</i> lasting %1 MS";
	Out.scriptName = "NextLayout";
	Out.aceListName = "Next layout";
	Out.aceCategory = "Application";
	Out.params.push_back(param);
	Out.params.push_back(param2);
}

void SO_PreviousFrame(ACESEntry2& Out)
{
	PARAM2 param;
	param.desc = "Transition to use";
	param.name = "Transition";
	param.type = 17; // trannie

	PARAM2 param2;
	param2.desc = "Duration (ms)";
	param2.name = "Duration of transition if picked, in milliseconds";
	param2.type = 1; // value

	Out.aceDisplayText = "Return to previous layout with transition <i>%0</i> lasting %1 MS";
	Out.scriptName = "PrevLayout";	//"PreviousLayout";
	Out.aceListName = "Previous layout";
	Out.aceCategory = "Application";
	Out.params.push_back(param);
	Out.params.push_back(param2);
}

void SO_GoToFrame(ACESEntry2& Out)
{
	PARAM2 param;
	param.desc = "Layout number";
	param.name = "Layout";
	param.type = 1;

	PARAM2 param2;
	param2.desc = "Transition to use";
	param2.name = "Transition";
	param2.type = 17; // trannie

	PARAM2 param3;
	param3.desc = "Duration (ms)";
	param3.name = "Duration of transition if picked, in milliseconds";
	param3.type = 1; // value

	Out.aceDisplayText = "Go to layout <i>%0</i> with transition <i>%1</i> lasting %2 MS";
	Out.scriptName = "GoToLayout";
	Out.aceListName = "Go to layout";
	Out.aceCategory = "Application";
	Out.params.push_back(param);
	Out.params.push_back(param2);
	Out.params.push_back(param3);
	Out.retrn = 0;
}

void SO_Quit(ACESEntry2& Out)
{
	Out.aceDisplayText = "Close application";
	Out.scriptName = "Close";
	Out.aceListName = "Close";
	Out.aceCategory = "Application";
}

void SO_CancelClose(ACESEntry2& Out)
{
	Out.aceDisplayText = "Cancel attempted close";
	Out.scriptName = "CancelClose";
	Out.aceListName = "Cancel close";
	Out.aceCategory = "Application";
}

void SO_SetFPS(ACESEntry2& Out)
{
	PARAM2 param;
	param.desc = "New FPS rate";
	param.name = "SetFPS";
	param.type = EDITORPARAM_INTEGER;

	Out.aceDisplayText = "Set FPS to <i>%0</i>";
	Out.scriptName = "SetFPS";
	Out.aceListName = "Set fixed FPS rate";
	Out.aceCategory = "Application";
	Out.params.push_back(param);
	Out.retrn = 0;
}

void SO_SetGlobal(ACESEntry2& Out)
{
	PARAM2 param;
	param.desc = "Global variable to set";
	param.name = "Variable";
	param.type = EDITORPARAM_GLOBALVARIABLE;

	PARAM2 param2;
	param2.desc = "New value of variable";
	param2.name = "Value";
	param2.type = EDITORPARAM_INTEGER;

	Out.aceDisplayText = "Set global variable %0 to <i>%1</i>";
	Out.scriptName = "SetGlobalVar";	//"SetGlobalVariable";
	Out.aceListName = "*Set value";
	Out.aceCategory = "Global variables";
	Out.params.push_back(param);
	Out.params.push_back(param2);
	Out.retrn = 0;
}

void SO_AddGlobal(ACESEntry2& Out)
{
	PARAM2 param;
	param.desc = "Global variable to set";
	param.name = "Variable";
	param.type = EDITORPARAM_GLOBALVARIABLE;

	PARAM2 param2;
	param2.desc = "Value to add";
	param2.name = "Value";
	param2.type = EDITORPARAM_INTEGER;

	Out.aceDisplayText = "Add <i>%1</i> to global variable %0";
	Out.scriptName = "AddGlobalVar";	//"AddToGlobalVariable";
	Out.aceListName = "Add to value";
	Out.aceCategory = "Global variables";
	Out.params.push_back(param);
	Out.params.push_back(param2);
	Out.retrn = 0;
}

void SO_SubGlobal(ACESEntry2& Out)
{
	PARAM2 param;
	param.desc = "Global variable to set";
	param.name = "Variable";
	param.type = EDITORPARAM_GLOBALVARIABLE;

	PARAM2 param2;
	param2.desc = "Value to subtract";
	param2.name = "Value";
	param2.type = EDITORPARAM_INTEGER;

	Out.aceDisplayText = "Subtract <i>%1</i> from global variable %0";
	Out.scriptName = "SubGlobalVar";	//"SubtractFromGlobalVariable";
	Out.aceListName = "Subtract from value";
	Out.aceCategory = "Global variables";
	Out.params.push_back(param);
	Out.params.push_back(param2);
	Out.retrn = 0;
}

void SO_EnableGroup(ACESEntry2& Out)
{
	PARAM2 param;
	param.desc = "";
	param.name = "Enable";
	param.initStr = "Disable|Enable";
	param.desc = "Enable or disable group";
	param.type = 9;

	Out.aceDisplayText = "<b>%0</b> group <i>%1</i>";
	Out.scriptName = "EnableGroup";
	Out.aceListName = "Set group enabled";
	Out.aceCategory = "Event groups";
	Out.params.push_back(param);
	Out.retrn = 0;

	param.desc = "Group name to toggle";
	param.name = "Group";
	param.initStr = "";
	param.type = 2;
	Out.params.push_back(param);
}

void SO_ScrollToObject(ACESEntry2& Out)
{
	PARAM2 param;
	param.desc = "Object to scroll to";
	param.name = OBJECTS;
	param.type = 6;

	Out.aceDisplayText = "Scroll to object %0";
	Out.scriptName = "ScrollToObj";	//"ScrollToObject";
	Out.aceListName = "Scroll to object";
	Out.aceCategory = "Display";
	Out.params.push_back(param);
	Out.retrn = 0;
}

void SO_Serialize(ACESEntry2& Out)
{
	PARAM2 param;
	param.desc = "";
	param.name = "Operation";
	param.initStr = "Load|Save";
	param.desc = "Save or load";
	param.type = 9;

	Out.aceDisplayText = "<b>%0</b> file <i>%1</i>";
	Out.scriptName = "Serialize";
	Out.aceListName = "Save/load to disk";
	Out.aceCategory = "Application";
	Out.params.push_back(param);
	Out.retrn = 0;

	param.desc = "File name to serialize";
	param.name = "File";
	param.initStr = "";
	param.type = 2;
	Out.params.push_back(param);
}


void SO_SetLayerXScrollRatio(ACESEntry2& Out)
{
	Out.aceDisplayText = "Set Layer %0 Scroll X Ratio to %1";
	Out.scriptName = "SetLayerScrollXRatio";
	Out.aceListName = "Set scroll X ratio";
	Out.aceCategory = "Layers";
	Out.retrn = 0;

	PARAM2 param;
	param.desc = "Layer that is affected";
	param.name = "Layer name or number";
	param.initStr = "1";
	param.type = 1;
	Out.params.push_back(param);

	param.desc = "100 scrolls at standard speed, 50 half speed, etc.";
	param.name = "Scroll X Ratio";
	param.initStr = "100";
	param.type = 1;
	Out.params.push_back(param);
}
void SO_SetLayerYScrollRatio(ACESEntry2& Out)
{
	Out.aceDisplayText = "Set Layer %0 Scroll Y Ratio to %1";
	Out.scriptName = "SetLayerScrollYRatio";
	Out.aceListName = "Set scroll Y ratio";
	Out.aceCategory = "Layers";
	Out.retrn = 0;

	PARAM2 param;
	param.desc = "Layer that is affected";
	param.name = "Layer name or number";
	param.initStr = "1";
	param.type = 1;
	Out.params.push_back(param);

	param.desc = "100 scrolls at standard speed, 50 half speed, etc.";
	param.name = "Scroll Y Ratio";
	param.initStr = "100";
	param.type = 1;
	Out.params.push_back(param);
}
void SO_SetLayerZoomRate(ACESEntry2& Out)
{
	Out.aceDisplayText = "Set Layer %0 zoom rate to (%1, %2)";
	Out.scriptName = "SetLayerZoomRatio";
	Out.aceListName = "Set zoom";
	Out.aceCategory = "Layers";
	Out.retrn = 0;

	PARAM2 param;
	param.desc = "Layer that is affected";
	param.name = "Layer name or number";
	param.initStr = "1";
	param.type = 1;
	Out.params.push_back(param);

	param.desc = "100 is original display, 50 is zoomed out to half size, 200 is zoomed in to double size, etc.";
	param.name = "X Zoom Ratio";
	param.initStr = "100";
	param.type = 1;
	Out.params.push_back(param);

	param.desc = "100 is original display, 50 is zoomed out to half size, 200 is zoomed in to double size, etc.";
	param.name = "Y Zoom Ratio";
	param.initStr = "100";
	param.type = 1;
	Out.params.push_back(param);

}
void SO_SetLayerZoomOffset(ACESEntry2& Out)
{
	Out.aceDisplayText = "Set Layer %0 zoom offset to (%1, %2)";
	Out.scriptName = "SetLayerZoomOffset";
	Out.aceListName = "Set zoom offset";
	Out.aceCategory = "Layers";
	Out.retrn = 0;

	PARAM2 param;
	param.desc = "Layer that is affected";
	param.name = "Layer name or number";
	param.initStr = "1";
	param.type = 1;
	Out.params.push_back(param);

	param.desc = "The offset is added onto the current X zoom amount for this layer";
	param.name = "X Ratio";
	param.initStr = "0.0";
	param.type = 1;
	Out.params.push_back(param);

	param.desc = "The offset is added onto the current Y zoom amount for this layer";
	param.name = "Y Ratio";
	param.initStr = "0.0";
	param.type = 1;
	Out.params.push_back(param);
}
void SO_SetLayerXScrollOffset(ACESEntry2& Out)
{
}
void SO_SetLayerYScrollOffset(ACESEntry2& Out)
{
}

void SO_SetLayerVisible(ACESEntry2& Out)
{
	Out.aceDisplayText = "Set Layer %0 visible: %1";
	Out.scriptName = "SetLayerVisible";
	Out.aceListName = "Set visible";
	Out.aceCategory = "Layers";
	Out.retrn = 0;

	PARAM2 param;
	param.desc = "Layer that is affected";
	param.name = "Layer name or number";
	param.initStr = "1";
	param.type = 1;
	Out.params.push_back(param);

	param.desc = "Choose whether to hide or show the layer.";
	param.name = "Visibility";
	param.initStr = "Invisible|Visible";
	param.type = 9;
	Out.params.push_back(param);

}
void SO_SetLayerOpacity(ACESEntry2& Out)
{
	Out.aceDisplayText = "Set Layer %0 opacity to %1";
	Out.scriptName = "SetLayerOpacity";
	Out.aceListName = "Set opacity";
	Out.aceCategory = "Layers";
	Out.retrn = 0;

	PARAM2 param;
	param.desc = "Layer that is affected";
	param.name = "Layer name or number";
	param.initStr = "1";
	param.type = 1;
	Out.params.push_back(param);

	param.desc = "Layer opacity where 100 is opaque and 0 is transparent.";
	param.name = "Opacity";
	param.initStr = "100";
	param.type = 1;
	Out.params.push_back(param);
}
void SO_SetLayerFilter(ACESEntry2& Out)
{
	Out.aceDisplayText = "Set layer %0 filter to %1";
	Out.scriptName = "SetLayerFilter";	//"Set filter";
	Out.aceListName = "Set colour filter";
	Out.aceCategory = "Layers";
	Out.retrn = 0;

	PARAM2 param;
	param.desc = "Layer that is affected";
	param.name = "Layer name or number";
	param.initStr = "1";
	param.type = 1;
	Out.params.push_back(param);

	param.desc = "Choose the layer filter colour.";
	param.name = "Filter";
	param.initStr = "";
	param.type = 3;
	Out.params.push_back(param);
}
void SO_SetZoom(ACESEntry2& Out)
{
	Out.aceDisplayText = "Set Zoom to (%0, %1)";
	Out.scriptName = "SetZoom";
	Out.aceListName = "Set zoom";
	Out.aceCategory = "Display";
	Out.retrn = 0;

	PARAM2 param;
	param.desc = "100 - normal zoom, 200 - everything appears twice the size, 50 - everything is half the size";
	param.name = "X Zoom";
	param.initStr = "100";
	param.type = 1;
	Out.params.push_back(param);

	param.desc = "100 - normal zoom, 200 - everything appears twice the size, 50 - everything is half the size";
	param.name = "Y Zoom";
	param.initStr = "100";
	param.type = 1;
	Out.params.push_back(param);
}

void SO_SetLayoutSize(ACESEntry2& Out)
{
	Out.aceDisplayText = "Set layout size to %0 x %1";
	Out.scriptName = "SetLayoutSize";
	Out.aceListName = "Set layout size";
	Out.aceCategory = "Display";
	Out.retrn = 0;

	PARAM2 param;
	param.desc = "The width of the layout, in pixels.";
	param.name = "Width";
	param.initStr = "1000";
	param.type = 1;
	Out.params.push_back(param);

	param.desc = "The height of the layout, in pixels.";
	param.name = "Height";
	param.initStr = "1000";
	param.type = 1;
	Out.params.push_back(param);
}

void SO_SetResolution(ACESEntry2& Out)
{
	Out.aceDisplayText = "Change display size to %0 x %1";
	Out.scriptName = "SetResolution";
	Out.aceListName = "Set display resolution";
	Out.aceCategory = "Display";
	Out.retrn = 0;

	PARAM2 param;
	param.desc = "Horizontal resolution, eg. 1024";
	param.name = "Width";
	param.initStr = "800";
	param.type = 1;
	Out.params.push_back(param);

	param.desc = "Vertical resolution, eg. 768.";
	param.name = "Height";
	param.initStr = "600";
	param.type = 1;
	Out.params.push_back(param);
}

void SO_Quicksave(ACESEntry2& Out)
{
	Out.aceDisplayText = "%0";
	Out.scriptName = "Quicksave";
	Out.aceListName = "Quicksave/Quickload";
	Out.aceCategory = "Application";
	Out.retrn = 0;

	PARAM2 param;
	param.desc = "Choose whether to save or load.";
	param.name = "Save / Load";
	param.initStr = "Quicksave|Quickload";
	param.type = 9;
	Out.params.push_back(param);
}

void SO_EndModalFrame(ACESEntry2& Out)
{
	Out.aceDisplayText = "End modal layout";
	Out.scriptName = "EndModalLayout";
	Out.aceListName = "End modal layout";
	Out.aceCategory = "Application";
	Out.retrn = 0;
}

void SO_RunScript(ACESEntry2& Out)
{
	Out.aceDisplayText = "Run Script (%0)";
	Out.scriptName = "RunScript";
	Out.aceListName = "Run Script";
	Out.aceCategory = "Script";
	Out.retrn = 0;

	PARAM2 param;
	param.desc = "For a proper python editor, right click in the event tree editor, and select 'add script'";
	param.name = "Python Script";
	param.initStr = "";
	param.type = 2;
	Out.params.push_back(param);
}

void SO_SetMotionBlur(ACESEntry2& Out)
{
	Out.aceDisplayText = "Set Motion Blur %0";
	Out.scriptName = "SetMotionBlur";
	Out.aceListName = "Set Motion Blur";
	Out.aceCategory = "Display";
	Out.retrn = 0;

	PARAM2 param;
	param.desc = "Set the motion blur setting.";
	param.name = "Set motion blur";
	param.initStr = "Off|2x (lowest quality)|3x|4x|5x (recommended)|6x|7x|8x|9x|10x|15x|20x (highest quality)";
	param.type = 9;

	Out.params.push_back(param);
}

void SO_SetFullscreen(ACESEntry2& Out)
{
	Out.aceDisplayText = "Switch to %0 mode";
	Out.scriptName = "SetFullscreen";
	Out.aceListName = "Set fullscreen";
	Out.aceCategory = "Display";
	Out.retrn = 0;

	PARAM2 param;
	param.desc = "Choose windowed or fullscreen mode.";
	param.name = "Display mode";
	param.initStr = "Windowed|Fullscreen";
	param.type = 9;

	Out.params.push_back(param);
}

///////////////
//
//    Conditions
//
//////////////

void SO_OnLoop(ACESEntry2& Out)
{
	PARAM2 param;
	param.desc = "Loop name";
	param.name = "Name";
	param.type = 2;

	Out.aceDisplayText = "<img>0</img> On loop <i>%0</i>";
	Out.aceListName = "On loop";
	Out.aceCategory = "Loops";
	Out.params.push_back(param);
	Out.retrn = CND_TRIGGERED;
}

void SO_While(ACESEntry2& Out)
{
	Out.aceDisplayText = "<img>3</img> While";
	Out.aceListName = "While";
	Out.aceCategory = "Loops";
	Out.retrn = 0;
}

void SO_For(ACESEntry2& Out)
{	
	PARAM2 param;
	param.desc = "Loop name";
	param.name = "Name";
	param.type = 2;

	PARAM2 param2;
	param2.desc = "Beginning integer";
	param2.name = "Start";
	param2.type = 1;

	PARAM2 param3;
	param3.desc = "Ending integer";
	param3.name = "End";
	param3.type = 1;

	Out.aceDisplayText = "<img>3</img> For <i>%0</i> from <b>%1</b> to <b>%2</b>";
	Out.aceListName = "For";
	Out.aceCategory = "Loops";
	Out.params.push_back(param);
	Out.params.push_back(param2);
	Out.params.push_back(param3);
	Out.retrn = 0;
}

void SO_Repeat(ACESEntry2& Out)
{	
	PARAM2 param;
	param.desc = "Number of loops";
	param.name = "Count";
	param.type = 1;

	Out.aceDisplayText = "<img>3</img> Repeat <b>%0</b> times";
	Out.aceListName = "Repeat";
	Out.aceCategory = "Loops";
	Out.params.push_back(param);
	Out.retrn = 0;
}

void SO_ForEach(ACESEntry2& Out)
{	
	PARAM2 param;
	param.desc = "Object type to loop through";
	param.name = "Object";
	param.type = 6;

	Out.aceDisplayText = "<img>3</img> For each %0";
	Out.aceListName = "For Each Object";
	Out.aceCategory = "Loops";
	Out.params.push_back(param);
	Out.retrn = 0;
}

void SO_ForEachOrdered(ACESEntry2& Out)
{	
	PARAM2 param;
	param.desc = "Object type to loop through";
	param.name = "Object";
	param.type = 6;
	Out.params.push_back(param);

	param.desc = "eg. Type in Object.X if you want to order every object by its X position";
	param.name = "Order Expression";
	param.type = 1;
	Out.params.push_back(param);

	param.desc = "Ascending means starts at the lowest value and goes up to the highest";
	param.name = "Mode";
	param.initStr = "Ascending|Descending";
	param.type = 9;
	Out.params.push_back(param);

	Out.aceDisplayText = "<img>3</img> For each %0 ordered by %1 %2";
	Out.aceListName = "For Each Object (ordered)";
	Out.aceCategory = "Loops";
	
	Out.retrn = 0;
}


void SO_CompareGlobalVariable(ACESEntry2& Out)
{	
	PARAM2 param;
	param.desc = "Global variable name";
	param.name = "Variable";
	param.type = EDITORPARAM_GLOBALVARIABLE;

	PARAM2 param2;
	param2.initStr = "Equal to|Different to|Less than|Less or equal|Greater than|Greater or equal";
	param2.name = "Comparison";
	param2.desc = "Comparison to make";
	param2.type = 9;

	PARAM2 param3;
	param3.desc = "Value to compare to";
	param3.name = "Compare to";
	param3.type = 1;

	Out.aceDisplayText = "<img>0</img> Is global variable <i>%0</i> %1 <i>%2</i>";
	Out.aceListName = "Compare global variable";
	Out.aceCategory = "System";
	Out.params.push_back(param);
	Out.params.push_back(param2);
	Out.params.push_back(param3);
	Out.retrn = 0;
}

void SO_EveryXMilliseconds(ACESEntry2& Out)
{	
	PARAM2 param;
	param.desc = "Millisecond wait between triggers.  N.b. there are 1000 milliseconds in 1 second.  This is only accurate to a resolution of ~10 milliseconds.";
	param.name = "Milliseconds";
	param.initStr = "1000";
	param.type = 1;

	Out.aceDisplayText = "Every <b>%0</b> milliseconds";
	Out.aceListName = "Every X Milliseconds";
	Out.aceCategory = "Time";
	Out.params.push_back(param);
	Out.retrn = 0;
}

void SO_EveryXTicks(ACESEntry2& Out)
{	
	PARAM2 param;
	param.desc = "Ticks wait between trigger";
	param.name = "Ticks";
	param.type = 1;

	Out.aceDisplayText = "Every <b>%0</b> ticks";
	Out.aceListName = "Every X Ticks";
	Out.aceCategory = "Time";
	Out.params.push_back(param);
	Out.retrn = 0;
}

void SO_CompareTime(ACESEntry2& Out)
{	
	PARAM2 param;
	param.initStr = "Equal to|Different to|Lower than|Lower or equal|Greater than|Greater or equal";
	param.name = "Comparison";
	param.desc = "Comparison to make";
	param.type = 9;

	PARAM2 param2;
	param2.desc = "Milliseconds to compare with";
	param2.name = "Milliseconds";
	param2.type = 1;

	Out.aceDisplayText = "Timer is <i>%0</i> <b>%1</b>";
	Out.aceListName = "Compare time";
	Out.aceCategory = "Time";
	Out.params.push_back(param);
	Out.params.push_back(param2);
	Out.retrn = 0;
}