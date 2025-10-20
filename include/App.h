#pragma once
#include <SDL.h>
//#include "Scene.h"
#include "MainMenu.h"
#include "Game.h"
#include <iostream>
#include <memory>

class App {
public:
    App() : window(nullptr), renderer(nullptr) {}
    ~App() { cleanup(); }

    bool init();
    void run();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::unique_ptr<Scene> currentScene;

    void cleanup();
};
