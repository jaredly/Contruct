#pragma once

// Table size
int GetSystemTableCount(int Table);

static map<int, ACESEntry2> system_conditions;
static map<int, ACESEntry2> system_actions;
static map<int, ACESEntry2> system_expressions;


// Actions
void GetSystemAction(int ID, ACESEntry2*& Out);

void SO_FlushClipboard		(ACESEntry2& Out);
void SO_SetClipboardText	(ACESEntry2& Out);
void SO_WriteINI			(ACESEntry2& Out);
void SO_PlaySound			(ACESEntry2& Out);
void SO_MessageBox			(ACESEntry2& Out);
void SO_ScrollX				(ACESEntry2& Out);
void SO_ScrollY				(ACESEntry2& Out);
void SO_SetDisplayAngle		(ACESEntry2& Out);
void SO_SetGlobalFilter		(ACESEntry2& Out);
void SO_StartLoop			(ACESEntry2& Out);
void SO_StopLoop			(ACESEntry2& Out);
void SO_Break				(ACESEntry2& Out);
void SO_CreateObject		(ACESEntry2& Out);
void SO_CreateObjectByName	(ACESEntry2& Out);
void SO_NextFrame			(ACESEntry2& Out);
void SO_PreviousFrame		(ACESEntry2& Out);
void SO_GoToFrame			(ACESEntry2& Out);
void SO_Quit				(ACESEntry2& Out);
void SO_CancelClose			(ACESEntry2& Out);
void SO_SetFPS				(ACESEntry2& Out);
void SO_SetGlobal			(ACESEntry2& Out);
void SO_AddGlobal			(ACESEntry2& Out);
void SO_SubGlobal			(ACESEntry2& Out);
void SO_EnableGroup			(ACESEntry2& Out);
void SO_ScrollToObject		(ACESEntry2& Out);
void SO_Serialize			(ACESEntry2& Out);
void SO_EndModalFrame		(ACESEntry2& Out);
void SO_CreateRelativePP	(ACESEntry2& Out);
void SO_CreateRelativeIP	(ACESEntry2& Out);

void SO_SetLayerXScrollRatio	(ACESEntry2& Out);
void SO_SetLayerYScrollRatio	(ACESEntry2& Out);
void SO_SetLayerZoomRatio		(ACESEntry2& Out);
void SO_SetLayerZoomOffset		(ACESEntry2& Out);
void SO_SetLayerXScrollOffset	(ACESEntry2& Out);
void SO_SetLayerYScrollOffset	(ACESEntry2& Out);
void SO_SetLayerZoomOffset		(ACESEntry2& Out);
void SO_SetLayerVisible			(ACESEntry2& Out);
void SO_SetLayerOpacity			(ACESEntry2& Out);
void SO_SetLayerFilter			(ACESEntry2& Out);
void SO_SetZoom					(ACESEntry2& Out);
void SO_SetLayoutSize			(ACESEntry2& Out);
void SO_SetResolution			(ACESEntry2& Out);
void SO_Quicksave				(ACESEntry2& Out);
void SO_RunScript				(ACESEntry2& Out);
void SO_SetMotionBlur			(ACESEntry2& Out);
void SO_SetFullscreen			(ACESEntry2& Out);


// Conditions
void GetSystemCondition(int ID, ACESEntry2*& Out);

void SO_OnLoop					(ACESEntry2& Out);
void SO_While					(ACESEntry2& Out);
void SO_For						(ACESEntry2& Out);
void SO_Repeat					(ACESEntry2& Out);
void SO_ForEach					(ACESEntry2& Out);
void SO_ForEachOrdered          (ACESEntry2& Out);
void SO_CompareGlobalVariable	(ACESEntry2& Out);
void SO_EveryXMilliseconds		(ACESEntry2& Out);
void SO_EveryXTicks				(ACESEntry2& Out);
void SO_CompareTime				(ACESEntry2& Out);

// Expresions
void GetSystemExpression(int ID, ACESEntry2*& Out);