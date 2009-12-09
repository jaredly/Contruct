#ifndef LANGUAGES_H
#define LANGUAGES_H

// Functions
CString GetStrFromFile(LPCSTR file, LPCSTR ID);
void    ConvertMenu(CMenu& Menu);

bool    LoadLanguage(LPCSTR Language);
CString GetString(LPCSTR Group, LPCSTR ID);

// Include macro files
#include "Languages - Dialogs.h"
#include "Languages - Editors.h"
#include "Languages - Ribbon.h"
#include "Languages - Bars.h"
#include "Languages - Layer Properties.h"
#include "Languages - Layout Properties.h"
#include "Languages - Application Properties.h"
#include "Languages - Properties.h"


// Confirmations
#define CONF_DELETE						GetString("Confirmations", "1")
#define CONF_CLOSE						GetString("Confirmations", "2")

#define CONF_REMOVEEVENTSHEET			GetString("Confirmations", "3")
#define CONF_REMOVEEVENTSHEETTITLE		GetString("Confirmations", "4")

// General
#define ADD								GetString("general", "add")
#define BACK							GetString("general", "back")
#define CANCEL							GetString("general", "cancel")
#define CLOSE							GetString("general", "close")
#define INSERT							GetString("general", "insert")
#define NEXT							GetString("general", "next")
#define NONE							GetString("general", "none")
#define REMOVE							GetString("general", "remove")
#define RESIZE							GetString("general", "resize")

#define CONFIRMATION					GetString("general", "confirmation")
#define PROPERTIES						GetString("general", "properties")
#define SYSTEM							GetString("general", "system")

#define APPLICATION						GetString("general", "application")
#define LAYOUT							GetString("general", "layout")
#define LAYER							GetString("general", "layer")
#define RESOURCE						GetString("general", "resource")
#define OBJECTS							GetString("general", "objects")

// Object type macros
#define OT_ALL							GetString("ObjectCategories", "1")
#define OT_AUDIO						GetString("ObjectCategories", "2")
#define OT_CONTROLS						GetString("ObjectCategories", "3")
#define OT_DATAFILES					GetString("ObjectCategories", "4")
#define OT_GAME							GetString("ObjectCategories", "5")
#define OT_GRAPHICS						GetString("ObjectCategories", "6")
#define OT_INPUT						GetString("ObjectCategories", "7")
#define OT_INTERNET						GetString("ObjectCategories", "8")
#define OT_OTHER						GetString("ObjectCategories", "9")
#define OT_SYSTEM						GetString("ObjectCategories", "10")
#define OT_COMMON						GetString("ObjectCategories", "11")
#define OT_PROFESSIONAL					GetString("ObjectCategories", "12")

// Attributes
#define ATTRIBS_SOLID						GetString("Attributes", "1")
#define ATTRIBS_DESC_SOLID					GetString("Attributes", "2")
#define ATTRIBS_LADDER						GetString("Attributes", "3")
#define ATTRIBS_DESC_LADDER					GetString("Attributes", "4")
#define ATTRIBS_PLATFORM					GetString("Attributes", "5")
#define ATTRIBS_DESC_PLATFORM				GetString("Attributes", "6")
#define ATTRIBS_HIFRIC						GetString("Attributes", "7")
#define ATTRIBS_DESC_HIFRIC					GetString("Attributes", "8")
#define ATTRIBS_LOFRIC						GetString("Attributes", "9")
#define ATTRIBS_DESC_LOFRIC					GetString("Attributes", "10")
#define ATTRIBS_CENTERVIEW					GetString("Attributes", "11")
#define ATTRIBS_DESC_CENTER					GetString("Attributes", "12")
#define ATTRIBS_DESTROYSTART				GetString("Attributes", "13")
#define ATTRIBS_DESC_DESTROYSTART			GetString("Attributes", "14")
#define ATTRIBS_DESC_USER					GetString("Attributes", "15")
#define ATTRIBS_DESTROYIFFAR				GetString("Attributes", "16")
#define ATTRIBS_DESC_DESTROYIFFAR			GetString("Attributes", "17")
#define ATTRIBS_DESTROYIFNOTONSCREEN		GetString("Attributes", "18")
#define ATTRIBS_DESC_DESTROYIFNOTONSCREEN	GetString("Attributes", "19")
#define ATTRIBS_NOSERIALIZE					GetString("Attributes", "20")
#define ATTRIBS_DESC_NOSERIALIZE			GetString("Attributes", "21")

