#include "OptionsMenuState.h"

OptionsMenuState::OptionsMenuState()
    : back_button("Resources/Fonts/GameFont.ttf", 24, "Back", 20, 20, false, { 255, 255, 255, 255 }),
    fullscreen({"On", "Off"}, Application::getRenderSize().x / 2, (Application::getRenderSize().y / 2) - 50),
    resolution({"640x480", "800x600", "1024x576", "1280x720", "1920x1080"}, Application::getRenderSize().x / 2, Application::getRenderSize().y / 2),
    volume({"0", "1", "2", "3", "4", "5"}, Application::getRenderSize().x / 2, (Application::getRenderSize().y / 2) + 50),
    instructions(Application::getFont("Resources/Fonts/GameFont.ttf", 20), "Pressing the \"Back\" button will apply any changes you have made.", Application::getRenderSize().x / 2, Application::getRenderSize().y - 50, true, { 255, 255, 255, 255 })
{
    click_sound = Application::getSound("Resources/Sounds/Click.wav");
    SDL_SetWindowTitle(Application::getWindow(), (Application::getTitle() + " - Options Menu").c_str());
}

void OptionsMenuState::startUp()
{
    fullscreen.setCurrentOption(Application::getConfigMap()["screen"]["fullscreen"]);
    resolution.setCurrentOption(Application::getConfigMap()["screen"]["size"]);
    volume.setCurrentOption(Application::getConfigMap()["audio"]["volume"]);

    SDL_Surface* cursor_surface = IMG_Load("Resources/Images/MenuCursor.png");
    cursor = SDL_CreateColorCursor(cursor_surface, 0, 0);
    SDL_SetCursor(cursor);
    SDL_FreeSurface(cursor_surface);
}

void OptionsMenuState::handleEvents()
{
    if (Application::getEvent().type == SDL_KEYDOWN)
    {
        if (Application::getEvent().key.keysym.sym == SDLK_ESCAPE)
        {
            Application::changeState("MAIN");
        }
    }
    else if (Application::getEvent().type == SDL_MOUSEBUTTONDOWN)
    {
        if (Application::getEvent().button.button == SDL_BUTTON_LEFT)
        {
            if (back_button.isBig())
            {
                Mix_PlayChannel(-1, click_sound, 0);
                Application::changeState("MAIN");
            }
            else
            {
                fullscreen.pressed();
                resolution.pressed();
                volume.pressed();
            }
        }
    }
}

void OptionsMenuState::update()
{
    back_button.update();
    fullscreen.update();
    resolution.update();
    volume.update();
}

void OptionsMenuState::draw()
{
    back_button.draw();
    fullscreen.draw();
    resolution.draw();
    volume.draw();
    instructions.draw();
}

void OptionsMenuState::shutDown()
{
    if (Application::getConfigMap()["screen"]["fullscreen"] != fullscreen.getCurrentOption())
    {
        Application::getConfigMap()["screen"]["fullscreen"] = fullscreen.getCurrentOption();
        if (Application::getConfigMap()["screen"]["fullscreen"] == "On")
            SDL_SetWindowFullscreen(Application::getWindow(), SDL_WINDOW_FULLSCREEN);
        else SDL_SetWindowFullscreen(Application::getWindow(), SDL_WINDOW_SHOWN);
    }

    if (Application::getConfigMap()["screen"]["size"] != resolution.getCurrentOption())
    {
        Application::getConfigMap()["screen"]["size"] = resolution.getCurrentOption();
        auto size = Tools::splitText(Application::getConfigMap()["screen"]["size"], 'x');
        int width = atoi(size[0].c_str());
        int height = atoi(size[1].c_str());
        SDL_SetWindowSize(Application::getWindow(), width, height);
        SDL_SetWindowPosition(Application::getWindow(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    }

    if (Application::getConfigMap()["audio"]["volume"] != volume.getCurrentOption())
    {
        Application::getConfigMap()["audio"]["volume"] = volume.getCurrentOption();
        int volume = atoi(Application::getConfigMap()["audio"]["volume"].c_str()) * 20;
        Mix_Volume(-1, volume);
        Mix_VolumeMusic(volume / 2);
    }

    Application::writeConfig();
}