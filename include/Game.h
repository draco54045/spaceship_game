#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <memory>
#include "Camera.h"
#include "TextureManager.h"
#include "EntityManager.h"
#include "Player.h"
#include "Bullet.h"
#include "Config.h"

class Game {
public:
    Game();
    ~Game();
    bool init();
    void run();
    void cleanup();
    void renderGrid();
    void renderCrosshair(int sx, int sy);

    SDL_Window* window;
    SDL_Renderer* renderer;
    std::unique_ptr<TextureManager> texman;
    std::unique_ptr<EntityManager> entities;
    Player* player;
    Camera camera;
private:
    Uint64 lastTicks;
};
