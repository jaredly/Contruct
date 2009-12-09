// OnPropertyChanged
#include "..\Common\PropReturn.hpp"

#define MODE_INIT  0	// Initialising property
#define MODE_READ  1	// Reading property from Construct
#define MODE_WRITE 2	// Writing property back to Construct

// Assign a const char* to a CString in Construct, compiler-agnostic (void* represents CString)
#define ASSIGN_CSTRING(s, v) pm->Assign((s), (v))

void EndProperties(int iMode, struct CVirtualPropItem* item, class MicroEdittime* pm);

#define BEGIN_PROPERTY_TABLE() void WINAPI ETOnPropertyChanged(editInfo* editObject, LPCSTR szLabel, int iMode, CVirtualPropItem* item, PropReturn* Property, MicroEdittime* pm) \
	{ \
		CString SLabel = szLabel;

// Property #0: Edit
// Parameters: variable name (accepting operator= with const char*), label text, description text
#define PROPERTY_EDIT(paramV, paramL, paramD) if (iMode == MODE_INIT) { \
		item->Type = 0; \
		ASSIGN_CSTRING(item->Label, paramL); \
		ASSIGN_CSTRING(item->Description, paramD); \
		pm->Proc(item); \
		} \
		else if (iMode == MODE_READ) { \
		if (SLabel == paramL ) \
			editObject->objectPtr-> paramV = Property->chstr; \
		} \
		else if (iMode == MODE_WRITE) { \
		if (SLabel == paramL ) \
			ASSIGN_CSTRING(Property->CSPtr, editObject->objectPtr-> paramV ); \
		}

// For std::string
#define PROPERTY_EDIT_STL(paramV, paramL, paramD) if (iMode == MODE_INIT) { \
		item->Type = 0; \
		ASSIGN_CSTRING(item->Label, paramL); \
		ASSIGN_CSTRING(item->Description, paramD); \
		pm->Proc(item); \
		} \
		else if (iMode == MODE_READ) { \
		if (SLabel == paramL ) \
			editObject->objectPtr-> paramV = Property->chstr; \
		} \
		else if (iMode == MODE_WRITE) { \
		if (SLabel == paramL ) \
			ASSIGN_CSTRING(Property->CSPtr, editObject->objectPtr-> paramV .c_str() ); \
		}

// Property #1: Static text
// Parameters: variable name, label text, description text
#define PROPERTY_STATIC(paramV, paramL, paramD) if (iMode == MODE_INIT) { \
		item->Type = 1; \
		ASSIGN_CSTRING(item->Label, paramL); \
		ASSIGN_CSTRING(item->Description, paramD); \
		pm->Proc(item); \
		} \
		else if (iMode == MODE_WRITE) { \
		if (SLabel == paramL ) \
			ASSIGN_CSTRING(Property->CSPtr, editObject->objectPtr-> paramV ); \
		}

// for std::string
#define PROPERTY_STATIC_STL(paramV, paramL, paramD) if (iMode == MODE_INIT) { \
		item->Type = 1; \
		ASSIGN_CSTRING(item->Label, paramL); \
		ASSIGN_CSTRING(item->Description, paramD); \
		pm->Proc(item); \
		} \
		else if (iMode == MODE_WRITE) { \
		if (SLabel == paramL ) \
			ASSIGN_CSTRING(Property->CSPtr, editObject->objectPtr-> paramV .c_str()); \
		}

// Property #2: Button
// Parameters: button text, label text, description text, ExtObject member function to call
#define PROPERTY_BUTTON(paramV, paramL, paramD, Function) if (iMode == MODE_INIT) { \
		item->Type = 2; \
		ASSIGN_CSTRING(item->Label, paramL); \
		ASSIGN_CSTRING(item->Description, paramD); \
		ASSIGN_CSTRING(item->Text, paramV); \
		pm->Proc(item); \
		} \
		else if (iMode == MODE_READ) { \
		if (SLabel == paramL ) \
			editObject->objectPtr-> Function ; \
		}

// Property #3:  Integer value
// Parameters: variable name (accepting operator= with __int64), label text, description text
#define PROPERTY_VALUE(paramV, paramL, paramD) if (iMode == MODE_INIT) { \
		item->Type = 3; \
		ASSIGN_CSTRING(item->Label, paramL); \
		ASSIGN_CSTRING(item->Description, paramD); \
		pm->Proc(item); \
		} \
		else if (iMode == MODE_READ) { \
		if (SLabel == paramL ) \
			editObject->objectPtr-> paramV = Property->val; \
		} \
		else if (iMode == MODE_WRITE) { \
		if (SLabel == paramL ) \
			Property->val = editObject->objectPtr-> paramV ; \
		}

// Property #4:  Combo box
// Parameters: Variable name (integer for combo index), label text, description text, items list
// Items pipe separated eg. "First|Second|Third".
#define PROPERTY_COMBO(paramV, paramL, paramD, Items) if (iMode == MODE_INIT) { \
		item->Type = 4; \
		ASSIGN_CSTRING(item->Label, paramL); \
		ASSIGN_CSTRING(item->Description, paramD); \
		ASSIGN_CSTRING(item->Text, Items); \
		pm->Proc(item); \
		} \
		else if (iMode == MODE_READ) { \
		if (SLabel == paramL ) \
			editObject->objectPtr-> paramV = Property->val; \
		} \
		else if (iMode == MODE_WRITE) { \
		if (SLabel == paramL ) \
			Property->val = editObject->objectPtr-> paramV ; \
		}

