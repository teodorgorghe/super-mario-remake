#include "ScoreboardSystem.h"

std::string scoreToString(int score)
{
    auto scoreString = std::to_string(score);
    auto final = std::string{};
    auto zeros = 6 - scoreString.length();
    while (zeros > 0) {
        zeros--;
        final += '0';
    }
    final += scoreString;
    return final;
}

void ScoreboardSystem::onAddedToWorld(World* world) {
    auto* entity = world->create();
    entity->assign<TileSetComponent>(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT);
    tileSet = entity->get<TileSetComponent>();

    //Incarcare mapa curenta
    std::ifstream infile("assets/maps/scoreboard.lvl", std::ios::out | std::ios::binary);
    if (!infile) throw std::invalid_argument("Map path not found");
    if (infile) {
        int mw;
        int mh;
        infile.read(reinterpret_cast<char*>(&mw), sizeof(uint16_t));
        infile.read(reinterpret_cast<char*>(&mh), sizeof(uint16_t));

        world->create()->assign<TileMapComponent>(mw, mh);

        TileType texture;

        for (int x = 0; x < DEFAULT_MAP_WIDTH; x++) {
            for (int y = 0; y < DEFAULT_MAP_HEIGHT; y++) {
                infile.read(reinterpret_cast<char*>(&texture), sizeof(TileType));
                tileSet->set(x, y, texture);
            }
        }
        infile.close();
    }

    // Read score data
    std::ifstream infile2("assets/stats.dat", std::ios::out | std::ios::binary);
    if (!infile2) throw std::invalid_argument("Stats not found");
    if (infile2)
    {
        int length;
        int val;
        infile2.read(reinterpret_cast<char*>(&length), sizeof(int));

        for (int i = 0; i < length; i++)
        {
            infile2.read(reinterpret_cast<char*>(&val), sizeof(int));
            scoreVector.push_back(val);
        }
        scoreVector.push_back(score);
        std::sort(scoreVector.begin(), scoreVector.end(), std::greater<int>());
        scoreVector.pop_back();

        for (int i = 1; i <= length; i++)
        {
            auto scoreind = world->create();
            scoreind->assign<TextComponent>(std::to_string(i) + ". " + scoreToString(scoreVector[i - 1]));
            scoreind->assign<TransformComponent>(SNES_RESOLUTION_WIDTH / 3 + 15, SNES_RESOLUTION_HEIGHT / 6 + 10 + i*10, 60, 8);
        }
        infile2.close();
    }
    saveToDisk();
}

void ScoreboardSystem::saveToDisk()
{
    std::ofstream outfile;
    outfile.open("assets/stats.dat", std::ios::out | std::ios::binary);
    if (!outfile) throw std::invalid_argument("Cannot save score data!");
    if (outfile)
    {
        int length = (int)scoreVector.size();
        outfile.write(reinterpret_cast<char*>(&length), sizeof(int));

        for (int i = 0; i < length; i++)
        {
            outfile.write(reinterpret_cast<char*>(&scoreVector[i]), sizeof(int));
        }
        outfile.close();
    }
    
}

void ScoreboardSystem::onRemovedFromWorld(World* world) {
    world->destroy(world->findFirst<TileSetComponent>());
}

ScoreboardSystem::ScoreboardSystem(int score) { this->score = score; }