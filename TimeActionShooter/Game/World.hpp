#ifndef included_World
#define included_World
#pragma once

//-------------------------------------------------------------
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <vector>
#include "../Engine/Math/MyMathFiles.hpp"
#include "../Engine/Constants.hpp"
#include "../Engine/Camera/Camera.hpp"
#include "../Engine/Time/Time.hpp"
#include "../Engine/Renderer/OpenGLRenderer.hpp"
//#include "CubeCell.hpp"

//-------------------------------------------------------------
class World
{
public:
	World();
	bool ProcessKeyDownEvent( unsigned char virtualKeyCode);
	bool ProcessKeyUpEvent( unsigned char virtualKeyCode);
	void UpdatePlayerFromInput( float deltaseconds );
	void UpdateCameraFromInput( float deltaseconds );
	void UpdateFromMouseInput();
	void CheckForGimbleLock();
	void Initialize();

	void Update();
	void Render();

	bool m_isKeyDown[ 256 ];
	bool m_keyIsHeld;

	bool m_isPaused;

	Camera m_camera;
	OpenGLRenderer m_renderer;

	float m_mousePositionXDifference;
	float m_mousePositionZDifference;
};

#endif //included_World