#pragma once

#include "Level.h"
#include "Player.h"
#include "Guard.h"
#include "Soldier.h"
#include "Officer.h"
#include "SchutzstaffelSoldier.h"
#include "Projectile.h"
#include "AmmoPickup.h"
#include "HealthPickup.h"
#include <memory>
#include <experimental\filesystem>

/**
* This state is the state in which the actual game runs. It handles the levels,
* the enemies, and more.
*/
class GameState : public Application::BaseState
{
public:
    void startUp();
    void handleEvents();
    void update();
    void draw();
    void shutDown();

private:
    void setLevel();

private:
    static constexpr float BFS_UPDATE_TIME = 0.5;
    float ai_timer = 0.0;
    int level_num;

    Level level;
    Player player;
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<HealthPickup> health_pickups;
    std::vector<AmmoPickup> ammo_pickups;
    std::vector<WeaponPickup> weapon_pickups;
    std::vector<Projectile> player_projectiles;
    std::vector<Projectile> enemy_projectiles;
    std::vector<std::tuple<SDL_Texture*, SDL_Rect, int>> bodies;
    std::pair<SDL_Texture*, SDL_Rect> exit;
    SDL_Cursor* cursor;
    Mix_Chunk* death_sound;
    Mix_Chunk* health_pickup_sound;
    Mix_Chunk* ammo_pickup_sound;
    Mix_Chunk* weapon_pickup_sound;
};