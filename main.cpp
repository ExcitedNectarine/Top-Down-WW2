#include "MainMenuState.h"
#include "GameState.h"
#include "OptionsMenuState.h"

int main(int argc, char* argv[])
{
    try
    {
        Application::loadConfig("Resources/Config.xml");
        bool fullscreen = Application::getConfigMap()["screen"]["fullscreen"] == "On";
        auto size = Tools::splitText(Application::getConfigMap()["screen"]["size"], 'x');
        int width = atoi(size[0].c_str());
        int height = atoi(size[1].c_str());

        Application::startUp("Top Down WW2", width, height, 1024, 576, fullscreen, 60);

        int volume = atoi(Application::getConfigMap()["audio"]["volume"].c_str()) * 20;
        Mix_Volume(-1, volume);
        Mix_VolumeMusic(volume / 2);

        Mix_Music* music = Mix_LoadMUS("Resources/Sounds/Music.wav");
        if (music == nullptr)
        {
            throw Application::Error::Mix;
        }
        Mix_PlayMusic(music, -1);

        Application::StateMap states = {
            { "MAIN", std::make_shared<MainMenuState>() },
            { "GAME", std::make_shared<GameState>() },
            { "OPTIONS", std::make_shared<OptionsMenuState>() }
        };
        Application::setupStates(states, "MAIN");

        Application::run();

        Mix_FreeMusic(music);
        Application::shutDown();
    }
    catch (const Application::Error& error)
    {
        switch (error)
        {
        case Application::Error::SDL:
            OUTPUT("SDL Error: " << SDL_GetError());
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL Error", SDL_GetError(), Application::getWindow());
            break;
        case Application::Error::IMG:
            OUTPUT("IMG Error: " << IMG_GetError());
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "IMG Error", IMG_GetError(), Application::getWindow());
            break;
        case Application::Error::TTF:
            OUTPUT("TTF Error: " << TTF_GetError());
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "TTF Error", TTF_GetError(), Application::getWindow());
            break;
        case Application::Error::Mix:
            OUTPUT("Mix Error: " << Mix_GetError());
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Mix Error", Mix_GetError(), Application::getWindow());
            break;
        case Application::Error::XML:
            OUTPUT("XML Error: " << SDL_GetError());
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "XML Error", SDL_GetError(), Application::getWindow());
            break;
        default:
            break;
        }
    }

    return 0;
}