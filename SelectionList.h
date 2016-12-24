#pragma once

#include "Application.h"
#include "Text.h"

class SelectionList
{
public:
    SelectionList(const std::vector<std::string>& options_list, const int x, const int y);
    void draw();
    void pressed();
    void update();
    void setCurrentOption(const std::string& string);
    const std::string& getCurrentOption();

private:
    void setSelection();

private:
    std::vector<std::string> options;
    int index;
    Text option;
    SDL_Texture* arrow;
    SDL_Rect left;
    SDL_Rect right;
    bool right_hovering;
    bool left_hovering;
    Mix_Chunk* click_sound;
};