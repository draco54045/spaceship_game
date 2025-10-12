#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <memory>
#include <string>
#include "Board.h"
#include "Camera.h"
#include "TextureManager.h"
#include "EntityManager.h"
#include "FontManager.h"
#include "UIManager.h"
#include "UI.h"
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
    void renderCrosshair(int sx, int sy);

    SDL_Window* window;
    SDL_Renderer* renderer;
    std::unique_ptr<TextureManager> texman;
    std::unique_ptr<EntityManager> entities;
    std::unique_ptr<FontManager> fonts;
    std::unique_ptr<UIManager> uiMGR;
    Player* player;
    Camera camera;
    TTF_Font* font = nullptr;
    Board board;
    UI ui;
private:
    Uint64 lastTicks;
};
