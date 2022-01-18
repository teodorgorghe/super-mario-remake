#include "SoundSystem.h"

void SoundSystem::onAddedToWorld(World* world) {
    System::onAddedToWorld(world);
    soundManager = new SoundManager();
    switch (theme)
    {
    case Theme::UNDERGROWND:
        world->create()->assign<MusicComponent>(Music::Id::UNDERGROUND);
        break;
    case Theme::CASTLE:
        world->create()->assign<MusicComponent>(Music::Id::CASTLE);
        break;
    default:
        world->create()->assign<MusicComponent>(Music::Id::BACKGROUND);
        break;
    }
}

void SoundSystem::tick(World* world) {
    auto music = world->findFirst<MusicComponent>();
    if (music) soundManager->playMusic(music->get<MusicComponent>()->music);

    for (auto sound : world->find<SoundComponent>()) {
        if (sound->get<SoundComponent>()->sound == Sound::DEATH) soundManager->stopMusic();
        soundManager->playSound(sound->get<SoundComponent>()->sound);
        world->destroy(sound);
    }
    world->destroy(music);
}

void SoundSystem::handleEvent(SDL_Event& event) {
    System::handleEvent(event);
}

void SoundSystem::onRemovedFromWorld(World* world) {
    System::onRemovedFromWorld(world);
    delete soundManager;
}
