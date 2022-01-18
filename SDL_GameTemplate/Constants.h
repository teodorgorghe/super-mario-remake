#pragma once

namespace Theme
{
    enum Id {
        OVERWORLD,
        UNDERGROWND,
        CASTLE
    };
}

constexpr int FPS = 60;
constexpr int ZOOM = 3;
constexpr int FRAME_DURATION = 1000 / FPS;

constexpr int SNES_RESOLUTION_WIDTH = 256;
constexpr int SNES_RESOLUTION_HEIGHT = 224;

constexpr int TILE_SIZE = 16;
constexpr int DEFAULT_MAP_WIDTH = 230;
constexpr int DEFAULT_MAP_HEIGHT = 14;

constexpr int SKY_RED = 90;
constexpr int SKY_GREEN = 147;
constexpr int SKY_BLUE = 254;

constexpr int INTRO_SCREEN_TIME = 130;

constexpr int EDITOR_CAMERA_PAN_SPEED = 3;

constexpr int CAMERA_WORLD_OFFSET = 4 * TILE_SIZE;

constexpr float GRAVITY = .040;
constexpr float FRICTION = .94;
constexpr float MAX_SPEED_X = 3;
constexpr float MAX_SPEED_Y = 3;

constexpr float MARIO_ACCELERATION_X = .08f;
constexpr float MARIO_JUMP_ACCELERATION = 0.9f;
constexpr float MARIO_PIPE_DOWN = 0.04f;
constexpr float MARIO_BOUNCE = 4.0f;
constexpr int   TILE_ROUNDNESS = 4;

constexpr float MUSHROOM_GROW_SPEED = .25f;
constexpr float MUSHROOM_MOVE_SPEED = 0.8f;
constexpr float FIREBALL_SPEED = 0.2f;
constexpr float FIREBALL_BOUNCE = 0.1f;
constexpr float FIREBALL_ACCELERATION = 0.35f;