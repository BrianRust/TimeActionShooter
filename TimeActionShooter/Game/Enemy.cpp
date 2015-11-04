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
