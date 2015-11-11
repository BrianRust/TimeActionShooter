#include "World.hpp"

#include <Xinput.h>
#pragma comment( lib, "xinput" ) // Link in the xinput.lib static library

static bool leftMouseButtonDown = false;
static bool rightMouseButtonDown = false;

//----------------------------------------------------
World::World(  ) 
	: m_mousePositionXDifference(0.f)
	, m_mousePositionZDifference(0.f)
	, m_isPaused(false)
	, m_player(Player())
	, m_camera(Camera2D())
	, m_renderer(OpenGLRenderer())
	, m_keyIsHeld(false)
	, m_isButtonHeld(false)
	, m_lastPauseTimer(0.0)
	, m_lastGameStateUpdate(0.0)
	, m_lastGameStateSwitch(0.0)
{
	srand ((unsigned int)(time(NULL)));
}

//----------------------------------------------------
void World::Initialize() 
{
	m_enemies.push_back(Enemy());
	m_enemies[m_enemies.size()-1].m_position = Vector2(0.f, 15.f);
	m_enemies[m_enemies.size()-1].m_bulletType = BULLETTYPE_DRAG;
	m_enemies.push_back(Enemy());
	m_enemies[m_enemies.size()-1].m_position = Vector2(15.f, 10.f);
	m_enemies.push_back(Enemy());
	m_enemies[m_enemies.size()-1].m_position = Vector2(-15.f, 10.f);

	m_enemies.push_back(Enemy());
	m_enemies[m_enemies.size()-1].m_position = Vector2(25.f, 10.f);
	m_enemies[m_enemies.size()-1].m_shotPattern = AISHOTPATTERN_SINGLEDIRECT;

	m_enemies.push_back(Enemy());
	m_enemies[m_enemies.size()-1].m_position = Vector2(-27.f, -10.f);
	m_enemies[m_enemies.size()-1].m_shotPattern = AISHOTPATTERN_SINGLEDIRECT;

	UpdateGameStateBuffer();
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
// 	m_camera.m_cameraPitch -= deltaSeconds * m_mousePositionZDifference * ConstantParameters::MOUSE_SPEED;
// 	m_camera.m_cameraYaw += deltaSeconds * m_mousePositionXDifference * ConstantParameters::MOUSE_SPEED;
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

		if ( m_isPaused )
		{
			UpdatePauseTimers();
		}
		else
		{
			m_lastPauseTimer = Time::GetCurrentTimeSeconds();
		}

		m_isPaused = !m_isPaused;
	}
	
	if (m_isKeyDown[ VK_LEFT ] && !m_player.m_isDead) 
	{
		if ( !m_isPaused )
		{
			if (m_player.m_position.x > -ConstantParameters::PLAYER_X_AXIS_LIMIT)
			{
				m_player.m_position.x -= deltaSeconds * ConstantParameters::PLAYER_NORMAL_SPEED;
			}
		}
	}

	if (m_isKeyDown[ VK_RIGHT ] && !m_player.m_isDead) 
	{
		if ( !m_isPaused )
		{
			if (m_player.m_position.x < ConstantParameters::PLAYER_X_AXIS_LIMIT)
			{
				m_player.m_position.x += deltaSeconds * ConstantParameters::PLAYER_NORMAL_SPEED;
			}
		}
	}

	if (m_isKeyDown[ VK_UP ] && !m_player.m_isDead) 
	{
		if ( !m_isPaused )
		{
			if (m_player.m_position.y < ConstantParameters::PLAYER_Y_AXIS_LIMIT)
			{
				m_player.m_position.y += deltaSeconds * ConstantParameters::PLAYER_NORMAL_SPEED;
			}
		}
	}

	if (m_isKeyDown[ VK_DOWN ] && !m_player.m_isDead) 
	{
		if ( !m_isPaused )
		{
			if (m_player.m_position.y > -ConstantParameters::PLAYER_Y_AXIS_LIMIT)
			{
				m_player.m_position.y -= deltaSeconds * ConstantParameters::PLAYER_NORMAL_SPEED;
			}
		}
	}

	if (m_isKeyDown[ VK_SPACE ]) 
	{
		if ( !m_isPaused )
		{
			FirePlayerBullets(deltaSeconds);
		}
	}

	if (m_isKeyDown[VK_CONTROL]) 
	{
		//m_camera.m_cameraPosition.z -= deltaSeconds * ConstantParameters::CAMERA_SPEED;
	}

	if ( !( m_isKeyDown[ 'T' ] || m_isKeyDown[ 'R' ] || m_isKeyDown[ 'P' ]) ) 
	{
		m_keyIsHeld = false;
	}
}

