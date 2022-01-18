#include "PlayerSystem.h"
#include "../WorldManager.h"
#include "../Game.h"

int dirX = 0;
bool jump = false;
bool duck = false;
bool sprint = false;
bool fire = false;
int left = 0;
int right = 0;
int lookingLeft = 0;

const int SMALL_MARIO_COLLIDER_HEIGHT = TILE_SIZE - 2;
const int SUPER_MARIO_COLLIDER_HEIGHT = (TILE_SIZE * 2) - 4;

constexpr int RUNNING_ANIMATION_SPEED = 4;
constexpr int FIRE_ANIMATION_SPEED = 12;

// Transform mario from flower to super, or from super to small
void shrink(World* world, Entity* player) {
    world->create()->assign<SoundComponent>(Sound::Id::SHRINK);
    player->assign<AnimationComponent>(
            std::vector<TextureId>{
                    TextureId::MARIO_STAND,
                    TextureId::MARIO_GROWING,
                    TextureId::MARIO_STAND,
                    TextureId::MARIO_GROWING,
                    TextureId::SUPER_MARIO_STAND,
                    TextureId::MARIO_STAND,
                    TextureId::SUPER_MARIO_STAND,
                    TextureId::MARIO_STAND
            }, 8, false, false, false);

    player->assign<FrozenComponent>();
    player->assign<BlinkingComponent>(10);
    if (player->has<SuperMarioComponent>()) {
        player->remove<SuperMarioComponent>();
    }
    else if (player->has<FlowerMarioComponent>()) {
        player->remove<FlowerMarioComponent>();
    }

    player->assign<CallbackComponent>([player]() {
        player->remove<FrozenComponent>();
        player->assign<CallbackComponent>([player]() { player->remove<BlinkingComponent>(); }, 150);
    }, 64);
}

