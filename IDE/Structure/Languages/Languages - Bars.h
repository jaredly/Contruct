#pragma once

// bars
#define BAR_PROPERTIES					GetString("bars", "properties")
#define BAR_PROJECT						GetString("bars", "project")
#define BAR_LAYERS						GetString("bars", "layers")
#define BAR_RESOURCES					GetString("bars", "resources")
#define BAR_ANIMATOR					GetString("bars", "animator")
#define BAR_OBJECTS						GetString("bars", "objects")

// animator bar
#define AB_ANIMATION					GetString("animator-bar", "animation")
#define AB_ANIMATIONPROPERTIES			GetString("animator-bar", "animation-properties")

#define AB_ANIMATIONNAME				GetString("animator-bar", "animation-name")
#define AB_ANIMATIONNAMEDESC			GetString("animator-bar", "animation-name-desc")
#define AB_ANIMATIONTAG					GetString("animator-bar", "animation-tag")
#define AB_ANIMATIONTAGDESC				GetString("animator-bar", "animation-tag-desc")
	
#define AB_STOPPED						GetString("animator-bar", "stopped")
#define AB_WALKING						GetString("animator-bar", "walking")
#define AB_JUMPING						GetString("animator-bar", "jumping")
#define AB_FALLING						GetString("animator-bar", "falling")

#define AB_ANIMATIONSPEED				GetString("animator-bar", "animation-speed")
#define AB_ANIMATIONSPEEDDESC			GetString("animator-bar", "animation-speed-desc")
#define AB_ANIMATIONLOOP				GetString("animator-bar", "animation-loop")
#define AB_ANIMATIONLOOPDESC			GetString("animator-bar", "animation-loop-desc")
#define AB_ANIMATIONREPEAT				GetString("animator-bar", "animation-repeat")
#define AB_ANIMATIONREPEATDESC			GetString("animator-bar", "animation-repeat-desc")
#define AB_ANIMATIONREPEATTO			GetString("animator-bar", "animation-repeat-to")
#define AB_ANIMATIONREPEATTODESC		GetString("animator-bar", "animation-repeat-to-desc")
#define AB_ANIMATIONPINGPONG			GetString("animator-bar", "animation-pingpong")
#define AB_ANIMATIONPINGPONGDESC		GetString("animator-bar", "animation-pingpong-desc")

#define AB_FRAME						GetString("animator-bar", "frame")
#define AB_FRAMEDESC					GetString("animator-bar", "frame-desc")
#define AB_FRAMESPEED					GetString("animator-bar", "frame-speed")
#define AB_FRAMESPEEDDESC				GetString("animator-bar", "frame-speed-desc")

#define AB_DEFAULT						GetString("animator-bar", "default")
#define AB_ANGLE						GetString("animator-bar", "angle")
#define AB_RIGHT						GetString("animator-bar", "right")
#define AB_BOTTOM						GetString("animator-bar", "bottom")
#define AB_LEFT							GetString("animator-bar", "left")
#define AB_TOP							GetString("animator-bar", "top")
#define AB_ADDNEWANIMATION				GetString("animator-bar", "add-new-animation")
#define AB_ADDSUBANIMATION				GetString("animator-bar", "add-sub-animation")
#define AB_ADDNEWANGLE					GetString("animator-bar", "add-new-angle")
#define AB_ADDFRAME						GetString("animator-bar", "add-frame")
#define AB_REMOVEFRAME					GetString("animator-bar", "remove-frames")
#define AB_IMPORTFRAME					GetString("animator-bar", "import-frames")
#define AB_COPYFRAMES					GetString("animator-bar", "copy-frames")
#define AB_CUTFRAMES					GetString("animator-bar", "cut-frames")
#define AB_PASTEFRAMES					GetString("animator-bar", "paste-frames")
#define AB_DUPLICATEFRAMES				GetString("animator-bar", "duplicate-frames")
#define AB_LAUNCHEXPLORER				GetString("animator-bar", "launch-explorer")
#define AB_COPYCOLLISIONMASKTOEACHFRAME GetString("animator-bar", "copy-collision-mask-to-each-frame")
#define AB_ADDFROMGIF					GetString("animator-bar", "add-from-gif")
#define AB_MIRRORFRAMES					GetString("animator-bar", "mirror-frames")
#define AB_FLIPFRAMES					GetString("animator-bar", "flip-frames")

#define AB_CANNOTDELETE					GetString("animator-bar", "cannot-remove-frame")
#define AB_CANNOTDELETEDESC				GetString("animator-bar", "cannot-remove-frame-desc")
#define AB_CANNOTDELETEANIMATION		GetString("animator-bar", "cannot-remove-animation")
#define AB_CANNOTDELETEANIMATIONDESC	GetString("animator-bar", "cannot-remove-animation-desc")
#define AB_CANNOTDELETEANGLE			GetString("animator-bar", "cannot-remove-angle")
#define AB_CANNOTDELETEANGLEDESC		GetString("animator-bar", "cannot-remove-angle-desc")

// resource bar
#define RBT_BINARY						GetString("resource-bar", "binary")
#define RBT_CURSOR						GetString("resource-bar", "cursor")
#define RBT_ICON						GetString("resource-bar", "icon")
#define RBT_MENU						GetString("resource-bar", "menu")
#define RBT_FONT						GetString("resource-bar", "font")

#define RB_ADDFILE						GetString("resource-bar", "add-file")
#define RB_REMOVEFILE					GetString("resource-bar", "remove-file")

#define RB_DELETEICON					GetString("resource-bar", "delete-icon")
#define RB_EDITICON						GetString("resource-bar", "edit-icon")

#define RB_ADDMENU						GetString("resource-bar", "add-menu")
#define RB_DELETEMENU					GetString("resource-bar", "delete-menu")
#define RB_EDITMENU						GetString("resource-bar", "edit-menu")

// project bar
#define PB_ADDLAYOUT					GetString("project-bar", "add-layout")
#define PB_MANAGEFAMILIES				GetString("project-bar", "manage-families")
#define PB_MANAGEEVENTSHEETS			GetString("project-bar", "manage-event-sheets")
#define PB_MANAGEGLOBALVARIABLES		GetString("project-bar", "manage-global-variables")
#define PB_SHOWPLUGINS					GetString("project-bar", "show-plugins")
#define PB_CLOSEPROJECT					GetString("project-bar", "close-project")

#define PB_ALREADYEVENTSHEETNAMED		GetString("project-bar", "already-event-sheet-named-this")