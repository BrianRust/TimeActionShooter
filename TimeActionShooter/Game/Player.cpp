#include "Player.hpp"
#include "../Engine/Renderer/OpenGLRenderer.hpp"

//------------------------------------------
Player::Player()
	: m_position(Vector2(0.f, -18.f))
	, m_timeMeter(0.f)
	, m_shotPower(0)
	, m_shotPattern(PLAYERSHOTPATTERN_SINGLE)
	, m_isDead(false)
	, m_playerRadius(1.f)
	, m_playerHitBoxRadius(0.25f)
	, m_playerGrazeRadius(0.5f)
	, m_playerColor(RGBA(0.f, 1.f, 0.f, 1.f))
	, m_hitBoxColor(RGBA(1.f, 1.f, 1.f, 1.f))
{	

}

//------------------------------------------
void Player::Update()
{

}

//------------------------------------------
void Player::Render()
{
	if (m_isDead) {
		return;
	}
	
	const float triangleSegments = 30.f;

	OpenGLRenderer::DrawTriangleFan( m_position, m_playerColor, m_playerRadius, triangleSegments );

	OpenGLRenderer::DrawTriangleFan( m_position, m_hitBoxColor, m_playerHitBoxRadius, triangleSegments );
}

