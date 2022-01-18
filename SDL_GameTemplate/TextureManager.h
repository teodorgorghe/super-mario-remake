#pragma once

#include <SDL_render.h>
#include <iostream>
#include <SDL_image.h>
#include <unordered_map>
#include "Constants.h"

enum TextureId {
    EMPTY,
    MARIO_STAND,
    MARIO_RUN_1,
    MARIO_RUN_2,
    MARIO_RUN_3,
    MARIO_JUMP,
    MARIO_DRIFT,
    MARIO_DEAD,
    MARIO_GROWING,
    SUPER_MARIO_STAND,
    SUPER_MARIO_RUN_1,
    SUPER_MARIO_RUN_2,
    SUPER_MARIO_RUN_3,
    SUPER_MARIO_JUMP,
    SUPER_MARIO_DRIFT,
    SUPER_MARIO_DUCK,
    GOOMBA,
    GOOMBA_CRUSHED,
    QUESTION_BLOCK_1,
    QUESTION_BLOCK_2,
    QUESTION_BLOCK_3,
    QUESTION_BLOCK_OFF,
    MUSHROOM,
    FLOOR,
    TUBE_TOP_LEFT,
    TUBE_TOP_RIGHT,
    TUBE_RIGHT,
    TUBE_LEFT,
    CLOUD,
    BRICK,
    BLOCK,
    BUSH_RIGHT,
    BUSH_CENTER,
    BUSH_LEFT,
    BACKGROUND_CLOUD_TOP_LEFT,
    BACKGROUND_CLOUD_TOP_CENTER,
    BACKGROUND_CLOUD_TOP_RIGHT,
    BACKGROUND_CLOUD_BOTTOM_LEFT,
    BACKGROUND_CLOUD_BOTTOM_CENTER,
    BACKGROUND_CLOUD_BOTTOM_RIGHT,
    BRICK_DEBRIS_1,
    BRICK_DEBRIS_2,
    BRICK_DEBRIS_3,
    BRICK_DEBRIS_4,
    BACKGROUND_MOUNTAIN_1,
    BACKGROUND_MOUNTAIN_2,
    BACKGROUND_MOUNTAIN_3,
    BACKGROUND_MOUNTAIN_4,
    BACKGROUND_MOUNTAIN_5,
    COIN_1,
    COIN_2,
    COIN_3,
    COIN_4,
    TURTLE_1,
    TURTLE_2,
    TURTLE_SHELL_1,
    TURTLE_SHELL_2,
    FLAG_TOP,
    FLAG_RIGHT,
    FLAG_LEFT,
    FLAG_POLE,
    CASTLE_1,
    CASTLE_2,
    CASTLE_3,
    CASTLE_4,
    CASTLE_5,
    CASTLE_6,
    CASTLE_DOOR,
    CASTLE_8,
    COIN_SMALL_1,
    COIN_SMALL_2,
    COIN_SMALL_3,
    ONE_UP,
    ONE_UP_LABEL,
    ONEHUNDRED,
    TWOHUNDRED,
    THOUSAND,
    MARIO_FLAG_1,
    MARIO_FLAG_2,
    SUPER_MARIO_FLAG_1,
    SUPER_MARIO_FLAG_2,
    FLOWER_MARIO_FLAG_1,
    FLOWER_MARIO_FLAG_2,
    FLOWER_MARIO_GROW,
    FLOWER_MARIO_STAND,
    FLOWER_MARIO_RUN_1,
    FLOWER_MARIO_RUN_2,
    FLOWER_MARIO_RUN_3,
    FLOWER_MARIO_JUMP,
    FLOWER_MARIO_DRIFT,
    FLOWER_MARIO_DUCK,
    FLOWER,
    FIRING_MARIO,
    FIREBALL_1,
    FIREBALL_2,
    FIREBALL_3,
    FIREBALL_4,
    EXPLOSION_1,
    EXPLOSION_2,
    EXPLOSION_3,
    FLOWER_1,
    FLOWER_2,
    FLOWER_3,
    FLOWER_4,
    COIN_ITEM_1,
    COIN_ITEM_2,
    COIN_ITEM_3,
    QUESTION_BLOCK_MUSHROOM,
    QUESTION_BLOCK_COIN,
    TUBE_HORIZONTAL_WARP_UP,
    TUBE_HORIZONTAL_WARP_DOWN,
    TUBE_HORIZONTAL_UP,
    TUBE_HORIZONTAL_DOWN,
    TUBE_INTERSECTION_UP,
    TUBE_INTERSECTION_DOWN
};

class TextureManager {

public:
    explicit TextureManager(SDL_Renderer* renderer, Theme::Id theme = Theme::Id::OVERWORLD);

    void renderTexture(TextureId textureId, SDL_Rect& dstRect, bool flipH = false, bool flipV = false);

    SDL_Texture* texture;

    ~TextureManager();

private:

    SDL_Renderer* renderer;
    std::unordered_map<TextureId, SDL_Rect*> textures{};
};