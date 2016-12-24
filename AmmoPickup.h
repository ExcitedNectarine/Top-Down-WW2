#pragma once

#include "Application.h"
#include "Weapons.h"

/**
* This class handles all pickups that can give ammo to the player.
*/
class AmmoPickup
{
public:
    AmmoPickup(const Weapon& weapon, const SDL_Rect& tile_rect);

    /**
    * This method draws the pickup to the screen.
    */
    void draw();

    /**
    * This method returns the amount of ammo the pickup will give.
    */
    int getAmmo();

    /**
    * This method returns the rect of the pickup.
    */
    const SDL_Rect& getRect();

    /**
    * This method returns the type of weapon the ammo is for.
    */
    const Weapon& getWeapon();

protected:
    SDL_Texture* texture;
    SDL_Rect rect;
    Weapon weapon;
    int ammo;
};

/**
* This class is for weapon pickups instead of ammo pickups. It uses
* different textures and ammo values so it needs to be its own class.
*/
class WeaponPickup : public AmmoPickup
{
public:
    WeaponPickup(const Weapon& weapon, const SDL_Rect& tile_rect);
};