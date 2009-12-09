#include "CXBOXController.h"



CXBOXController::CXBOXController(int playerNumber)
{
	// Set the Controller Number
	_controllerNum = playerNumber - 1;
	valid_state = false;
}

float CXBOXController::LeftThumbX()
{
	float dx;
	LeftThumb(&dx, NULL);
	return dx;
}

float CXBOXController::LeftThumbY()
{
	float dy;
	LeftThumb(NULL, &dy);
	return dy;
}

float CXBOXController::RightThumbX()
{
	float dx;
	RightThumb(&dx, NULL);
	return dx;
}

float CXBOXController::RightThumbY()
{
	float dy;
	RightThumb(NULL, &dy);
	return dy;
}

void CXBOXController::LeftThumb(float* dx, float* dy)
{
	if(dx)		*dx = 0;
	if(dy)		*dy = 0;
	float x =  GetState().Gamepad.sThumbLX / 32768.0f;
	float y =  GetState().Gamepad.sThumbLY / 32768.0f;
	float angle = atan2(y,x);
	float mag = sqrt( x*x + y*y);
	if(mag < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / 32768.0f)
		return;
	if(mag > 1)
		mag = 1; // It sometimes goes over due to inaccuracy
	
	mag -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / 32768.0f;
	mag *= 32768 / (32768.0f - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE); 

	if(dx)		*dx = cos(angle) * mag;
	if(dy)		*dy = sin(angle) * -mag;
}

void CXBOXController::RightThumb(float* dx, float* dy)
{
	if(dx)		*dx = 0;
	if(dy)		*dy = 0;
	float x =  GetState().Gamepad.sThumbRX / 32768.0f;
	float y =  GetState().Gamepad.sThumbRY / 32768.0f;
	float angle = atan2(y,x);
	float mag = sqrt( x*x + y*y);
	if(mag < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE / 32768.0f)
		return;
	
	mag -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE / 32768.0f;
	mag *= 32768 / (32768.0f - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE); 

	if(dx)		*dx = cos(angle) * mag;
	if(dy)		*dy = sin(angle) * -mag;
}

float CXBOXController::LeftTrigger()
{
	float val = GetState().Gamepad.bLeftTrigger;
	if(val < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return 0;
	else 
		val -= XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
	val *= 1.0f / (255.0f-XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
	return val;
}
float CXBOXController::RightTrigger()
{
	float val = GetState().Gamepad.bRightTrigger;
	if(val < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return 0;
	else 
		val -= XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
	val *= 1.0f / (255.0f-XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
	return val;
}

float CXBOXController::ButtonState(XBoxControl::Control button)
{
	WORD buttons = GetState().Gamepad.wButtons;
	switch(button){
		case XBoxControl::DPadUp:
			return buttons & XINPUT_GAMEPAD_DPAD_UP ? 1.0f : 0.0f;

		case XBoxControl::DPadDown:
			return buttons & XINPUT_GAMEPAD_DPAD_DOWN ? 1.0f : 0.0f;

		case XBoxControl::DPadLeft:
			return buttons & XINPUT_GAMEPAD_DPAD_LEFT ? 1.0f : 0.0f;

		case  XBoxControl::DPadRight:
			return buttons & XINPUT_GAMEPAD_DPAD_RIGHT ? 1.0f : 0.0f;

		case XBoxControl::Start:
			return buttons & XINPUT_GAMEPAD_START ? 1.0f : 0.0f;

		case XBoxControl::Back:
			return buttons & XINPUT_GAMEPAD_BACK ? 1.0f : 0.0f;

		case XBoxControl::LeftThumb:
			return buttons & XINPUT_GAMEPAD_LEFT_THUMB ? 1.0f : 0.0f;

		case XBoxControl::RightThumb:
			return buttons & XINPUT_GAMEPAD_RIGHT_THUMB ? 1.0f : 0.0f;

		case XBoxControl::LeftShoulder:
			return buttons & XINPUT_GAMEPAD_LEFT_SHOULDER ? 1.0f : 0.0f;

		case XBoxControl::RightShoulder:
			return buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER ? 1.0f : 0.0f;

		case XBoxControl::LeftTrigger:
			return LeftTrigger();

		case XBoxControl::RightTrigger:
			return RightTrigger();

		case XBoxControl::A:
			return buttons & XINPUT_GAMEPAD_A ? 1.0f : 0.0f;

		case XBoxControl::B:
			return buttons & XINPUT_GAMEPAD_B ? 1.0f : 0.0f;

		case XBoxControl::X:
			return buttons & XINPUT_GAMEPAD_X ? 1.0f : 0.0f;

		case XBoxControl::Y:
			return buttons & XINPUT_GAMEPAD_Y ? 1.0f : 0.0f;

		case XBoxControl::LeftThumbUp:
			return max(0, -LeftThumbY());

		case XBoxControl::LeftThumbDown:
			return max(0, LeftThumbY());

		case XBoxControl::LeftThumbLeft:
			return max(0, -LeftThumbX());

		case XBoxControl::LeftThumbRight:
			return max(0, LeftThumbX());

		case XBoxControl::RightThumbUp:
			return max(0, -RightThumbY());

		case XBoxControl::RightThumbDown:
			return max(0, RightThumbY());

		case XBoxControl::RightThumbLeft:
			return max(0, -RightThumbX());

		case XBoxControl::RightThumbRight:
			return max(0, RightThumbX());

		default:
			return 0;
	}
}

XINPUT_STATE CXBOXController::GetState()
{
	if(valid_state)
		return _controllerState;
	else
	{
		// Zeroise the state
		ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

		// Get the state
		XInputGetState(_controllerNum, &_controllerState);

		valid_state = true;
	}
	return _controllerState;
}

bool CXBOXController::IsConnected()
{
	// Zeroise the state
	ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

	// Get the state
	DWORD Result = XInputGetState(_controllerNum, &_controllerState);

	if(Result == ERROR_SUCCESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Vibration::Update(CXBOXController* player)
{
	curStrength = 0;
	if(total_time == 0) 
		return;
	if(time > total_time)
	{
		total_time = 0;
		return;
	}
	float vibStrength = strength;
	float ratio = time / total_time;
	if(method == 1)
		vibStrength *= (1-ratio);
	curStrength = (int)(vibStrength * 65535);
}


void CXBOXController::Vibrate(int leftVal, int rightVal)
{
	// Create a Vibraton State
	XINPUT_VIBRATION Vibration;

	// Zeroise the Vibration
	ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

	// Set the Vibration Values
	Vibration.wLeftMotorSpeed = leftVal;
	Vibration.wRightMotorSpeed = rightVal;

	// Vibrate the controller
	XInputSetState(_controllerNum, &Vibration);
}