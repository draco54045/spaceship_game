#include "MusicManager.h"

MusicManager::MusicManager(){ openMusic(); }
MusicManager::~MusicManager(){ closeMusic(); }

void MusicManager::openMusic(){
    test_track = Mix_LoadMUS("./assets/music/temp.ogg");
    if (!test_track) {
        std::cerr << "Mix_LoadMUS error: " << Mix_GetError() << std::endl;
    }
    music.push_back(test_track);
}

void MusicManager::closeMusic(){
    for (auto m : music) {
            if (m) Mix_FreeMusic(m);
        }
        music.clear();
        test_track = nullptr;
}