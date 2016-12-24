#include "GameState.h"

void GameState::startUp()
{
    level_num = 1;
    level.load("Resources/Levels/" + std::to_string(level_num) + ".xml");
    level.render();

    player.spawn();
    setLevel();

    SDL_Surface* cursor_surface = IMG_Load("Resources/Images/GameCursor.png");
    cursor = SDL_CreateColorCursor(cursor_surface, cursor_surface->w / 2, cursor_surface->h / 2);
    SDL_SetCursor(cursor);
    SDL_FreeSurface(cursor_surface);

    death_sound = Application::getSound("Resources/Sounds/Death.wav");
    health_pickup_sound = Application::getSound("Resources/Sounds/HealthPickup.wav");
    ammo_pickup_sound = Application::getSound("Resources/Sounds/AmmoPickup.wav");
    weapon_pickup_sound = Application::getSound("Resources/Sounds/WeaponPickup.wav");
}

void GameState::handleEvents()
{
    if (Application::getEvent().type == SDL_KEYDOWN)
    {
        if (Application::getEvent().key.keysym.sym == SDLK_ESCAPE)
        {
            Application::changeState("MAIN");
        }
        else
        {
            player.handleKeyPresses();
        }
    }
    else if (Application::getEvent().type == SDL_KEYUP)
    {
        player.handleKeyReleases();
    }
    else if (Application::getEvent().type == SDL_MOUSEBUTTONDOWN)
    {
        if (Application::getEvent().button.button == SDL_BUTTON_LEFT)
        {
            player.setShooting(true);
        }
    }
    else if (Application::getEvent().type == SDL_MOUSEBUTTONUP)
    {
        if (Application::getEvent().button.button == SDL_BUTTON_LEFT)
        {
            player.setShooting(false);
        }
    }
    else if (Application::getEvent().type == SDL_MOUSEWHEEL)
    {
        player.changeWeapon();
    }
}

