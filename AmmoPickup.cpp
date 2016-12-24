#include "AmmoPickup.h"

AmmoPickup::AmmoPickup(const Weapon& weapon, const SDL_Rect& tile_rect)
{
    this->weapon = weapon;

    texture = Application::getTexture(AMMO_TEXTURES.at(this->weapon));
    SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
    rect.x = (tile_rect.x + (tile_rect.w / 2)) - rect.w / 2;
    rect.y = (tile_rect.y + (tile_rect.h / 2)) - rect.h / 2;

    ammo = AMMO_PICKUP_AMOUNT.at(this->weapon);
}

/**
* This method draws the pickup to the screen.
*/
void AmmoPickup::draw()
{
    SDL_Rect draw_rect = Application::applyCamera(rect);
    SDL_RenderCopy(Application::getRenderer(), texture, nullptr, &draw_rect);
}

/**
* This method returns the amount of ammo the pickup will give.
*/
int AmmoPickup::getAmmo()
{
    return ammo;
}

/**
* This method returns the rect of the pickup.
*/
const SDL_Rect& AmmoPickup::getRect()
{
    return rect;
}

/**
* This method returns the type of weapon the ammo is for.
*/
const Weapon& AmmoPickup::getWeapon()
{
    return weapon;
}

WeaponPickup::WeaponPickup(const Weapon& weapon, const SDL_Rect& tile_rect) : AmmoPickup(weapon, tile_rect)
{
    texture = Application::getTexture(WEAPON_TEXTURES.at(weapon));
    SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
    rect.x = (tile_rect.x + (tile_rect.w / 2)) - rect.w / 2;
    rect.y = (tile_rect.y + (tile_rect.h / 2)) - rect.h / 2;
    ammo = WEAPON_PICKUP_AMOUNT.at(this->weapon);
}