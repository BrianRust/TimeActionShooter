#ifndef included_Bullet
#define included_Bullet
#pragma once

#include "../Engine/Math/MyMathFiles.hpp"
#include "../Engine/Constants.hpp"

//-------------------------------------------------------------
class Bullet
{
public:
	Bullet();
	void Update(float deltaSeconds);
	void Render();

	void CheckAndKillIfOutOfBounds();
	void UpdatePosition(float deltaSeconds);

	Vector2 m_position;
	Vector2 m_velocity;
	BulletType m_bulletType;
	bool m_isDead;
	bool m_fromEnemy;

	float m_bulletRadius;



	RGBA m_bulletColor;
};


#endif //included_Bullet