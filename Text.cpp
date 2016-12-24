#include "Text.h"

Text::Text(TTF_Font* font, const std::string& text, const int x, const int y, const bool centered, const SDL_Colour& colour, const int width)
{
    texture = nullptr;

    this->font = font;
    this->centered = centered;
    this->width = width;
    this->colour = colour;

    original_position.x = x;
    original_position.y = y;
    setText(text);
}
Text::~Text()
{
    SDL_DestroyTexture(texture);
}

/**
* This method draws the text to the screen.
*/
void Text::draw()
{
    SDL_RenderCopy(Application::getRenderer(), texture, nullptr, &rect);
}

/**
* This method sets the text.
*/
void Text::setText(const std::string& text)
{
    SDL_Surface* text_surface;
    if (width != 0)
    {
        text_surface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), colour, width);
    }
    else
    {
        text_surface = TTF_RenderText_Blended(font, text.c_str(), colour);
    }

    if (texture != nullptr)
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    texture = SDL_CreateTextureFromSurface(Application::getRenderer(), text_surface);
    SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
    SDL_FreeSurface(text_surface);

    rect.x = original_position.x;
    rect.y = original_position.y;
    if (this->centered)
    {
        rect.x -= rect.w / 2;
        rect.y -= rect.h / 2;
    }
}

/**
* This method returns the rect of the text.
*/
const SDL_Rect& Text::getRect()
{
    return rect;
}