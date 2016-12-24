#pragma once

#include "Application.h"
#include "Tools.h"
#include "Weapons.h"

/**
* This class represents every projectile in the game. Each projectile
* has a starting position, a speed, a spread and an amount to damage things it
* hits.
*/
class Projectile
{
public:
    Projectile(const Weapon& weapon, const SDL_Point& start, const int direction);

    /**
    * This method draws the projectile to the screen.
    */
    void draw();

    /**
    * This method updates the projectile. It just moves it.
    */
    void update();

    /**
    * This method gets the rect of the projectile.
    */
    const SDL_Rect& getRect();

    /**
    * This method gets the damage of the projectile.
    */
    int getDamage();

    /**
    * This method gets the centre of the projectile.
    */
    const SDL_Point& getCentre();

private:
    SDL_Texture* texture;
    SDL_Rect rect;
    Tools::FloatVector movement;
    int angle = 0;
    Weapon weapon;
};