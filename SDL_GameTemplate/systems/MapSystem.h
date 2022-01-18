#pragma once

#include "../ecs/ecs.h"
#include <queue>
#include "../Components.h"
#include <fstream>
#include <exception>
#include "../Constants.h"

struct Tile {
    int x, y, w, h;
    TextureId textureId;
    uint32_t properties;

    [[nodiscard]] bool hasProperty(Properties property) const {
        return properties & property;
    }
};

class Map {

public:
    Map() = delete;

    explicit Map(std::string mapPath) {
        std::cout << "Parsing" << std::endl;
        std::ifstream infile(mapPath, std::ios::out | std::ios::binary);
        if (!infile) throw std::invalid_argument("Map path not found: " + mapPath);
        TileType texture;
        Pipe pipeData;
        
        infile.read(reinterpret_cast<char*>(&mapWidth), sizeof(uint16_t));
        infile.read(reinterpret_cast<char*>(&mapHeight), sizeof(uint16_t));

        for (int x = 0; x < mapWidth; x++) {
            for (int y = 0; y < mapHeight; y++) {
                infile.read(reinterpret_cast<char*>(&texture), sizeof(TileType));
                if (texture.texture != 0) {
                    auto tile = new Tile{x * TILE_SIZE,
                                         y * TILE_SIZE,
                                         TILE_SIZE,
                                         TILE_SIZE,
                                         texture.texture,
                                         texture.properties
                    };
                    tiles.emplace(tile);

                    //if the title finds a pipe, reads the pipe info
                    if (tile->hasProperty(PIPE))
                    {
                        infile.read(reinterpret_cast<char*>(&pipeData), sizeof(Pipe));
                        auto pipe = new Pipe{ pipeData.teleport, pipeData.posX, pipeData.posY, pipeData.worldID, pipeData.theme};
                        pipes.emplace(pipe);
                    }
                }
            }
        }

        infile.close();
    }

    uint16_t mapWidth{};
    uint16_t mapHeight{};
    Theme::Id theme;
    std::queue<Tile*> tiles;
    std::queue<Pipe*> pipes;
};

class MapSystem : public System {
public:

    MapSystem(std::string mapPath = "assets/maps/world1-1.lvl") : map{mapPath} {}

    void onAddedToWorld(World* world) override;

    void tick(World* world) override;

    void onRemovedFromWorld(World* world) override;

    ~MapSystem() override;

private:
    Map map;
    CameraComponent* camera{};
};