// tab macros
#define TAB_LAYOUTEDITOR					GetString("tabs", "layout-editor")
#define TAB_EVENTSHEETEDITOR				GetString("tabs", "event-sheet-editor")

// System object
#define SOBJ_FLUSHCLIPBOARD_D			GetString("SystemObject", "10")
#define SOBJ_FLUSHCLIPBOARD_S			GetString("SystemObject", "11")
#define SOBJ_SETCLIPBOARDTEXT_P1_D		GetString("SystemObject", "12")
#define SOBJ_SETCLIPBOARDTEXT_P1_N		GetString("SystemObject", "13")
#define SOBJ_SETCLIPBOARDTEXT_D			GetString("SystemObject", "14")
#define SOBJ_SETCLIPBOARDTEXT_S			GetString("SystemObject", "15")
#define SOBJ_WRITEINI_P1_D				GetString("SystemObject", "16")
#define SOBJ_WRITEINI_P1_N				GetString("SystemObject", "17")
#define SOBJ_WRITEINI_P2_D				GetString("SystemObject", "18")
#define SOBJ_WRITEINI_P2_N				GetString("SystemObject", "19")
#define SOBJ_WRITEINI_P3_D				GetString("SystemObject", "20")
#define SOBJ_WRITEINI_P3_N				GetString("SystemObject", "21")
#define SOBJ_WRITEINI_P4_D				GetString("SystemObject", "22")
#define SOBJ_WRITEINI_P4_N				GetString("SystemObject", "23")
#define SOBJ_WRITEINI_D					GetString("SystemObject", "24")
#define SOBJ_WRITEINI_S					GetString("SystemObject", "25")
#define SOBJ_PLAYSOUND_P1_D				GetString("SystemObject", "26")
#define SOBJ_PLAYSOUND_P1_N				GetString("SystemObject", "27")
#define SOBJ_PLAYSOUND_P2_D				GetString("SystemObject", "28")
#define SOBJ_PLAYSOUND_P2_N				GetString("SystemObject", "29")
#define SOBJ_PLAYSOUND_D				GetString("SystemObject", "30")
#define SOBJ_PLAYSOUND_S				GetString("SystemObject", "31")
#define SOBJ_MESSAGEBOX_P1_D			GetString("SystemObject", "32")
#define SOBJ_MESSAGEBOX_P1_N			GetString("SystemObject", "33")
#define SOBJ_MESSAGEBOX_P2_D			GetString("SystemObject", "34")
#define SOBJ_MESSAGEBOX_P2_N			GetString("SystemObject", "35")
#define SOBJ_MESSAGEBOX_P3_D			GetString("SystemObject", "36")
#define SOBJ_MESSAGEBOX_P3_N			GetString("SystemObject", "37")
#define SOBJ_MESSAGEBOX_P3_I			GetString("SystemObject", "38")
#define SOBJ_MESSAGEBOX_P4_D			GetString("SystemObject", "39")
#define SOBJ_MESSAGEBOX_P4_N			GetString("SystemObject", "40")
#define SOBJ_MESSAGEBOX_P4_I			GetString("SystemObject", "41")
#define SOBJ_MESSAGEBOX_D				GetString("SystemObject", "42")
#define SOBJ_MESSAGEBOX_S				GetString("SystemObject", "43")
#define SOBJ_SCROLLX_P1_D				GetString("SystemObject", "44")
#define SOBJ_SCROLLX_P1_N				GetString("SystemObject", "45")
#define SOBJ_SCROLLX_D					GetString("SystemObject", "46")
#define SOBJ_SCROLLX_S					GetString("SystemObject", "47")
#define SOBJ_SCROLLY_P1_D				GetString("SystemObject", "48")
#define SOBJ_SCROLLY_P1_N				GetString("SystemObject", "49")
#define SOBJ_SCROLLY_D					GetString("SystemObject", "50")
#define SOBJ_SCROLLY_S					GetString("SystemObject", "51")
#define SOBJ_SETDISPLAYANGLE_P1_D		GetString("SystemObject", "52")
#define SOBJ_SETDISPLAYANGLE_P1_N		GetString("SystemObject", "53")
#define SOBJ_SETDISPLAYANGLE_D			GetString("SystemObject", "54")
#define SOBJ_SETDISPLAYANGLE_S			GetString("SystemObject", "55")
#define SOBJ_SETGLOBALFILTER_P1_D		GetString("SystemObject", "56")
#define SOBJ_SETGLOBALFILTER_P1_N		GetString("SystemObject", "57")
#define SOBJ_SETGLOBALFILTER_D			GetString("SystemObject", "58")
#define SOBJ_SETGLOBALFILTER_S			GetString("SystemObject", "59")
#define SOBJ_STARTLOOP_P1_D				GetString("SystemObject", "60")
#define SOBJ_STARTLOOP_P1_N				GetString("SystemObject", "61")
#define SOBJ_STARTLOOP_P2_D				GetString("SystemObject", "62")
#define SOBJ_STARTLOOP_P2_N				GetString("SystemObject", "63")
#define SOBJ_STARTLOOP_D				GetString("SystemObject", "64")
#define SOBJ_STARTLOOP_S				GetString("SystemObject", "65")
#define SOBJ_STOPLOOP_D					GetString("SystemObject", "66")
#define SOBJ_STOPLOOP_S					GetString("SystemObject", "67")
#define SOBJ_BREAK_D					GetString("SystemObject", "68")
#define SOBJ_BREAK_S					GetString("SystemObject", "69")
#define SOBJ_CREATEOBJECT_P1_D			GetString("SystemObject", "70")