// Pipe collision
void PlayerSystem::pipeInteractions(World* world, Entity* player)
{
    auto playerTransform = player->get<TransformComponent>();
    world->find<PipeComponent>([&](Entity* pipe) {
        if (player->has<PipeTravelComponent>())
        {
            return;
        }
        auto pipeComponent = pipe->get<PipeComponent>();
        if (AABBCollision(playerTransform, pipe->get<TransformComponent>())) {
            // Pipe teleport directions
            if (pipeComponent->teleport == Teleport::DOWN && duck && player->has<BottomCollisionComponent>())
            {
                //player->assign<FrozenComponent>();
                player->remove<SolidComponent>();
                player->remove<GravityComponent>();
                player->assign<PipeTravelComponent>();
                auto kinetic = player->get<KineticComponent>();
                if (pipe->get<TextureComponent>()->id == TUBE_TOP_LEFT)
                {
                    playerTransform->x = pipe->get<TransformComponent>()->x + 8;
                }
                else if (pipe->get<TextureComponent>()->id == TUBE_TOP_RIGHT)
                {
                    playerTransform->x = pipe->get<TransformComponent>()->x - 8;
                }
                kinetic->accX = 0;
                kinetic->speedX = 0;
                kinetic->accY = MARIO_PIPE_DOWN;
                kinetic->speedY = MARIO_PIPE_DOWN;
                world->create()->assign<SoundComponent>(Sound::Id::PIPE);

                player->assign<CallbackComponent>([&]() { gameOverCallback(); }, 100);
                // Transfer level stats
                
                WorldManager* worldmanager = new WorldManager;
                WorldID worldID = pipeComponent->worldID;

                Game::currentScene->SceneList.insert(Game::currentScene->SceneList.end() - 1,
                    SceneData(SceneType::GAME, std::get<0>(worldmanager->worldStr[worldID]),
                        std::get<2>(worldmanager->worldStr[worldID]), std::get<1>(worldmanager->worldStr[worldID])));

                auto stats = world->create();
                stats->assign<TransferStatsComponent>();
                stats->get<TransferStatsComponent>()->index = Game::currentScene->SceneList.size() - 2;
                stats->get<TransferStatsComponent>()->transferBitset[0] = 1;
                stats->get<TransferStatsComponent>()->transferBitset[1] = 1;
                delete worldmanager;
                return;
            }
            else if (pipeComponent->teleport == Teleport::RIGHT && player->has<RightCollisionComponent>())
            {
                player->assign<PipeTravelComponent>();
                pipe->remove<SolidComponent>();
                // TO DO: Corectat animatiile cand se intra in pipe.
                if (pipe->get<TextureComponent>()->id == TUBE_HORIZONTAL_WARP_UP)
                {
                    world->find<PipeComponent>([&](Entity* pipe2) {
                        if (pipe2->get<TextureComponent>()->id == TUBE_HORIZONTAL_WARP_DOWN)
                        {
                            pipe2->remove<SolidComponent>();
                        }
                    });
                } else if (pipe->get<TextureComponent>()->id == TUBE_HORIZONTAL_WARP_DOWN)
                {
                    world->find<PipeComponent>([&](Entity* pipe2) {
                        if (pipe2->get<TextureComponent>()->id == TUBE_HORIZONTAL_WARP_UP)
                        {
                            pipe2->remove<SolidComponent>();
                        }
                    });
                }

                world->create()->assign<SoundComponent>(Sound::Id::PIPE);
                player->assign<CallbackComponent>([&]() { gameOverCallback(); }, 100);
                // Transfer level stats

                WorldManager* worldmanager = new WorldManager;
                WorldID worldID = pipeComponent->worldID;

                Game::currentScene->SceneList.insert(Game::currentScene->SceneList.end() - 1,
                    SceneData(SceneType::GAME, std::get<0>(worldmanager->worldStr[worldID]),
                        std::get<2>(worldmanager->worldStr[worldID]), std::get<1>(worldmanager->worldStr[worldID])));

                auto stats = world->create();
                stats->assign<TransferStatsComponent>();
                stats->get<TransferStatsComponent>()->index = Game::currentScene->SceneList.size() - 2;
                stats->get<TransferStatsComponent>()->transferBitset[0] = 1;
                stats->get<TransferStatsComponent>()->transferBitset[1] = 1;
                delete worldmanager;
                return;
            }
        }
    });
}

// Create block break debris
void createDebris(World* world, TransformComponent* brickTransform) {
    auto debris1 = world->create();
    debris1->assign<GravityComponent>();
    debris1->assign<KineticComponent>(-2.0f, -16.0f);
    debris1->assign<TileComponent>();
    debris1->assign<TextureComponent>(TextureId::BRICK_DEBRIS_1);
    debris1->assign<TransformComponent>(brickTransform->x, brickTransform->y, 8, 8);

    auto debris2 = world->create();
    debris2->assign<GravityComponent>();
    debris2->assign<KineticComponent>(2.0f, -16.0f);
    debris2->assign<TileComponent>();
    debris2->assign<TextureComponent>(TextureId::BRICK_DEBRIS_2);
    debris2->assign<TransformComponent>(brickTransform->x + 8, brickTransform->y, 8, 8);

    auto debris3 = world->create();
    debris3->assign<GravityComponent>();
    debris3->assign<KineticComponent>(-2.0f, -10.0f);
    debris3->assign<TileComponent>();
    debris3->assign<TextureComponent>(TextureId::BRICK_DEBRIS_3);
    debris3->assign<TransformComponent>(brickTransform->x, brickTransform->y + 8, 8, 8);

    auto debris4 = world->create();
    debris4->assign<GravityComponent>();
    debris4->assign<KineticComponent>(2.0f, -10.0f);
    debris4->assign<TileComponent>();
    debris4->assign<TextureComponent>(TextureId::BRICK_DEBRIS_4);
    debris4->assign<TransformComponent>(brickTransform->x + 8, brickTransform->y + 8, 8, 8);

    world->create()->assign<SoundComponent>(Sound::Id::BLOCK_BREAK);
}

