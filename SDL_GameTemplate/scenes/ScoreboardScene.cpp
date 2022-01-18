#include "ScoreboardScene.h"

ScoreboardScene::ScoreboardScene(SDL_Window* window, int score) {
    world->registerSystem<RenderSystem>(window, SNES_RESOLUTION_WIDTH, SNES_RESOLUTION_HEIGHT, Theme::CASTLE);
    world->registerSystem<ScoreboardSystem>(score);
    world->registerSystem<TileSystem>();
    world->registerSystem<PhysicsSystem>();

    auto title = world->create();
    title->assign<TextComponent>("Top scores:");
    title->assign<TransformComponent>(SNES_RESOLUTION_WIDTH / 3 + 15, SNES_RESOLUTION_HEIGHT / 6, 60, 8);

    auto yourscore = world->create();
    auto scoreString = std::to_string(score);
    auto final = std::string{};
    auto zeros = 6 - scoreString.length();
    while (zeros > 0) {
        zeros--;
        final += '0';
    }
    final += scoreString;
    yourscore->assign<TextComponent>("Your score: " + std::move(final));
    yourscore->assign<BlinkingComponent>(10);
    yourscore->assign<TransformComponent>(SNES_RESOLUTION_WIDTH/3, SNES_RESOLUTION_HEIGHT/2 + 64, 90, 8);
}

bool ScoreboardScene::isFinished() {
    return finished;
}


