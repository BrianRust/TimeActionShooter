#include "World.hpp"

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
{
	srand ((unsigned int)(time(NULL)));

}

//----------------------------------------------------
void World::Initialize() 
{
	m_enemies.push_back(Enemy());
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
		m_isPaused = !m_isPaused;
	}
	
	if (m_isKeyDown[ VK_LEFT ]) 
	{
		m_player.m_position.x -= deltaSeconds * ConstantParameters::PLAYER_NORMAL_SPEED;
	}

	if (m_isKeyDown[ VK_RIGHT ]) 
	{
		m_player.m_position.x += deltaSeconds * ConstantParameters::PLAYER_NORMAL_SPEED;
	}

	if (m_isKeyDown[ VK_UP ]) 
	{
		m_player.m_position.y += deltaSeconds * ConstantParameters::PLAYER_NORMAL_SPEED;
	}

	if (m_isKeyDown[ VK_DOWN ]) 
	{
		m_player.m_position.y -= deltaSeconds * ConstantParameters::PLAYER_NORMAL_SPEED;
	}

	if (m_isKeyDown[ VK_SPACE ]) 
	{
		FirePlayerBullets();
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

	UpdatePlayerFromInput( deltaSeconds );
	UpdateCameraFromInput( deltaSeconds );
	//UpdateFromMouseInput();

	CheckAndResolveCollisions();

	if ( !m_isPaused ) 
	{
		m_player.Update();

		for ( unsigned int index = 0; index < m_enemies.size(); index++ )
		{
			m_enemies[index].Update();

			if ( m_enemies[index].m_readyToFire )
			{
				BeginEnemyShotPattern( m_enemies[index] );
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
void World::BeginEnemyShotPattern( const Enemy &firingEnemy )
{
	switch(firingEnemy.m_shotPattern)
	{
	case AISHOTPATTERN_SINGLEDIRECT:
		SpawnBullet( true, firingEnemy.m_shotType, m_player.m_position, firingEnemy.m_position, Vector2(3.2f, 3.2f) );
		break;
	}
}

//---------------------------------------------------
void World::SpawnBullet( bool FromEnemy, AIShotType enemyShotType, Vector2 playerPosition, Vector2 enemyPosition, Vector2 initialVelocity )
{
	m_bullets.push_back(Bullet());

	Bullet& newBullet = m_bullets[m_bullets.size()-1];

	newBullet.m_fromEnemy = FromEnemy;
	newBullet.m_aiShotType = enemyShotType;
	newBullet.m_position = enemyPosition;

	if ( FromEnemy )
	{
		newBullet.m_velocity = initialVelocity * Normalize( playerPosition - enemyPosition );
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
					//m_enemies[enemyIndex].m_isDead = true;
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
void World::FirePlayerBullets()
{
	if ( m_player.m_readyToFire )
	{
		SpawnBullet( false, AISHOTTYPE_NORMAL, m_player.m_position, m_player.m_position, Vector2(0.f, 18.f));
		
		m_player.m_lastShotTime = Time::GetCurrentTimeSeconds();
		m_player.m_readyToFire = false;
	}
}

