#include "Bullet.hpp"
#include "../Engine/Renderer/OpenGLRenderer.hpp"

//------------------------------------------
Bullet::Bullet()
	: m_position(Vector2(0.f, 0.f))
	, m_aiShotType(AISHOTTYPE_NORMAL)
	, m_isDead(false)
	, m_bulletRadius(0.1f)
	, m_bulletColor(RGBA(1.f, 0.f, 0.f, 1.f))
{

}

//------------------------------------------
void Bullet::Update()
{

}

//------------------------------------------
void Bullet::Render()
{
	if (m_isDead) {
		return;
	}

	const float triangleSegments = 30.f;

	OpenGLRenderer::DrawTriangleFan( m_position, m_bulletColor, m_bulletRadius, triangleSegments );
}