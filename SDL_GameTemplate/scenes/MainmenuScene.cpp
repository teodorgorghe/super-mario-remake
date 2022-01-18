#include "MainmenuScene.h"

MainmenuScene::MainmenuScene(SDL_Window* window) {
    world->registerSystem<RenderSystem>(window, SNES_RESOLUTION_WIDTH, SNES_RESOLUTION_HEIGHT);
    world->registerSystem<MainmenuSystem>();
    world->registerSystem<TileSystem>();
    world->registerSystem<PhysicsSystem>();

    auto title = world->create();
    title->assign<TextComponent>("PRESS ENTER TO PLAY");
    title->assign<BlinkingComponent>(10);
    title->assign<TransformComponent>(SNES_RESOLUTION_WIDTH/2-20, SNES_RESOLUTION_HEIGHT/2 + 32, 120, 8);
}

bool MainmenuScene::isFinished() {
    return finished;
}


