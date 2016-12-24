#pragma once

#include "Text.h"

class Button
{
public:
    Button(const std::string& font_name, const int font_size, const std::string& text, const int x, const int y, const bool centered, const SDL_Colour& colour, const int width = 0);
    void draw();
    void update();
    bool isBig();

private:
    bool is_big;
    Text small_text;
    Text big_text;
    Text* current_text;
    Mix_Chunk* hover_sound;
};