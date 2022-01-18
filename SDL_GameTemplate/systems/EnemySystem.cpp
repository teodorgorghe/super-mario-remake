#include "EnemySystem.h"

// EnemySystem handles all about entities that has EnemyComponent.
// It also handles the fireball interactions and the Koopa Shell jump, kick.

void EnemySystem::onAddedToWorld(World* world) {
    System::onAddedToWorld(world);
}

// Koopa Troopa transforms into a shell when it get steped by Mario
void stepOnTurtle(Entity* enemy) {
    enemy->get<EnemyComponent>()->type = Enemy::Type::TURTLE_SHELL;
    enemy->remove<AnimationComponent>();
    enemy->remove<WalkComponent>();
    enemy->get<TextureComponent>()->id = TextureId::TURTLE_SHELL_1;
    enemy->get<TextureComponent>()->offSetY = 0;
    enemy->get<TextureComponent>()->h = TILE_SIZE;
    // After a certain time, if Mario doesn't kick the shell, Koopa will wake up.
    enemy->assign<CallbackComponent>([=]() {
        enemy->assign<AnimationComponent>(std::vector<TextureId>{
                                                  TextureId::TURTLE_SHELL_1,
                                                  TextureId::TURTLE_SHELL_2,
                                          },
                                          10,
                                          false);

        enemy->assign<CallbackComponent>([=]() {
            enemy->get<TextureComponent>()->id = TextureId::TURTLE_1;
            enemy->get<TextureComponent>()->offSetY = -TILE_SIZE;
            enemy->get<TextureComponent>()->h = TILE_SIZE * 2;
            enemy->get<TextureComponent>()->flipH = false;
            enemy->get<EnemyComponent>()->type = Enemy::Type::TURTLE;
            enemy->get<KineticComponent>()->accX = 0.0f;
            enemy->get<KineticComponent>()->speedX = 0.0f;
            enemy->assign<AnimationComponent>(std::vector<TextureId>{
                                                      TextureId::TURTLE_1,
                                                      TextureId::TURTLE_2,
                                              },
                                              10,
                                              false);
            enemy->assign<WalkComponent>(-.4f);
        }, 100);

    }, 200);
}

// If the enemy was killed, it will show the enemy falling animation.
void flipEnemy(Entity* enemy) {
    if (enemy->has<EnemyComponent>()) {
        enemy->remove<EnemyComponent>();
        enemy->remove<SolidComponent>();
        enemy->remove<AnimationComponent>();
        enemy->assign<TileComponent>();
        enemy->get<TextureComponent>()->flipV = true;
        enemy->get<KineticComponent>()->speedY = -8;
    }
}

// Koopa shell interactions - if the shell is spinning and collides with an enemy, then it will destroy de enemy and will add score to the player.
void turtleShellInteractions(World* world, Entity* shell) {
    if (shell->hasAny<LeftCollisionComponent, RightCollisionComponent>()) {
        world->find<EnemyComponent, KineticComponent>([&](Entity* other) {
            if (shell == other) return;
            auto transform = other->get<TransformComponent>();
            if (AABBCollision(shell->get<TransformComponent>(), other->get<TransformComponent>())) {
                world->create()->assign<FloatingPointsComponent>(Points::ONEHOUNDRED, transform->getCenterX(),
                                                                 transform->y);
                world->create()->assign<AddScoreComponent>(100);
                flipEnemy(other);
                shell->remove<LeftCollisionComponent>();
                shell->remove<RightCollisionComponent>();
            }
        });
    }

    // If has colliders with wall, or Mario
    if (shell->has<LeftCollisionComponent>()) {
        shell->get<KineticComponent>()->accX = 3.0f;
        shell->get<KineticComponent>()->speedX = 3.0f;
    }
    if (shell->has<RightCollisionComponent>()) {
        shell->get<KineticComponent>()->accX = -3.0f;
        shell->get<KineticComponent>()->speedX = -3.0f;
    }
}

// Fireball bounces in the ground and interaction with enemy
void fireballInteractions(World* world) {
    auto playerTransform = world->findFirst<PlayerComponent>()->get<TransformComponent>();
    world->find<FireballComponent, KineticComponent, TransformComponent>([&](Entity* fireball) {
        bool destroyed = false;
        if (fireball->get<TransformComponent>()->top() > TILE_SIZE * DEFAULT_MAP_HEIGHT)
        {
            fireball->clearComponents();
        }
        else
        {
                if (fireball->has<BottomCollisionComponent>())
                {
                    fireball->get<KineticComponent>()->accY = -FIREBALL_ACCELERATION;
                }
                world->find<EnemyComponent, TransformComponent>([&](Entity* enemy)
                {
                    if (fireball->hasAny<RightCollisionComponent, LeftCollisionComponent>()) {
                        if (enemy->hasAny<LeftCollisionComponent, RightCollisionComponent>())
                        {
                            if (AABBCollision(fireball->get<TransformComponent>(), enemy->get<TransformComponent>()) ||
                                AABBCollision(enemy->get<TransformComponent>(), fireball->get<TransformComponent>()))
                            {
                                auto enemyTransform = enemy->get<TransformComponent>();
                                world->create()->assign<FloatingPointsComponent>(Points::ONEHOUNDRED, enemyTransform->getCenterX(),
                                    enemyTransform->y);
                                world->create()->assign<SoundComponent>(Sound::Id::KICK);
                                world->create()->assign<AddScoreComponent>(100);
                                fireball->remove<AnimationComponent>();
                                fireball->remove<TransformComponent>();
                                fireball->remove<KineticComponent>();
                                fireball->remove<FireballComponent>();
                                fireball->assign<TransformComponent>(enemyTransform->x, enemyTransform->y, TILE_SIZE, TILE_SIZE);
                                fireball->assign<AnimationComponent>(
                                    std::vector<TextureId>{
                                    TextureId::EXPLOSION_1,
                                        TextureId::EXPLOSION_2,
                                        TextureId::EXPLOSION_3
                                }, 6, false, false
                                    );
                                flipEnemy(enemy);
                                fireball->assign<DestroyDelayedComponent>(60);
                                destroyed = true;
                            }
                        }
                    }
                });
                if (!destroyed)
                {
                    if (fireball->hasAny<RightCollisionComponent, LeftCollisionComponent>())
                    {
                        fireball->clearComponents();
                    }
                }

        }
    });
}

