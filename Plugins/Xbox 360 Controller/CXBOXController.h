#ifndef _XBOX_CONTROLLER_H_
#define _XBOX_CONTROLLER_H_

// No MFC
#define WIN32_LEAN_AND_MEAN

// We need the Windows Header and the XInput Header
#include <windows.h>
#include <XInput.h>
#include <math.h>
// Now, the XInput Library
// NOTE: COMMENT THIS OUT IF YOU ARE NOT USING A COMPILER THAT SUPPORTS THIS METHOD OF LINKING LIBRARIES
#pragma comment(lib, "XInput.lib")

class Vibration{
public:
	float time;
	float total_time;
	int method;
	float strength;
	int curStrength;

	Vibration(){
		time = 0;
		total_time = 0;
		int method = 0;
		strength = 0;
	}
	void Update(class CXBOXController* player);
};

#define XboxControlString "Dpad Up|Dpad Down|Dpad Left|Dpad Right|Start|Back|Left Thumb Click|Right Thumb Click|Left Shoulder|Right shoulder|Left Trigger|Right Trigger|Button A|Button B|Button X|Button Y|Left Thumb -  Up|Left Thumb - Down|Left Thumb - Left|Left Thumb - Right|Right Thumb -  Up|Right Thumb - Down|Right Thumb - Left|Right Thumb - Right"

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


// XBOX Controller Class Definition
class CXBOXController
{
private:
	XINPUT_STATE _controllerState;
	int _controllerNum;
public:
	CXBOXController(int playerNumber);
	XINPUT_STATE GetState();
	bool IsConnected();
	void Vibrate(int leftVal = 0, int rightVal = 0);

	float LeftThumbX();
	float LeftThumbY();
	float RightThumbX();
	float RightThumbY();

	void LeftThumb(float* dx, float* dy);
	void RightThumb(float* dx, float* dy);

	float LeftTrigger();
	float RightTrigger();

	float ButtonState(XBoxControl::Control button);

	Vibration leftVib;
	Vibration rightVib;

	bool valid_state;
};

#endif