#ifndef included_Constants
#define included_Constants
#pragma once

//-------------------------------------------------------------


//-------------------------------------------------------------
namespace ConstantParameters
{
	const float PI = 3.1415926535897932384626433832795f;
	const float CONVERTING_TO_RADIANS = PI/180.f;
	const float CONVERTING_TO_DEGREES = 180.f/PI;
	const float NINETY_DEGREES_AS_RADIANS = 90.f * CONVERTING_TO_RADIANS;
	const float DELTA_SECONDS = 1/60.f;
	const float CAMERA_SPEED = 6.f;
	const float MOUSE_SPEED = 0.2f;
	const float CAMERA_Z_MAX = PI/2;

	const float ASPECT_RATIO = 16.f/9.f;
	const float INVERSE_ASPECT_RATIO = 9.f/16.f;

	const float PLAYER_NORMAL_SPEED = 12.f;
	const float PLAYER_FOCUS_SPEED = 5.f;

	const float PLAYER_Y_AXIS_LIMIT = 17.f;
	const float PLAYER_X_AXIS_LIMIT = 31.f;

	const unsigned int GAMESTATE_BUFFER_SIZE = 601;
	const double GAMESTATE_UPDATE_RATE = 0.05;
	const double GAMESTATE_REWIND_RATE = 0.05;

	const float TIMEMETER_DRAIN = 0.05f;
}

enum PlayerShotPattern
{
	PLAYERSHOTPATTERN_SINGLE,
	PLAYERSHOTPATTERN_TWIN,
	PLAYERSHOTPATTERN_SPREAD,
	PLAYERSHOTPATTERN_HOMING,
	NUM_PLAYERSHOTPATTERNS
};

enum AIShotPattern
{
	AISHOTPATTERN_SINGLEDIRECT,
	AISHOTPATTERN_SPREAD,
	AISHOTPATTERN_HORIZONTAL,
	AISHOTPATTERN_VERTICAL,
	NUM_AISHOTPATTERNS
};

enum BulletType
{
	BULLETTYPE_NORMAL,
	BULLETTYPE_STILL,
	BULLETTYPE_SPLIT,
	BULLETTYPE_DRAG,
	NUM_AISHOTTYPES
};

#endif //included_Constants