//----------------------------------------------------
void World::Update() 
{
	float currentTime = (float) Time::GetCurrentTimeSeconds();
	float deltaSeconds = ConstantParameters::DELTA_SECONDS; // Hack: assume 60 FPS

	UpdatePlayerFromController( deltaSeconds );
	UpdatePlayerFromInput( deltaSeconds );
	UpdateCameraFromInput( deltaSeconds );

	CheckAndResolveCollisions();

	if ( !m_isPaused ) 
	{
		m_player.Update();

		for ( unsigned int index = 0; index < m_enemies.size(); index++ )
		{
			m_enemies[index].Update(deltaSeconds);

			if ( m_enemies[index].m_readyToFire )
			{
				BeginEnemyShotPattern( m_enemies[index], deltaSeconds);
				m_enemies[index].m_lastShotTime = Time::GetCurrentTimeSeconds();
				m_enemies[index].m_readyToFire = false;
			}

			if ( m_enemies[index].m_isDead )
			{
				m_enemies.erase( m_enemies.begin() + index );
				index--;
			}
		}

		for ( unsigned int index = 0; index < m_bullets.size(); index++ )
		{
			m_bullets[index].Update(deltaSeconds);

			if ( m_bullets[index].m_isDead )
			{
				m_bullets.erase( m_bullets.begin() + index );
				index--;
			}
		}

		if ( (currentTime - m_lastGameStateUpdate) >= ConstantParameters::GAMESTATE_UPDATE_RATE )
		{
			UpdateGameStateBuffer();
		}
	}
}

//----------------------------------------------------
void World::Render() 
{	
	//m_renderer.SetModelViewProjectionMatrix(m_camera);

	glUseProgram(0);
	glDisable(GL_TEXTURE_2D);
	glDisable( GL_CULL_FACE );
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();

	m_renderer.SetOrthoMatrix( m_camera );
	m_renderer.SetTranslationMatrix( m_camera );

	m_player.Render();

	for (unsigned int index = 0; index < m_enemies.size(); index++)
	{
		m_enemies[index].Render();
	}

	for (unsigned int index = 0; index < m_bullets.size(); index++)
	{
		m_bullets[index].Render();
	}

	glPopMatrix();
	//m_renderer.SendCubeVBO();
	//m_renderer.PopMatrix();
}

//---------------------------------------------------
void World::BeginEnemyShotPattern( const Enemy &firingEnemy, float deltaSeconds )
{
	switch(firingEnemy.m_shotPattern)
	{
	case AISHOTPATTERN_SINGLEDIRECT:
		SpawnBullet( true, true, firingEnemy.m_bulletType, m_player.m_position, firingEnemy.m_position, Vector2(firingEnemy.m_shotSpeed, firingEnemy.m_shotSpeed), deltaSeconds );
		break;
	case AISHOTPATTERN_SPREAD:
		SpawnBullet( true, false, firingEnemy.m_bulletType, m_player.m_position, firingEnemy.m_position, Vector2(-firingEnemy.m_shotSpeed, -firingEnemy.m_shotSpeed), deltaSeconds );
		SpawnBullet( true, false, firingEnemy.m_bulletType, m_player.m_position, firingEnemy.m_position, Vector2(firingEnemy.m_shotSpeed, -firingEnemy.m_shotSpeed), deltaSeconds );
		SpawnBullet( true, false, firingEnemy.m_bulletType, m_player.m_position, firingEnemy.m_position, Vector2(0.f, -firingEnemy.m_shotSpeed), deltaSeconds );
		SpawnBullet( true, false, firingEnemy.m_bulletType, m_player.m_position, firingEnemy.m_position, Vector2(-firingEnemy.m_shotSpeed * 0.5f, -firingEnemy.m_shotSpeed), deltaSeconds );
		SpawnBullet( true, false, firingEnemy.m_bulletType, m_player.m_position, firingEnemy.m_position, Vector2(firingEnemy.m_shotSpeed * 0.5f, -firingEnemy.m_shotSpeed), deltaSeconds );
		break;
	case AISHOTPATTERN_HORIZONTAL:
		SpawnBullet( true, false, firingEnemy.m_bulletType, m_player.m_position, firingEnemy.m_position, Vector2(firingEnemy.m_shotSpeed, 0.f), deltaSeconds );
		SpawnBullet( true, false, firingEnemy.m_bulletType, m_player.m_position, firingEnemy.m_position, Vector2(-firingEnemy.m_shotSpeed, 0.f), deltaSeconds );
		break;
	case AISHOTPATTERN_VERTICAL:
		SpawnBullet( true, false, firingEnemy.m_bulletType, m_player.m_position, firingEnemy.m_position, Vector2(0.f, firingEnemy.m_shotSpeed), deltaSeconds );
		SpawnBullet( true, false, firingEnemy.m_bulletType, m_player.m_position, firingEnemy.m_position, Vector2(0.f, -firingEnemy.m_shotSpeed), deltaSeconds );
		break;
	}
}

