#include "ScenePlayer.h"

ScenePlayer::ScenePlayer(SDL_Window* window, SDL_Event* e): window(window), e(e)
{
    SceneList.push_back(SceneData(SceneType::MAIN_MENU));

    SceneData currentSceneData = SceneList.back();
    
    switch (currentSceneData.type)
    {
    case SceneType::INTRO:
        currentScene = new IntroScene(window, currentSceneData.worldname, currentSceneData.lives);
        break;
    case SceneType::GAME:
        currentScene = new GameScene(window, currentSceneData.worldname, currentSceneData.mapPath, currentSceneData.theme, currentSceneData.score, currentSceneData.time, currentSceneData.lives);
        break;
    case SceneType::EDITOR_MODE:
        currentScene = new EditorScene(window);
        break;
    case SceneType::SCORE_BOARD:
        currentScene = new ScoreboardScene(window, currentSceneData.score);
        break;
    default:
        currentScene = new MainmenuScene(window);
        break;
    }
}

ScenePlayer::ScenePlayer(SDL_Window* window, SDL_Event* e, std::vector<SceneData> SceneList): window(window), e(e), SceneList(SceneList)
{
    if (!SceneList.empty())
    {
        SceneData currentSceneData = SceneList.back();

        switch (currentSceneData.type)
        {
        case SceneType::INTRO:
            currentScene = new IntroScene(window, currentSceneData.worldname, currentSceneData.lives);
            break;
        case SceneType::GAME:
            currentScene = new GameScene(window, currentSceneData.worldname, currentSceneData.mapPath, currentSceneData.theme, currentSceneData.score, currentSceneData.time, currentSceneData.lives);
            break;
        case SceneType::EDITOR_MODE:
            currentScene = new EditorScene(window);
            break;
        case SceneType::MAIN_MENU:
            currentScene = new MainmenuScene(window);
            break;
        case SceneType::SCORE_BOARD:
            currentScene = new ScoreboardScene(window, currentSceneData.score);
            break;
        default:
            currentScene = new MainmenuScene(window);
            break;
        }
    } else {
        isRunning = false;
    }
}

void ScenePlayer::update() {
    if (currentScene->isFinished()) {
        SceneList.pop_back();
        if (!SceneList.empty())
        {
            SceneData currentSceneData = SceneList.back();
            if (currentScene)
            {
                delete currentScene;
            }

            switch (currentSceneData.type)
            {
            case SceneType::INTRO:
                currentScene = new IntroScene(window, currentSceneData.worldname, currentSceneData.lives);
                break;
            case SceneType::GAME:
                currentScene = new GameScene(window, currentSceneData.worldname, currentSceneData.mapPath, currentSceneData.theme, currentSceneData.score, currentSceneData.time, currentSceneData.lives);
                break;
            case SceneType::MAIN_MENU:
                currentScene = new MainmenuScene(window);
                break;
            case SceneType::SCORE_BOARD:
                currentScene = new ScoreboardScene(window, currentSceneData.score);
                break;
            case SceneType::EDITOR_MODE:
                currentScene = new EditorScene(window);
                break;
            default:
                break;
            }
        }
        else
        {
            this->isRunning = false;
        }
    } else {
        currentScene->update();
    }
    
}