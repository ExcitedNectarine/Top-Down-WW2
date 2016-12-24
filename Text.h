#pragma once

#include "Application.h"

/**
* This class simply represents a piece of text on-screen.
*/
class Text
{
public:
    Text(TTF_Font* font, const std::string& text, const int x, const int y, const bool centered, const SDL_Colour& colour, const int width = 0);
    ~Text();

    /**
    * This method draws the text to the screen.
    */
    void draw();

    /**
    * This method sets the text.
    */
    void setText(const std::string& text);

    /**
    * This method returns the rect of the text.
    */
    const SDL_Rect& getRect();

private:
    SDL_Texture* texture;
    SDL_Rect rect;
    TTF_Font* font;
    SDL_Point original_position;
    SDL_Colour colour;
    bool centered;
    int width;
};