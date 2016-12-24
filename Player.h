#pragma once

#include "Application.h"
#include "Tools.h"
#include "Text.h"
#include "Weapons.h"
#include "Projectile.h"

class Level;
class Enemy;

/**
* This class represents the player. It handles everything directly related
* to the player such as movement, health and inventory.
*/
class Player
{
public:
    Player();

    /**
    * This method spaws the player. It basically sets all of its
    * variables.
    */
    void spawn();

    /**
    * This method sets the player to the centre of the tile. A tile
    * is represented by a rect.
    */
    void setTile(const SDL_Rect& tile_rect);

    /**
    * This method draws the player to the screen.
    */
    void draw();

    /**
    * This method updates the player. It moves the player, handles all player
    * collisions and makes the player look towards the mouse.
    */
    void update(Level& level, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<Projectile>& player_projectiles);

    /**
    * This method handles all key presses related to the player. It mostly
    * handles movement key presses.
    */
    void handleKeyPresses();

    /**
    * This method handles all key releases related to the player. It mostly
    * handles releases key presses.
    */
    void handleKeyReleases();

    /**
    * This method returns an SDL_Point of the centre of the player.
    */
    SDL_Point getCentre();

    /**
    * This method returns the player's rect.
    */
    const SDL_Rect& getRect();

    /**
    * This method damages the player.
    */
    void damage(const int damage);

    /**
    * This method determines if the player is dead.
    */
    bool isDead();

    /**
    * This method either makes the player start or stop shooting.
    */
    void setShooting(const bool shooting);

    /**
    * This method returns the angle the player is facing.
    */
    int getAngle();

    /**
    * This method changes the current weapon depending on how much the player
    * has scrolled using the scroll wheel.
    */
    void changeWeapon();

    /**
    * This method adds ammo to the ammo count of the weapon. It returns
    * whether or not any ammo has been added.
    */
    bool addAmmo(const Weapon& weapon, const int ammo);

    /**
    * Thie method returns whether or not the player has a certain weapon.
    */
    bool hasWeapon(const Weapon& weapon);

    /**
    * This method adds a weapon to the player's inventory.
    */
    void addWeapon(const Weapon& weapon);

    /**
    * This method adds health to the player. It returns whether or not any health
    * has been added.
    */
    bool addHealth(const int health);

    /**
    * This method stops the player's movement.
    */
    void stop();

private:
    /**
    * This method determines if the player can shoot or not.
    */
    bool canShoot();

private:
    static const int SPEED = 250;
    static const int MAX_HEALTH = 100;

    bool shooting;
    Weapon current_weapon;
    int weapon_index;
    std::vector<Weapon> weapons;
    float shoot_timer;
    std::map<Weapon, int> ammo = {
        { Weapon::Handgun, 25 },
        { Weapon::SubmachineGun, 0 },
        { Weapon::Rifle, 0 },
        { Weapon::Shotgun, 0 },
        { Weapon::AssaultRifle, 0 }
    };

    SDL_Texture* current_texture;
    SDL_Rect rect;
    SDL_Rect draw_rect;
    SDL_Point movement;

    int angle;
    int health;

    Text health_counter;
    Text ammo_counter;
    SDL_Texture* weapon_texture;
    SDL_Rect weapon_rect;
};