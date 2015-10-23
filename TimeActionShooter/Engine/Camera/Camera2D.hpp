#ifndef included_Camera2D
#define included_Camera2D
#pragma once

//-------------------------------------------------------------
//#include "MyGameTypedefs.hpp"
#include "../Math/Vector2.hpp"

//-------------------------------------------------------------
class Camera2D
{
public:
	Camera2D();

	void Update();

	Vector2 m_cameraCenter;
	Vector2 m_size;
};

#endif //included_Camera2D