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
	, m_timeMeter(100.f)
{
	srand ((unsigned int)(time(NULL)));
}

//----------------------------------------------------
void World::Initialize() 
{
	m_lastCurrentTime = Time::GetCurrentTimeSeconds();
	m_stageNumber = 0;

	m_stageTimer = ConstantParameters::LEVEL_DELAY;
	
	SpawnEnemies();
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

		if ( !m_isPaused )
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
	double currentTime = Time::GetCurrentTimeSeconds();
	float deltaSeconds = ConstantParameters::DELTA_SECONDS; // Hack: assume 60 FPS

	deltaSeconds = currentTime - m_lastCurrentTime;

	UpdatePlayerFromController( deltaSeconds );
	UpdatePlayerFromInput( deltaSeconds );
	UpdateCameraFromInput( deltaSeconds );

	CheckAndResolveCollisions();

	if ( !m_isPaused ) 
	{
		m_player.Update();

		if ( !m_player.m_isDead )
		{
			if (m_player.m_position.y >= ConstantParameters::POWERUP_GRAB_LINE)
			{
				TriggerPowerUpLine();
			}
		}

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
				m_powerUps.push_back(PowerUp());
				m_powerUps[m_powerUps.size()-1].m_position = m_enemies[index].m_position;
				m_powerUps[m_powerUps.size()-1].m_position.x += m_enemies[index].m_playerRadius;

				m_powerUps.push_back(PowerUp());
				m_powerUps[m_powerUps.size()-1].m_position = m_enemies[index].m_position;
				m_powerUps[m_powerUps.size()-1].m_position.x -= m_enemies[index].m_playerRadius;
				m_powerUps[m_powerUps.size()-1].m_powerUpColor = RGBA(0.f, 0.f, 1.f, 1.f);
				m_powerUps[m_powerUps.size()-1].m_powerUpType = POWERUPTYPE_TIME;

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
			else
			{
				if ( m_bullets[index].m_bulletType == BULLETTYPE_SPLIT )
				{
					currentTime = Time::GetCurrentTimeSeconds();

					if ( m_bullets[index].m_splitTime <= 0.0 )
					{
						BeginSplitShot( m_bullets[index], deltaSeconds );
						m_bullets[index].m_splitTime = ConstantParameters::SPLIT_BULLET_FREQUENCY;
					} 
				}
			}
		}

		for (unsigned int index = 0; index < m_powerUps.size(); index++)
		{
			m_powerUps[index].Update(m_player.m_position, deltaSeconds);

			if ( m_powerUps[index].m_isDead )
			{
				m_powerUps.erase( m_powerUps.begin() + index );
				index--;
			}
		}

		if ( (currentTime - m_lastGameStateUpdate) >= ConstantParameters::GAMESTATE_UPDATE_RATE )
		{
			UpdateGameStateBuffer();
		}
	}

	if( m_enemies.empty() )
	{
		m_stageTimer -= (double) deltaSeconds;

		if (m_stageTimer <= 0.0)
		{
			m_stageNumber++;

			SpawnEnemies();
		}
	}

	m_lastCurrentTime = currentTime;
}

//----------------------------------------------------
void World::Render() 
{
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

	for (unsigned int index = 0; index < m_powerUps.size(); index++)
	{
		m_powerUps[index].Render();
	}

	RenderTimeMeter();

	RenderGrabLine();

	glPopMatrix();
}