void PlayerSystem::onGameOver(World* world, Entity* player) {
    if (player->has<SuperMarioComponent>()) {
        //time was runned out when mario was Super Mario.
        player->remove<SuperMarioComponent>();
        auto texture = player->get<TextureComponent>();
        auto transform = player->get<TransformComponent>();
        transform->h = SMALL_MARIO_COLLIDER_HEIGHT;
        transform->y += SUPER_MARIO_COLLIDER_HEIGHT - SMALL_MARIO_COLLIDER_HEIGHT;
        texture->h = TILE_SIZE;
        texture->offSetY = -1;
    }
    player->assign<DeadComponent>();
    world->create()->assign<SoundComponent>(Sound::Id::DEATH);
    auto music = world->findFirst<MusicComponent>();
    if (music) world->destroy(music);

    auto kinetic = player->get<KineticComponent>();
    player->get<TextureComponent>()->id = TextureId::MARIO_DEAD;
    player->assign<DeadComponent>();

    // Make entity static
    player->remove<AnimationComponent>();
    kinetic->speedY = 0.0f;
    kinetic->speedX = 0.0f;
    kinetic->accY = 0.0f;
    kinetic->accX = 0.0f;

    world->findAny<WalkComponent, KineticComponent>([=](Entity* walker) {
        if (walker != player) {
            walker->remove<WalkComponent>();
            walker->remove<KineticComponent>();
            walker->remove<AnimationComponent>();
            walker->remove<CallbackComponent>();
        }
    });

    // Make entity dead animation
    player->assign<CallbackComponent>([=]() {
        player->remove<SolidComponent>();
        kinetic->speedY = -10.0f;
        kinetic->speedX = 0.0f;
        kinetic->accY = 0.0f;
        kinetic->accX = 0.0f;

        player->assign<CallbackComponent>([&]() { gameOverCallback(); }, 200);
    }, 50);
    
    // Repeta nivelul
    std::string& mapPath = Game::currentScene->SceneList.back().mapPath;
    WorldManager* worldmanager = new WorldManager;
    WorldID worldID = WorldID::WORLD11;
    for (auto& w : worldmanager->worldStr)
    {
        if (std::get<1>(w.second) == mapPath)
        {
            worldID = w.first;
            break;
        }
    }

    Game::currentScene->SceneList.insert(Game::currentScene->SceneList.end() - 1,
        SceneData(SceneType::GAME, std::get<0>(worldmanager->worldStr[worldID]),
            std::get<2>(worldmanager->worldStr[worldID]), std::get<1>(worldmanager->worldStr[worldID])));
    Game::currentScene->SceneList.insert(Game::currentScene->SceneList.end() - 1,
        SceneData(SceneType::INTRO, std::get<0>(worldmanager->worldStr[worldID])));

    auto stats = world->create();
    stats->assign<TransferStatsComponent>();
    stats->get<TransferStatsComponent>()->index = Game::currentScene->SceneList.size() - 3;
    stats->get<TransferStatsComponent>()->transferBitset[0] = 1;
    delete worldmanager;
}

