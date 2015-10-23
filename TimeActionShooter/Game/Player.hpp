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

	Vector2 m_position;
	float m_timeMeter;
	int m_shotPower;
	PlayerShotPattern m_shotPattern;
	bool m_isDead;

	float m_playerRadius;
	float m_playerHitBoxRadius;
	float m_playerGrazeRadius;

	RGBA m_playerColor;
	RGBA m_hitBoxColor;
};


#endif //included_Player