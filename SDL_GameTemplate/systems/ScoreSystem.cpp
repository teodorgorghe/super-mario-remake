#include "ScoreSystem.h"
#include "../Game.h"
void ScoreSystem::onAddedToWorld(World* world) {
    System::onAddedToWorld(world);

    auto paddingH = 22;
    auto paddingV = 16;
    auto textHeight = 8;
    auto spacingV = -0;

    auto availableWidth = SNES_RESOLUTION_WIDTH - paddingH * 2;
    auto columns = 4;
    auto columnWidth = availableWidth / columns;

    //Prima coloana
    auto marioEntity = world->create();
    marioEntity->assign<TextComponent>("MARIO");
    marioEntity->assign<TransformComponent>(paddingH, paddingV, 40, textHeight);

    scoreEntity = world->create();
    auto scoreString = std::to_string(score);
    auto final = std::string{};
    auto zeros = 6 - scoreString.length();
    while (zeros > 0) {
        zeros--;
        final += '0';
    }
    final += scoreString;
    scoreEntity->assign<TextComponent>(std::move(final));
    scoreEntity->assign<TransformComponent>(paddingH, paddingV + textHeight + spacingV, 48, textHeight);

    //A doua coloana
    coinsEntity = world->create();
    coinsEntity->assign<TextComponent>("x00");
    auto w = 24;
    coinsEntity->assign<TransformComponent>(paddingH + columnWidth + (columnWidth - w) / 2 + 5,
                                            paddingV + textHeight + spacingV, w, textHeight);
    auto coinIco = world->create();
    coinIco->assign<TextureComponent>(TextureId::COIN_SMALL_1);
    coinIco->assign<AnimationComponent>(
            std::vector<TextureId>{
                    TextureId::COIN_SMALL_1,
                    TextureId::COIN_SMALL_1,
                    TextureId::COIN_SMALL_1,
                    TextureId::COIN_SMALL_3,
                    TextureId::COIN_SMALL_2,
                    TextureId::COIN_SMALL_3
            }, 10);

    coinIco->assign<TextComponent>("");
    coinIco->assign<TransformComponent>(
            paddingH + columnWidth + (columnWidth - w) / 2 - 4,
            paddingV + textHeight + spacingV, 5, 8
    );

    // A treia coloana
    auto worldEntity = world->create();
    worldEntity->assign<TextComponent>("WORLD");
    w = 40;
    worldEntity->assign<TransformComponent>(paddingH + columnWidth * 2 + (columnWidth - w) / 2,
                                            paddingV, w, textHeight);

    auto worldNumberEntity = world->create();
    worldNumberEntity->assign<TextComponent>(worldname.c_str() + 6*sizeof(char));
    w = 24;
    worldNumberEntity->assign<TransformComponent>(paddingH + columnWidth * 2 + (columnWidth - w) / 2,
                                                  paddingV + textHeight + spacingV, w, textHeight);

    //A patra coloana
    auto timeEntity = world->create();
    timeEntity->assign<TextComponent>("TIME");
    timeEntity->assign<TransformComponent>(SNES_RESOLUTION_WIDTH - paddingH - 32, paddingV, 32, textHeight);

    timeLeftEntity = world->create();
    timeLeftEntity->assign<TextComponent>("000");
    timeLeftEntity->assign<TransformComponent>(SNES_RESOLUTION_WIDTH - paddingH - 24,
                                               paddingV + textHeight + spacingV, 24, textHeight);
}

void ScoreSystem::tick(World* world) {

    bool updateScore = false;
    bool updateCoins = false;

    // Find AddScoreComponent entities and update stats to the scoreboard
    for (auto e : world->find<AddScoreComponent>()) {
        auto s = e->get<AddScoreComponent>();
        score += s->score;
        updateScore = true;
        if (s->coin) {
            coins++;
            updateCoins = true;
        }
        world->destroy(e);
    }

    if (updateScore) {
        auto scoreString = std::to_string(score);
        auto final = std::string{};
        auto zeros = 6 - scoreString.length();
        while (zeros > 0) {
            zeros--;
            final += '0';
        }
        final += scoreString;
        scoreEntity->assign<TextComponent>(std::move(final));
    }

    if (updateCoins) {
        auto final = std::string{};
        auto coinString = std::to_string(coins);
        auto zeros = 2 - coinString.length();
        while (zeros > 0) {
            zeros--;
            final += '0';
        }
        final += coinString;
        coinsEntity->assign<TextComponent>("x" + final);
    }

    if (showTime)
    {
        time--;
        if (time >= 0) {
            auto final = std::string{};
            auto timeString = std::to_string(time / 60);
            auto zeros = 3 - timeString.length();
            while (zeros > 0) {
                zeros--;
                final += '0';
            }
            final += timeString;
            timeLeftEntity->assign<TextComponent>(std::move(final));
            if (time <= 0) timeLeftEntity->assign<GameOverComponent>();
        }
    }

    for (auto points : world->find<FloatingPointsComponent>()) {
        auto camera = world->findFirst<CameraComponent>()->get<CameraComponent>();
        auto pointsComponent = points->get<FloatingPointsComponent>();

        auto pointEntity = world->create();

        TextureId textureId;
        switch (pointsComponent->points) {
        case Points::ONEHOUNDRED:
            textureId = TextureId::ONEHUNDRED;
            break;
        case Points::TWOHOUNDRED:
            textureId = TextureId::TWOHUNDRED;
            break;
        case Points::ONETHOUSAND:
            textureId = TextureId::THOUSAND;
            break;
        case Points::ONEUP:
            textureId = TextureId::ONE_UP_LABEL;
            break;
        }
        pointEntity->assign<TextureComponent>(textureId);
        pointEntity->assign<TextComponent>("");
        pointEntity->assign<TransformComponent>((pointsComponent->x - camera->left()),
            pointsComponent->y, 0, 0);
        pointEntity->assign<KineticComponent>(0, 0);
        pointEntity->get<KineticComponent>()->speedY = -2.0f;
        pointEntity->assign<CallbackComponent>([=]() { world->destroy(pointEntity); }, 50);
        world->destroy(points);
    }
    for (auto stats : world->find<TransferStatsComponent>())
    {
        auto statsComponent = stats->get<TransferStatsComponent>();
        auto& SceneList = Game::currentScene->SceneList;
        if (statsComponent->index >= 0)
        {
            if (statsComponent->transferBitset[0]) SceneList[statsComponent->index].score = score;
            if (statsComponent->transferBitset[1]) SceneList[statsComponent->index].time = ceil((double)time / 60);
            // Lives needs to be added
        }
        world->destroy(stats);
    }
}

void ScoreSystem::handleEvent(SDL_Event& event) {
    System::handleEvent(event);
}

void ScoreSystem::onRemovedFromWorld(World* world) {
    System::onRemovedFromWorld(world);
    world->destroy(coinsEntity);
    world->destroy(scoreEntity);
}
