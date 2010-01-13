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

#define PROPERTY_STATIC(paramV, paramL, paramD) if (iMode == MODE_INIT) { \
		item->Type = 1; \
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

#define END_PROPERTY_TABLE() EndProperties(iMode, item, pm); \
							if (iMode == MODE_READ) editObject->objectPtr->OnPropertiesUpdate(); \
							}

// ..