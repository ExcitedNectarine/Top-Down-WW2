#include "Projectile.h"

Projectile::Projectile(const Weapon& weapon, const SDL_Point& start, const int direction)
{
    this->weapon = weapon;
    texture = Application::getTexture(PROJECTILE_TEXTURES.at(weapon));

    SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
    rect.x = start.x - (rect.w / 2);
    rect.y = start.y - (rect.h / 2);

    // Apply spread to projectile.
    angle = direction;
    angle += Tools::randomInt(-WEAPON_SPREAD.at(weapon), WEAPON_SPREAD.at(weapon));

    movement.x = static_cast<float>(std::cos(angle * 0.0174533));
    movement.y = static_cast<float>(std::sin(angle * 0.0174533));
    movement = Tools::normalizeVector(movement);
    movement.x *= WEAPON_SPEED.at(weapon);
    movement.y *= WEAPON_SPEED.at(weapon);
}

/**
* This method draws the projectile to the screen.
*/
void Projectile::draw()
{
    SDL_Rect draw_rect = Application::applyCamera(rect);
    SDL_RenderCopyEx(Application::getRenderer(), texture, nullptr, &draw_rect, angle, nullptr, SDL_FLIP_NONE);
}

/**
* This method updates the projectile. It just moves it.
*/
void Projectile::update()
{
    rect.x += static_cast<int>(std::round(movement.x * Application::getDeltaTime()));
    rect.y += static_cast<int>(std::round(movement.y * Application::getDeltaTime()));
}

/**
* This method gets the rect of the projectile.
*/
const SDL_Rect& Projectile::getRect()
{
    return rect;
}

/**
* This method gets the damage of the projectile.
*/
int Projectile::getDamage()
{
    return WEAPON_DAMAGE.at(weapon);
}

/**
* This method gets the centre of the projectile.
*/
const SDL_Point& Projectile::getCentre()
{
    return { rect.x + (rect.w / 2), rect.y + (rect.h / 2) };
}