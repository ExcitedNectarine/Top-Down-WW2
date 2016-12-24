#pragma once

#include "Application.h"
#include "Button.h"
#include "SelectionList.h"
#include "Tools.h"

class OptionsMenuState : public Application::BaseState
{
public:
    OptionsMenuState();
    void startUp();
    void handleEvents();
    void update();
    void draw();
    void shutDown();

private:
    Button back_button;
    SelectionList fullscreen;
    SelectionList resolution;
    SelectionList volume;
    Text instructions;
    SDL_Cursor* cursor;
    Mix_Chunk* click_sound;
};

