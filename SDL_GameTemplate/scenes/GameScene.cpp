#include "GameScene.h"

GameScene::GameScene(SDL_Window* window, std::string worldname, std::string mapPath, Theme::Id theme, int score, int time, int lives) {
    world->registerSystem<SoundSystem>(theme);
    world->registerSystem<RenderSystem>(window, SNES_RESOLUTION_WIDTH, SNES_RESOLUTION_HEIGHT, theme);
    world->registerSystem<PlayerSystem>([&](){ this->gameOver = true; });
    world->registerSystem<MapSystem>(mapPath);
    world->registerSystem<EnemySystem>();
    world->registerSystem<CallbackSystem>();
    world->registerSystem<AnimationSystem>();
    world->registerSystem<ScoreSystem>(score, time, worldname);
    world->registerSystem<FlagSystem>(([&](){ this->gameOver = true; }));
    world->registerSystem<TileSystem>();
    world->registerSystem<PhysicsSystem>();
}

bool GameScene::isFinished() {
    return gameOver;
}