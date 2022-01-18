#pragma once
#include "../Constants.h"
#include <string>

namespace SceneType {
	enum scenetype{
		INTRO,
		GAME,
		EDITOR_MODE,
		MAIN_MENU,
		SCORE_BOARD
	};
}

struct SceneData
{
	SceneData(SceneType::scenetype type, int score): score(score), type(type), worldname(""), time(0), lives(0), mapPath(""), PlayerX(0), PlayerY(0), direction(0), theme(Theme::OVERWORLD) {}
	SceneData(SceneType::scenetype type = SceneType::MAIN_MENU, std::string worldname = "WORLD 1-1", Theme::Id theme = Theme::OVERWORLD, std::string mapPath = "assets/maps/world1-1.lvl", int score = 0, int time = 300, int lives = 3, int PlayerX = 40, int PlayerY = 140, int direction = 1) :
		type(type), score(score), time(time), worldname(worldname), lives(lives), mapPath(mapPath), theme(theme), PlayerX(PlayerX), PlayerY(PlayerY), direction(direction) {}
	
	SceneType::scenetype type;
	std::string worldname;
	int score;
	int time;
	int lives;
	std::string mapPath;
	int PlayerX;
	int PlayerY;
	int direction;
	Theme::Id theme;
};