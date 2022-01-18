#pragma once

#include "../ecs/ecs.h"
#include <SDL.h>
#include <SDL_image.h>
#include "../TileType.h"
#include "../Components.h"
#include "../Constants.h"
#include <fstream>
#include "../TextureManager.h"

struct InputState {
    int x{SNES_RESOLUTION_WIDTH / 2}, y{SNES_RESOLUTION_HEIGHT / 2};
    bool clicked{};
    bool performSave{};
    int pan{};
    int item = 0;
};

class EditorSystem : public System {

public:
    explicit EditorSystem();

    void onAddedToWorld(World* world) override;

    void tick(World* world) override;

    void onRemovedFromWorld(World* world) override;

    ~EditorSystem() override = default;

private:
    Entity* cursor{};

    InputState inputState;

    void saveToDisk();

    TileSetComponent* tileSet{};

    std::vector<TileType> tileTypes = {
            Tiles::FLOOR,
            Tiles::QUESTION_BLOCK,
            Tiles::QUESTION_BLOCK_MUSHROOM,
            Tiles::QUESTION_BLOCK_FLOWER,
            Tiles::TUBE_TOP_LEFT,
            Tiles::TUBE_TOP_RIGHT,
            Tiles::TUBE_LEFT,
            Tiles::TUBE_RIGHT,
            Tiles::BLOCK,
            Tiles::BRICK,
            Tiles::BUSH_LEFT,
            Tiles::BUSH_CENTER,
            Tiles::BUSH_RIGHT,
            Tiles::CLOUD,
            Tiles::GOOMBA,
            Tiles::BACKGROUND_CLOUD_TOP_LEFT,
            Tiles::BACKGROUND_CLOUD_TOP_CENTER,
            Tiles::BACKGROUND_CLOUD_TOP_RIGHT,
            Tiles::BACKGROUND_CLOUD_BOTTOM_LEFT,
            Tiles::BACKGROUND_CLOUD_BOTTOM_CENTER,
            Tiles::BACKGROUND_CLOUD_BOTTOM_RIGHT,
            Tiles::MOUNTAIN_1,
            Tiles::MOUNTAIN_2,
            Tiles::MOUNTAIN_3,
            Tiles::MOUNTAIN_4,
            Tiles::MOUNTAIN_5,
            Tiles::QUESTION_BLOCK_COIN,
            Tiles::TURTLE,
            Tiles::FLAG_TOP,
            Tiles::FLAG_LEFT,
            Tiles::FLAG_RIGHT,
            Tiles::FLAG_POLE,
            Tiles::CASTLE_1,
            Tiles::CASTLE_2,
            Tiles::CASTLE_3,
            Tiles::CASTLE_4,
            Tiles::CASTLE_5,
            Tiles::CASTLE_6,
            Tiles::CASTLE_7,
            Tiles::CASTLE_8,
            Tiles::QUESITON_ONE_UP,
            Tiles::COIN_TITLE,
            Tiles::TUBE_HORIZONTAL_UP,
            Tiles::TUBE_HORIZONTAL_DOWN,
            Tiles::TUBE_HORIZONTAL_WARP_UP,
            Tiles::TUBE_HORIZONTAL_WARP_DOWN,
            Tiles::TUBE_INTERSECTION_UP,
            Tiles::TUBE_INTERSECTION_DOWN
    };
};

