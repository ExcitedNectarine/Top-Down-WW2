#pragma once

#include "Application.h"

class HealthPickup
{
public:
    HealthPickup(const SDL_Rect& tile_rect);
    void draw();
    const SDL_Rect& getRect();
    int getHealth();

public:
    static const int HEALTH = 25;

private:
    SDL_Texture* texture;
    SDL_Rect rect;
    int health;
};