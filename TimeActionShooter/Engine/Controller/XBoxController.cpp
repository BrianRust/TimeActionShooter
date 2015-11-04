#include "XBoxController.hpp"

//-------------------------------------------------------------
XBoxController::XBoxController()
{
	StickDeadZone = 0.1f;
}

//-------------------------------------------------------------

float XBoxController::GetControlStickMagnitudeX(float XStick)
{
	float XThumb = (float)(XStick + 32768.f);
	XThumb = XThumb / 65535.f;
	XThumb = XThumb * 2.f;
	XThumb = XThumb - 1.f;
	return XThumb;
}

//-------------------------------------------------------------

float XBoxController::GetControlStickMagnitudeY(float YStick)
{
	float YThumb = (float)(YStick + 32768.f);
	YThumb = YThumb / 65535.f;
	YThumb = YThumb * 2.f;
	YThumb = YThumb - 1.f;
	return YThumb;
}