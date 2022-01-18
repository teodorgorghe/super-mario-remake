#pragma once

#include "../ecs/ecs.h"
#include "../Components.h"
#include "../Constants.h"
#include <string>

class ScoreSystem : public System {

public:
    void onAddedToWorld(World* world) override;

    void tick(World* world) override;

    void handleEvent(SDL_Event& event) override;

    void onRemovedFromWorld(World* world) override;

    ~ScoreSystem() override = default;
    ScoreSystem() = default;
    ScoreSystem(bool showTime) : showTime(showTime) {}
    ScoreSystem(int score, int time, std::string worldname, bool showTime = true) : showTime(showTime), score(score), time(time * 60), worldname(worldname) {}

    void toggleShowTime() { showTime = true; }
    void toggleHiddenTime() { showTime = false; }

private:
    std::string worldname = "WORLD 1-1";
    bool showTime = true;
    Entity* scoreEntity = 0;
    Entity* coinsEntity = 0;
    Entity* timeLeftEntity = 0;

    int score = 0;
    int coins = 0;
    int time = 255 * 60;
};