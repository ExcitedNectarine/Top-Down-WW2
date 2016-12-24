#include "Level.h"

Level::~Level()
{
    SDL_DestroyTexture(texture);
}

/**
* This method opens and loads a level file. It gathers all of the information
* about the level, layers and objects.
*/
void Level::load(const std::string& file_name)
{
    map_data.clear();
    solids.clear();
    objects.clear();
    solids_rects.clear();
    std::string solid_tiles;

    this->file_name = file_name;
    OUTPUT("Loading level: " << this->file_name);

    // Open the level file.
    tinyxml2::XMLDocument doc(true, tinyxml2::COLLAPSE_WHITESPACE);
    if (doc.LoadFile(this->file_name.c_str()) != tinyxml2::XML_SUCCESS)
    {
        SDL_SetError(doc.ErrorName());
        throw Application::Error::XML;
    }
    tinyxml2::XMLNode* root_node = doc.FirstChild();

    // Loop through every node in the level file.
    for (tinyxml2::XMLNode* node = root_node->FirstChild(); node; node = node->NextSibling())
    {
        std::string node_name = node->ToElement()->Name();
        if (node_name == "about")
        {
            // Find all of the metadata for the level.
            for (tinyxml2::XMLNode* about_node = node->FirstChild(); about_node; about_node = about_node->NextSibling())
            {
                std::string name = about_node->ToElement()->Name();
                std::string text = about_node->ToElement()->GetText();

                if (name == "title")
                {
                    title = text;
                    OUTPUT("Level title: " << title);
                }
                else if (name == "width")
                {
                    width = atoi(text.c_str());
                    OUTPUT("Level width: " << width);
                }
                else if (name == "height")
                {
                    height = atoi(text.c_str());
                    OUTPUT("Level height: " << height);
                }
                else if (name == "solids")
                {
                    solid_tiles = text;
                    OUTPUT("Level solids: " << solid_tiles);
                }
            }
        }
        else if (node_name == "data")
        {
            // Find all of the layer and object data for the level.
            for (tinyxml2::XMLNode* data_node = node->FirstChild(); data_node; data_node = data_node->NextSibling())
            {
                std::string data_node_name = data_node->ToElement()->Name();

                if (data_node_name == "layers")
                {
#ifdef OUTPUT_ENABLED
                    int layer_count = 0;
#endif

                    // Loop through every layer and store its data.
                    for (tinyxml2::XMLNode* layer_node = data_node->FirstChild(); layer_node; layer_node = layer_node->NextSibling())
                    {
                        std::string layer_text = layer_node->ToElement()->GetText();
                        auto layer_data = Tools::splitText(layer_text, ' ');
                        map_data.push_back(layer_data);

#ifdef OUTPUT_ENABLED
                        std::replace(layer_text.begin(), layer_text.end(), ' ', '\n');
                        OUTPUT("Layer " << ++layer_count << ":" << std::endl << layer_text);
#endif
                    }

                    SDL_Rect solid_rect = { 0, 0, TILE_SIZE, TILE_SIZE };
                    solids = std::vector<std::string>(height, std::string(width, ' '));
                    for (const auto& layer : map_data)
                    {
                        for (int y = 0; y < height; y++)
                        {
                            for (int x = 0; x < width; x++)
                            {
                                // A '1' represents a solid tile.
                                if (std::find(solid_tiles.begin(), solid_tiles.end(), layer[y][x]) != solid_tiles.end() || solids[y][x] == '1')
                                {
                                    solids[y][x] = '1';
                                    solid_rect.x = x * TILE_SIZE;
                                    solid_rect.y = y * TILE_SIZE;
                                    solids_rects.push_back(solid_rect);
                                }
                                // A '0' represents a non-solid tile.
                                else
                                {
                                    solids[y][x] = '0';
                                }
                            }
                        }
                    }
                }
                else if (data_node_name == "objects")
                {
                    std::string object_text = data_node->ToElement()->GetText();
                    objects = Tools::splitText(object_text, ' ');

#ifdef OUTPUT_ENABLED
                    std::replace(object_text.begin(), object_text.end(), ' ', '\n');
                    OUTPUT("Objects: " << std::endl << object_text);
#endif
                }
            }
        }
    }
}

/**
* This method renders the level onto a single texture, so that it may be
* drawn to the screen with a single draw call.
*/
void Level::render()
{
    // If we are rendering a new map, we need to free the old map texture.
    if (texture != nullptr)
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    // Create a new map texture and set it as the render target.
    texture = SDL_CreateTexture(Application::getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width * TILE_SIZE, height * TILE_SIZE);
    SDL_SetRenderTarget(Application::getRenderer(), texture);
    SDL_RenderClear(Application::getRenderer());

    // Tile variables.
    SDL_Texture* tile_texture;
    SDL_Rect tile_rect = { 0, 0, TILE_SIZE, TILE_SIZE };

    // Loop through every layer in the map and draw it to the map texture.
    for (const auto& layer : map_data)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                // Ignore empty tiles.
                if (layer[y][x] == '0')
                {
                    continue;
                }

                std::string image_file = "Resources/Images/Tiles/";
                image_file += layer[y][x];
                image_file += ".png";

                tile_texture = Application::getTexture(image_file);
                tile_rect.x = x * TILE_SIZE;
                tile_rect.y = y * TILE_SIZE;
                SDL_RenderCopy(Application::getRenderer(), tile_texture, nullptr, &tile_rect);
            }
        }
    }

    // Finish rendering map texture and set the renderer back to the screen.
    SDL_RenderPresent(Application::getRenderer());
    SDL_SetRenderTarget(Application::getRenderer(), nullptr);

    // Set the map rect.
    rect = { 0, 0, width * TILE_SIZE, height * TILE_SIZE };
}

