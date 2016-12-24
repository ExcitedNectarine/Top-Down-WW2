#pragma once

#include "Enemy.h"

class SchutzstaffelSoldier : public Enemy
{
public:
    SchutzstaffelSoldier(const SDL_Rect& tile_rect);
    void update(Level& level, Player& player, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<Projectile>& enemy_projectiles) override;

private:
    static const int ATTACK_CHANCE = 3;
    float attack_timer = 0.0;
    float shooting_timer = 0.0;
    bool shooting = false;
};