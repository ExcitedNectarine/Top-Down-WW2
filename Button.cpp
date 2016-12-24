#include "Button.h"

Button::Button(const std::string& font_name, const int font_size, const std::string& text, const int x, const int y, const bool centered, const SDL_Colour& colour, const int width)
    : small_text(Application::getFont(font_name, font_size), text, x, y, centered, colour, width), big_text(Application::getFont(font_name, font_size * 1.25), text, x, y, centered, colour, width)
{
    current_text = &small_text;
    hover_sound = Application::getSound("Resources/Sounds/Hover.wav");
}

void Button::draw()
{
    current_text->draw();
}

void Button::update()
{
    if (SDL_PointInRect(&Application::getMousePosition(), &current_text->getRect()) && !is_big)
    {
        Mix_PlayChannel(-1, hover_sound, 0);
        is_big = true;
        current_text = &big_text;
    }
    else if (!SDL_PointInRect(&Application::getMousePosition(), &current_text->getRect()) && is_big)
    {
        is_big = false;
        current_text = &small_text;
    }
}

bool Button::isBig()
{
    return is_big;
}