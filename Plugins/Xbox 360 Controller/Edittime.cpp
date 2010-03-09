// Edittime.cpp - Edittime implementation of object
//

// Include StdAfx and Properties.h
#include "StdAfx.h"

#ifndef RUN_ONLY

//////////////////////////////////////////////////////////////////////////////////
// Property table
//////////////////////////////////////////////////////////////////////////////////

// The property table defines the list of properties that appear in the property
// grid when your object is selected in the layout editor.
//
// See the SDK docs for full documentation, but to get you started:-
//
// PROPERTY_EDIT  (myVariable, "Label", "Description");		// A text field property.
// PROPERTY_VALUE (myVariable, "Label 2", "Description");	// An integer property.
// PROPERTY_FLOAT (myVariable, "Label 3", "Description");	// A double/float property
//
// The labels must be unique!



BEGIN_PROPERTY_TABLE();
//PROPERTY_NEW_CATEGORY("Xbox Controller 1", "Controller 1");
if(editObject){
	int count = editObject->pEditTime->GetControlCount();
	string Controls = "-";
	map<string, int> controlStringToCombo[10]; // 10 players
	vector<Control> comboToControlString;

	controlStringToCombo[0][""] = 0;
	comboToControlString.push_back(Control(0, "",0));

	for(int c = 0; c < count; c++)
	{	
		int controlPlayer = editObject->pEditTime->GetControlPlayer(c);
		string controlName = editObject->pEditTime->GetControlName(c);
		int controlPlayerDisplay = controlPlayer+1;

		Controls += "|" + controlName + " (" + controlPlayerDisplay + ")";
	
		controlStringToCombo[controlPlayer][controlName] = c+1;
		comboToControlString.push_back(Control(controlPlayer, controlName, c+1));
	}
	// Fill in the combo index of the control map
	if(iMode == MODE_WRITE)
	{
		for(int i = 0; i < CONTROLLERS; i++)
		{
			for(int j = 0; j < BUTTONS; j++)
			{
				Control& control = editObject->objectPtr->controls[i][j];
				if(control.player == -1)
					control.c = 0;
				else
					control.c = controlStringToCombo[control.player][control.control];
			}
		}
	}

	//for(int i = 0; i < CONTROLLERS; i++)
	{
		PROPERTY_COMBO(controllerCombo, "Controller", "Select the controller to configure", "Controller 1|Controller 2|Controller 3|Controller 4");

		if (iMode == MODE_READ && "Controller" == SLabel ) 
			editObject->pEditTime->RefreshProperties();
		

		int i = editObject->objectPtr->controllerCombo;

			PROPERTY_COMBO(controls[i][0].c, "DPad-Up", "", Controls.c_str());
			PROPERTY_COMBO(controls[i][1].c, "DPad-Down", "", Controls.c_str());
			PROPERTY_COMBO(controls[i][2].c, "DPad-Left", "", Controls.c_str());
			PROPERTY_COMBO(controls[i][3].c, "DPad-Right", "", Controls.c_str());
			PROPERTY_COMBO(controls[i][4].c, "Start", "", Controls.c_str());
			PROPERTY_COMBO(controls[i][5].c, "Back", "", Controls.c_str());
			PROPERTY_COMBO(controls[i][6].c, "Left Thumb Click", "", Controls.c_str());
			PROPERTY_COMBO(controls[i][7].c, "Right Thumb Click", "", Controls.c_str());
			PROPERTY_COMBO(controls[i][8].c, "Left Shoulder", "", Controls.c_str());
			PROPERTY_COMBO(controls[i][9].c, "Right Shoulder", "", Controls.c_str());
			PROPERTY_COMBO(controls[i][10].c, "Left Trigger", "", Controls.c_str());
			PROPERTY_COMBO(controls[i][11].c, "Right Trigger", "", Controls.c_str());
			PROPERTY_COMBO(controls[i][12].c, "A", "", Controls.c_str());
			PROPERTY_COMBO(controls[i][13].c, "B", "", Controls.c_str());
//			PROPERTY_COMBO(controls[i][14].c, "C", "", Controls.c_str());
//			PROPERTY_COMBO(controls[i][15].c, "D", "", Controls.c_str());
			PROPERTY_COMBO(controls[i][14].c, "X", "", Controls.c_str());
			PROPERTY_COMBO(controls[i][15].c, "Y", "", Controls.c_str());
			PROPERTY_COMBO(controls[i][16].c, "Left Thumb-Up", "", Controls.c_str());
			PROPERTY_COMBO(controls[i][17].c, "Left Thumb-Down", "", Controls.c_str());
			PROPERTY_COMBO(controls[i][18].c, "Left Thumb-Left", "", Controls.c_str());
			PROPERTY_COMBO(controls[i][19].c, "Left Thumb-Right", "", Controls.c_str());
			PROPERTY_COMBO(controls[i][20].c, "Right Thumb-Up", "", Controls.c_str());
			PROPERTY_COMBO(controls[i][21].c, "Right Thumb-Down", "", Controls.c_str());
			PROPERTY_COMBO(controls[i][22].c, "Right Thumb-Left", "", Controls.c_str());
			PROPERTY_COMBO(controls[i][23].c, "Right Thumb-Right", "", Controls.c_str());

	}
	// Fill in the combo index of the control map
	if(iMode == MODE_READ)
	{
		for(int i = 0; i < CONTROLLERS; i++)
		{
			for(int j = 0; j < BUTTONS; j++)
			{
				Control& control = editObject->objectPtr->controls[i][j];
				control = comboToControlString[control.c];
			}
		}
	}
}
END_PROPERTY_TABLE  ();


