#pragma once

#include <string>
#include <map>
#include <memory>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <tinyxml2.h>

#include "Output.h"

/**
* This namespace is used to contain all of the core game information. It is responsible
* for handling resources, handling game states, the main game loop and configurations.
*/
namespace Application
{
    /**
    * This enum contains all of the errors that can be thrown within this namespace,
    * these errors must be handled outside of the namespace.
    */
    enum class Error
    {
        SDL,
        IMG,
        TTF,
        Mix,
        XML
    };

    /**
    * This is a pure virtual class that should be used for every state in the game.
    * Every state class must inherit from this.
    */
    class BaseState
    {
    public:
        virtual void startUp() = 0;
        virtual void handleEvents() = 0;
        virtual void update() = 0;
        virtual void draw() = 0;
        virtual void shutDown() = 0;
    };

    /**
    * This typedef is a map of every state that can be in the game.
    */
    typedef std::map<std::string, std::shared_ptr<BaseState>> StateMap;

    /**
    * This anonymous namespace holds all of the game information. Some of the
    * information here can be accessed through functions, but not directly.
    */
    namespace
    {
        // SDL variables.
        extern SDL_Window* window;
        extern SDL_Renderer* renderer;
        extern SDL_Event event;
        extern SDL_Point camera;
        extern SDL_Point window_size;
        extern SDL_Point render_size;
        extern SDL_Point mouse_position;

        // State variables.
        extern StateMap states;
        extern std::shared_ptr<BaseState> current_state;
        extern std::string current_state_name;

        // Resource variables.
        extern std::map<std::string, SDL_Texture*> textures;
        extern std::map<std::string, std::map<int, TTF_Font*>> fonts;
        extern std::map<std::string, Mix_Chunk*> sounds;

        // Configuration variables.
        extern std::map<std::string, std::map<std::string, std::string>> config;
        extern std::string config_file_name;
        extern std::vector<std::string> catagory_order;
        extern std::vector<std::string> setting_order;

        // Miscellaneous variables.
        extern bool running;
        extern float delta_time;
        extern int frame_rate_limit;
        extern std::string window_title;
    }

    /**
    * This function initializes SDL and its extensions. It also creates
    * a window, renderer. This should be one of the first functions in this namespace
    * that is called.
    */
    void startUp(const std::string& title, const int screen_width, const int screen_height, const int render_width, const int render_height, const bool fullscreen, const int fps_limit);

    /**
    * This function sets up the game states. The "first_state_name" parameter
    * is the name of the state that the game will start on.
    */
    void setupStates(const StateMap& state_map, const std::string& first_state_name);

    /**
    * This function changes the current game state. It calls the "shutDown" method
    * of the previous state and then calls the "startUp" method of the new state.
    */
    void changeState(const std::string& next_state_name);

    /**
    * This function runs the main game loop.
    */
    void run();

    /**
    * This function destroys the window, the renderer and all of the resources loaded in the game.
    * It also shuts down SDL and its extensions.
    */
    void shutDown();

    /**
    * This function returns a texture. If the texture hasn't been loaded, this function
    * will load it and cache it so it doesn't have to be loaded later.
    */
    SDL_Texture* getTexture(const std::string& file_name);

    /**
    * This function returns a font. If the font hasn't been loaded, this function will load it and cache it so
    * it doesn't have to be loaded later. Because SDL_ttf needs a font to be loaded with a size,
    * a font has to be identified by its name and size.
    */
    TTF_Font* getFont(const std::string& file_name, const int font_size);

    /**
    * This function returns a sound. If the sound hasn't been loaded, this function
    * will load it and cache it so it doesn't have to be loaded later.
    */
    Mix_Chunk* getSound(const std::string& file_name);

    /**
    * This function loads an XML config file for use in the game. The config object is a map
    * of maps, so individual settings have to be grouped together otherwise an error will occur.
    */
    void loadConfig(const std::string& file_name);

    /**
    * This function takes the config map and writes it the same file it was loaded
    * from.
    */
    void writeConfig();

    /**
    * This function returns the config map, which contains settings for the game.
    */
    std::map<std::string, std::map<std::string, std::string>>& getConfigMap();

    /**
    * This function returns a pointer to the game window.
    */
    SDL_Window* getWindow();

    /**
    * This function returns a pointer to the game renderer.
    */
    SDL_Renderer* getRenderer();

    /**
    * This function returns a reference to the game event.
    */
    const SDL_Event& getEvent();

    /**
    * This function returns a reference to the camera, which can be
    * modified directly.
    */
    SDL_Point& getCamera();

    /**
    * This function converts a rect into a rect with a camera applied
    * to it, and then returns the converted rect.
    */
    SDL_Rect applyCamera(const SDL_Rect& rect);

    /**
    * This function returns an SDL_Point containing the size of the window.
    */
    const SDL_Point& getWindowSize();

    /**
    * This function returns an SDL_Point containing the size the game is rendered at.
    */
    const SDL_Point& getRenderSize();

    /**
    * This function returns the current position of the mouse.
    */
    const SDL_Point& getMousePosition();

    /**
    * This function returns the time passed since the last frame.
    */
    float getDeltaTime();

    /**
    * This function returns the title of the window.
    */
    const std::string& getTitle();

    /**
    * This function ends the game.
    */
    void quit();
}