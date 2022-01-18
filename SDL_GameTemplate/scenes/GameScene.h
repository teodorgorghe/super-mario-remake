#include "Scene.h"

class GameScene : public Scene {

public:
    explicit GameScene(SDL_Window* window, std::string worldname = "WORLD 1-1", std::string mapPath = "assets/maps/world1-1.lvl", Theme::Id theme = Theme::OVERWORLD, int score = 0, int time = 255, int lives = 3);

    bool isFinished() override;

private:
    bool gameOver = false;

};