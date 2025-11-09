#pragma once
#include <SDL_mixer.h>
#include <iostream>
#include <vector>

class MusicManager {
public:
    MusicManager();
    ~MusicManager();
    void closeMusic();
    void openMusic();
    std::vector<Mix_Music*> music;
    Mix_Music* test_track = nullptr;
};