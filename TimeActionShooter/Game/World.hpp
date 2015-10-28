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
#include "../Engine/Camera/Camera2D.hpp"
#include "../Engine/Time/Time.hpp"
#include "../Engine/Renderer/OpenGLRenderer.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Bullet.hpp"
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
	void Initialize();

	void Update();
	void Render();

	void CheckAndResolveCollisions();
	void CheckAndResolveBulletCollisions();
	void CheckAndResolvePlayerVsEnemyCollisions();

	void BeginEnemyShotPattern( const Enemy &firingEnemy );
	void FirePlayerBullets();
	void SpawnBullet( bool FromEnemy, AIShotType enemyShotType, Vector2 playerPosition, Vector2 enemyPosition, Vector2 initialVelocity );

	bool m_isKeyDown[ 256 ];
	bool m_keyIsHeld;

	bool m_isPaused;

	Camera2D m_camera;
	OpenGLRenderer m_renderer;

	float m_mousePositionXDifference;
	float m_mousePositionZDifference;

	Player m_player;
	std::vector<Enemy> m_enemies;
	std::vector<Bullet> m_bullets;
};

#endif //included_World