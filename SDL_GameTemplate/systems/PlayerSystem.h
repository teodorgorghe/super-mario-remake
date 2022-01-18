#pragma once

#include <utility>

#include "../ecs/ecs.h"
#include "../Components.h"
#include "../Constants.h"
#include "../TextureManager.h"
#include "../AABB.h"

enum ANIMATION_STATE {
    BLINKING,
    RUNNING,
    STANDING,
    DRIFTING,
    DUCKING,
    JUMPING,
    FIRING,
    PIPE_WALK,
    PIPE_DOWN,
    PIPE_UP
};

class PlayerSystem : public System {

    std::function<void(void)> gameOverCallback;

    Entity* player{};
    CameraComponent* camera{};

    ANIMATION_STATE currentState = STANDING;

    void tick(World* world) override;
    void pipeInteractions(World* world, Entity* player);
    void handleEvent(SDL_Event& event) override;

    void onRemovedFromWorld(World* world) override;

    void onGameOver(World* world, Entity* player);

    void setAnimation(ANIMATION_STATE animationState);

    void eatMushroom(World* world,  bool oneup = false);
    void eatFlower(World* world);
    void fireAbility(World* world);

public:
    explicit PlayerSystem(std::function<void(void)> gameOverCallback) : gameOverCallback{ std::move(gameOverCallback) } {}

    void onAddedToWorld(World* world) override;

    ~PlayerSystem() override = default;
};