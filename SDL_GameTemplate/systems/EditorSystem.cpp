#include "EditorSystem.h"
// The map has got complicated and the need of the editor was bigger.
// The map seems like an matrix of TileTypes, writen in transpose order.
// Level editor's use is not intended for the user.

int InputWatcher(void* userData, SDL_Event* event); // Was added to handle cursor events and rendering simultaneos

void EditorSystem::onAddedToWorld(World* world) {
    SDL_AddEventWatch(InputWatcher, &inputState);

    auto* entity = world->create();
    entity->assign<TileSetComponent>(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT);
    tileSet = entity->get<TileSetComponent>();

    cursor = world->create();
    cursor->assign<TextureComponent>(TextureId::EMPTY);
    cursor->assign<TileComponent>();
    cursor->assign<TransformComponent>(0, 0, TILE_SIZE, TILE_SIZE);

    //Incarcare mapa curenta
    std::ifstream infile("assets/editor.lvl", std::ios::out | std::ios::binary);
    if (infile) {
        int mw;
        int mh;
        infile.read(reinterpret_cast<char*>(&mw), sizeof(uint16_t));
        infile.read(reinterpret_cast<char*>(&mh), sizeof(uint16_t));

        world->create()->assign<TileMapComponent>(mw, mh);

        TileType texture;
        Pipe pipe;

        for (int x = 0; x < DEFAULT_MAP_WIDTH; x++) {
            for (int y = 0; y < DEFAULT_MAP_HEIGHT; y++) {
                infile.read(reinterpret_cast<char*>(&texture), sizeof(TileType));
                tileSet->set(x, y, texture);
                if (texture.properties & PIPE)
                {
                    infile.read(reinterpret_cast<char*>(&pipe), sizeof(Pipe));
                }
            }
        }
        infile.close();
    }
}

void EditorSystem::onRemovedFromWorld(World* world) {
    world->destroy(world->findFirst<TileSetComponent>());
    world->destroy(cursor);
    SDL_DelEventWatch(InputWatcher, &inputState);
}

void EditorSystem::tick(World* world) {
    if (inputState.performSave) {
        inputState.performSave = false;
        saveToDisk();
    }

    auto* camera = world->findFirst<CameraComponent>()->get<CameraComponent>();
    auto selectedTileType = inputState.item % tileTypes.size();
    camera->x += inputState.pan;
    camera->x =  std::max(0, camera->x);
    if (inputState.clicked) {
        inputState.clicked = false;
        int tileX = (inputState.x + camera->left()) / TILE_SIZE;
        int tileY = (inputState.y + camera->top()) / TILE_SIZE;
        auto existing = tileSet->get(tileX, tileY).properties != NONE;
        if (existing) {
            tileSet->clear(tileX, tileY);
            std::cout << "[Editor] Tile removed" << std::endl;
        } else {
            tileSet->set(tileX, tileY, tileTypes[selectedTileType]);
            std::cout << "[Editor] Tile added" << std::endl;
        }
    }

    cursor->get<TextureComponent>()->id = tileTypes[selectedTileType].editor_texture;
    cursor->get<TransformComponent>()->x = ((inputState.x + camera->left()) / TILE_SIZE) * TILE_SIZE;
    cursor->get<TransformComponent>()->y = ((inputState.y + camera->top()) / TILE_SIZE) * TILE_SIZE;
}

void EditorSystem::saveToDisk() {
    int i;
    std::ofstream outfile;
    outfile.open("assets/editor.lvl", std::ios::out | std::ios::binary);

    Pipe pipe;

    outfile.write(reinterpret_cast<const char*>(&(tileSet->mapWidth)), sizeof(const uint16_t));
    outfile.write(reinterpret_cast<const char*>(&(tileSet->mapHeight)), sizeof(const uint16_t));

    for (int x = 0; x < tileSet->mapWidth; x++) {
        for (int y = 0; y < tileSet->mapHeight; y++) {
            auto tile = tileSet->get(x, y);
            outfile.write(reinterpret_cast<char*>(&tile), sizeof(TileType));
            if (tile.properties & PIPE)
            {
                std::cout << "Pipe " << x << " " << y << ":\n";
                std::cout << "Teleport property (0-NONE, 1-UP, 2-DOWN, 3-LEFT, 4-RIGHT): ";
                std::cin >> i;
                pipe.teleport = (Teleport) i;
                if (i)
                {
                    std::cout << "Enter player position spawn:\n";
                    std::cout << "x: ";
                    std::cin >> pipe.posX;
                    std::cout << "y: ";
                    std::cin >> pipe.posY;
                    std::cout << "Theme: ";
                    std::cin >> i;
                    pipe.theme = (Theme::Id) i;
                    std::cout << "World: ";
                    std::cin >> i;
                    pipe.worldID = (WorldID) i;
                }
                
                outfile.write(reinterpret_cast<char*>(&pipe), sizeof(Pipe));
            }
        }
    }
    std::cout << "[EditorSystem] Map saved to disk" << std::endl;
    outfile.close();
}

EditorSystem::EditorSystem() {

}

// Input Handle (cursor, camera, selected tile, insert, delete, save map)
int InputWatcher(void* userData, SDL_Event* event) {
    auto* m = (InputState*) userData;

    if (event->type == SDL_MOUSEMOTION) {
        m->x = std::max(0, event->motion.x);
        m->y = std::max(0, event->motion.y);
    }
    if (event->type == SDL_MOUSEBUTTONDOWN) m->clicked = true;
    if (event->type == SDL_KEYUP) {
        switch (event->key.keysym.scancode) {
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_D:
                m->pan = 0;
                break;
            default:
                break;
        }
    }

    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.scancode) {
            case SDL_SCANCODE_S:
                m->performSave = true;
                break;
            case SDL_SCANCODE_A:
                m->pan = -EDITOR_CAMERA_PAN_SPEED;
                break;
            case SDL_SCANCODE_D:
                m->pan = EDITOR_CAMERA_PAN_SPEED;
                break;
            case SDL_SCANCODE_UP:
                m->item++;
                break;
            case SDL_SCANCODE_DOWN:
                m->item--;
            default:
                break;
        }
    }
    return 0;
}