void PlayerSystem::setAnimation(ANIMATION_STATE state) {
    if (player->has<BlinkingComponent>()) {
        auto blink = player->get<BlinkingComponent>();
        blink->current++;
        if ((blink->current / blink->speed) % 2) {
            player->remove<AnimationComponent>();
            currentState = BLINKING;
            player->get<TextureComponent>()->id = TextureId::EMPTY;
            return;
        }
    }
    if (currentState == state) return;
    player->remove<AnimationComponent>();

    switch (state) {
        case RUNNING: {
            if (player->has<SuperMarioComponent>()) {
                player->assign<AnimationComponent>(
                        std::vector<TextureId>{
                                TextureId::SUPER_MARIO_RUN_1,
                                TextureId::SUPER_MARIO_RUN_2,
                                TextureId::SUPER_MARIO_RUN_3},
                        RUNNING_ANIMATION_SPEED
                );
            } else if(player->has<FlowerMarioComponent>()) {
                player->assign<AnimationComponent>(
                    std::vector<TextureId>{
                    TextureId::FLOWER_MARIO_RUN_1,
                        TextureId::FLOWER_MARIO_RUN_2,
                        TextureId::FLOWER_MARIO_RUN_3},
                    RUNNING_ANIMATION_SPEED
                        );
            } else {
                player->assign<AnimationComponent>(
                        std::vector<TextureId>{
                                TextureId::MARIO_RUN_1,
                                TextureId::MARIO_RUN_2,
                                TextureId::MARIO_RUN_3},
                        RUNNING_ANIMATION_SPEED
                );
            }

            break;
        }
        case STANDING:
            player->assign<TextureComponent>(
                    player->has<FlowerMarioComponent>() ?
                    TextureId::FLOWER_MARIO_STAND :
                    player->has<SuperMarioComponent>() ? TextureId::SUPER_MARIO_STAND : TextureId::MARIO_STAND);
            break;
        case JUMPING:
            player->assign<TextureComponent>(
                    player->has<FlowerMarioComponent>() ?
                    TextureId::FLOWER_MARIO_JUMP :
                    player->has<SuperMarioComponent>() ? TextureId::SUPER_MARIO_JUMP : TextureId::MARIO_JUMP);
            break;
        case DRIFTING:
            player->assign<TextureComponent>(
                    player->has<FlowerMarioComponent>() ?
                    TextureId::FLOWER_MARIO_DRIFT :
                    player->has<SuperMarioComponent>()? TextureId::SUPER_MARIO_DRIFT : TextureId::MARIO_DRIFT);
            break;
        case DUCKING:
            if (player->has<SuperMarioComponent>()) {
                player->assign<TextureComponent>(TextureId::SUPER_MARIO_DUCK);
            }
            if (player->has<FlowerMarioComponent>())
            {
                player->assign<TextureComponent>(TextureId::FLOWER_MARIO_DUCK);
            }
            break;
        case BLINKING:
            break;
    }

    if (player->has<SuperMarioComponent>() || player->has<FlowerMarioComponent>()) {
        player->get<TextureComponent>()->h = TILE_SIZE * 2;
        player->get<TextureComponent>()->offSetY = -3;
    } else {
        player->get<TextureComponent>()->h = TILE_SIZE;
        player->get<TextureComponent>()->offSetY = -1;
    }
    player->get<TextureComponent>()->w = TILE_SIZE;
    player->get<TextureComponent>()->offSetX = -2;
    currentState = state;
}

