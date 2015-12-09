#ifndef included_Turret
#define included_Turret
#pragma once

#include "../Engine/Math/MyMathFiles.hpp"
#include "../Engine/Constants.hpp"

//-------------------------------------------------------------
class Turret
{
public:
	Turret();
	void Update(float deltaSeconds);
	void Render();
	void ChangeMaxHealth(float newHealth);
	void ChangeDelayTillNextShot(double newTime);
	bool CheckCollision( const Vector2& entityPosition );

	Vector2 m_position;

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

	double m_delayTillNextShot;
	double m_maxDelayTillNextShot;

	bool m_readyToFire;

	RGBA m_playerColor;
};


#endif //included_Turret