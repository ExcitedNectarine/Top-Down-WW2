#include "HealthPickup.h"

HealthPickup::HealthPickup(const SDL_Rect& tile_rect)
{
    texture = Application::getTexture("Resources/Images/HealthPickup.png");
    SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
    rect.x = (tile_rect.x + (tile_rect.w / 2)) - rect.w / 2;
    rect.y = (tile_rect.y + (tile_rect.h / 2)) - rect.h / 2;
}

void HealthPickup::draw()
{
    SDL_Rect draw_rect = Application::applyCamera(rect);
    SDL_RenderCopy(Application::getRenderer(), texture, nullptr, &draw_rect);
}

const SDL_Rect& HealthPickup::getRect()
{
    return rect;
}

int HealthPickup::getHealth()
{
    return health;
}