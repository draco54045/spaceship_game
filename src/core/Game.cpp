#include "Game.h"

Game::Game(): window(nullptr), renderer(nullptr), texman(nullptr), entities(nullptr), player(nullptr), camera(WINDOW_W, WINDOW_H) {}
Game::~Game() { cleanup(); }

bool Game::init() {
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n"; return false;
    }
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "IMG_Init failed: " << IMG_GetError() << "\n";
    }
    window = SDL_CreateWindow("TopDown - SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
    if(!window) { std::cerr << "Window creation failed: "<<SDL_GetError()<<"\n"; return false; }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer) { std::cerr<<"Renderer failed: "<<SDL_GetError()<<"\n"; return false; }
    texman = std::make_unique<TextureManager>(renderer);
    entities = std::make_unique<EntityManager>();
    player = entities->create<Player>(VIRTUAL_WORLD_W/2.0f - 24.0f, VIRTUAL_WORLD_H/2.0f - 24.0f);
    lastTicks = SDL_GetPerformanceCounter();
    return true;
}

void Game::run() {
    bool quit = false;
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
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        player->handleInput(keystate, dt);

        entities->updateAll(*this, dt);
        camera.follow(Vec2(player->pos.x + player->w/2.0f, player->pos.y + player->h/2.0f), dt);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        renderGrid();
        entities->renderAll(*this, renderer);
        player->render(*this, renderer);

        int mx,my; SDL_GetMouseState(&mx,&my);
        renderCrosshair(mx, my);

        SDL_RenderPresent(renderer);
    }
}

void Game::renderGrid() {
    const int cell = 128;
    
    int left = int(camera.pos.x);
    int top = int(camera.pos.y);
    int right = left + WINDOW_W;
    int bottom = top + WINDOW_H;
    int i;
    for(int gx = (left / cell) * cell; gx <= right; gx += cell) {
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_Rect line = {gx, top, 1, WINDOW_H};
        auto s = camera.worldToScreen(line);
        SDL_RenderFillRect(renderer, &s);

        //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        //SDL_Rect line2 = {gx-10, top, 1, WINDOW_H};
        //auto ss = camera.worldToScreen(line2);
        //SDL_RenderFillRect(renderer, &ss);
    }
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    for(int gy = (top / cell) * cell; gy <= bottom; gy += cell) {
        SDL_Rect line = {left, gy, WINDOW_W, 1};
        auto s = camera.worldToScreen(line);
        SDL_RenderFillRect(renderer, &s);
    }
}

void Game::renderCrosshair(int sx, int sy) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect r{sx-8, sy-8, 16, 16};
    SDL_RenderDrawRect(renderer, &r);
    SDL_RenderDrawLine(renderer, sx-12, sy, sx+12, sy);
    SDL_RenderDrawLine(renderer, sx, sy-12, sx, sy+12);
}

void Game::cleanup() {
    texman.reset();
    entities.reset();
    if(renderer) SDL_DestroyRenderer(renderer);
    if(window) SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}
