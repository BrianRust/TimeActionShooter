#include "Enemy.hpp"
#include "../Engine/Renderer/OpenGLRenderer.hpp"

//------------------------------------------
Enemy::Enemy()
	: m_position(Vector2(0.f, 15.f))
	, m_health(100.f)
	, m_velocity(Vector2(0.f, 0.f))
	, m_shotPattern(AISHOTPATTERN_SPREAD)
	, m_bulletType(BULLETTYPE_NORMAL)
	, m_isDead(false)
	, m_playerRadius(1.5f)
	, m_playerHitBoxRadius(0.25f)
	, m_playerGrazeRadius(0.5f)
	, m_playerColor(RGBA(1.f, 0.f, 0.f, 1.f))
	, m_readyToFire(false)
	, m_delayTillNextShot(1.0)
	, m_lastShotTime(Time::GetCurrentTimeSeconds())
	, m_shotSpeed(20.f)
{	
	
}

//------------------------------------------
void Enemy::Update(float deltaSeconds)
{
	if ( m_health <= 0.f )
	{
		m_isDead = true;
	}
	
	if ( m_isDead )
	{
		m_readyToFire = false;
		return;
	}
	
	double currentTime = Time::GetCurrentTimeSeconds();

	if ( (currentTime - m_lastShotTime) > m_delayTillNextShot )
	{
		m_readyToFire = true;
	} 
}

//------------------------------------------
void Enemy::Render()
{
	if (m_isDead) 
	{
		return;
	}

	const float triangleSegments = 30.f;

	OpenGLRenderer::DrawTriangleFan( m_position, m_playerColor, m_playerRadius, triangleSegments );

	Vector2 position1 = m_position;
	Vector2 position2 = m_position;
	Vector2 position3 = m_position;
	Vector2 position4 = m_position;
	position1.x -= 1.5f;
	position1.y += 2.2f;
	position2.x -= 1.5f;
	position2.y += 1.8f;
	position3.x += 1.5f;
	position3.y += 1.8f;
	position4.x += 1.5f;
	position4.y += 2.2f;

	OpenGLRenderer::DrawQuad( position1, position2, position3, position4, RGBA(1.f, 0.f, 0.f, 1.f) );

	float lifeRatio = m_health * 0.01f;
	lifeRatio *= 3.f;

	position3.x = position1.x + lifeRatio;
	position4.x = position1.x + lifeRatio;

	OpenGLRenderer::DrawQuad( position1, position2, position3, position4, RGBA(0.f, 1.f, 0.f, 1.f) );
}

//------------------------------------------
bool Enemy::CheckCollision( const Vector2& entityPosition )
{
	float distanceBetweenPlayerAndEntity = VectorMagnitude( m_position - entityPosition );
	if ( distanceBetweenPlayerAndEntity < m_playerRadius )
	{
		return true;
	}
	else
	{
		return false;
	}
}

//-------------------------------------------------------
void Enemy::UpdatePosition( float deltaSeconds )
{
	m_position = m_position + ( m_velocity * deltaSeconds );
}
