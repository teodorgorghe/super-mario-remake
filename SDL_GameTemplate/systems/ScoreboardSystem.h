#pragma once

#include "../ecs/ecs.h"
#include <SDL.h>
#include <SDL_image.h>
#include "../TileType.h"
#include "../Components.h"
#include "../Constants.h"
#include <fstream>
#include "../TextureManager.h"

class ScoreboardSystem : public System {

public:
    explicit ScoreboardSystem(int score = 0);

    void onAddedToWorld(World* world) override;

    void tick(World* world) override {};

    void onRemovedFromWorld(World* world) override;

    ~ScoreboardSystem() override = default;

    void saveToDisk();

private:
    TileSetComponent* tileSet{};
    int score;
    std::vector<int> scoreVector;
};

