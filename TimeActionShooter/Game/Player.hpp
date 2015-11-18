#ifndef included_Player
#define included_Player
#pragma once

#include "../Engine/Math/MyMathFiles.hpp"
#include "../Engine/Constants.hpp"

//-------------------------------------------------------------
class Player
{
public:
	Player();
	void Update();
	void Render();

	bool CheckCollision( const Vector2& entityPosition );

	Vector2 m_position;
	float m_timeMeter;
	PlayerShotPattern m_shotPattern;
	bool m_isDead;
	bool m_isGrazing;
	bool m_readyToFire;

	float m_playerRadius;
	float m_playerHitBoxRadius;
	float m_playerGrazeRadius;

	double m_delayTillNextShot;
	double m_lastShotTime;

	RGBA m_playerColor;
	RGBA m_hitBoxColor;
};


#endif //included_Player