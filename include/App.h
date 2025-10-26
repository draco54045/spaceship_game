#pragma once
#include <SDL.h>
#include "MainMenu.h"
#include "Game.h"
#include <iostream>
#include <memory>
#include "FontManager.h"

class App {
public:
    App() : window(nullptr), renderer(nullptr), fonts(nullptr) {}
    ~App() { cleanup(); }

    bool init();
    void run();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::unique_ptr<Scene> currentScene;
    std::unique_ptr<FontManager> fonts;

    void cleanup();
};
