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
	void Update();
	void Render();

	Vector2 m_position;
	AIShotPattern m_shotPattern;
	AIShotType m_shotType;
	bool m_isDead;

	float m_playerRadius;
	float m_playerHitBoxRadius;
	float m_playerGrazeRadius;

	double m_timeTillFirstShot;
	double m_delayTillNextShot;
	double m_lastShotTime;

	bool m_readyToFire;

	RGBA m_playerColor;
};


#endif //included_Enemy