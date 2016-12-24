#include "Enemy.h"

Enemy::Enemy(const SDL_Rect& tile_rect, SDL_Texture* texture, SDL_Texture* dead_texture, const int health, const int speed)
{
    this->texture = texture;
    SDL_QueryTexture(this->texture, nullptr, nullptr, &rect.w, &rect.h);
    rect.x = (tile_rect.x + (tile_rect.w / 2)) - rect.w / 2;
    rect.y = (tile_rect.y + (tile_rect.h / 2)) - rect.h / 2;

    node_rect.w = NODE_SIZE;
    node_rect.h = NODE_SIZE;

    this->dead_texture = dead_texture;
    this->health = health;
    this->speed = speed;

    // This gives the enemy some individuality. It means that all enemies won't
    // change direction at the same time.
    ai_time = Tools::randomFloat(AI_TIME_MINIUM, AI_TIME_MAXIMUM);

    shout = Application::getSound("Resources/Sounds/Shout.wav");
}

/**
* This method draws the enemy to the screen.
*/
void Enemy::draw()
{
    SDL_Rect draw_rect = Application::applyCamera(rect);
    SDL_RenderCopyEx(Application::getRenderer(), texture, nullptr, &draw_rect, angle, nullptr, SDL_FLIP_NONE);
}

/**
* This method updates the enemy. It checks for collisions and moves the
* enemy along its path to the player.
*/
void Enemy::update(Level& level, Player& player, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<Projectile>& enemy_projectiles)
{
    // These variables are used for line of sight. They have to be their own variables
    // because the "SDL_IntersetRectAndLine" function changes the variables, and I just
    // want to find out if the player is in this enemy's line of sight or not.
    x1 = rect.x + (rect.w / 2);
    y1 = rect.y + (rect.h / 2);
    x2 = player.getCentre().x;
    y2 = player.getCentre().y;

    if (!alerted)
    {
        // This checks if the player is in this enemy's line of sight.
        if (std::all_of(level.getAllSolids().begin(), level.getAllSolids().end(), [&](const SDL_Rect& solid) {return !SDL_IntersectRectAndLine(&solid, &x1, &y1, &x2, &y2); }))
        {
            Mix_PlayChannel(-1, shout, 0);
            alerted = true;
        }
    }
    else
    {
        // The path to the player has to be regularly updated, otherwise the will move
        // to the player's old position.
        ai_timer += Application::getDeltaTime();
        if (ai_timer >= ai_time || current_path.empty())
        {
            ai_timer = 0.0;
            current_path = level.getPathToTile({ (rect.x + (rect.w / 2)) / level.TILE_SIZE, (rect.y + (rect.h / 2)) / level.TILE_SIZE });
            current_path.pop_back();

            // Set the node rect to the centre of the next tile in the path.
            node_rect.x = (current_path.back().x * level.TILE_SIZE) + (level.TILE_SIZE / 2) - (node_rect.w / 2);
            node_rect.y = (current_path.back().y * level.TILE_SIZE) + (level.TILE_SIZE / 2) - (node_rect.h / 2);
        }

        // Check if the player's centre is in the path node.
        SDL_Point rect_centre = { rect.x + (rect.w / 2), rect.y + (rect.h / 2) };
        if (SDL_PointInRect(&rect_centre, &node_rect))
        {
            // If it is, we need to move to the next path node.
            current_path.pop_back();
            if (!current_path.empty())
            {
                // Set the node rect to the centre of the next tile in the path.
                node_rect.x = (current_path.back().x * level.TILE_SIZE) + (level.TILE_SIZE / 2) - (node_rect.w / 2);
                node_rect.y = (current_path.back().y * level.TILE_SIZE) + (level.TILE_SIZE / 2) - (node_rect.h / 2);
            }
        }

        // If the path is empty, stop moving.
        if (current_path.empty())
        {
            movement.x = 0.0;
            movement.y = 0.0;
        }
        else
        {
            // This makes sure the enemy faces and moves towards the next node in the path.
            angle = Tools::angleBetweenPoints(
                (rect.x + (rect.w / 2)),
                (rect.y + (rect.h / 2)),
                (node_rect.x + (node_rect.w / 2)),
                (node_rect.y + (node_rect.h / 2))
            ) - 180;

            // Set the movement vector so that it moves towards the next node in the path.
            movement.x = static_cast<float>(std::cos(angle * 0.0174533));
            movement.y = static_cast<float>(std::sin(angle * 0.0174533));
            movement = Tools::normalizeVector(movement);
            movement.x *= speed;
            movement.y *= speed;

            // Check if the player is in the line of sights.
            if (std::all_of(level.getAllSolids().begin(), level.getAllSolids().end(), [&](const SDL_Rect& solid) {return !SDL_IntersectRectAndLine(&solid, &x1, &y1, &x2, &y2); }))
            {
                if (!facing_player)
                {
                    facing_player = true;
                }

                // Make the enemy face the player.
                angle = Tools::angleBetweenPoints(
                    (rect.x + (rect.w / 2)),
                    (rect.y + (rect.h / 2)),
                    player.getCentre().x,
                    player.getCentre().y
                ) - 180;
            }
            else
            {
                if (facing_player)
                {
                    facing_player = false;
                }
            }

            // Create a vector of solids.
            std::vector<SDL_Rect> solids = level.getSurroundingSolids((rect.x + (rect.w / 2)) / level.TILE_SIZE, (rect.y + (rect.h / 2)) / level.TILE_SIZE);
            solids.push_back(player.getRect());
            for (auto& enemy : enemies)
            {
                // Don't add the rect of this enemy to the list of rects.
                if (!SDL_RectEquals(&rect, &enemy->getRect()))
                {
                    solids.push_back(enemy->getRect());
                }
            }

            // Movement and collisions on the X axis.
            rect.x += static_cast<int>(std::round(movement.x * Application::getDeltaTime()));
            for (const auto& solid : solids)
            {
                if (SDL_HasIntersection(&rect, &solid))
                {
                    if (static_cast<int>(movement.x) > 0)
                    {
                        rect.x = solid.x - rect.w;
                    }
                    else if (static_cast<int>(movement.x) < 0)
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
                    if (static_cast<int>(movement.y) > 0)
                    {
                        rect.y = solid.y - rect.h;
                    }
                    else if (static_cast<int>(movement.y) < 0)
                    {
                        rect.y = solid.y + solid.h;
                    }
                }
            }
        }
    }
}

/**
* This method returns the enemy's rect.
*/
const SDL_Rect& Enemy::getRect()
{
    return rect;
}

/**
* This method damages the enemy.
*/
void Enemy::damage(const int damage)
{
    health -= damage;
}

/**
* This method determines if the enemy is dead and should be removed
* from the game or not.
*/
bool Enemy::isDead()
{
    return health <= 0;
}

/**
* This method returns the angle of the enemy.
*/
int Enemy::getAngle()
{
    return angle;
}

/**
* This method returns the texture of the enemy when the enemy dies.
*/
SDL_Texture* Enemy::getDeadTexture()
{
    return dead_texture;
}

/**
* This method returns the weapon that the enemy uses.
*/
const Weapon& Enemy::getWeapon()
{
    return weapon;
}