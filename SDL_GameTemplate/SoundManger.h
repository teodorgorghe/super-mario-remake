#pragma once
#include "SDL_mixer.h"
#include <unordered_map>

namespace Sound {
    enum Id {
        JUMP,
        SUPER_JUMP,
        BLOCK_HIT,
        BLOCK_BREAK,
        STOMP,
        MUSHROOM_GROW,
        MUSHROOM_EAT,
        FIREBALL,
        COIN,
        DEATH,
        SHRINK,
        ONE_UP,
        BUMP,
        KICK,
        PIPE
    };
}

namespace Music {
    enum Id {
        NONE,
        BACKGROUND,
        LEVEL_END,
        UNDERGROUND,
        CASTLE
    };
}

class SoundManager {

public:
    SoundManager();

    void playSound(Sound::Id sound);
    void playMusic(Music::Id sound);
    void stopMusic();

    ~SoundManager();

    std::unordered_map<Sound::Id, Mix_Chunk*> sounds;
    std::unordered_map<Music::Id, Mix_Music*> musics;
};