// Macros
#define ERRORTITLE_PREVIEW GetStr("1")
#define ERROR_PREVIEW GetStr("2")
#define CONSTRUCT_ERROR GetStr("3")

#define DEVELOPMENT_TEAM GetStr("23")

#define MORE_COLOURS GetStr("35")
#define NEW_LAYER GetStr("36")
#define TITLE_ERROR GetStr("37")
#define IMAGE_EDITOR GetStr("41")

#define CONDITION_NAME GetStr("48")
#define EVENT_TRIGGERS GetStr("50")

#define NO_ACTIONS GetStr("55")
#define EXPRESSION_VALID GetStr("56")
#define EP_FLOAT GetStr("57")
#define EP_INTEGER GetStr("58")
#define EP_STRING GetStr("59")
#define UNKNOWN_ERROR GetStr("60")
#define EP_TYPE GetStr("61")
#define EP_VALUE GetStr("62")

#define EP_BINARY GetStr("64")
#define EP_OBJECT GetStr("65")
#define ARGUMENT GetStr("66")
#define NO_DESCRIPTION_AVAILABLE GetStr("67")
#define NO_ARGUMENT_SELECTED GetStr("68")
#define EVALUATE GetStr("69")
#define FAIL_LOAD_SCINTILLA_TITLE GetStr("70")
#define FAIL_LOAD_SCINTILLA GetStr("71")
#define EDIT_APPLICATION_MENU GetStr("72")
#define EDIT_APPLICATION_MENU_SUB GetStr("73")
#define EDIT GetStr("74")

#define LANGUAGE_PREFERENCES GetStr("76")

#define ACES_INFORMATION GetStr("81")
#define ACTIONS GetStr("82")
#define CONDITIONS GetStr("83")
#define EXPRESSIONS GetStr("84")
#define ACE_NAME GetStr("85")
#define DESCRIPTION GetStr("86")

#endif