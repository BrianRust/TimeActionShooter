#include "Player.hpp"
#include "../Engine/Renderer/OpenGLRenderer.hpp"

//------------------------------------------
Player::Player()
	: m_position(Vector2(0.f, -15.f))
	, m_timeMeter(0.f)
	//, m_shotPattern(PLAYERSHOTPATTERN_SINGLE)
	, m_shotPattern(PLAYERSHOTPATTERN_TWIN)
	, m_isDead(false)
	, m_isGrazing(false)
	, m_playerRadius(1.f)
	, m_playerHitBoxRadius(0.2f)
	, m_playerGrazeRadius(0.65f)
	, m_playerColor(RGBA(0.f, 1.f, 0.f, 1.f))
	, m_hitBoxColor(RGBA(1.f, 1.f, 1.f, 1.f))
	, m_delayTillNextShot(0.02f)
	, m_lastShotTime(Time::GetCurrentTimeSeconds())
	, m_readyToFire(true)
{	

}

//------------------------------------------
void Player::Update()
{
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
void Player::Render()
{
	if (m_isDead) 
	{
		return;
	}

	const float triangleSegments = 30.f;
	
	if (m_isGrazing)
	{
		OpenGLRenderer::DrawTriangleFan( m_position, RGBA(0.f, 0.f, 1.f, 1.f), m_playerRadius, triangleSegments );
	}
	else
	{
		OpenGLRenderer::DrawTriangleFan( m_position, m_playerColor, m_playerRadius, triangleSegments );
	}
	
	OpenGLRenderer::DrawTriangleFan( m_position, m_hitBoxColor, m_playerHitBoxRadius, triangleSegments );
}

//-------------------------------------
bool Player::CheckCollision( const Vector2& entityPosition )
{
	float distanceBetweenPlayerAndEntity = VectorMagnitude( m_position - entityPosition );
	if ( distanceBetweenPlayerAndEntity < m_playerHitBoxRadius )
	{
		return true;
	}
	else
	{
		if ( distanceBetweenPlayerAndEntity < m_playerGrazeRadius )
		{
			m_isGrazing = true;
		}
		return false;
	}
}

