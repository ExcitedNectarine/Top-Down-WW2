#include "Soldier.h"

Soldier::Soldier(const SDL_Rect& tile_rect) : Enemy(tile_rect, Application::getTexture("Resources/Images/Soldier.png"), Application::getTexture("Resources/Images/DeadSoldier.png"), 60, 150)
{
    weapon = Weapon::SubmachineGun;
}

void Soldier::update(Level& level, Player& player, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<Projectile>& enemy_projectiles)
{
    Enemy::update(level, player, enemies, enemy_projectiles);

    if (alerted)
    {
        shooting_timer += Application::getDeltaTime();
        if (shooting_timer >= 1.0)
        {
            shooting_timer = 0.0;
            if (facing_player)
            {
                int chance = Tools::randomInt(0, ATTACK_CHANCE);
                if (!chance && !shooting)
                {
                    shooting = true;
                }
                else if (!chance && shooting)
                {
                    shooting = false;
                }
            }
        }

        if (!facing_player && shooting)
        {
            shooting = false;
        }

        attack_timer += Application::getDeltaTime();
        if (shooting && attack_timer >= WEAPON_DELAYS.at(weapon))
        {
            attack_timer = 0.0;
            SDL_Point centre = { rect.x + (rect.w / 2), rect.y + (rect.h / 2) };

            Mix_PlayChannel(-1, Application::getSound(GUN_SOUNDS.at(weapon)), 0);
            enemy_projectiles.emplace_back(weapon, centre, angle);
        }
    }
}