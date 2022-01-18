#pragma once
#include <vector>
#include "SceneData.h"
#include "EditorScene.h"
#include "IntroScene.h"
#include "MainmenuScene.h"
#include "ScoreboardScene.h"
#include "GameScene.h"

class ScenePlayer
{
public:
    ScenePlayer(SDL_Window* window, SDL_Event* e);
    ScenePlayer(SDL_Window* window, SDL_Event* e, std::vector<SceneData> SceneList);

    void handleEvents() { currentScene->handleEvents(*e); }
    void update();

    [[nodiscard]] bool running() const { return isRunning; };

    std::vector<SceneData> SceneList;
private:
    Scene* currentScene;
    SDL_Window* window;
    SDL_Event* e;
    bool isRunning = true;
};