/////////////////////////////////////////////////////////////////////////////////
// Edittime functions
//////////////////////////////////////////////////////////////////////////////////

// Constructor
EditExt::EditExt(VEditTime* pVEditTime, editInfo* pEInfo)
{
	// Store the edittime and einfo pointers
	pEditTime = pVEditTime;
	pInfo = pEInfo;
}

// Destructor: when your object is deleted or the application closed:
// close any allocated resources here
EditExt::~EditExt()
{

}

// Initialization:  when the frame editor is opened.  Create textures, fonts,
// and any other DirectX volatile data here.
void EditExt::Initialize()
{

}

// Called when any property has been changed.
void EditExt::OnPropertiesUpdate()
{
}

// Copied in from CXBOXController.h which is excluded in edittime builds
namespace XBoxControl {
	enum Control{
		DPadUp,
		DPadDown,
		DPadLeft,
		DPadRight,
		Start,
		Back,
		LeftThumb,
		RightThumb,
		LeftShoulder,
		RightShoulder,
		LeftTrigger,
		RightTrigger,
		A,
		B,
		X,
		Y,
		LeftThumbUp,
		LeftThumbDown,
		LeftThumbLeft,
		LeftThumbRight,
		RightThumbUp,
		RightThumbDown,
		RightThumbLeft,
		RightThumbRight,
	};
};

// When the object is inserted to the layout editor by the user.
void EditExt::OnPut()
{
	// Default object size
	pInfo->objectWidth = 64;
	pInfo->objectHeight = 64;

	controls[0][XBoxControl::LeftThumbLeft] = Control(0, "Move Left",1);
	controls[0][XBoxControl::LeftThumbRight] = Control(0, "Move Right",2);
	controls[0][XBoxControl::LeftThumbUp] = Control(0, "Move Up",3);
	controls[0][XBoxControl::LeftThumbDown] = Control(0, "Move Down",4);
	controls[0][XBoxControl::A] = Control(0, "Jump",5);
	controls[0][XBoxControl::RightTrigger] = Control(0, "Accelerate",6);
	controls[0][XBoxControl::LeftTrigger] = Control(0, "Brake",7);

	controllerCombo = 0;

}


// When the object is removed from the layout editor by the user.
void EditExt::OnRemoved()
{

}

// On object resized
BOOL EditExt::OnSizeObject()
{
	return TRUE;
}

void EditExt::GetAnimationHandle(int& handle)
{
	
}

#else //ifndef RUN_ONLY

BOOL WINAPI ETCreateObject(editInfo* editObject) {return FALSE;}
BOOL WINAPI ETInitializeObject(editInfo* editObject, VEditTime* pVEditTime) {return FALSE;}
BOOL WINAPI ETDestroyObject(editInfo* editObject) {return FALSE;}
int WINAPI ETDrawObject(editInfo *editObject) {return 0;}
void ETOnPropertyChanged(editInfo* editObject) {}
long LinkPropTable() {return 0;}
void WINAPI ETSerialize(editInfo* editObject, bin& ar) {}
void WINAPI ETPutObject(editInfo* editObject) {}
void WINAPI ETRemoveObject(editInfo* editObject) {}
void WINAPI GetInfo(LPOINFO objectInfo) {}
int WINAPI ETGetMenuIcons(editInfo* editObject, int ACEMenu) {return 0;}
int WINAPI ETShowACEMenu(editInfo* editObject, TABLE ACEType)  {return 0;}
BOOL WINAPI ETSizeObject(editInfo* editObject)  {return FALSE;}

#endif // RUN_ONLY