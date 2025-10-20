#include "Game.h"

GameplayScene::GameplayScene(SDL_Renderer* renderer): 
renderer(renderer),
texman(nullptr), 
entities(nullptr), 
player(nullptr), 
fonts(nullptr), 
camera(WINDOW_W, WINDOW_H), 
board(), 
ui(), 
uiMGR(), 
lastTicks(SDL_GetPerformanceCounter()) {}

GameplayScene::~GameplayScene() { cleanup(); }

bool GameplayScene::init() {
    if(!renderer) { std::cerr<<"Renderer failed: "<<SDL_GetError()<<"\n"; return false; }
    texman = std::make_unique<TextureManager>(renderer);
    entities = std::make_unique<EntityManager>();
    player = entities->create<Player>(VIRTUAL_WORLD_W/2.0f - 24.0f, VIRTUAL_WORLD_H/2.0f - 24.0f);
    player->texture = texman->load("./assets/graphics/player_test.png");
    fonts = std::make_unique<FontManager>();
    fonts->openFonts();
    board.Set(renderer, &camera, fonts.get());
    uiMGR = std::make_unique<UIManager>();
    ui.Set(player, *this);
    uiMGR->Set(renderer, fonts.get(), &ui);
    return true;
}

void GameplayScene::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_QUIT) {
        // You might want to mark this scene as finished
    }

    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        Vec2 mouseWorld{float(mx) + camera.pos.x, float(my) + camera.pos.y};
        Vec2 dir = mouseWorld - Vec2(player->pos.x + player->w / 2.0f, player->pos.y + player->h / 2.0f);
        Vec2 spawn = Vec2(player->pos.x + player->w / 2.0f - 4, player->pos.y + player->h / 2.0f - 4);
        entities->create<Bullet>(spawn.x, spawn.y, dir);
    }
}

void GameplayScene::update(float dt) {
    camera.follow(Vec2(player->pos.x + player->w / 2.0f, player->pos.y + player->h / 2.0f), dt);
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    player->handleInput(keystate, dt, *this);
    player->update(*this, dt);
    entities->updateAll(*this, dt);
    frameCount++;
    fpsTimer += dt;
    if (fpsTimer >= 1.0f) {
        fps = frameCount / fpsTimer;
        frameCount = 0;
        fpsTimer = 0.0f;
    }
}

void GameplayScene::render(SDL_Renderer* renderer, float dt) {
    board.renderBoard();
    uiMGR->drawUI();
    entities->renderAll(*this, renderer, dt);
    //player->render(*this, renderer, 0.0f);

    int mx, my;
    SDL_GetMouseState(&mx, &my);
    renderCrosshair(mx, my);
}



/*void GameplayScene::run() {
    bool quit = false;
    float fpsTimer = 0.0f;
    int frameCount = 0;
    //SDL_SetWindowSize(window,1600,900);
    //camera.Set(1600,900);
    SDL_DisplayMode test;
    SDL_GetCurrentDisplayMode(0, &test);
    //SDL_GetDisplayMode(0,0,&test);
    std::cout << test.w << "/" << test.h << "/" << test.refresh_rate << "/" << test.driverdata << std::endl;
    while(!quit) {
        Uint64 now = SDL_GetPerformanceCounter();
        float dt = float((now - lastTicks) / double(SDL_GetPerformanceFrequency()));
        lastTicks = now;
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) quit = true;
            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) quit = true;
            if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT){
                int mx, my; SDL_GetMouseState(&mx, &my);
                Vec2 mouseWorld{ float(mx) + camera.pos.x, float(my) + camera.pos.y };
                Vec2 dir = mouseWorld - Vec2(player->pos.x + player->w/2.0f, player->pos.y + player->h/2.0f);
                Vec2 spawn = Vec2(player->pos.x + player->w/2.0f - 4, player->pos.y + player->h/2.0f - 4);
                entities->create<Bullet>(spawn.x, spawn.y, dir);
            }
        }
        frameCount++;
        fpsTimer += dt;
        if (fpsTimer >= 1.0f) {
            fps = frameCount / fpsTimer;
            //std::cout << "FPS: " << fps << std::endl;
            frameCount = 0;
            fpsTimer = 0.0f;
        }
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        player->handleInput(keystate, dt, *this);
        int mx,my; SDL_GetMouseState(&mx,&my);
        player->update(*this, dt);
        entities->updateAll(*this, dt);
        camera.follow(Vec2(player->pos.x + player->w/2.0f, player->pos.y + player->h/2.0f), dt);
        board.renderBoard();
        uiMGR->drawUI();
        entities->renderAll(*this, renderer, dt);
        player->render(*this, renderer, dt);
        renderCrosshair(mx, my);
        SDL_RenderPresent(renderer);
    }
}*/

void GameplayScene::renderCrosshair(int sx, int sy) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect r{sx-8, sy-8, 16, 16};
    SDL_RenderDrawRect(renderer, &r);
    SDL_RenderDrawLine(renderer, sx-12, sy, sx+12, sy);
    SDL_RenderDrawLine(renderer, sx, sy-12, sx, sy+12);
}

void GameplayScene::cleanup() {
    texman.reset();
    entities.reset();
    fonts.reset();
    uiMGR.reset();
}
