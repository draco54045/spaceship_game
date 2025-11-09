#pragma once
#include <SDL.h>
#include "MainMenu.h"
#include "Game.h"
#include <iostream>
#include <memory>
#include "FontManager.h"
#include "GameState.h"
#include <typeinfo>

class App {
public:
    App() : window(nullptr), renderer(nullptr), fonts(nullptr), sfx(nullptr), music(nullptr), stMenu(nullptr) {}
    ~App() { cleanup(); }

    bool init();
    void run();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::unique_ptr<Scene> currentScene;
    std::unique_ptr<FontManager> fonts;
    std::unique_ptr<SFXManager> sfx;
    std::unique_ptr<MusicManager> music;
    std::unique_ptr<SettingsMenu> stMenu;

    void cleanup();
};