void EnemySystem::tick(World* world) {
    // Searches for every koopa in the screen and checks the state for each
    world->find<EnemyComponent, KineticComponent>([&](Entity* shell) {
        if (shell->has<EnemyComponent>() && shell->get<EnemyComponent>()->type == Enemy::Type::TURTLE_SHELL) {
            turtleShellInteractions(world, shell);
        }
    });

    fireballInteractions(world);

    // if Mario step in a Goomba, to the enemy will be assigned GrowComponent. After, this block checkes for the entity and changes the texture, add score and delay destruction
    // if Mario step in a koopa, it will call stepOnTurtle and adds score to the player.
    // if Mario step in a koopa shell, it will kick the shell.
    world->find<EnemyComponent, TransformComponent, CrushedComponent>([&](Entity* enemy) {
        auto enemyTransform = enemy->get<TransformComponent>();
        world->create()->assign<SoundComponent>(Sound::Id::STOMP);
        switch (enemy->get<EnemyComponent>()->type) {
            case Enemy::Type::GOOMBA: {
                enemy->clearComponents();
                enemy->assign<TileComponent>();
                enemy->assign<DestroyDelayedComponent>(50);
                enemy->assign<TextureComponent>(TextureId::GOOMBA_CRUSHED);
                enemy->assign<TransformComponent>(*enemyTransform);
                world->create()->assign<FloatingPointsComponent>(Points::ONEHOUNDRED, enemyTransform->getCenterX(),
                                                                 enemyTransform->y);
                world->create()->assign<AddScoreComponent>(100);
            }
                break;
            case Enemy::Type::TURTLE:
                stepOnTurtle(enemy);
                world->create()->assign<FloatingPointsComponent>(Points::TWOHOUNDRED, enemyTransform->getCenterX(),
                                                                 enemyTransform->y);
                world->create()->assign<AddScoreComponent>(200);
                break;
            case Enemy::Type::TURTLE_SHELL: {
                if (enemy->get<KineticComponent>()->accX == 0) {
                    auto player = world->findFirst<PlayerComponent>()->get<TransformComponent>()->getCenterX();
                    auto turtle = enemy->get<TransformComponent>()->getCenterX();
                    auto direction = turtle > player ? 3.0f : -3.0f;
                    enemy->get<KineticComponent>()->accX = direction;
                } else {
                    enemy->get<KineticComponent>()->accX = 0.0f;
                    enemy->get<KineticComponent>()->speedX = 0.0f;
                }
            }
        }
    });

    world->find<WalkComponent, LeftCollisionComponent>([&](Entity* entity) {
        auto walkComponent = entity->get<WalkComponent>();
        walkComponent->speed = std::abs(walkComponent->speed);
        if (entity->has<TextureComponent>() && entity->has<EnemyComponent>()) {
            entity->get<TextureComponent>()->flipH = true;
        }
    });

    world->find<BottomCollisionComponent, TransformComponent, KineticComponent, EnemyComponent>([&](Entity* entity) {
        auto transform = entity->get<TransformComponent>();
        auto tileMap = world->findFirst<TileMapComponent>()->get<TileMapComponent>();
        auto bottomTile = tileMap->get(
                (int) (transform->getCenterX() / TILE_SIZE),
                (int) (transform->getCenterY() / TILE_SIZE) + 1
        );
        if (bottomTile && bottomTile->has<BreakableComponent>()) {
            if (bottomTile->get<BreakableComponent>()->hit) {
                world->create()->assign<FloatingPointsComponent>(Points::ONEHOUNDRED, transform->getCenterX(),
                                                                 transform->y);
                world->create()->assign<AddScoreComponent>(100);
                flipEnemy(entity);
            }
        }
    });

    // Changes the enemy direction if it collides at right
    world->find<WalkComponent, RightCollisionComponent>([&](Entity* entity) {
        auto walkComponent = entity->get<WalkComponent>();
        walkComponent->speed = -std::abs(walkComponent->speed);
        if (entity->has<TextureComponent>() && entity->has<EnemyComponent>()) {
            entity->get<TextureComponent>()->flipH = false;
        }
    });
    world->find<WalkComponent, KineticComponent>([&](Entity* entity) {
        entity->get<KineticComponent>()->speedX = entity->get<WalkComponent>()->speed;
    });

    world->find<EnemyComponent>([&](Entity* entity) {
        entity->remove<BottomCollisionComponent>();
        entity->remove<TopCollisionComponent>();
        entity->remove<LeftCollisionComponent>();
        entity->remove<RightCollisionComponent>();
        entity->remove<CrushedComponent>();
    });
}

void EnemySystem::onRemovedFromWorld(World* world) {
    System::onRemovedFromWorld(world);
}
