#pragma once

#include "Application.h"
#include "Tools.h"
#include <algorithm>
#include <deque>
#include <queue>
#include <map>

/**
* This class handles the game levels. It loads all of the information
* from a level file, renders the level and also handles AI pathfinding.
*/
class Level
{
public:
    ~Level();

    /**
    * This method opens and loads a level file. It gathers all of the information
    * about the level, layers and objects.
    */
    void load(const std::string& file_name);

    /**
    * This method renders the level onto a single texture, so that it may be
    * drawn to the screen with a single draw call.
    */
    void render();

    /**
    * This method draws the level to the screen.
    */
    void draw();

    /**
    * This method returns a vector of rects. Each rect is a solid
    * that is adjascent to the the tile coordinate arguments.
    */
    std::vector<SDL_Rect> getSurroundingSolids(const int x, const int y);

    /**
    * This method returns a vector of rects. Each rect is a solid
    * in the level. This shouldn't be used for collisions.
    */
    const std::vector<SDL_Rect>& getAllSolids();

    /**
    * This method returns a vector of the rects of every object of a
    * certain type.
    */
    std::vector<SDL_Rect> getObjects(const char type);

    /**
    * This method returns the level's title.
    */
    const std::string& getTitle();

    /**
    * This method returns the level's width.
    */
    int getWidth();

    /**
    * This method returns the level's height.
    */
    int getHeight();

    /**
    * This method generates a map of every tile in the grid. Any point in this map
    * can be traced back to "start_tile" which can be used to create a path.
    */
    void breadthFirstSearch(const SDL_Point& start_tile);

    /**
    * This method returns a deque of each tile from "end_tile" to "start_tile".
    */
    std::deque<SDL_Point> getPathToTile(const SDL_Point& end_tile);

private:

    /**
    * This method checks a tile in the "solids" grid and finds out if the
    * tile is solid or not and if the tile has been checked before.
    */
    void checkTile(std::queue<SDL_Point>& frontier, const int x, const int y, const SDL_Point& previous);

public:
    static const int TILE_SIZE = 50;

private:
    SDL_Texture* texture;
    SDL_Rect rect;

    std::string file_name;
    std::string title;
    int width;
    int height;

    std::vector<std::vector<std::string>> map_data;
    std::vector<std::string> solids;
    std::vector<std::string> objects;
    std::vector<SDL_Rect> solids_rects;

    std::map<std::pair<int, int>, std::pair<int, int>> came_from;
    SDL_Point start_tile;
};