//---------------------------------------------------
void World::SpawnBullet( bool FromEnemy, bool IsDirect, BulletType bulletType, Vector2 playerPosition, Vector2 enemyPosition, Vector2 initialVelocity, float deltaSeconds )
{
	m_bullets.push_back(Bullet());

	Bullet& newBullet = m_bullets[m_bullets.size()-1];

	newBullet.m_fromEnemy = FromEnemy;
	newBullet.m_bulletType = bulletType;
	newBullet.m_position = enemyPosition;

	if ( FromEnemy )
	{
		if (IsDirect)
		{
			newBullet.m_velocity = initialVelocity * Normalize( playerPosition - enemyPosition );
		}
		else 
		{
			newBullet.m_velocity = initialVelocity;
		}
	}
	else
	{
		newBullet.m_velocity = initialVelocity;
	}
	
}

//--------------------------------------------------
void World::CheckAndResolveCollisions()
{
	CheckAndResolveBulletCollisions();
	CheckAndResolvePlayerVsEnemyCollisions();
}

//--------------------------------------------------
void World::CheckAndResolveBulletCollisions()
{
	m_player.m_isGrazing = false;

	for (unsigned int bulletIndex = 0; bulletIndex < m_bullets.size(); bulletIndex++)
	{
		if ( m_bullets[bulletIndex].m_isDead )
		{
			continue;
		}

		if ( m_bullets[bulletIndex].m_fromEnemy )
		{
			if (m_player.m_isDead)
			{
				continue;
			}
			
			if ( m_player.CheckCollision( m_bullets[bulletIndex].m_position ) )
			{
				m_player.m_isDead = true;
				m_bullets[bulletIndex].m_isDead = true;
			}
		}
		else
		{
			for (unsigned int enemyIndex = 0; enemyIndex < m_enemies.size(); enemyIndex++)
			{
				if ( m_bullets[bulletIndex].m_isDead )
				{
					break;
				}
				
				if ( m_enemies[enemyIndex].m_isDead )
				{
					continue;
				}

				if ( m_enemies[enemyIndex].CheckCollision( m_bullets[bulletIndex].m_position ) )
				{
					m_enemies[enemyIndex].m_health -= 1.f;
					m_bullets[bulletIndex].m_isDead = true;
				}
			}
		}
	}
}

//--------------------------------------------------
void World::CheckAndResolvePlayerVsEnemyCollisions()
{



}

//--------------------------------------------------
void World::FirePlayerBullets( float deltaSeconds )
{
	if ( m_player.m_readyToFire )
	{
		SpawnBullet( false, false, BULLETTYPE_NORMAL, m_player.m_position, m_player.m_position, Vector2(0.f, 72.f), deltaSeconds);
		
		m_player.m_lastShotTime = Time::GetCurrentTimeSeconds();
		m_player.m_readyToFire = false;
	}
}