void PlayerSystem::tick(World* world) {
    // if player has GameOverComponent, then call onGameOver.
    if (world->findFirst<GameOverComponent>()) {
        world->findFirst<GameOverComponent>()->remove<GameOverComponent>();
        onGameOver(world, player);
    }
    if (player->has<DeadComponent>()) return;
    if (player->has<PipeTravelComponent>()) return;
    auto transform = player->get<TransformComponent>();
    auto texture = player->get<TextureComponent>();

    // Player transformation
    if (player->has<FrozenComponent>()) {
        if (texture) {
            if ((texture->id == TextureId::FLOWER_MARIO_STAND || texture->id == TextureId::SUPER_MARIO_STAND ||
                 player->get<TextureComponent>()->id == TextureId::MARIO_GROWING) &&
                transform->h != SUPER_MARIO_COLLIDER_HEIGHT) {
                texture->h = TILE_SIZE * 2;
                transform->h = SUPER_MARIO_COLLIDER_HEIGHT;
                transform->y -= SUPER_MARIO_COLLIDER_HEIGHT - SMALL_MARIO_COLLIDER_HEIGHT;
                texture->offSetY = -3;
            }
            if (texture->id == TextureId::MARIO_STAND
                && transform->h != SMALL_MARIO_COLLIDER_HEIGHT) {
                texture->h = TILE_SIZE;
                texture->offSetY = -1;
                transform->h = SMALL_MARIO_COLLIDER_HEIGHT;
                transform->y += SUPER_MARIO_COLLIDER_HEIGHT - SMALL_MARIO_COLLIDER_HEIGHT;
            }
        }
        return;
    }

    auto kinetic = player->get<KineticComponent>();

    // Evitare mergere in partea din stanga a camerei
    if (transform->left() < camera->left()) {
        transform->setLeft(camera->left());
        kinetic->accX = 0;
        kinetic->accX = 0;
        kinetic->speedX = 0;
    }

    pipeInteractions(world, player);

    if (duck && (player->has<SuperMarioComponent>() || player->has<FlowerMarioComponent>())) {
        setAnimation(DUCKING);
        kinetic->accX = 0;
    } else {
        if (player->has<BottomCollisionComponent>()) {
            kinetic->accX = (float) dirX * MARIO_ACCELERATION_X * 1.7f;
            if (sprint) kinetic->accX *= 1.5;
            if (fire && player->has<FlowerMarioComponent>())
            {
                fireAbility(world);
            }
            if (jump) {
                player->get<KineticComponent>()->accY = -MARIO_JUMP_ACCELERATION;
                world->create()->assign<SoundComponent>(Sound::Id::JUMP);
            }
            if ((bool) std::abs(kinetic->speedX) || (bool) std::abs(kinetic->accX)) {
                if ((kinetic->speedX > 0 && kinetic->accX < 0) ||
                    (kinetic->speedX < 0 && kinetic->accX > 0)) {
                    setAnimation(DRIFTING);
                } else {
                    setAnimation(RUNNING);
                }
            } else {
                setAnimation(STANDING);
            }

        } else {
            kinetic->accX = (float) (dirX) * (MARIO_ACCELERATION_X);
            if (sprint) kinetic->accX *= 1.5;
            setAnimation(JUMPING);
        }
    }

    // Enemy collision and shrink/die
    auto bounce = false;
    for (auto enemy : world->find<EnemyComponent, TransformComponent>()) {
        if (!AABBCollision(enemy->get<TransformComponent>(), transform)) continue;
        if (kinetic->speedY > 0) {
            enemy->assign<CrushedComponent>();
            transform->setBottom(enemy->get<TransformComponent>()->top());
            bounce = true;
        } else {
            if (enemy->get<EnemyComponent>()->type == Enemy::TURTLE_SHELL) continue; // ?
            if (!player->has<BlinkingComponent>()) {
                if (player->has<SuperMarioComponent>() || player->has<FlowerMarioComponent>()) {
                    currentState = ANIMATION_STATE::STANDING;
                    shrink(world, player);
                } else {
                    onGameOver(world, player);
                }
                return;
            }
        }
    }

    // Collect coin
    auto coinVector = world->find<CoinComponent>();
    for (auto& coin : coinVector)
    {
        if (AABBCollision(coin->get<TransformComponent>(), transform))
        {
            auto coinTransform = coin->get<TransformComponent>();
            world->create()->assign<FloatingPointsComponent>(Points::ONEHOUNDRED, coinTransform->getCenterX(), coinTransform->y);
            world->create()->assign<SoundComponent>(Sound::Id::COIN);
            world->create()->assign<AddScoreComponent>(100);
            coin->clearComponents();
        }
    }

    if (bounce) {
        kinetic->accY = -0.2f;
        kinetic->speedY = -MARIO_BOUNCE;
    }

    // Sparge blocuri
    if (player->has<SuperMarioComponent>() || player->has<FlowerMarioComponent>()) {
        for (auto breakable : world->find<BreakableComponent, TransformComponent, BottomCollisionComponent>()) {
            if (!breakable->has<QuestionBlockComponent>() &&
                AABBCollision(breakable->get<TransformComponent>(), transform)) {
                createDebris(world, breakable->get<TransformComponent>());
                breakable->remove<SolidComponent>();
                breakable->remove<TextureComponent>();
                breakable->assign<BreakableComponent>();
                breakable->get<BreakableComponent>()->hit = true;
            }
        }
    }

    // Eat mushroom and flower
    for (auto collectible : world->find<CollectibleComponent, TransformComponent>()) {
        if (AABBCollision(collectible->get<TransformComponent>(), player->get<TransformComponent>())) {
            currentState = ANIMATION_STATE::STANDING;
            if (collectible->has<FlowerComponent>())
            {
                eatFlower(world);
            }
            else
            {
                eatMushroom(world, collectible->get<TextureComponent>()->id == TextureId::ONE_UP);
            }
            world->destroy(collectible);
        }
    }

    // Mario die from falling
    float mapHeight = world->findFirst<TileMapComponent>()->get<TileMapComponent>()->mapHeight;
    if (transform->top() > mapHeight*TILE_SIZE)
    {
        player->assign<GameOverComponent>();
    }

    if (left || right) lookingLeft = left;
    player->get<TextureComponent>()->flipH = lookingLeft;
    jump = false;
    fire = false;
    player->remove<BottomCollisionComponent>();
    player->remove<LeftCollisionComponent>();
    player->remove<RightCollisionComponent>();
    player->remove<TopCollisionComponent>();
}

