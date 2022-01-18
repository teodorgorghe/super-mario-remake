#pragma once

#include "../Constants.h"
#include "../ecs/ecs.h"
#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include <iostream>
#include "../Components.h"
#include "../TextureManager.h"
#include <filesystem>
#include <SDL_ttf.h>

class RenderSystem : public System {

public:
    explicit RenderSystem(SDL_Window* window, int gameResolutionWidth, int gameResolutionHeight, Theme::Id theme = Theme::OVERWORLD);

    void onAddedToWorld(World* world) override;

    void setBackgroundColor(int r, int g, int b);

    void tick(World* world) override;

    void onRemovedFromWorld(World* world) override;

    ~RenderSystem() override;

private:
    SDL_Renderer* renderer;

    const int GAME_RESOLUTION_WIDTH;
    const int GAME_RESOLUTION_HEIGHT;
    TextureManager* textureManager{};
    CameraComponent* camera{};

    void renderEntity(Entity* entity, bool followCamera = true);
    void renderText(Entity* entities);

    TTF_Font* font;

    int r = 0;
    int g = 0;
    int b = 0;
};
