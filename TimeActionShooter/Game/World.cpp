#include "World.hpp"

static bool leftMouseButtonDown = false;
static bool rightMouseButtonDown = false;

//----------------------------------------------------
World::World(  ) 
	: m_mousePositionXDifference(0.f)
	, m_mousePositionZDifference(0.f)
	, m_isPaused(true)
{
	srand ((unsigned int)(time(NULL)));

	m_camera = Camera();
	m_renderer = OpenGLRenderer();
	m_keyIsHeld = false;
}

//----------------------------------------------------
void World::Initialize() 
{
	
}

//----------------------------------------------------
bool World::ProcessKeyDownEvent( unsigned char virtualKeyCode) 
{
	m_isKeyDown[ virtualKeyCode ] = true;
	return true;
}

//----------------------------------------------------
bool World::ProcessKeyUpEvent( unsigned char virtualKeyCode) 
{
	m_isKeyDown[ virtualKeyCode ] = false;
	return true;
}

//----------------------------------------------------
void World::UpdateCameraFromInput( float deltaSeconds ) 
{
	m_camera.m_cameraPitch -= deltaSeconds * m_mousePositionZDifference * ConstantParameters::MOUSE_SPEED;
	m_camera.m_cameraYaw += deltaSeconds * m_mousePositionXDifference * ConstantParameters::MOUSE_SPEED;
}

//----------------------------------------------------
void World::UpdateFromMouseInput() 
{
	// Only allow one press per frame
	bool mouseButtonPressed = false;
	
	//Check the mouse left button 
	if ( (GetKeyState(VK_LBUTTON) & 0x8000) != 0 ) 
	{
		if ( !leftMouseButtonDown ) 
		{
			//TurnTargetedCellToSolid();
		}

		leftMouseButtonDown = true;
		mouseButtonPressed = true;
	} 
	else 
	{
		leftMouseButtonDown = false;
	}

	if ( mouseButtonPressed )
	{
		return;
	}

	//Check the mouse right button 
	if ( (GetKeyState(VK_RBUTTON) & 0x8000) != 0 ) 
	{
		if ( !rightMouseButtonDown ) 
		{
			//TurnTargetedCellToNonSolid();
		} 
		rightMouseButtonDown = true;
	} 
	else 
	{
		rightMouseButtonDown = false;
	}
}

//----------------------------------------------------
void World::UpdatePlayerFromInput( float deltaSeconds ) 
{	
	if ( m_isKeyDown[ 'P' ] && !m_keyIsHeld ) 
	{
		m_keyIsHeld = true;
		m_isPaused = !m_isPaused;
	}
	
	if (m_isKeyDown[ VK_LEFT ]) 
	{
		m_camera.m_cameraYaw += deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[ VK_RIGHT ]) 
	{
		m_camera.m_cameraYaw -= deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[ VK_UP ]) 
	{
		m_camera.m_cameraPitch -= deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[ VK_DOWN ]) 
	{
		m_camera.m_cameraPitch += deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[ 'W' ]) 
	{
		m_camera.m_cameraPosition.x += cos(m_camera.m_cameraYaw) * deltaSeconds * ConstantParameters::CAMERA_SPEED;
		m_camera.m_cameraPosition.y += sin(m_camera.m_cameraYaw) * deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[ 'D' ]) 
	{
		m_camera.m_cameraPosition.y -= cos(m_camera.m_cameraYaw) * deltaSeconds * ConstantParameters::CAMERA_SPEED;
		m_camera.m_cameraPosition.x += sin(m_camera.m_cameraYaw) * deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[ 'S' ]) 
	{
		m_camera.m_cameraPosition.x -= cos(m_camera.m_cameraYaw) * deltaSeconds * ConstantParameters::CAMERA_SPEED;
		m_camera.m_cameraPosition.y -= sin(m_camera.m_cameraYaw) * deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[ 'A' ]) 
	{
		m_camera.m_cameraPosition.y += cos(m_camera.m_cameraYaw) * deltaSeconds * ConstantParameters::CAMERA_SPEED;
		m_camera.m_cameraPosition.x -= sin(m_camera.m_cameraYaw) * deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[ VK_SPACE ]) 
	{
		m_camera.m_cameraPosition.z += deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if (m_isKeyDown[VK_CONTROL]) 
	{
		m_camera.m_cameraPosition.z -= deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if ( !( m_isKeyDown[ 'T' ] || m_isKeyDown[ 'R' ] || m_isKeyDown[ 'P' ]) ) 
	{
		m_keyIsHeld = false;
	}
}

//----------------------------------------------------
void World::CheckForGimbleLock() 
{
	if (m_camera.m_cameraPitch >= ConstantParameters::CAMERA_Z_MAX) 
	{
		m_camera.m_cameraPitch = ConstantParameters::CAMERA_Z_MAX - 0.005f;
	}

	if (m_camera.m_cameraPitch <= -ConstantParameters::CAMERA_Z_MAX) 
	{
		m_camera.m_cameraPitch = -ConstantParameters::CAMERA_Z_MAX + 0.005f;
	}
}

//----------------------------------------------------
void World::Update() 
{
	float currentTime = (float) Time::GetCurrentTimeSeconds();
	float deltaSeconds = ConstantParameters::DELTA_SECONDS; // Hack: assume 60 FPS

	UpdatePlayerFromInput( deltaSeconds );
	UpdateCameraFromInput( deltaSeconds );
	//UpdateFromMouseInput();
	//CheckForGimbleLock();

	if (!m_isPaused) 
	{
		
	}
}

//----------------------------------------------------
void World::Render() 
{	
	m_renderer.SetModelViewProjectionMatrix(m_camera);
	//m_renderer.SendCubeVBO();
	//m_renderer.PopMatrix();
}