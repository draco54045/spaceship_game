#include "SFXManager.h"

SFXManager::SFXManager(){ openSFX(); }
SFXManager::~SFXManager(){ closeSFX(); }

void SFXManager::openSFX(){
    player_shoot = Mix_LoadWAV("./assets/sfx/test.wav");
    if (!player_shoot) {
        std::cerr << "Mix_LoadWAV error: " << Mix_GetError() << std::endl;
    }
    sfx.push_back(player_shoot);
    button_press = Mix_LoadWAV("./assets/sfx/click.wav");
    if (!button_press) {
        std::cerr << "Mix_LoadWAV error: " << Mix_GetError() << std::endl;
    }
    sfx.push_back(button_press);
}

void SFXManager::closeSFX(){
    for (auto s : sfx) {
            if (s) Mix_FreeChunk(s);
        }
        sfx.clear();
        player_shoot = nullptr;
        button_press = nullptr;
}