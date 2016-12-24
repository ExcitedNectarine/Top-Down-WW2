#include "SelectionList.h"

SelectionList::SelectionList(const std::vector<std::string>& options_list, const int x, const int y)
    : options(options_list),
    option(Application::getFont("Resources/Fonts/GameFont.ttf", 26), options[0], x, y, true, {255, 255, 255, 255})
{
    click_sound = Application::getSound("Resources/Sounds/Click.wav");

    index = 0;

    arrow = Application::getTexture("Resources/Images/Arrow.png");
    SDL_QueryTexture(arrow, nullptr, nullptr, &right.w, &right.h);
    left = right;

    right.x = option.getRect().x + option.getRect().w + 10;
    right.y = option.getRect().y;
    left.x = option.getRect().x - left.w - 10;
    left.y = option.getRect().y;
}

void SelectionList::draw()
{
    option.draw();
    SDL_RenderCopy(Application::getRenderer(), arrow, nullptr, &right);
    SDL_RenderCopyEx(Application::getRenderer(), arrow, nullptr, &left, 0, nullptr, SDL_FLIP_HORIZONTAL);
}

void SelectionList::pressed()
{
    if (right_hovering)
    {
        Mix_PlayChannel(-1, click_sound, 0);
        if (++index > (options.size() - 1))
        {
            index = 0;
        }
        setSelection();
    }
    else if (left_hovering)
    {
        Mix_PlayChannel(-1, click_sound, 0);
        if (--index < 0)
        {
            index = options.size() - 1;
        }
        setSelection();
    }
}

void SelectionList::update()
{
    if (SDL_PointInRect(&Application::getMousePosition(), &right))
    {
        right_hovering = true;
    }
    else if (!SDL_PointInRect(&Application::getMousePosition(), &right))
    {
        right_hovering = false;
    }
    if (SDL_PointInRect(&Application::getMousePosition(), &left))
    {
        left_hovering = true;
    }
    else if (!SDL_PointInRect(&Application::getMousePosition(), &left))
    {
        left_hovering = false;
    }
}

void SelectionList::setCurrentOption(const std::string& string)
{
    std::vector<std::string>::iterator it;
    if ((it = std::find(options.begin(), options.end(), string)) != options.end())
    {
        index = std::distance(options.begin(), it);
        setSelection();
    }
}

const std::string& SelectionList::getCurrentOption()
{
    return options[index];
}

void SelectionList::setSelection()
{
    option.setText(options[index]);
    right.x = option.getRect().x + option.getRect().w + 10;
    right.y = option.getRect().y;
    left.x = option.getRect().x - left.w - 10;
    left.y = option.getRect().y;
}
