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
	void Update();
	void Render();

	Vector2 m_position;
	AIShotType m_aiShotType;
	bool m_isDead;

	float m_bulletRadius;

	RGBA m_bulletColor;
};


#endif //included_Bullet