// Property #5:  Floating point value
// Parameters: variable name (accepting operator= with double), label text, description text
#define PROPERTY_FLOAT(paramV, paramL, paramD) if (iMode == MODE_INIT) { \
		item->Type = 5; \
		ASSIGN_CSTRING(item->Label, paramL); \
		ASSIGN_CSTRING(item->Description, paramD); \
		pm->Proc(item); \
		} \
		else if (iMode == MODE_READ) { \
		if (SLabel == paramL ) \
			editObject->objectPtr-> paramV = Property->fVal; \
		} \
		else if (iMode == MODE_WRITE) { \
		if (SLabel == paramL ) \
			Property->fVal = editObject->objectPtr-> paramV ; \
		}

// Property #6:  Colour
// Parameters: variable name, label text, description text
#define PROPERTY_COLOR(paramV, paramL, paramD) if (iMode == MODE_INIT) { \
		item->Type = 6; \
		ASSIGN_CSTRING(item->Label, paramL); \
		ASSIGN_CSTRING(item->Description, paramD); \
		pm->Proc(item); \
		} \
		else if (iMode == MODE_READ) { \
		if (SLabel == paramL ) \
			editObject->objectPtr-> paramV = Property->val; \
		} \
		else if (iMode == MODE_WRITE) { \
		if (SLabel == paramL ) \
			Property->val = editObject->objectPtr-> paramV ; \
		}

// Property #7:  Bool (checkbox)
// Parameters: variable name, label text, description text
#define PROPERTY_BOOL(paramV, paramL, paramD) if (iMode == MODE_INIT) { \
		item->Type = 7; \
		ASSIGN_CSTRING(item->Label, paramL); \
		ASSIGN_CSTRING(item->Description, paramD); \
		pm->Proc(item); \
		} \
		else if (iMode == MODE_READ) { \
		if (SLabel == paramL ) \
			editObject->objectPtr-> paramV = Property->val != 0; \
		} \
		else if (iMode == MODE_WRITE) { \
		if (SLabel == paramL ) \
			Property->val = editObject->objectPtr-> paramV ; \
		}

// Property #8:  Font
// Parameters: variable name, label text, description text, items (| seperated, eg "First|Second|Third")
#define PROPERTY_FONT(paramV, paramL, paramD, Info) if (iMode == MODE_INIT) { \
		item->Type = 8; \
		ASSIGN_CSTRING(item->Label, paramL); \
		ASSIGN_CSTRING(item->Description, paramD); \
		ASSIGN_CSTRING(item->Text, Info); \
		pm->Proc(item); \
		} \
		else if (iMode == MODE_READ) { \
		if (SLabel == paramL ) \
			editObject->objectPtr-> paramV = Property->val; \
		} \
		else if (iMode == MODE_WRITE) { \
		if (SLabel == paramL ) \
			Property->val = editObject->objectPtr-> paramV ; \
		}

// Property #9:  Percentage (same as float but 0.0 for 0% up to 1.0 for 100%)
// Parameters: variable name (accepting operator= with double), label text, description text
#define PROPERTY_PERCENT(paramV, paramL, paramD) if (iMode == MODE_INIT) { \
		item->Type = 9; \
		ASSIGN_CSTRING(item->Label, paramL); \
		ASSIGN_CSTRING(item->Description, paramD); \
		pm->Proc(item); \
		} \
		else if (iMode == MODE_READ) { \
		if (SLabel == paramL ) \
			editObject->objectPtr-> paramV = Property->fVal; \
		} \
		else if (iMode == MODE_WRITE) { \
		if (SLabel == paramL ) \
			Property->fVal = editObject->objectPtr-> paramV ; \
		}

// Property #10:  Slider 
// Parameters: variable name (accepting operator= with __int64), label text, description text
#define PROPERTY_SLIDER(paramV, paramL, paramD) if (iMode == MODE_INIT) { \
		item->Type = 10; \
		ASSIGN_CSTRING(item->Label, paramL); \
		ASSIGN_CSTRING(item->Description, paramD); \
		pm->Proc(item); \
		} \
		else if (iMode == MODE_READ) { \
		if (SLabel == paramL ) \
			editObject->objectPtr-> paramV = Property->val; \
		} \
		else if (iMode == MODE_WRITE) { \
		if (SLabel == paramL ) \
			Property->val = editObject->objectPtr-> paramV ; \
		}

// Property #11:  Frame
// Parameters: variable name (accepting operator= with __int64), label text, description text
#define PROPERTY_FRAME(paramV, paramL, paramD) if (iMode == MODE_INIT) { \
		item->Type = 11; \
		ASSIGN_CSTRING(item->Label, paramL); \
		ASSIGN_CSTRING(item->Description, paramD); \
		pm->Proc(item); \
		} \
		else if (iMode == MODE_READ) { \
		if (SLabel == paramL ) \
			editObject->objectPtr-> paramV = Property->val; \
		} \
		else if (iMode == MODE_WRITE) { \
		if (SLabel == paramL ) \
			Property->val = editObject->objectPtr-> paramV ; \
		}

// Property #20:  New category
#define PROPERTY_NEW_CATEGORY(paramL, paramD) if (iMode == MODE_INIT) { \
		item->Type = 20;	\
		ASSIGN_CSTRING(item->Label, paramL);	\
		ASSIGN_CSTRING(item->Description, paramD);	\
		pm->Proc(item);	\
		}

#define END_PROPERTY_TABLE() EndProperties(iMode, item, pm); \
							if (iMode == MODE_READ) editObject->objectPtr->OnPropertiesUpdate(); \
							}

#define PROPERTY_PLAYER(var) PROPERTY_COMBO(var, "Control", "Choose the player the behavior is controlled by.", "Player 1|Player 2|Player 3|Player 4|Player 5|Player 6|Player 7|Player 8|Player 9|Player 10")
// ..