#ifndef included_Boss
#define included_Boss
#pragma once

#include "../Engine/Math/MyMathFiles.hpp"
#include "../Engine/Constants.hpp"
#include "Turret.hpp"

//-------------------------------------------------------------
class Boss
{
public:
	Boss();
	void Update(float deltaSeconds);
	void Render();

	void UpdatePosition(float deltaSeconds);
	void UpdateBossPhase();
	bool CheckCollision( const Vector2& entityPosition );

	Turret m_bossParts[4];

	Vector2 m_position;
	Vector2 m_destination;
	Vector2 m_velocity;

	AIMovementPattern m_movementPattern;

	AIShotPattern m_shotPattern;
	BulletType m_bulletType;
	AIShotPattern m_splitPattern;
	BulletType m_splitBulletType;
	bool m_isDead;
	bool m_isVulnerable;
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
	int m_livingTurrets;
};


#endif //included_Boss