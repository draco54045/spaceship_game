#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <memory>
#include <string>
#include <iostream>
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
#include "Scene.h"

class GameplayScene : public Scene {
public:
    GameplayScene(SDL_Renderer* r);
    ~GameplayScene();
    bool init();
    void run();
    void cleanup();
    void renderCrosshair(int sx, int sy);

    void handleEvent(const SDL_Event& e) override;
    void update(float dt) override;
    void render(SDL_Renderer* renderer, float dt) override;

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

    float fps;
private:
    Uint64 lastTicks;
    float fpsTimer = 0.0f;
    int frameCount = 0;
};
