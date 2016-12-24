#pragma once

#include "Level.h"
#include "Player.h"
#include "Projectile.h"

/**
* This class handles all enemy AI and movement. Other enemy classes
* should inherit from this.
*/
class Enemy
{
public:
    Enemy(const SDL_Rect& tile_rect, SDL_Texture* texture, SDL_Texture* dead_texture, const int health, const int speed);

    /**
    * This method draws the enemy to the screen.
    */
    void draw();

    /**
    * This method updates the enemy. It checks for collisions and moves the
    * enemy along its path to the player.
    */
    virtual void update(Level& level, Player& player, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<Projectile>& enemy_projectiles);

    /**
    * This method returns the enemy's rect.
    */
    const SDL_Rect& getRect();

    /**
    * This method damages the enemy.
    */
    void damage(const int damage);

    /**
    * This method determines if the enemy is dead and should be removed
    * from the game or not.
    */
    bool isDead();

    /**
    * This method returns the angle of the enemy.
    */
    int getAngle();

    /**
    * This method returns the texture of the enemy when the enemy dies.
    */
    SDL_Texture* getDeadTexture();

    /**
    * This method returns the weapon that the enemy uses.
    */
    const Weapon& getWeapon();

private:
    static constexpr float AI_TIME_MINIUM = 0.5;
    static constexpr float AI_TIME_MAXIMUM = 1.5;
    static const int NODE_SIZE = 10;
    int speed;

    SDL_Texture* texture;
    SDL_Texture* dead_texture;
    SDL_Rect node_rect;
    Mix_Chunk* shout;

    float ai_timer = 0.0;
    float ai_time;
    std::deque<SDL_Point> current_path;
    Tools::FloatVector movement;
    int x1, y1, x2, y2;
    int health;

protected:
    SDL_Rect rect;
    int angle = 0;
    bool facing_player = false;
    bool alerted = false;
    Weapon weapon;
};