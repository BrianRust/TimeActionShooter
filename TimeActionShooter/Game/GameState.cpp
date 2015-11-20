#include "GameState.hpp"

//-----------------------------------

GameState::GameState( const Player &player, const std::vector<Enemy> &enemies, const std::vector<Bullet> &bullets, const std::vector<PowerUp> &powerUps, int stage, int timer  )
{
	m_player = player;
	m_enemies = enemies;
	m_bullets = bullets;
	m_powerUps = powerUps;

	m_stageNumber = stage;
	m_stageTimer = timer;
}
