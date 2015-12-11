#include "Boss.hpp"
#include "../Engine/Renderer/OpenGLRenderer.hpp"

//------------------------------------------
const double DELAY_TILL_NEXT_SHOT = 1.0;
const float OUT_OF_BOUNDS_NORTH = 23.f;
const float OUT_OF_BOUNDS_SOUTH = -23.f;
const float OUT_OF_BOUNDS_EAST = 37.f;
const float OUT_OF_BOUNDS_WEST = -37.f;

//------------------------------------------
Boss::Boss()
	: m_position(Vector2(0.f, 30.f))
	, m_health(100.f)
	, m_velocity(Vector2(0.f, 0.f))
	, m_shotPattern(AISHOTPATTERN_SPREAD)
	, m_bulletType(BULLETTYPE_NORMAL)
	, m_splitPattern(AISHOTPATTERN_SPREAD)
	, m_splitBulletType(BULLETTYPE_NORMAL)
	, m_isDead(false)
	, m_playerRadius(3.f)
	, m_playerHitBoxRadius(0.25f)
	, m_playerColor(RGBA(1.f, 1.f, 1.f, 1.f))
	, m_readyToFire(false)
	, m_delayTillNextShot(DELAY_TILL_NEXT_SHOT)
	, m_shotSpeed(20.f)
	, m_movementPattern(AIMOVEMENTPATTERN_DESTINATION)
	, m_movementSpeed(5.f)
	, m_isFiring(false)
	, m_destination(Vector2(0.f, 11.f))
	, m_isVulnerable(false)
	, m_livingTurrets(4)
	, m_bossPhase(0)
{	
	m_maxHealthRatio = 1.f/m_health;
	m_maxDelayTillNextShot = m_delayTillNextShot;

	m_bossParts[0] = Turret();
	m_bossParts[0].m_playerColor = RGBA(0.f, 1.f, 0.f, 1.f);
	m_bossParts[1] = Turret();
	m_bossParts[1].m_playerColor = RGBA(0.f, 0.f, 1.f, 1.f);
	m_bossParts[2] = Turret();
	m_bossParts[2].m_playerColor = RGBA(1.f, 1.f, 0.f, 1.f);
	m_bossParts[3] = Turret();
	m_bossParts[3].m_playerColor = RGBA(0.f, 1.f, 1.f, 1.f);

	UpdateBossPhase();
}

//------------------------------------------
void Boss::Update(float deltaSeconds)
{
	int TurretCount = 0;
	
	UpdatePosition( deltaSeconds );

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

	if ( m_bossParts > 0 )
	{
		for ( unsigned int counter = 0; counter < 4; counter++ )
		{
			m_bossParts[counter].Update( deltaSeconds );

			if ( !m_bossParts[counter].m_isDead )
			{
				TurretCount++;
			}
		}
	}

	if ( m_livingTurrets != TurretCount )
	{
		m_bossPhase++;
		m_livingTurrets = TurretCount;
		UpdateBossPhase();
	}
}

//------------------------------------------
void Boss::Render()
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

	if ( m_isVulnerable )
	{
		OpenGLRenderer::DrawQuad( position1, position2, position3, position4, RGBA(1.f, 0.f, 0.f, 1.f) );

		float lifeRatio = m_health * m_maxHealthRatio;
		lifeRatio *= 3.f;

		position3.x = position1.x + lifeRatio;
		position4.x = position1.x + lifeRatio;

		OpenGLRenderer::DrawQuad( position1, position2, position3, position4, RGBA(0.f, 1.f, 0.f, 1.f) );
	}

	for ( unsigned int counter = 0; counter < 4; counter++ )
	{
		m_bossParts[counter].Render();
	}
}

//------------------------------------------
bool Boss::CheckCollision( const Vector2& entityPosition )
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

