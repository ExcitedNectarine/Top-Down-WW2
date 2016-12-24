#include "Player.h"
#include "Enemy.h"

Player::Player() : health_counter(Application::getFont("Resources/Fonts/GameFont.ttf", 24), "", 20, 20, false, { 255, 255, 255, 255 }),
                   ammo_counter(Application::getFont("Resources/Fonts/GameFont.ttf", 24), "", Application::getRenderSize().x - 150, 20, false, { 255, 255, 255, 255 })
{
    current_texture = Application::getTexture(PLAYER_TEXTURES.at(current_weapon));
    SDL_QueryTexture(current_texture, nullptr, nullptr, &rect.w, &rect.h);
    SDL_QueryTexture(current_texture, nullptr, nullptr, &draw_rect.w, &draw_rect.h);
}

/**
* This method spaws the player. It basically sets all of its
* variables.
*/
void Player::spawn()
{
    stop();

    ammo = {
        { Weapon::Handgun, 25 },
        { Weapon::SubmachineGun, 0 },
        { Weapon::Rifle, 0 },
        { Weapon::Shotgun, 0 },
        { Weapon::AssaultRifle, 0 }
    };

    health = MAX_HEALTH;
    shooting = false;
    shoot_timer = 0.0;
    current_weapon = Weapon::Handgun;
    weapon_index = 0;
    weapons = { Weapon::Handgun };

    current_texture = Application::getTexture(PLAYER_TEXTURES.at(current_weapon));
    SDL_QueryTexture(current_texture, nullptr, nullptr, &draw_rect.w, &draw_rect.h);

    health_counter.setText("Health: " + std::to_string(health));
    ammo_counter.setText("Ammo: " + std::to_string(ammo[current_weapon]));

    weapon_texture = Application::getTexture("Resources/Images/Handgun.png");
    SDL_QueryTexture(weapon_texture, nullptr, nullptr, &weapon_rect.w, &weapon_rect.h);
    weapon_rect.w *= 2;
    weapon_rect.h *= 2;
    weapon_rect.x = (Application::getRenderSize().x / 2) - (weapon_rect.w / 2);
    weapon_rect.y = 20;
}

/**
* This method sets the player to the centre of the tile. A tile
* is represented by a rect.
*/
void Player::setTile(const SDL_Rect& tile_rect)
{
    rect.x = (tile_rect.x + (tile_rect.w / 2)) - rect.w / 2;
    rect.y = (tile_rect.y + (tile_rect.h / 2)) - rect.h / 2;
}

/**
* This method draws the player to the screen.
*/
void Player::draw()
{
    draw_rect.x = rect.x;
    draw_rect.y = rect.y;
    draw_rect = Application::applyCamera(draw_rect);

    SDL_RenderCopyEx(Application::getRenderer(), current_texture, nullptr, &draw_rect, angle, nullptr, SDL_FLIP_NONE);
    health_counter.draw();
    ammo_counter.draw();
    SDL_RenderCopy(Application::getRenderer(), weapon_texture, nullptr, &weapon_rect);
}

/**
* This method updates the player. It moves the player, handles all player
* collisions and makes the player look towards the mouse.
*/
void Player::update(Level& level, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<Projectile>& player_projectiles)
{
    // Create a vector of all solids the player will encounter.
    std::vector<SDL_Rect> solids = level.getSurroundingSolids((rect.x + (rect.w / 2)) / level.TILE_SIZE, (rect.y + (rect.h / 2)) / level.TILE_SIZE);
    for (auto& enemy : enemies)
    {
        solids.push_back(enemy->getRect());
    }

    // Movement and collisions on the X axis.
    rect.x += static_cast<int>(std::round(movement.x * Application::getDeltaTime()));
    for (const auto& solid : solids)
    {
        if (SDL_HasIntersection(&rect, &solid))
        {
            if (movement.x > 0)
            {
                rect.x = solid.x - rect.w;
            }
            else if (movement.x < 0)
            {
                rect.x = solid.x + solid.w;
            }
        }
    }

    // Movement and collisions on the Y axis.
    rect.y += static_cast<int>(std::round(movement.y * Application::getDeltaTime()));
    for (const auto& solid : solids)
    {
        if (SDL_HasIntersection(&rect, &solid))
        {
            if (movement.y > 0)
            {
                rect.y = solid.y - rect.h;
            }
            else if (movement.y < 0)
            {
                rect.y = solid.y + solid.h;
            }
        }
    }

    // Make the player face the mouse.
    angle = Tools::angleBetweenPoints(
        rect.x + (rect.w / 2),
        rect.y + (rect.h / 2),
        Application::getMousePosition().x - Application::getCamera().x,
        Application::getMousePosition().y - Application::getCamera().y
    ) - 180;

    // Set the camera to the position of the player.
    Application::getCamera().x = (Application::getRenderSize().x / 2) - rect.x;
    Application::getCamera().y = (Application::getRenderSize().y / 2) - rect.y;

    shoot_timer += Application::getDeltaTime();
    if (canShoot())
    {
        Mix_PlayChannel(-1, Application::getSound(GUN_SOUNDS.at(current_weapon)), 0);
        if (current_weapon == Weapon::Shotgun)
        {
            for (int i = 0; i < 3; i++)
            {
                player_projectiles.push_back(Projectile(current_weapon, { rect.x + (rect.w / 2), rect.y + (rect.h / 2) }, angle));
            }
        }
        else
        {
            player_projectiles.push_back(Projectile(current_weapon, { rect.x + (rect.w / 2), rect.y + (rect.h / 2) }, angle));
        }
    }
}

