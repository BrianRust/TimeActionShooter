#ifndef included_Boss
#define included_Boss
#pragma once

#include "../Engine/Math/MyMathFiles.hpp"
#include "../Engine/Constants.hpp"

//-------------------------------------------------------------
class Boss
{
public:
	Boss();
	void Update(float deltaSeconds);
	void Render();

	void UpdatePosition(float deltaSeconds);
	bool CheckCollision( const Vector2& entityPosition );

	Vector2 m_position;
	Vector2 m_destination;
	Vector2 m_velocity;

	AIMovementPattern m_movementPattern;

	AIShotPattern m_shotPattern;
	BulletType m_bulletType;
	AIShotPattern m_splitPattern;
	BulletType m_splitBulletType;
	bool m_isDead;
	bool m_isFiring;

	float m_playerRadius;
	float m_playerHitBoxRadius;

	float m_health;
	float m_maxHealthRatio;

	float m_shotSpeed;
	float m_movementSpeed;

	double m_delayTillNextShot;
	double m_maxDelayTillNextShot;

	bool m_readyToFire;

	RGBA m_playerColor;

	int m_bossPhase;
};


#endif //included_Boss