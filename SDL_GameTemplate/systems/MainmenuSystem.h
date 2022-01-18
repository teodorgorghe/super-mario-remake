#pragma once

#include "../ecs/ecs.h"
#include <SDL.h>
#include <SDL_image.h>
#include "../TileType.h"
#include "../Components.h"
#include "../Constants.h"
#include <fstream>
#include "../TextureManager.h"

class MainmenuSystem : public System {

public:
    explicit MainmenuSystem();

    void onAddedToWorld(World* world) override;

    void tick(World* world) override {};

    void onRemovedFromWorld(World* world) override;

    ~MainmenuSystem() override = default;

private:
    TileSetComponent* tileSet{};
};