void createFireball(World* world, Entity* player)
{
    int direction = lookingLeft ? -1 : 1;
    auto fireball = world->create();
    auto playerTransform = player->get<TransformComponent>();
    fireball->assign<TransformComponent>(playerTransform->left() + TILE_SIZE*direction + 1, playerTransform->getCenterY(), TILE_SIZE/2, TILE_SIZE/2);
    fireball->assign<FireballComponent>();
    fireball->assign<AnimationComponent>(
        std::vector<TextureId> {
        TextureId::FIREBALL_1,
            TextureId::FIREBALL_2,
            TextureId::FIREBALL_3,
            TextureId::FIREBALL_4,
    }, 8
        );
    fireball->assign<TextureComponent>(TextureId::FIREBALL_1);
    fireball->get<TextureComponent>()->h = TILE_SIZE / 2;
    fireball->get<TextureComponent>()->w = TILE_SIZE / 2;
    fireball->assign<SolidComponent>();
    fireball->assign<TileComponent>();
    fireball->assign<GravityComponent>();
    fireball->assign<KineticComponent>(FIREBALL_SPEED, FIREBALL_BOUNCE, direction);

}

void PlayerSystem::fireAbility(World* world)
{
    fire = false;

    if (player->has<FlowerMarioComponent>())
    {
        world->create()->assign<SoundComponent>(Sound::Id::FIREBALL);
        player->assign<AnimationComponent>(
            std::vector<TextureId>{
                TextureId::FIRING_MARIO,
                TextureId::FLOWER_MARIO_STAND},
            FIRE_ANIMATION_SPEED
        );
        createFireball(world, player);
    }
}

