#ifndef included_PowerUp
#define included_PowerUp
#pragma once

#include "../Engine/Math/MyMathFiles.hpp"
#include "../Engine/Constants.hpp"

//-------------------------------------------------------------
class PowerUp
{
public:
	PowerUp();
	void Update(float deltaSeconds);
	void Render();

	void CheckAndKillIfOutOfBounds();
	void UpdatePosition(float deltaSeconds);

	Vector2 m_position;
	Vector2 m_velocity;

	float m_powerUpRadius;

	bool m_isDead;

	PowerUpType m_powerUpType;
	RGBA m_powerUpColor;
};


#endif //included_PowerUp