//-------------------------------------------------------
void Boss::UpdatePosition( float deltaSeconds )
{
	switch( m_movementPattern )
	{
		case AIMOVEMENTPATTERN_STILL:
			m_velocity = Vector2(0.f, 0.f);
			break;
		case AIMOVEMENTPATTERN_LEFTTORIGHT:
			m_velocity = Vector2(1.f, 0.f);
			break;
		case AIMOVEMENTPATTERN_RIGHTTOLEFT:
			m_velocity = Vector2(-1.f, 0.f);
			break;
		case AIMOVEMENTPATTERN_DOWN:
			m_velocity = Vector2(0.f, -1.f);
			break;
		case AIMOVEMENTPATTERN_DESTINATION:
			if ( VectorMagnitude( ( m_destination - m_position ) ) <= ( deltaSeconds * m_movementSpeed ) )
			{
				m_position = m_destination;
				m_movementPattern = AIMOVEMENTPATTERN_STILL;
			}
			else
			{
				m_velocity = Normalize( (m_destination - m_position) );
			}
			break;
	}

	m_position = m_position + ( m_velocity * deltaSeconds * m_movementSpeed );

	m_bossParts[0].m_position = m_position + Vector2( -4.f, -4.f );
	m_bossParts[1].m_position = m_position + Vector2( 4.f, -4.f );
	m_bossParts[2].m_position = m_position + Vector2( -2.97f, 4.f );
	m_bossParts[3].m_position = m_position + Vector2( 2.97f, 4.f );
}

//---------------------------------------------------
void Boss::UpdateBossPhase()
{
	switch( m_bossPhase )
	{
		case 0:
			m_bossParts[0].m_isFiring = true;
			m_bossParts[0].m_isVulnerable = true;
			//m_bossParts[0].m_maxDelayTillNextShot = 2.0;
			m_bossParts[0].m_shotPattern = AISHOTPATTERN_SINGLEDIRECT;
			m_bossParts[0].m_bulletType = BULLETTYPE_SPLIT;
			m_bossParts[0].m_splitPattern = AISHOTPATTERN_CROSS;
			m_bossParts[0].m_splitBulletType = BULLETTYPE_DRAG;

			m_bossParts[1].m_isFiring = true;
			m_bossParts[1].m_isVulnerable = true;
			//m_bossParts[1].m_maxDelayTillNextShot = 2.0;
			m_bossParts[1].m_shotPattern = AISHOTPATTERN_SINGLEDIRECT;
			m_bossParts[1].m_bulletType = BULLETTYPE_SPLIT;
			m_bossParts[1].m_splitPattern = AISHOTPATTERN_X;
			m_bossParts[1].m_splitBulletType = BULLETTYPE_DRAG;
			break;
		case 1:
			break;
		case 2:
			m_bossParts[2].m_isFiring = true;
			m_bossParts[2].m_isVulnerable = true;
			m_bossParts[2].m_maxDelayTillNextShot = 2.0;
			m_bossParts[2].m_shotPattern = AISHOTPATTERN_HORIZONTAL;
			m_bossParts[2].m_bulletType = BULLETTYPE_SPLIT;
			m_bossParts[2].m_splitPattern = AISHOTPATTERN_SINGLEDIRECT;
			m_bossParts[2].m_splitBulletType = BULLETTYPE_NORMAL;

			m_bossParts[3].m_isFiring = true;
			m_bossParts[3].m_isVulnerable = true;
			m_bossParts[3].m_maxDelayTillNextShot = 2.0;
			m_bossParts[3].m_shotPattern = AISHOTPATTERN_HORIZONTAL;
			m_bossParts[3].m_bulletType = BULLETTYPE_SPLIT;
			m_bossParts[3].m_splitPattern = AISHOTPATTERN_SINGLEDIRECT;
			m_bossParts[3].m_splitBulletType = BULLETTYPE_NORMAL;
			break;
		case 3:
			m_bossParts[2].m_maxDelayTillNextShot = 0.4;
			m_bossParts[2].m_splitBulletType = BULLETTYPE_NORMAL;
			m_bossParts[3].m_maxDelayTillNextShot = 0.4;
			m_bossParts[3].m_splitBulletType = BULLETTYPE_NORMAL;
			break;
		case 4:
			m_isFiring = true;
			m_isVulnerable = true;
			m_maxDelayTillNextShot = 0.2;
			m_shotPattern = AISHOTPATTERN_SPREAD;
			m_bulletType = BULLETTYPE_SPLIT;
			m_splitPattern = AISHOTPATTERN_X;
			m_splitBulletType = BULLETTYPE_DRAG;
			break;
	}
}