/**
* This method draws the level to the screen.
*/
void Level::draw()
{
    SDL_Rect draw_rect = Application::applyCamera(rect);
    SDL_RenderCopy(Application::getRenderer(), texture, nullptr, &draw_rect);
}

/**
* This method returns a vector of rects. Each rect is a solid
* that is adjascent to the the tile coordinate arguments.
*/
std::vector<SDL_Rect> Level::getSurroundingSolids(const int x, const int y)
{
    std::vector<SDL_Rect> rects;
    SDL_Rect solid_rect = { 0, 0, TILE_SIZE, TILE_SIZE };

    // This holds the coordinates of the 8 tiles that surround
    // the input tile.
    SDL_Point tiles[8] = {
        { x - 1, y },
        { x + 1, y },
        { x, y - 1 },
        { x, y + 1 },
        { x - 1, y - 1 },
        { x + 1, y + 1 },
        { x - 1, y + 1 },
        { x + 1, y - 1 }
    };

    for (const auto& tile : tiles)
    {
        if (solids[tile.y][tile.x] != '0')
        {
            solid_rect.x = tile.x * TILE_SIZE;
            solid_rect.y = tile.y * TILE_SIZE;
            rects.push_back(solid_rect);
        }
    }

    return rects;
}

/**
* This method returns a vector of rects. Each rect is a solid
* in the level. This shouldn't be used for collisions.
*/
const std::vector<SDL_Rect>& Level::getAllSolids()
{
    return solids_rects;
}

/**
* This method returns a vector of the rects of every object of a
* certain type.
*/
std::vector<SDL_Rect> Level::getObjects(const char type)
{
    std::vector<SDL_Rect> rects;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (objects[y][x] == type)
            {
                SDL_Rect object_rect = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                rects.push_back(object_rect);
            }
        }
    }
    return rects;
}

/**
* This method returns the level's title.
*/
const std::string& Level::getTitle()
{
    return title;
}

/**
* This method returns the level's width.
*/
int Level::getWidth()
{
    return width;
}

/**
* This method returns the level's height.
*/
int Level::getHeight()
{
    return height;
}

/**
* This method generates a map of every tile in the grid. Any point in this map
* can be traced back to start_tile which can be used to create a path.
*/
void Level::breadthFirstSearch(const SDL_Point& start_tile)
{
    this->start_tile = start_tile;
    std::queue<SDL_Point> frontier;
    frontier.push(this->start_tile);

    if (!came_from.empty())
    {
        came_from.clear();
    }

    // Every path found by the algorithm starts here, so we set it to itself.
    came_from[std::make_pair(this->start_tile.x, this->start_tile.y)] = std::make_pair(this->start_tile.x, this->start_tile.y);

    SDL_Point current;
    while (!frontier.empty())
    {
        current = frontier.front();

        // Check every tile above, below and to the sides of the current tile.
        checkTile(frontier, current.x + 1, current.y, current);
        checkTile(frontier, current.x - 1, current.y, current);
        checkTile(frontier, current.x, current.y + 1, current);
        checkTile(frontier, current.x, current.y - 1, current);

        // Check every tile diagonally to the current tile, to allow for diagonal pathfinding.
        checkTile(frontier, current.x + 1, current.y - 1, current);
        checkTile(frontier, current.x + 1, current.y + 1, current);
        checkTile(frontier, current.x - 1, current.y - 1, current);
        checkTile(frontier, current.x - 1, current.y + 1, current);

        frontier.pop();
    }
}

/**
* This method returns a deque of each tile from "end_tile" to "start_tile".
*/
std::deque<SDL_Point> Level::getPathToTile(const SDL_Point& end_tile)
{
    std::deque<SDL_Point> path;
    if (solids[end_tile.y][end_tile.x] == '0')
    {
        std::pair<int, int> current_tile(end_tile.x, end_tile.y);
        path.push_front({ current_tile.first, current_tile.second });

        while (came_from[current_tile] != std::make_pair(start_tile.x, start_tile.y))
        {
            current_tile = came_from[current_tile];
            path.push_front({ current_tile.first, current_tile.second });
        }

        path.push_front(start_tile);
    }
    return path;
}

/**
* This method checks a tile in the "solids" grid and finds out if the
* tile is solid or not and if the tile has been checked before.
*/
void Level::checkTile(std::queue<SDL_Point>& frontier, const int x, const int y, const SDL_Point& previous)
{
    if (solids[y][x] == '0')
    {
        if (came_from.find(std::make_pair(x, y)) == came_from.end())
        {
            frontier.push({ x, y });
            came_from[std::make_pair(x, y)] = std::make_pair(previous.x, previous.y);
        }
    }
}