//---------------------------------------------------
void World::BeginEnemyShotPattern( const Enemy &firingEnemy, float deltaSeconds )
{
	switch(firingEnemy.m_shotPattern)
	{
	case AISHOTPATTERN_SINGLEDIRECT:
		SpawnBullet( true, true, firingEnemy.m_bulletType, m_player.m_position, firingEnemy.m_position, Vector2(firingEnemy.m_shotSpeed, firingEnemy.m_shotSpeed), deltaSeconds );
		m_bullets[m_bullets.size()-1].m_splitPattern = firingEnemy.m_splitPattern;
		m_bullets[m_bullets.size()-1].m_splitBulletType = firingEnemy.m_splitBulletType;
		break;
	case AISHOTPATTERN_SPREAD:
		SpawnBullet( true, false, firingEnemy.m_bulletType, m_player.m_position, firingEnemy.m_position, Vector2(-firingEnemy.m_shotSpeed, -firingEnemy.m_shotSpeed), deltaSeconds );
		m_bullets[m_bullets.size()-1].m_splitPattern = firingEnemy.m_splitPattern;
		m_bullets[m_bullets.size()-1].m_splitBulletType = firingEnemy.m_splitBulletType;
		SpawnBullet( true, false, firingEnemy.m_bulletType, m_player.m_position, firingEnemy.m_position, Vector2(firingEnemy.m_shotSpeed, -firingEnemy.m_shotSpeed), deltaSeconds );
		m_bullets[m_bullets.size()-1].m_splitPattern = firingEnemy.m_splitPattern;
		m_bullets[m_bullets.size()-1].m_splitBulletType = firingEnemy.m_splitBulletType;
		SpawnBullet( true, false, firingEnemy.m_bulletType, m_player.m_position, firingEnemy.m_position, Vector2(0.f, -firingEnemy.m_shotSpeed), deltaSeconds );
		m_bullets[m_bullets.size()-1].m_splitPattern = firingEnemy.m_splitPattern;
		m_bullets[m_bullets.size()-1].m_splitBulletType = firingEnemy.m_splitBulletType;
		SpawnBullet( true, false, firingEnemy.m_bulletType, m_player.m_position, firingEnemy.m_position, Vector2(-firingEnemy.m_shotSpeed * 0.5f, -firingEnemy.m_shotSpeed), deltaSeconds );
		m_bullets[m_bullets.size()-1].m_splitPattern = firingEnemy.m_splitPattern;
		m_bullets[m_bullets.size()-1].m_splitBulletType = firingEnemy.m_splitBulletType;
		SpawnBullet( true, false, firingEnemy.m_bulletType, m_player.m_position, firingEnemy.m_position, Vector2(firingEnemy.m_shotSpeed * 0.5f, -firingEnemy.m_shotSpeed), deltaSeconds );
		m_bullets[m_bullets.size()-1].m_splitPattern = firingEnemy.m_splitPattern;
		m_bullets[m_bullets.size()-1].m_splitBulletType = firingEnemy.m_splitBulletType;
		break;
	case AISHOTPATTERN_HORIZONTAL:
		SpawnBullet( true, false, firingEnemy.m_bulletType, m_player.m_position, firingEnemy.m_position, Vector2(firingEnemy.m_shotSpeed, 0.f), deltaSeconds );
		m_bullets[m_bullets.size()-1].m_splitPattern = firingEnemy.m_splitPattern;
		m_bullets[m_bullets.size()-1].m_splitBulletType = firingEnemy.m_splitBulletType;
		SpawnBullet( true, false, firingEnemy.m_bulletType, m_player.m_position, firingEnemy.m_position, Vector2(-firingEnemy.m_shotSpeed, 0.f), deltaSeconds );
		m_bullets[m_bullets.size()-1].m_splitPattern = firingEnemy.m_splitPattern;
		m_bullets[m_bullets.size()-1].m_splitBulletType = firingEnemy.m_splitBulletType;
		break;
	case AISHOTPATTERN_VERTICAL:
		SpawnBullet( true, false, firingEnemy.m_bulletType, m_player.m_position, firingEnemy.m_position, Vector2(0.f, firingEnemy.m_shotSpeed), deltaSeconds );
		m_bullets[m_bullets.size()-1].m_splitPattern = firingEnemy.m_splitPattern;
		m_bullets[m_bullets.size()-1].m_splitBulletType = firingEnemy.m_splitBulletType;
		SpawnBullet( true, false, firingEnemy.m_bulletType, m_player.m_position, firingEnemy.m_position, Vector2(0.f, -firingEnemy.m_shotSpeed), deltaSeconds );
		m_bullets[m_bullets.size()-1].m_splitPattern = firingEnemy.m_splitPattern;
		m_bullets[m_bullets.size()-1].m_splitBulletType = firingEnemy.m_splitBulletType;
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
	CheckAndResolvePlayerVsPowerUpCollisions();
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
				m_timeMeter = m_timeMeter * 0.5;
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
		switch(m_player.m_shotPattern)
		{
		case PLAYERSHOTPATTERN_SINGLE:
			SpawnBullet( false, false, BULLETTYPE_NORMAL, m_player.m_position, m_player.m_position, Vector2(0.f, 72.f), deltaSeconds);
			break;
		case PLAYERSHOTPATTERN_TWIN:
			SpawnBullet( false, false, BULLETTYPE_NORMAL, m_player.m_position, Vector2( (m_player.m_position.x - ( m_player.m_playerRadius * 0.5f ) ), m_player.m_position.y ),  Vector2(0.f, 72.f), deltaSeconds);
			SpawnBullet( false, false, BULLETTYPE_NORMAL, m_player.m_position, Vector2( (m_player.m_position.x + ( m_player.m_playerRadius * 0.5f ) ), m_player.m_position.y ),  Vector2(0.f, 72.f), deltaSeconds);
			break;
		case PLAYERSHOTPATTERN_SPREAD:
			break;
		}
		
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
			else
			{
				m_isPaused = false;
			}
		}

		if((xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0)
		{
			if ( !m_isPaused )
			{
				m_isPaused = true;
				m_lastPauseTimer = Time::GetCurrentTimeSeconds();
			}

			double currentTime = Time::GetCurrentTimeSeconds();
			if ( ( currentTime - m_lastGameStateSwitch ) >= ConstantParameters::GAMESTATE_REWIND_RATE )
			{
				LoadGameState( m_gameStateBuffer.size()-1 );
				m_lastGameStateSwitch = Time::GetCurrentTimeSeconds();
			}
		}

		if ( (xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0 )
		{
			if ( !m_isButtonHeld )
			{
				if ( !m_isPaused )
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

//-----------------------------------------------------
void World::UpdateGameStateBuffer()
{
	m_gameStateBuffer.push_back(GameState(m_player, m_enemies, m_bullets, m_powerUps, m_stageNumber, m_stageTimer));
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

	if ( m_timeMeter <= 0.f )
	{
		return;
	}

	m_player = m_gameStateBuffer[index].m_player;
	m_bullets = m_gameStateBuffer[index].m_bullets;
	m_enemies = m_gameStateBuffer[index].m_enemies;
	m_powerUps = m_gameStateBuffer[index].m_powerUps;

	m_stageNumber = m_gameStateBuffer[index].m_stageNumber;
	m_stageTimer = m_gameStateBuffer[index].m_stageTimer;

	m_gameStateBuffer.erase( m_gameStateBuffer.begin()+index, m_gameStateBuffer.end() );

	m_timeMeter -= ConstantParameters::GAMESTATE_UPDATE_RATE;
}

//----------------------------------------------------
void World::RenderTimeMeter()
{
	Vector2 position1 = Vector2(20.f, 17.f);
	Vector2 position2 = Vector2(20.f, 16.f);
	Vector2 position3 = Vector2(28.f, 16.f);
	Vector2 position4 = Vector2(28.f, 17.f);

	OpenGLRenderer::DrawQuad( position1, position2, position3, position4, RGBA(0.f, 0.f, 0.f, 1.f) );
	
	if (m_timeMeter <= 0.f)
	{
		m_timeMeter = 0.f;
		return;
	}

	float timeRatio = m_timeMeter * 0.01f;
	timeRatio *= 8.f;

	position3.x = position1.x + timeRatio;
	position4.x = position1.x + timeRatio;

	OpenGLRenderer::DrawQuad( position1, position2, position3, position4, RGBA(0.f, 0.f, 1.f, 1.f) );
}

//----------------------------------------------------
void World::BeginSplitShot( const Bullet &splittingBullet, float deltaSeconds )
{
	switch(splittingBullet.m_splitPattern)
	{
	case AISHOTPATTERN_SINGLEDIRECT:
		SpawnBullet( true, true, splittingBullet.m_splitBulletType, m_player.m_position, splittingBullet.m_position, Vector2(splittingBullet.m_shotSpeed, splittingBullet.m_shotSpeed), deltaSeconds );
		m_bullets[m_bullets.size()-1].m_bulletColor = RGBA(0.8f, 0.f, 0.f, 1.f);
		break;
	case AISHOTPATTERN_SPREAD:
		SpawnBullet( true, false, splittingBullet.m_splitBulletType, m_player.m_position, splittingBullet.m_position, Vector2(-splittingBullet.m_shotSpeed, -splittingBullet.m_shotSpeed), deltaSeconds );
		m_bullets[m_bullets.size()-1].m_bulletColor = RGBA(0.8f, 0.f, 0.f, 1.f);
		SpawnBullet( true, false, splittingBullet.m_splitBulletType, m_player.m_position, splittingBullet.m_position, Vector2(splittingBullet.m_shotSpeed, -splittingBullet.m_shotSpeed), deltaSeconds );
		m_bullets[m_bullets.size()-1].m_bulletColor = RGBA(0.8f, 0.f, 0.f, 1.f);
		SpawnBullet( true, false, splittingBullet.m_splitBulletType, m_player.m_position, splittingBullet.m_position, Vector2(0.f, -splittingBullet.m_shotSpeed), deltaSeconds );
		m_bullets[m_bullets.size()-1].m_bulletColor = RGBA(0.8f, 0.f, 0.f, 1.f);
		SpawnBullet( true, false, splittingBullet.m_splitBulletType, m_player.m_position, splittingBullet.m_position, Vector2(-splittingBullet.m_shotSpeed * 0.5f, -splittingBullet.m_shotSpeed), deltaSeconds );
		m_bullets[m_bullets.size()-1].m_bulletColor = RGBA(0.8f, 0.f, 0.f, 1.f);
		SpawnBullet( true, false, splittingBullet.m_splitBulletType, m_player.m_position, splittingBullet.m_position, Vector2(splittingBullet.m_shotSpeed * 0.5f, -splittingBullet.m_shotSpeed), deltaSeconds );
		m_bullets[m_bullets.size()-1].m_bulletColor = RGBA(0.8f, 0.f, 0.f, 1.f);
		break;
	case AISHOTPATTERN_HORIZONTAL:
		SpawnBullet( true, false, splittingBullet.m_splitBulletType, m_player.m_position, splittingBullet.m_position, Vector2(splittingBullet.m_shotSpeed, 0.f), deltaSeconds );
		SpawnBullet( true, false, splittingBullet.m_splitBulletType, m_player.m_position, splittingBullet.m_position, Vector2(-splittingBullet.m_shotSpeed, 0.f), deltaSeconds );
		break;
	case AISHOTPATTERN_VERTICAL:
		SpawnBullet( true, false, splittingBullet.m_splitBulletType, m_player.m_position, splittingBullet.m_position, Vector2(0.f, splittingBullet.m_shotSpeed), deltaSeconds );
		m_bullets[m_bullets.size()-1].m_bulletColor = RGBA(0.8f, 0.f, 0.f, 1.f);
		SpawnBullet( true, false, splittingBullet.m_splitBulletType, m_player.m_position, splittingBullet.m_position, Vector2(0.f, -splittingBullet.m_shotSpeed), deltaSeconds );
		m_bullets[m_bullets.size()-1].m_bulletColor = RGBA(0.8f, 0.f, 0.f, 1.f);
		break;
	}
}

//----------------------------------------------------
void World::CheckAndResolvePlayerVsPowerUpCollisions()
{
	for (unsigned int powerUpIndex = 0; powerUpIndex < m_powerUps.size(); powerUpIndex++)
	{
		if ( m_powerUps[powerUpIndex].m_isDead )
		{
			continue;
		}

		if (m_player.m_isDead)
		{
			continue;
		}

		float distanceBetweenPlayerAndEntity = VectorMagnitude( m_player.m_position - m_powerUps[powerUpIndex].m_position );
		if ( distanceBetweenPlayerAndEntity < ( m_player.m_playerHitBoxRadius + m_powerUps[powerUpIndex].m_powerUpRadius ) )
		{
			m_powerUps[powerUpIndex].m_isDead = true;
			CollectPowerUp( m_powerUps[powerUpIndex].m_powerUpType );
		}
	}
}

//--------------------------------------------------
void World::CollectPowerUp( PowerUpType powerUp )
{
	switch(powerUp)
	{
	case POWERUPTYPE_POINTS:
		break;
	case POWERUPTYPE_TIME:
		m_timeMeter += ConstantParameters::POWERUP_TIME_BONUS;
		if ( m_timeMeter > 100.f )
		{
			m_timeMeter = 100.f;
		}
		break;
	case POWERUPTYPE_SHOT:
		
		switch(m_player.m_shotPattern)
		{
		case PLAYERSHOTPATTERN_SINGLE:
			m_player.m_shotPattern = PLAYERSHOTPATTERN_TWIN;
			break;
		case PLAYERSHOTPATTERN_TWIN:
			break;
		case PLAYERSHOTPATTERN_SPREAD:
			break;
		}

		break;
	}
}

//--------------------------------------------------
void World::RenderGrabLine()
{
	Vector2 position1 = Vector2(-ConstantParameters::SCREEN_X_AXIS_LIMIT, ConstantParameters::POWERUP_GRAB_LINE);
	Vector2 position2 = Vector2(ConstantParameters::SCREEN_X_AXIS_LIMIT, ConstantParameters::POWERUP_GRAB_LINE);

	OpenGLRenderer::DrawLine(position1, position2, RGBA(1.f, 1.f, 1.f, 1.f));
}

//--------------------------------------------------
void World::TriggerPowerUpLine()
{
	for (unsigned int index = 0; index < m_powerUps.size(); index++)
	{
		m_powerUps[index].m_isHeadingForPlayer = true;
	}
}

//--------------------------------------------------
void World::SpawnEnemies()
{
	switch(m_stageNumber)
	{
	case 0:
		m_enemies.push_back(Enemy());
		m_enemies[m_enemies.size()-1].m_position = Vector2(0.f, 15.f);
		m_enemies[m_enemies.size()-1].m_bulletType = BULLETTYPE_SPLIT;
		//m_enemies[m_enemies.size()-1].m_splitPattern = AISHOTPATTERN_SPREAD;
		//m_enemies[m_enemies.size()-1].m_splitPattern = AISHOTPATTERN_VERTICAL;
		m_enemies[m_enemies.size()-1].m_splitPattern = AISHOTPATTERN_SINGLEDIRECT;
		m_enemies[m_enemies.size()-1].m_splitBulletType = BULLETTYPE_DRAG;
		break;
	case 1:
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
		break;
	case 2:
		m_enemies.push_back(Enemy());
		m_enemies[m_enemies.size()-1].m_position = Vector2(0.f, 15.f);
		m_enemies[m_enemies.size()-1].m_bulletType = BULLETTYPE_SPLIT;
		//m_enemies[m_enemies.size()-1].m_splitPattern = AISHOTPATTERN_SPREAD;
		//m_enemies[m_enemies.size()-1].m_splitPattern = AISHOTPATTERN_VERTICAL;
		m_enemies[m_enemies.size()-1].m_splitPattern = AISHOTPATTERN_SINGLEDIRECT;
		m_enemies[m_enemies.size()-1].m_splitBulletType = BULLETTYPE_DRAG;
		break;
	case 3:
		break;
	}
}
