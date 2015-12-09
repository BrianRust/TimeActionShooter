#include "Bullet.hpp"
#include "../Engine/Renderer/OpenGLRenderer.hpp"

//------------------------------------------
const float OUT_OF_BOUNDS_NORTH = 20.f;
const float OUT_OF_BOUNDS_SOUTH = -20.f;
const float OUT_OF_BOUNDS_EAST = 34.f;
const float OUT_OF_BOUNDS_WEST = -34.f;

//------------------------------------------
Bullet::Bullet()
	: m_position(Vector2(0.f, 0.f))
	, m_velocity(Vector2(0.f, 0.f))
	, m_bulletType(BULLETTYPE_NORMAL)
	, m_isDead(false)
	, m_splitPattern(AISHOTPATTERN_SPREAD)
	, m_splitBulletType(BULLETTYPE_NORMAL)
	, m_bulletRadius(0.25f)
	, m_bulletColor(RGBA(0.f, 0.f, 0.f, 1.f))
	, m_fromEnemy(true)
	, m_shotSpeed(20.f)
{
	m_splitTime = ConstantParameters::SPLIT_BULLET_FREQUENCY;
	m_maxSplitTime = ConstantParameters::SPLIT_BULLET_FREQUENCY;
}

//------------------------------------------
void Bullet::Update(float deltaSeconds)
{
	CheckAndKillIfOutOfBounds();
	UpdatePosition(deltaSeconds);

	m_splitTime -= (double) deltaSeconds;
}

void Bullet::CheckAndKillIfOutOfBounds()
{
	if ( m_position.x > OUT_OF_BOUNDS_EAST )
	{
		m_isDead = true;
	}

	if ( m_position.x < OUT_OF_BOUNDS_WEST )
	{
		m_isDead = true;
	}

	if ( m_position.y > OUT_OF_BOUNDS_NORTH )
	{
		m_isDead = true;
	}

	if ( m_position.y < OUT_OF_BOUNDS_SOUTH )
	{
		m_isDead = true;
	}
}

//------------------------------------------
void Bullet::UpdatePosition(float deltaSeconds)
{
	m_position = m_position + ( m_velocity * deltaSeconds );

	if ( m_bulletType == BULLETTYPE_DRAG )
	{
		if (VectorMagnitude(m_velocity) > 2.f)
		{
			m_velocity = m_velocity * 0.99f;
		}
	}
}

//------------------------------------------
void Bullet::Render()
{
	if (m_isDead) 
	{
		return;
	}

	const float triangleSegments = 30.f;

	if (m_fromEnemy)
	{
		OpenGLRenderer::DrawTriangleFan( m_position, m_bulletColor, m_bulletRadius, triangleSegments );
	}
	else
	{
		OpenGLRenderer::DrawTriangleFan( m_position, RGBA(0.5f, 0.f, 0.5f, 1.f), m_bulletRadius, triangleSegments );
	}
	
}