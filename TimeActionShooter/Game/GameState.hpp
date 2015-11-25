#ifndef included_GameState
#define included_GameState
#pragma once

#include <vector>
#include "Bullet.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "PowerUp.hpp"

//-------------------------------------------------------------
class GameState
{
public:
	GameState( const Player &player, const std::vector<Enemy> &enemies, const std::vector<Bullet> &bullets, const std::vector<PowerUp> &powerUps, int stage, double timer );
	
	Player m_player;
	std::vector<Enemy> m_enemies;
	std::vector<Bullet> m_bullets;
	std::vector<PowerUp> m_powerUps;

	int m_stageNumber;
	double m_stageTimer;
};


#endif //included_Bullet