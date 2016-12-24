#include "MainMenuState.h"

MainMenuState::MainMenuState()
    : play_button("Resources/Fonts/GameFont.ttf", 24, "Play", 20, 20, false, { 255, 255, 255, 255 }),
    options_button("Resources/Fonts/GameFont.ttf", 24, "Options", 20, 60, false, { 255, 255, 255, 255 }),
    quit_button("Resources/Fonts/GameFont.ttf", 24, "Quit", 20, 100, false, { 255, 255, 255, 255 }),
    title(Application::getFont("Resources/Fonts/GameFont.ttf", 40), "Top Down WW2", Application::getRenderSize().x - 350, 20, false, { 255, 255, 255, 255 })
{
    SDL_SetWindowTitle(Application::getWindow(), (Application::getTitle() + " - Main Menu").c_str());
}

void MainMenuState::startUp()
{
    SDL_Surface* cursor_surface = IMG_Load("Resources/Images/MenuCursor.png");
    cursor = SDL_CreateColorCursor(cursor_surface, 0, 0);
    SDL_SetCursor(cursor);
    SDL_FreeSurface(cursor_surface);

    click_sound = Application::getSound("Resources/Sounds/Click.wav");
}

void MainMenuState::handleEvents()
{
    if (Application::getEvent().type == SDL_MOUSEBUTTONDOWN)
    {
        if (Application::getEvent().button.button == SDL_BUTTON_LEFT)
        {
            if (play_button.isBig())
            {
                Mix_PlayChannel(-1, click_sound, 0);
                Application::changeState("GAME");
            }
            else if (options_button.isBig())
            {
                Mix_PlayChannel(-1, click_sound, 0);
                Application::changeState("OPTIONS");
            }
            else if (quit_button.isBig())
            {
                Mix_PlayChannel(-1, click_sound, 0);
                Application::quit();
            }
        }
    }
    else if (Application::getEvent().type == SDL_KEYDOWN)
    {
        if (Application::getEvent().key.keysym.sym == SDLK_ESCAPE)
        {
            Application::quit();
        }
    }
}

void MainMenuState::update()
{
    play_button.update();
    options_button.update();
    quit_button.update();
    title.draw();
}

void MainMenuState::draw()
{
    play_button.draw();
    options_button.draw();
    quit_button.draw();
    title.draw();
}

void MainMenuState::shutDown()
{
}