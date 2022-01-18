#include "Scene.h"

class ScoreboardScene : public Scene {
    bool finished = false;
public:
    explicit ScoreboardScene(SDL_Window* window, int score = 0);

    virtual void handleEvents(SDL_Event& event) override {
        world->handleEvent(event);
        switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_RETURN:
            finished = true;
            break;
        default:
            break;
        }
    };

    bool isFinished() override;
};