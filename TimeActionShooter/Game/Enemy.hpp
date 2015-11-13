#ifndef included_Enemy
#define included_Enemy
#pragma once

#include "../Engine/Math/MyMathFiles.hpp"
#include "../Engine/Constants.hpp"

//-------------------------------------------------------------
class Enemy
{
public:
	Enemy();
	void Update(float deltaSeconds);
	void Render();

	void UpdatePosition(float deltaSeconds);

	bool CheckCollision( const Vector2& entityPosition );

	Vector2 m_position;
	Vector2 m_velocity;
	AIShotPattern m_shotPattern;
	BulletType m_bulletType;
	AIShotPattern m_splitPattern;
	BulletType m_splitBulletType;
	bool m_isDead;

	float m_playerRadius;
	float m_playerHitBoxRadius;
	float m_playerGrazeRadius;

	float m_health;

	float m_shotSpeed;

	double m_delayTillNextShot;
	double m_lastShotTime;

	bool m_readyToFire;

	RGBA m_playerColor;
};


#endif //included_Enemy