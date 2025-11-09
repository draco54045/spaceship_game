#pragma once
#include <SDL_mixer.h>
#include <iostream>
#include <vector>

class SFXManager {
public:
    SFXManager();
    ~SFXManager();
    void closeSFX();
    void openSFX();
    std::vector<Mix_Chunk*> sfx;
    Mix_Chunk* player_shoot = nullptr;
    Mix_Chunk* button_press = nullptr;
};