#include "Enemy.hpp"
#include "../Engine/Renderer/OpenGLRenderer.hpp"

//------------------------------------------
Enemy::Enemy()
	: m_position(Vector2(0.f, 18.f))
	, m_shotPattern(AISHOTPATTERN_SINGLEDIRECT)
	, m_shotType(AISHOTTYPE_NORMAL)
	, m_isDead(false)
	, m_playerRadius(1.f)
	, m_playerHitBoxRadius(0.25f)
	, m_playerGrazeRadius(0.5f)
	, m_playerColor(RGBA(1.f, 0.f, 0.f, 1.f))
	, m_readyToFire(false)
	, m_timeTillFirstShot(3.0)
	, m_delayTillNextShot(4.0)
	, m_lastShotTime(Time::GetCurrentTimeSeconds())
{	
	
}

//------------------------------------------
void Enemy::Update()
{
	if ( m_isDead )
	{
		m_readyToFire = false;
		return;
	}
	
	double currentTime = Time::GetCurrentTimeSeconds();

	if ( (currentTime - m_lastShotTime) > m_timeTillFirstShot )
	{
		m_readyToFire = true;
		m_timeTillFirstShot = 1000000000;
	} 
	else if ( (currentTime - m_lastShotTime) > m_delayTillNextShot )
	{
		m_readyToFire = true;
	}
}

//------------------------------------------
void Enemy::Render()
{
	if (m_isDead) {
		return;
	}

	const float triangleSegments = 30.f;

	OpenGLRenderer::DrawTriangleFan( m_position, m_playerColor, m_playerRadius, triangleSegments );
}