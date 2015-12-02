#include "PowerUp.hpp"
#include "../Engine/Renderer/OpenGLRenderer.hpp"

//------------------------------------------
const float OUT_OF_BOUNDS_SOUTH = -20.f;
const float OUT_OF_BOUNDS_EAST = 34.f;
const float OUT_OF_BOUNDS_WEST = -34.f;
const float INITIAL_VERTICAL_VELOCITY = 10.f;
const float HEAD_TOWARD_PLAYER_SPEED = 50.f;

//------------------------------------------
PowerUp::PowerUp()
	: m_position(Vector2(0.f, 0.f))
	, m_velocity(Vector2(0.f, INITIAL_VERTICAL_VELOCITY))
	, m_powerUpType(POWERUPTYPE_SHOT)
	, m_isDead(false)
	, m_powerUpRadius(0.8f)
	, m_powerUpColor(RGBA(1.f, 0.f, 1.f, 1.f))
	, m_isHeadingForPlayer(false)
{
	
}

//------------------------------------------
void PowerUp::Update( const Vector2 &playerPosition, float deltaSeconds )
{
	CheckAndKillIfOutOfBounds();

	if ( m_isHeadingForPlayer )
	{
		UpdateVectorTowardsPlayer(playerPosition);
	}
	UpdatePosition(deltaSeconds);
}

void PowerUp::CheckAndKillIfOutOfBounds()
{
	if ( m_position.x > OUT_OF_BOUNDS_EAST )
	{
		m_isDead = true;
	}

	if ( m_position.x < OUT_OF_BOUNDS_WEST )
	{
		m_isDead = true;
	}

	if ( m_position.y < OUT_OF_BOUNDS_SOUTH )
	{
		m_isDead = true;
	}
}

//------------------------------------------
void PowerUp::UpdatePosition( float deltaSeconds )
{
	m_position = m_position + ( m_velocity * deltaSeconds );
	
	if ( !m_isHeadingForPlayer )
	{
		if ( m_velocity.y > -10.f )
		{
			m_velocity.y -= ConstantParameters::GRAVITY * deltaSeconds;
		}
	}
}

//------------------------------------------
void PowerUp::Render()
{
	if (m_isDead) 
	{
		return;
	}

	Vector2 position1 = Vector2( ( m_position.x - m_powerUpRadius ), ( m_position.y + m_powerUpRadius ) );
	Vector2 position2 = Vector2( ( m_position.x - m_powerUpRadius ), ( m_position.y - m_powerUpRadius ) );
	Vector2 position3 = Vector2( ( m_position.x + m_powerUpRadius ), ( m_position.y - m_powerUpRadius ) );
	Vector2 position4 = Vector2( ( m_position.x + m_powerUpRadius ), ( m_position.y + m_powerUpRadius ) );

	OpenGLRenderer::DrawQuad( position1, position2, position3, position4, m_powerUpColor );

}

//------------------------------------------
void PowerUp::UpdateVectorTowardsPlayer( const Vector2 &playerPosition )
{
	m_velocity = playerPosition - m_position;
	m_velocity = Normalize(m_velocity);
	m_velocity = m_velocity * HEAD_TOWARD_PLAYER_SPEED;
}
