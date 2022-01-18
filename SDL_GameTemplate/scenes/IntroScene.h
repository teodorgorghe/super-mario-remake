#include "Scene.h"

class IntroScene : public Scene {

public:
    explicit IntroScene(SDL_Window* window, std::string worldname = "WORLD 1-1", int lives = 3);

    bool isFinished() override;

    void update() override;

private:
    int timer = 0;
};