#include "MainmenuSystem.h"

void MainmenuSystem::onAddedToWorld(World* world) {
    auto* entity = world->create();
    entity->assign<TileSetComponent>(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT);
    tileSet = entity->get<TileSetComponent>();

    //Incarcare mapa curenta
    std::ifstream infile("assets/maps/mainmenu.lvl", std::ios::out | std::ios::binary);
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
}

void MainmenuSystem::onRemovedFromWorld(World* world) {
    world->destroy(world->findFirst<TileSetComponent>());
}

MainmenuSystem::MainmenuSystem() {}