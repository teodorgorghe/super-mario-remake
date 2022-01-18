#pragma once
#include <unordered_map>
#include "Constants.h"
#include <string>

enum WorldID
{
	WORLD11,
	WORLD12,
	WORLD12b,
	WORLD13,
	WORLD14
};

class WorldManager {
public:
	WorldManager()
	{
		worldStr.insert_or_assign(WORLD11, std::make_tuple("WORLD 1-1", "assets/maps/world1-1.lvl", Theme::OVERWORLD));
		worldStr.insert_or_assign(WORLD12, std::make_tuple("WORLD 1-2", "assets/maps/world1-2.lvl", Theme::UNDERGROWND));
		worldStr.insert_or_assign(WORLD12b, std::make_tuple("WORLD 1-2", "assets/maps/world1-2b.lvl", Theme::OVERWORLD));
		worldStr.insert_or_assign(WORLD13, std::make_tuple("WORLD 1-3", "assets/maps/world1-3.lvl", Theme::OVERWORLD));
		worldStr.insert_or_assign(WORLD14, std::make_tuple("WORLD 1-4", "assets/maps/world1-4.lvl", Theme::CASTLE));
	}
	std::unordered_map<WorldID, std::tuple<std::string, std::string, Theme::Id>> worldStr {};
};