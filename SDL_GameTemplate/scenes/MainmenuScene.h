#include "Scene.h"

class MainmenuScene : public Scene {
    bool finished = false;
public:
    explicit MainmenuScene(SDL_Window* window);

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