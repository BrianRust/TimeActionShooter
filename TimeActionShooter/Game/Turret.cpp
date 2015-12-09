#include "Turret.hpp"
#include "../Engine/Renderer/OpenGLRenderer.hpp"

//------------------------------------------
const double DELAY_TILL_NEXT_SHOT = 1.0;

//------------------------------------------
Turret::Turret()
	: m_position(Vector2(0.f, 15.f))
	, m_health(200.f)
	, m_shotPattern(AISHOTPATTERN_SPREAD)
	, m_bulletType(BULLETTYPE_NORMAL)
	, m_splitPattern(AISHOTPATTERN_SPREAD)
	, m_splitBulletType(BULLETTYPE_NORMAL)
	, m_isDead(false)
	, m_playerRadius(3.f)
	, m_playerHitBoxRadius(0.25f)
	, m_playerColor(RGBA(1.f, 0.f, 0.f, 1.f))
	, m_readyToFire(false)
	, m_delayTillNextShot(DELAY_TILL_NEXT_SHOT)
	, m_shotSpeed(20.f)
	, m_isFiring(false)
	, m_isVulnerable(false)
{	
	m_maxHealthRatio = 1.f/m_health;
	m_maxDelayTillNextShot = m_delayTillNextShot;
}

//------------------------------------------
void Turret::Update(float deltaSeconds)
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

	m_delayTillNextShot -= (double) deltaSeconds;

	if ( ( m_delayTillNextShot <= 0.0 ) && ( m_isFiring ) )
	{
		m_readyToFire = true;
		m_delayTillNextShot = m_maxDelayTillNextShot;
	}
}

//------------------------------------------
void Turret::Render()
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

	if ( m_isVulnerable )
	{
		position1.x -= 1.5f;
		position1.y += 2.2f;
		position2.x -= 1.5f;
		position2.y += 1.8f;
		position3.x += 1.5f;
		position3.y += 1.8f;
		position4.x += 1.5f;
		position4.y += 2.2f;

		OpenGLRenderer::DrawQuad( position1, position2, position3, position4, RGBA(1.f, 0.f, 0.f, 1.f) );

		float lifeRatio = m_health * m_maxHealthRatio;
		lifeRatio *= 3.f;

		position3.x = position1.x + lifeRatio;
		position4.x = position1.x + lifeRatio;

		OpenGLRenderer::DrawQuad( position1, position2, position3, position4, RGBA(0.f, 1.f, 0.f, 1.f) );
	}
}

//------------------------------------------
bool Turret::CheckCollision( const Vector2& entityPosition )
{
	if ( m_isDead )
	{
		return false;
	}
	
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

//----------------------------------------------------
void Turret::ChangeMaxHealth( float newHealth )
{
	m_health = newHealth;
	m_maxHealthRatio = 1.f/m_health;
}

//----------------------------------------------------
void Turret::ChangeDelayTillNextShot( double newTime )
{
	m_maxDelayTillNextShot = newTime;
}
