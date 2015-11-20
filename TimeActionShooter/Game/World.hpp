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
#include "PowerUp.hpp"
#include "Bullet.hpp"
#include "GameState.hpp"

//-------------------------------------------------------------
class World
{
public:
	World();
	bool ProcessKeyDownEvent( unsigned char virtualKeyCode);
	bool ProcessKeyUpEvent( unsigned char virtualKeyCode);
	void UpdatePlayerFromInput( float deltaseconds );
	void UpdateCameraFromInput( float deltaseconds );
	void UpdatePlayerFromController( float deltaSeconds );
	void UpdateFromMouseInput();
	void Initialize();

	void Update();
	void UpdateGameStateBuffer();
	void LoadGameState( unsigned int index );

	void Render();
	void RenderTimeMeter();
	void RenderGrabLine();

	void CheckAndResolveCollisions();
	void CheckAndResolveBulletCollisions();
	void CheckAndResolvePlayerVsEnemyCollisions();
	void CheckAndResolvePlayerVsPowerUpCollisions();

	void BeginEnemyShotPattern( const Enemy &firingEnemy, float deltaSeconds );
	void BeginSplitShot( const Bullet &splittingBullet, float deltaSeconds );
	void FirePlayerBullets( float deltaSeconds );
	void SpawnBullet( bool FromEnemy, bool IsDirect, BulletType bulletType, Vector2 playerPosition, Vector2 enemyPosition, Vector2 initialVelocity, float deltaSeconds );

	void SpawnEnemies();

	void CollectPowerUp( PowerUpType powerUp );
	void TriggerPowerUpLine(  );

	bool m_isKeyDown[ 256 ];
	bool m_keyIsHeld;

	bool m_isButtonHeld;

	bool m_isPaused;
	double m_lastPauseTimer;
	double m_lastGameStateSwitch;

	double m_lastGameStateUpdate;
	
	double m_lastCurrentTime;

	Camera2D m_camera;
	OpenGLRenderer m_renderer;

	float m_mousePositionXDifference;
	float m_mousePositionZDifference;

	Player m_player;
	float m_timeMeter;
	std::vector<Enemy> m_enemies;
	std::vector<Bullet> m_bullets;
	std::vector<PowerUp> m_powerUps;
	std::vector<GameState> m_gameStateBuffer;

	int m_stageNumber;
	double m_stageTimer;
};

#endif //included_World