/**
* This method handles all key presses related to the player. It mostly
* handles movement key presses.
*/
void Player::handleKeyPresses()
{
    switch (Application::getEvent().key.keysym.sym)
    {
    case SDLK_a:
        movement.x = -SPEED;
        break;
    case SDLK_d:
        movement.x = SPEED;
        break;
    case SDLK_w:
        movement.y = -SPEED;
        break;
    case SDLK_s:
        movement.y = SPEED;
        break;
    default:
        break;
    }
}

/**
* This method handles all key releases related to the player. It mostly
* handles releases key presses.
*/
void Player::handleKeyReleases()
{
    switch (Application::getEvent().key.keysym.sym)
    {
    case SDLK_a:
    case SDLK_d:
        movement.x = 0;
        break;
    case SDLK_w:
    case SDLK_s:
        movement.y = 0;
        break;
    default:
        break;
    }
}

/**
* This method returns an SDL_Point of the centre of the player.
*/
SDL_Point Player::getCentre()
{
    return { rect.x + (rect.w / 2), rect.y + (rect.h / 2) };
}

/**
* This method returns the player's rect.
*/
const SDL_Rect& Player::getRect()
{
    return rect;
}

/**
* This method damages the player.
*/
void Player::damage(const int damage)
{
    health -= damage;
    health_counter.setText("Health: " + std::to_string(health));
}

/**
* This method determines if the player is dead.
*/
bool Player::isDead()
{
    return health <= 0;
}

/**
* This method either makes the player start or stop shooting.
*/
void Player::setShooting(const bool shooting)
{
    this->shooting = shooting;
}

/**
* This method returns the angle the player is facing.
*/
int Player::getAngle()
{
    return angle;
}

/**
* This method changes the current weapon depending on how much the player
* has scrolled using the scroll wheel.
*/
void Player::changeWeapon()
{
    weapon_index += Application::getEvent().wheel.y;
    if (weapon_index < 0)
    {
        weapon_index = weapons.size() - 1;
    }
    else if (weapon_index > weapons.size() - 1)
    {
        weapon_index = 0;
    }
    current_weapon = weapons[weapon_index];
    ammo_counter.setText("Ammo: " + std::to_string(ammo[current_weapon]));
    
    weapon_texture = Application::getTexture(WEAPON_TEXTURES.at(current_weapon));
    SDL_QueryTexture(weapon_texture, nullptr, nullptr, &weapon_rect.w, &weapon_rect.h);
    weapon_rect.w *= 2;
    weapon_rect.h *= 2;
    weapon_rect.x = (Application::getRenderSize().x / 2) - (weapon_rect.w / 2);

    // We only change the rect used for drawing.
    current_texture = Application::getTexture(PLAYER_TEXTURES.at(current_weapon));
    SDL_QueryTexture(current_texture, nullptr, nullptr, &draw_rect.w, &draw_rect.h);
}

/**
* This method adds ammo to the ammo count of the weapon. It returns
* whether or not any ammo has been added.
*/
bool Player::addAmmo(const Weapon& weapon, const int ammo)
{
    if ((this->ammo[weapon] + ammo) >= (MAX_AMMO.at(weapon) + ammo))
    {
        return false;
    }
    else
    {
        this->ammo[weapon] += ammo;
        if (this->ammo[weapon] >= MAX_AMMO.at(weapon))
        {
            this->ammo[weapon] = MAX_AMMO.at(weapon);
        }
        if (weapon == current_weapon)
        {
            ammo_counter.setText("Ammo: " + std::to_string(this->ammo[current_weapon]));
        }
        return true;
    }
}

/**
* Thie method returns whether or not the player has a certain weapon.
*/
bool Player::hasWeapon(const Weapon& weapon)
{
    return std::find(weapons.begin(), weapons.end(), weapon) != weapons.end();
}

/**
* This method adds a weapon to the player's inventory.
*/
void Player::addWeapon(const Weapon& weapon)
{
    weapons.push_back(weapon);
}

/**
* This method adds health to the player. It returns whether or not any health
* has been added.
*/
bool Player::addHealth(const int health)
{
    if ((this->health + health) >= (MAX_HEALTH + health))
    {
        return false;
    }
    else
    {
        this->health += health;
        if (this->health >= MAX_HEALTH)
        {
            this->health = MAX_HEALTH;
        }
        health_counter.setText("Health: " + std::to_string(this->health));
        return true;
    }
}

/**
* This method stops the player's movement.
*/
void Player::stop()
{
    movement.x = 0;
    movement.y = 0;
}

/**
* This method determines if the player can shoot or not.
*/
bool Player::canShoot()
{
    // If we're shooting and we have enough ammo.
    if (shooting && (ammo[current_weapon] > 0))
    {
        // And if we have waited long enough.
        if (shoot_timer > WEAPON_DELAYS.at(current_weapon))
        {
            shoot_timer = 0.0;
            ammo_counter.setText("Ammo: " + std::to_string(--ammo[current_weapon]));
            return true;
        }
    }
    return false;
}