void PlayerSystem::eatMushroom(World* world, bool oneup) {
    if (oneup) {
        world->create()->assign<SoundComponent>(Sound::Id::ONE_UP);
        world->create()->assign<FloatingPointsComponent>(Points::ONEUP,
                                                         player->get<TransformComponent>()->getCenterX(),
                                                         player->get<TransformComponent>()->y
        );
        return;
    } else {
        world->create()->assign<SoundComponent>(Sound::Id::MUSHROOM_EAT);
        world->create()->assign<FloatingPointsComponent>(Points::ONETHOUSAND,
                                                         player->get<TransformComponent>()->getCenterX(),
                                                         player->get<TransformComponent>()->y
        );
        world->create()->assign<AddScoreComponent>(1000);
    }

    if (player->has<SuperMarioComponent>() || player->has<FlowerMarioComponent>()) return;
    player->assign<SuperMarioComponent>();
    player->assign<AnimationComponent>(
            std::vector<TextureId>{
                    TextureId::MARIO_STAND,
                    TextureId::MARIO_GROWING,
                    TextureId::MARIO_STAND,
                    TextureId::MARIO_GROWING,
                    TextureId::SUPER_MARIO_STAND,
                    TextureId::MARIO_STAND,
                    TextureId::MARIO_GROWING,
                    TextureId::SUPER_MARIO_STAND,
                    TextureId::MARIO_STAND,
                    TextureId::SUPER_MARIO_STAND
            }, 6, false, false, false);

    player->assign<FrozenComponent>();
    player->assign<CallbackComponent>([&]() {
        player->remove<FrozenComponent>();
    }, 60);
}

void PlayerSystem::eatFlower(World* world) {
    world->create()->assign<SoundComponent>(Sound::Id::MUSHROOM_EAT);
    world->create()->assign<FloatingPointsComponent>(Points::ONETHOUSAND,
        player->get<TransformComponent>()->getCenterX(),
        player->get<TransformComponent>()->y
        );
    world->create()->assign<AddScoreComponent>(1000);

    if (player->has<SuperMarioComponent>())
    {
        player->remove<SuperMarioComponent>();
    }

    if (!player->has<FlowerMarioComponent>())
    {
        player->assign<FlowerMarioComponent>();
    }
    
    player->assign<AnimationComponent>(
        std::vector<TextureId>{
            TextureId::SUPER_MARIO_STAND,
            TextureId::FLOWER_MARIO_GROW,
            TextureId::SUPER_MARIO_STAND,
            TextureId::FLOWER_MARIO_GROW,
            TextureId::FLOWER_MARIO_STAND,
            TextureId::SUPER_MARIO_STAND,
            TextureId::FLOWER_MARIO_GROW,
            TextureId::FLOWER_MARIO_STAND,
            TextureId::SUPER_MARIO_STAND,
            TextureId::FLOWER_MARIO_STAND
    }, 6, false, false, false);

    player->assign<FrozenComponent>();
    player->assign<CallbackComponent>([&]() {
        player->remove<FrozenComponent>();
    }, 60);
}

// Player controls
void PlayerSystem::handleEvent(SDL_Event& event) {
    if (event.type != SDL_KEYDOWN && event.type != SDL_KEYUP) return;

    switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_A:
                    left = true;
                    break;
                case SDL_SCANCODE_LSHIFT:
                    fire = !sprint;
                    sprint = true;
                    break;
                case SDL_SCANCODE_D:
                    right = true;
                    break;
                case SDL_SCANCODE_W:
                    jump = true;
                    break;
                case SDL_SCANCODE_S:
                    duck = true;
                    break;
                default:
                    break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_A:
                    left = false;
                    break;
                case SDL_SCANCODE_LSHIFT:
                    sprint = false;
                    fire = false;
                    break;
                case SDL_SCANCODE_D:
                    right = false;
                    break;
                case SDL_SCANCODE_S:
                    duck = false;
                    break;
                default:
                    break;
            }
    }

    dirX = right - left;
}

void PlayerSystem::onRemovedFromWorld(World* world) {
    camera = nullptr;
    System::onRemovedFromWorld(world);
    world->destroy(player);
}

void PlayerSystem::onAddedToWorld(World* world) {
    System::onAddedToWorld(world);

    camera = world->findFirst<CameraComponent>()->get<CameraComponent>();
    player = world->create();
    player->assign<PlayerComponent>();

    player->assign<GravityComponent>();
    player->assign<SolidComponent>();
    player->assign<KineticComponent>();
    player->assign<TransformComponent>(40, 140, TILE_SIZE - 4, SMALL_MARIO_COLLIDER_HEIGHT);
}
