#ifndef included_GameState
#define included_GameState
#pragma once

#include <vector>
#include "Bullet.hpp"
#include "Player.hpp"
#include "Enemy.hpp"

//-------------------------------------------------------------
class GameState
{
public:
	GameState(const Player &player, const std::vector<Enemy> &enemies, const std::vector<Bullet> &bullets);
	
	Player m_player;
	std::vector<Enemy> m_enemies;
	std::vector<Bullet> m_bullets;
};


#endif //included_Bullet