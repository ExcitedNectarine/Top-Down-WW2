#include "Application.h"

/**
* This namespace is used to contain all of the core game information. It is responsible
* for handling resources, handling game states, the main game loop and configurations.
*/
namespace Application
{
    /**
    * This anonymous namespace holds all of the game information. Some of the
    * information here can be accessed through functions, but not directly.
    */
    namespace
    {
        // SDL variables.
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Event event;
        SDL_Point camera;
        SDL_Point window_size;
        SDL_Point render_size;
        SDL_Point mouse_position;

        // State variables.
        StateMap states;
        std::shared_ptr<BaseState> current_state;
        std::string current_state_name;

        // Resource variables.
        std::map<std::string, SDL_Texture*> textures;
        std::map<std::string, std::map<int, TTF_Font*>> fonts;
        std::map<std::string, Mix_Chunk*> sounds;

        // Configuration variables.
        std::map<std::string, std::map<std::string, std::string>> config;
        std::string config_file_name;
        std::vector<std::string> catagory_order;
        std::vector<std::string> setting_order;

        // Miscellaneous variables.
        bool running;
        float delta_time;
        int frame_rate_limit;
        std::string window_title;
    }

    /**
    * This function initializes SDL and its extensions. It also creates
    * a window, renderer. This should be one of the first functions in this namespace
    * that is called.
    */
    void startUp(const std::string& title, const int screen_width, const int screen_height, const int render_width, const int render_height, const bool fullscreen, const int fps_limit)
    {
        // Initialize SDL.
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1)
        {
            throw Error::SDL;
        }

        // Initialize SDL_image.
        if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
        {
            throw Error::IMG;
        }

        // Initialize SDL_ttf.
        if (TTF_Init() == -1)
        {
            throw Error::TTF;
        }

        // Initialize SDL_mixer.
        if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
        {
            throw Error::Mix;
        }