void GameState::update()
{
    // Update the breadth first search every so often.
    ai_timer += Application::getDeltaTime();
    if (ai_timer >= BFS_UPDATE_TIME)
    {
        ai_timer = 0.0;
        level.breadthFirstSearch({ player.getCentre().x / level.TILE_SIZE, player.getCentre().y / level.TILE_SIZE });
    }

    // Update the player, the enemies and the projectiles.
    player.update(level, enemies, player_projectiles);
    for (auto& enemy : enemies)
    {
        enemy->update(level, player, enemies, enemy_projectiles);
    }
    for (auto& projectile : player_projectiles)
    {
        projectile.update();
    }
    for (auto& projectile : enemy_projectiles)
    {
        projectile.update();
    }

    // Update ammo pickups.
    for (auto& ammo_pickup = ammo_pickups.begin(); ammo_pickup != ammo_pickups.end();)
    {
        if (SDL_HasIntersection(&ammo_pickup->getRect(), &player.getRect()))
        {
            if (player.addAmmo(ammo_pickup->getWeapon(), ammo_pickup->getAmmo()))
            {
                Mix_PlayChannel(-1, ammo_pickup_sound, 0);
                ammo_pickup = ammo_pickups.erase(ammo_pickup);
            }
            else
            {
                ammo_pickup++;
            }
        }
        else
        {
            ammo_pickup++;
        }
    }

    // Update weapon pickups.
    for (auto& weapon_pickup = weapon_pickups.begin(); weapon_pickup != weapon_pickups.end();)
    {
        if (SDL_HasIntersection(&weapon_pickup->getRect(), &player.getRect()))
        {
            if (!player.hasWeapon(weapon_pickup->getWeapon()))
            {
                Mix_PlayChannel(-1, weapon_pickup_sound, 0);

                player.addWeapon(weapon_pickup->getWeapon());
                player.addAmmo(weapon_pickup->getWeapon(), weapon_pickup->getAmmo());
                weapon_pickup = weapon_pickups.erase(weapon_pickup);
            }
            else if (player.addAmmo(weapon_pickup->getWeapon(), weapon_pickup->getAmmo()))
            {
                Mix_PlayChannel(-1, weapon_pickup_sound, 0);
                weapon_pickup = weapon_pickups.erase(weapon_pickup);
            }
            else
            {
                weapon_pickup++;
            }
        }
        else
        {
            weapon_pickup++;
        }
    }

    // Update health pickups.
    for (auto& health_pickup = health_pickups.begin(); health_pickup != health_pickups.end();)
    {
        if (SDL_HasIntersection(&health_pickup->getRect(), &player.getRect()))
        {
            if (player.addHealth(health_pickup->HEALTH))
            {
                Mix_PlayChannel(-1, health_pickup_sound, 0);
                health_pickup = health_pickups.erase(health_pickup);
            }
            else
            {
                health_pickup++;
            }
        }
        else
        {
            health_pickup++;
        }
    }

    // Update projectile/wall collisions.
    for (auto& solid : level.getAllSolids())
    {
        for (auto& projectile = player_projectiles.begin(); projectile != player_projectiles.end();)
        {
            if (SDL_HasIntersection(&projectile->getRect(), &solid))
            {
                projectile = player_projectiles.erase(projectile);
            }
            else
            {
                projectile++;
            }
        }

        for (auto& projectile = enemy_projectiles.begin(); projectile != enemy_projectiles.end();)
        {
            if (SDL_HasIntersection(&projectile->getRect(), &solid))
            {
                projectile = enemy_projectiles.erase(projectile);
            }
            else
            {
                projectile++;
            }
        }
    }

    // Update projectile/enemy collisions.
    for (auto& enemy = enemies.begin(); enemy != enemies.end();)
    {
        for (auto& projectile = player_projectiles.begin(); projectile != player_projectiles.end();)
        {
            if (SDL_HasIntersection(&projectile->getRect(), &enemy->get()->getRect()))
            {
                enemy->get()->damage(projectile->getDamage());
                projectile = player_projectiles.erase(projectile);
            }
            else
            {
                projectile++;
            }
        }

        if (enemy->get()->isDead())
        {
            Mix_PlayChannel(-1, death_sound, 0);

            SDL_Texture* texture = enemy->get()->getDeadTexture();
            SDL_Rect rect = enemy->get()->getRect();
            SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
            bodies.push_back(std::make_tuple(texture, rect, enemy->get()->getAngle()));
            weapon_pickups.emplace_back(enemy->get()->getWeapon(), enemy->get()->getRect());
            enemy = enemies.erase(enemy);
        }
        else
        {
            enemy++;
        }
    }

    // Update projectile/player collisions.
    for (auto& projectile = enemy_projectiles.begin(); projectile != enemy_projectiles.end();)
    {
        if (SDL_HasIntersection(&projectile->getRect(), &player.getRect()))
        {
            player.damage(projectile->getDamage());
            projectile = enemy_projectiles.erase(projectile);
        }
        else
        {
            projectile++;
        }
    }

    if (SDL_HasIntersection(&player.getRect(), &exit.second))
    {
        if (std::experimental::filesystem::exists("Resources/Levels/" + std::to_string(++level_num) + ".xml"))
        {
            level.load("Resources/Levels/" + std::to_string(level_num) + ".xml");
            level.render();
            player.stop();
            setLevel();
        }
        else
        {
            Application::changeState("MAIN");
        }
    }

    if (player.isDead())
    {
        player.spawn();
        setLevel();
    }
}