//----------------------------------------------------
void World::UpdatePlayerFromController( float deltaSeconds )
{
	XINPUT_STATE xboxControllerState;
	DWORD errorStatus = XInputGetState( 0, &xboxControllerState );
	if( errorStatus == ERROR_SUCCESS )
	{
		if ( ((xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0) && !m_isKeyDown[ VK_LEFT ] ) 
		{
			if ( !m_isPaused )
			{
				if((xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0)
				{
					if (m_player.m_position.x > -ConstantParameters::PLAYER_X_AXIS_LIMIT)
					{
						m_player.m_position.x -= deltaSeconds * ConstantParameters::PLAYER_FOCUS_SPEED;
					}
				}
				else
				{
					if (m_player.m_position.x > -ConstantParameters::PLAYER_X_AXIS_LIMIT)
					{
						m_player.m_position.x -= deltaSeconds * ConstantParameters::PLAYER_NORMAL_SPEED;
					}
				}
			}
		}

		if ( ((xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0) && !m_isKeyDown[ VK_RIGHT ] ) 
		{
			if ( !m_isPaused )
			{
				if((xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0)
				{
					if (m_player.m_position.x < ConstantParameters::PLAYER_X_AXIS_LIMIT)
					{
						m_player.m_position.x += deltaSeconds * ConstantParameters::PLAYER_FOCUS_SPEED;
					}
				}
				else
				{
					if (m_player.m_position.x < ConstantParameters::PLAYER_X_AXIS_LIMIT)
					{
						m_player.m_position.x += deltaSeconds * ConstantParameters::PLAYER_NORMAL_SPEED;
					}
				}
			}
		}

		if ( ((xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0) && !m_isKeyDown[ VK_UP ] ) 
		{
			if ( !m_isPaused )
			{
				if((xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0)
				{
					if (m_player.m_position.y < ConstantParameters::PLAYER_Y_AXIS_LIMIT)
					{
						m_player.m_position.y += deltaSeconds * ConstantParameters::PLAYER_FOCUS_SPEED;
					}
				}
				else
				{
					if (m_player.m_position.y < ConstantParameters::PLAYER_Y_AXIS_LIMIT)
					{
						m_player.m_position.y += deltaSeconds * ConstantParameters::PLAYER_NORMAL_SPEED;
					}
				}
			}
		}

		if ( ((xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0) && !m_isKeyDown[ VK_DOWN ] ) 
		{
			if ( !m_isPaused )
			{
				if((xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0)
				{
					if (m_player.m_position.y > -ConstantParameters::PLAYER_Y_AXIS_LIMIT)
					{
						m_player.m_position.y -= deltaSeconds * ConstantParameters::PLAYER_FOCUS_SPEED;
					}
				}
				else
				{
					if (m_player.m_position.y > -ConstantParameters::PLAYER_Y_AXIS_LIMIT)
					{
						m_player.m_position.y -= deltaSeconds * ConstantParameters::PLAYER_NORMAL_SPEED;
					}
				}
			}
			else
			{
				double currentTime = Time::GetCurrentTimeSeconds();
				if ( ( currentTime - m_lastGameStateSwitch ) >= ConstantParameters::GAMESTATE_REWIND_RATE )
				{
					LoadGameState( m_gameStateBuffer.size()-1 );
					m_lastGameStateSwitch = Time::GetCurrentTimeSeconds();
				}
			}
		}

		if( (xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0 )
		{
			if ( !m_isPaused )
			{
				FirePlayerBullets(deltaSeconds);
			}
		}

		if ( (xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0 )
		{
			if ( !m_isButtonHeld )
			{
				if ( m_isPaused )
				{
					UpdatePauseTimers();
				}
				else
				{
					m_lastPauseTimer = Time::GetCurrentTimeSeconds();
				}

				m_isPaused = !m_isPaused;
				m_isButtonHeld = true;
			}
		}
		else
		{
			m_isButtonHeld = false;
		}
	}
}

//----------------------------------------------------
void World::UpdatePauseTimers()
{
	double currentTime = Time::GetCurrentTimeSeconds();
	double timePaused = currentTime - m_lastPauseTimer;
	m_lastGameStateUpdate += timePaused;

	for ( unsigned int index = 0; index < m_enemies.size(); index++ )
	{
		currentTime = Time::GetCurrentTimeSeconds();
		timePaused = currentTime - m_lastPauseTimer;
		m_enemies[index].m_lastShotTime += timePaused;
	}
}

//-----------------------------------------------------
void World::UpdateGameStateBuffer()
{
	m_gameStateBuffer.push_back(GameState(m_player, m_enemies, m_bullets));
	m_lastGameStateUpdate = Time::GetCurrentTimeSeconds();

	while ( m_gameStateBuffer.size() > ConstantParameters::GAMESTATE_BUFFER_SIZE )
	{
		m_gameStateBuffer.erase( m_gameStateBuffer.begin() );
	}
}

//-----------------------------------------------------
void World::LoadGameState( unsigned int index )
{
	if ( m_gameStateBuffer.size() <= index )
	{
		return;
	}

	m_player = m_gameStateBuffer[index].m_player;
	m_bullets = m_gameStateBuffer[index].m_bullets;
	m_enemies = m_gameStateBuffer[index].m_enemies;

	m_gameStateBuffer.erase( m_gameStateBuffer.begin()+index, m_gameStateBuffer.end() );

	for ( unsigned int bufferIndex = 0; bufferIndex < m_gameStateBuffer.size(); bufferIndex++ )
	{
		for ( unsigned int index = 0; index < m_gameStateBuffer[bufferIndex].m_enemies.size(); index++ )
		{
			m_gameStateBuffer[bufferIndex].m_enemies[index].m_lastShotTime += ConstantParameters::GAMESTATE_UPDATE_RATE;
		}
	}
}
