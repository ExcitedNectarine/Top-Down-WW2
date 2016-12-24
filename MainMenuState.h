#pragma once

#include "Application.h"
#include "Button.h"

class MainMenuState : public Application::BaseState
{
public:
    MainMenuState();
    void startUp();
    void handleEvents();
    void update();
    void draw();
    void shutDown();

private:
    Button play_button;
    Button options_button;
    Button quit_button;
    Text title;
    SDL_Cursor* cursor;
    Mix_Chunk* click_sound;
};

