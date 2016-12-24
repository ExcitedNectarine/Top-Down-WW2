#pragma once

#include "Enemy.h"

class Guard : public Enemy
{
public:
    Guard(const SDL_Rect& tile_rect);
    void update(Level& level, Player& player, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<Projectile>& enemy_projectiles) override;

private:
    static const int ATTACK_CHANCE = 1;
    float attack_timer = 0.0;
};