        // Create a window.
        window_title = title;
        window_size.x = screen_width;
        window_size.y = screen_height;
        if (fullscreen)
        {
            window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_size.x, window_size.y, SDL_WINDOW_FULLSCREEN);
        }
        else
        {
            window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_size.x, window_size.y, SDL_WINDOW_SHOWN);
        }
        if (window == nullptr)
        {
            throw Error::SDL;
        }

        // Create a renderer.
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == nullptr)
        {
            throw Error::SDL;
        }

        render_size.x = render_width;
        render_size.y = render_height;
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
        SDL_RenderSetLogicalSize(Application::getRenderer(), render_size.x, render_size.y);

        running = true;
        frame_rate_limit = fps_limit;
    }

    /**
    * This function sets up the game states. The "first_state_name" parameter
    * is the name of the state that the game will start on.
    */
    void setupStates(const StateMap& state_map, const std::string& first_state_name)
    {
        states = state_map;
        current_state_name = first_state_name;
        current_state = states[current_state_name];
    }

    /**
    * This function changes the current game state. It calls the "shutDown" method
    * of the previous state and then calls the "startUp" method of the new state.
    */
    void changeState(const std::string& next_state_name)
    {
        current_state->shutDown();
        current_state_name = next_state_name;
        current_state = states[current_state_name];
        current_state->startUp();
    }

    /**
    * This function runs the main game loop.
    */
    void run()
    {
        // Variables for limiting frame rate.
        int last_time = 0;
        int current_time = 0;
        int frame_time = 0;

        current_state->startUp();
        while (running)
        {
            // Work out the time since the last frame.
            last_time = current_time;
            current_time = SDL_GetTicks();
            frame_time = current_time - last_time;

            // Limit the framerate based on the time since the last frame.
            if ((1000.0 / frame_rate_limit) > frame_time)
            {
                SDL_Delay(static_cast<Uint32>((1000.0 / frame_rate_limit) - frame_time));
            }

            // Calculate the current delta time.
            delta_time = static_cast<float>(frame_time / 1000.0);

            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                }
                else if (event.type == SDL_MOUSEMOTION)
                {
                    mouse_position.x = event.motion.x;
                    mouse_position.y = event.motion.y;
                }
                current_state->handleEvents();
            }

            current_state->update();

            SDL_RenderClear(renderer);
            current_state->draw();
            SDL_RenderPresent(renderer);
        }
        current_state->shutDown();
    }

    /**
    * This function destroys the window, the renderer and all of the resources loaded in the game.
    * It also shuts down SDL and its extensions.
    */
    void shutDown()
    {
        // Free every texture.
        for (auto& texture : textures)
        {
            OUTPUT("Unloading texture: " << texture.first);
            SDL_DestroyTexture(texture.second);
        }

        // Free every font.
        for (auto& font : fonts)
        {
            for (auto& font_it : font.second)
            {
                OUTPUT("Unloading font, size " << font_it.first << ": " << font.first);
                TTF_CloseFont(font_it.second);
            }
        }

        // Free every sound.
        for (auto& sound : sounds)
        {
            OUTPUT("Unloading sound: " << sound.first);
            Mix_FreeChunk(sound.second);
        }

        // Destroy SDL variables.
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);

        // Close down SDL and its extensions.
        Mix_CloseAudio();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }

    /**
    * This returns a texture. If the texture hasn't been loaded, this function
    * will load it and cache it so it doesn't have to be loaded later.
    */
    SDL_Texture* getTexture(const std::string& file_name)
    {
        if (textures.find(file_name) == textures.end())
        {
            OUTPUT("Loading texture: " << file_name);

            SDL_Texture* texture = IMG_LoadTexture(renderer, file_name.c_str());
            if (texture == nullptr)
            {
                throw Error::IMG;
            }
            textures[file_name] = texture;
        }

        return textures[file_name];
    }

    /**
    * This function loads a font and then caches it, so that the next time the font is
    * needed it is available immediately. Because SDL_ttf needs a font to be loaded with a size,
    * a font has to be identified by its name and size.
    */
    TTF_Font* getFont(const std::string& file_name, const int font_size)
    {
        if (fonts.find(file_name) == fonts.end() || fonts.find(file_name)->second.find(font_size) == fonts.find(file_name)->second.end())
        {
            OUTPUT("Loading font, size " << font_size << ": " << file_name);

            TTF_Font* font = TTF_OpenFont(file_name.c_str(), font_size);
            fonts[file_name][font_size] = font;
            if (fonts[file_name][font_size] == nullptr)
            {
                throw Error::TTF;
            }
            fonts[file_name][font_size] = font;
        }

        return fonts[file_name][font_size];
    }

    /**
    * This function returns a sound. If the sound hasn't been loaded, this function
    * will load it and cache it so it doesn't have to be loaded later.
    */
    Mix_Chunk* getSound(const std::string& file_name)
    {
        if (sounds.find(file_name) == sounds.end())
        {
            OUTPUT("Loading sound: " << file_name);

            Mix_Chunk* sound = Mix_LoadWAV(file_name.c_str());
            if (sound == nullptr)
            {
                throw Error::IMG;
            }
            sounds[file_name] = sound;
        }

        return sounds[file_name];
    }

    /**
    * This function loads an XML config file for use in the game. The config object is a map
    * of maps, so individual settings have to be grouped together otherwise an error will occur.
    */
    void loadConfig(const std::string& file_name)
    {
        config_file_name = file_name;

        // Open the config file.
        tinyxml2::XMLDocument doc;
        if (doc.LoadFile(config_file_name.c_str()) != tinyxml2::XML_SUCCESS)
        {
            SDL_SetError(doc.ErrorName());
            throw Error::XML;
        }

        // Load every setting into a map.
        for (tinyxml2::XMLNode* catagory_node = doc.FirstChild(); catagory_node; catagory_node = catagory_node->NextSibling())
        {
            std::string catagory_name = catagory_node->ToElement()->Name();

            // Remember the order of the catagories.
            catagory_order.push_back(catagory_name);

            for (tinyxml2::XMLNode* setting_node = catagory_node->FirstChild(); setting_node; setting_node = setting_node->NextSibling())
            {
                std::string setting_name = setting_node->ToElement()->Name();

                // Remember the order of the individual settings.
                setting_order.push_back(setting_name);

                std::string setting = setting_node->ToElement()->GetText();
                config[catagory_name][setting_name] = setting;
            }
        }

    }

    /**
    * This function takes the config map and writes it the same file it was loaded
    * from.
    */
    void writeConfig()
    {
        tinyxml2::XMLDocument doc;

        tinyxml2::XMLElement* catagory_element;
        tinyxml2::XMLElement* setting_element;

        for (unsigned i = 0; i < catagory_order.size(); i++)
        {
            catagory_element = doc.NewElement(catagory_order[i].c_str());
            for (unsigned j = 0; j < setting_order.size(); j++)
            {
                if (config[catagory_order[i]].find(setting_order[j]) != config[catagory_order[i]].end())
                {
                    setting_element = doc.NewElement(setting_order[j].c_str());
                    setting_element->SetText(config[catagory_order[i]][setting_order[j]].c_str());
                    catagory_element->LinkEndChild(setting_element);
                }
            }
            doc.LinkEndChild(catagory_element);
        }

        doc.SaveFile(config_file_name.c_str());
    }

    /**
    * This function returns the config map, which contains settings for the game.
    */
    std::map<std::string, std::map<std::string, std::string>>& getConfigMap()
    {
        return config;
    }

    /**
    * This function returns the game window.
    */
    SDL_Window* getWindow()
    {
        return window;
    }

    /**
    * This function returns the game renderer.
    */
    SDL_Renderer* getRenderer()
    {
        return renderer;
    }

    /**
    * This function returns a pointer to the game event.
    */
    const SDL_Event& getEvent()
    {
        return event;
    }

    /**
    * This function returns a reference to the camera, which can be
    * modified directly.
    */
    SDL_Point& getCamera()
    {
        return camera;
    }

    /**
    * This function converts a rect into a rect with a camera applied
    * to it, and then returns the converted rect.
    */
    SDL_Rect applyCamera(const SDL_Rect& rect)
    {
        return { rect.x + camera.x, rect.y + camera.y, rect.w, rect.h };
    }

    /**
    * This function returns an SDL_Point containing the size of the window.
    */
    const SDL_Point& getWindowSize()
    {
        return window_size;
    }

    /**
    * This function returns an SDL_Point containing the size the game is rendered at.
    */
    const SDL_Point& getRenderSize()
    {
        return render_size;
    }

    /**
    * This function returns the current position of the mouse.
    */
    const SDL_Point& getMousePosition()
    {
        return mouse_position;
    }

    /**
    * This function returns the time passed since the last frame.
    */
    float getDeltaTime()
    {
        return delta_time;
    }

    /**
    * This function returns the title of the window.
    */
    const std::string& getTitle()
    {
        return window_title;
    }

    /**
    * This function ends the game.
    */
    void quit()
    {
        running = false;
    }
}