void GameState::draw()
{
    level.draw();
    SDL_Rect draw_rect = Application::applyCamera(exit.second);
    SDL_RenderCopy(Application::getRenderer(), exit.first, nullptr, &draw_rect);
    for (auto& weapon_pickup : weapon_pickups)
    {
        weapon_pickup.draw();
    }
    for (auto& ammo_pickup : ammo_pickups)
    {
        ammo_pickup.draw();
    }
    for (auto& health_pickup : health_pickups)
    {
        health_pickup.draw();
    }
    for (auto& body : bodies)
    {
        draw_rect = Application::applyCamera(std::get<1>(body));
        SDL_RenderCopyEx(Application::getRenderer(), std::get<0>(body), nullptr, &draw_rect, std::get<2>(body), nullptr, SDL_FLIP_NONE);
    }
    for (auto& projectile : player_projectiles)
    {
        projectile.draw();
    }
    for (auto& projectile : enemy_projectiles)
    {
        projectile.draw();
    }
    for (auto& enemy : enemies)
    {
        enemy->draw();
    }
    player.draw();
}

void GameState::shutDown()
{
}

void GameState::setLevel()
{
    // Clear all of the previous data.
    enemies.clear();
    weapon_pickups.clear();
    ammo_pickups.clear();
    health_pickups.clear();
    player_projectiles.clear();
    enemy_projectiles.clear();
    bodies.clear();

    player.setTile(level.getObjects('1')[0]);
    level.breadthFirstSearch({ player.getCentre().x / level.TILE_SIZE, player.getCentre().y / level.TILE_SIZE });

    // Disgusting amount of repitition, but it was taking me too long to figure
    // out how to compress this down.
    for (const auto& tile : level.getObjects('2'))
    {
        enemies.push_back(std::make_shared<Guard>(tile));
    }
    for (const auto& tile : level.getObjects('3'))
    {
        enemies.push_back(std::make_shared<Soldier>(tile));
    }
    for (const auto& tile : level.getObjects('4'))
    {
        enemies.push_back(std::make_shared<Officer>(tile));
    }
    for (const auto& tile : level.getObjects('5'))
    {
        enemies.push_back(std::make_shared<SchutzstaffelSoldier>(tile));
    }
    for (const auto& tile : level.getObjects('6'))
    {
        weapon_pickups.emplace_back(Weapon::Handgun, tile);
    }
    for (const auto& tile : level.getObjects('7'))
    {
        weapon_pickups.emplace_back(Weapon::SubmachineGun, tile);
    }
    for (const auto& tile : level.getObjects('8'))
    {
        weapon_pickups.emplace_back(Weapon::Rifle, tile);
    }
    for (const auto& tile : level.getObjects('9'))
    {
        weapon_pickups.emplace_back(Weapon::Shotgun, tile);
    }
    for (const auto& tile : level.getObjects('A'))
    {
        weapon_pickups.emplace_back(Weapon::AssaultRifle, tile);
    }
    for (const auto& tile : level.getObjects('B'))
    {
        ammo_pickups.emplace_back(Weapon::Handgun, tile);
    }
    for (const auto& tile : level.getObjects('C'))
    {
        ammo_pickups.emplace_back(Weapon::SubmachineGun, tile);
    }
    for (const auto& tile : level.getObjects('D'))
    {
        ammo_pickups.emplace_back(Weapon::Rifle, tile);
    }
    for (const auto& tile : level.getObjects('E'))
    {
        ammo_pickups.emplace_back(Weapon::Shotgun, tile);
    }
    for (const auto& tile : level.getObjects('F'))
    {
        ammo_pickups.emplace_back(Weapon::AssaultRifle, tile);
    }
    for (const auto& tile : level.getObjects('G'))
    {
        health_pickups.emplace_back(tile);
    }

    exit.first = Application::getTexture("Resources/Images/Exit.png");
    SDL_QueryTexture(exit.first, nullptr, nullptr, &exit.second.w, &exit.second.h);
    SDL_Rect exit_tile = level.getObjects('H')[0];
    exit.second.x = (exit_tile.x + (exit_tile.w / 2)) - exit.second.w / 2;
    exit.second.y = (exit_tile.y + (exit_tile.h / 2)) - exit.second.h / 2;

    SDL_SetWindowTitle(Application::getWindow(), (Application::getTitle() + " - " + level.getTitle()).c_str());
}