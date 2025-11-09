#pragma once
#include "SimpleIni.h"

//static const int WINDOW_W = 1280;
//static const int WINDOW_H = 800;
static const int VIRTUAL_WORLD_W = 16384;
static const int VIRTUAL_WORLD_H = 16384;
static const void forceFullRedraw(SDL_Renderer* renderer, int w, int h) { //helper for the settings menu
    //std::cout << "test here" << std::endl;
    SDL_RenderSetViewport(renderer, nullptr);
    SDL_RenderSetLogicalSize(renderer, w, h);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

class Config {
public:
    int windowWidth;
    int windowHeight;
    float scaleX;
    float scaleY;
    bool isFullScreened;

    int master_volume;
    int music_volume;
    int sfx_volume;

    SDL_Color red = {255,0,0,255};
    SDL_Color blue = {0,0,255,255};
    SDL_Color green = {0,255,0,255};
    SDL_Color white = {255,255,255,255};
    SDL_Color black = {0,0,0,255};
    SDL_Color none = {0,0,0,0};

    static Config& get() {
        static Config instance;
        return instance;
    }

    void updateScale() {
        scaleX = windowWidth / 1280.0f;
        scaleY = windowHeight / 720.0f;
    }

    void loadFromFile(const std::string& filename){
        CSimpleIniA ini;
        ini.SetUnicode();
        ini.LoadFile(filename.c_str());
        windowWidth = ini.GetLongValue("window", "width", 1280);
        windowHeight = ini.GetLongValue("window", "height", 720);
        isFullScreened = ini.GetLongValue("window", "fullscreen", false);
        master_volume = ini.GetLongValue("audio", "master", 64);
        music_volume = ini.GetLongValue("audio", "music", 64);
        sfx_volume = ini.GetLongValue("audio", "sfx", 64);
        updateScale();
    }
    void writeToFile(const std::string& filename){
        CSimpleIniA ini;
        ini.SetUnicode();
        ini.LoadFile(filename.c_str());
        ini.SetLongValue("window", "width", windowWidth);
        ini.SetLongValue("window", "height", windowHeight);
        ini.SetLongValue("window", "fullscreen", isFullScreened);
        ini.SetLongValue("audio", "master", master_volume);
        ini.SetLongValue("audio", "music", music_volume);
        ini.SetLongValue("audio", "sfx", sfx_volume);
        SI_Error rc = ini.SaveFile(filename.c_str());
        if (rc < 0) {
            std::cout << "failed to write to file : " << filename.c_str() << std::endl;
        }
    }

private:
    Config() : 
    windowWidth(1280), 
    windowHeight(720), 
    scaleX(1),
    scaleY(1),
    isFullScreened(false), 
    master_volume(0),
    music_volume(0),
    sfx_volume(0)
    {}
};
