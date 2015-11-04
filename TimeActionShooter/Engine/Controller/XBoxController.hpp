#ifndef included_XBoxController
#define included_XBoxController
#pragma once

//-------------------------------------------------------------


//-------------------------------------------------------------
class XBoxController
{
public:
	XBoxController();
	float GetControlStickMagnitudeX(float XStick);
	float GetControlStickMagnitudeY(float YStick);
	float StickDeadZone;
};

#endif //included_XBoxController