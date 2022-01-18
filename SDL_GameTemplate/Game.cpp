#include "Game.h"

SDL_Renderer* Game::renderer = NULL;
ScenePlayer* Game::currentScene = NULL;

void Game::init(const char* title, int width, int height, bool fullscreen) {
    // Window, video and audio initializing
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return;
    }

    Uint32 flags = fullscreen ? SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_FULLSCREEN : SDL_WINDOW_ALLOW_HIGHDPI;
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);

    if (!window) {
        SDL_Log("Unable to create window SDL: %s", SDL_GetError());
        SDL_Quit();
        return;
    }

    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    renderer = SDL_CreateRenderer(window, -1, render_flags);

    if (!renderer) {
        SDL_Log("Could not create SDL renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error("Could not initialize RenderSystem");
    }
    isRunning = true;
    
    // Scene order (must contains level order)
    currentScene = new ScenePlayer(window, &event, std::vector<SceneData> {
        SceneData(SceneType::SCORE_BOARD, 0),
        SceneData(SceneType::GAME,  "WORLD 1-2", Theme::UNDERGROWND, "assets/maps/world1-2.lvl"),
        SceneData(SceneType::INTRO, "WORLD 1-2"),
        SceneData(SceneType::GAME),
        SceneData(SceneType::INTRO),
        SceneData(SceneType::MAIN_MENU)
    });
}

void Game::update() {
    if (currentScene->running()) {
        currentScene->update();
    }
    else {
        isRunning = false;
    }
}

bool Game::running() const { return isRunning; }

void Game::handleEvents() {

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
            return;
        }
        currentScene->handleEvents();
        // Editor mode switch
        // By pressing E button, deletes every ingame event, and switches into editor mode screen
        
        switch (event.type) {
            case SDL_KEYUP:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_E:
                        if (dynamic_cast<ScenePlayer*>(currentScene)) {
                            delete currentScene;
                            currentScene = new ScenePlayer(window, &event, std::vector<SceneData> {SceneData(SceneType::EDITOR_MODE)});
                        } else {
                            delete currentScene;
                            currentScene = new ScenePlayer(window, &event);
                        }
                        break;
                    default:
                        break;
                }
                break;
        }
        
    }
}

void Game::clean() {
    delete currentScene;
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}