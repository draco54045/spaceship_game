#include "Game.h"

GameplayScene::GameplayScene(SDL_Renderer* renderer, SDL_Window* w, FontManager* f, SFXManager* s, MusicManager* m, SettingsMenu* stm) : 
renderer(renderer),
window(w),
texman(nullptr), 
entities(nullptr), 
player(nullptr), 
fonts(f),
sfx(s),
music(m),
board(), 
ui(), 
uiMGR(),
psMenu(f),
stMenu(stm),
lastTicks(SDL_GetPerformanceCounter()) {
    auto& cfg = Config::get();
    camera = Camera(cfg.windowWidth, cfg.windowHeight);
}

GameplayScene::~GameplayScene() { cleanup(); }

bool GameplayScene::init() {
    auto& gState = GameState::get();
    gState.paused = false;
    if(!renderer) { std::cerr<<"Renderer failed: "<<SDL_GetError()<<"\n"; return false; }
    texman = std::make_unique<TextureManager>(renderer);
    entities = std::make_unique<EntityManager>();
    player = entities->create<Player>(VIRTUAL_WORLD_W/2.0f - 24.0f, VIRTUAL_WORLD_H/2.0f - 24.0f);
    player->texture = texman->load("./assets/graphics/player_test.png");
    //fonts = std::make_unique<FontManager>();
    //fonts->openFonts();
    board.Set(renderer, &camera, fonts);
    uiMGR = std::make_unique<UIManager>();
    ui.Set(player, *this);
    uiMGR->Set(renderer, fonts, &ui);
    psMenu.Set(renderer, fonts);
    stMenu->Set(renderer, fonts);
    Mix_PlayMusic(music->test_track, -1);
    return true;
}

void GameplayScene::handleEvent(const SDL_Event& e) {
    auto& cfg = Config::get();
    auto& gState = GameState::get();
    if (e.type == SDL_QUIT) {

    }
    if (gState.paused == true && gState.settings == true){ //handle settings non clicks
        stMenu->handleEventOther(e);
    }
    if (e.type == SDL_MOUSEBUTTONDOWN && gState.paused == true && gState.settings == true) { // handle settings
        stMenu->handleEventClick(e, window, sfx, &psMenu, &camera, NULL);
        int width, height;
        SDL_GetWindowSize(window, &width, &height);
        forceFullRedraw(renderer, width, height); 
        SDL_RenderClear(renderer);
        return;
    }
    if (e.type == SDL_MOUSEBUTTONDOWN && gState.paused == true && gState.settings == false) { //handle pause
        int x = e.button.x, y = e.button.y;
        if (psMenu.buttons[0].wasClicked(x,y)){ //quit button 
            Mix_PlayChannel(-1, sfx->button_press, 0);
            SDL_Delay(500);
            SDL_Event quitEvent;
            quitEvent.type = SDL_QUIT;
            SDL_PushEvent(&quitEvent);
        } 
        if (psMenu.buttons[2].wasClicked(x,y)){ //savequit button 
            Mix_PlayChannel(-1, sfx->button_press, 0);
            SDL_Delay(100);
            return;
        }
        if (psMenu.buttons[1].wasClicked(x,y)){ //settings button
            Mix_PlayChannel(-1, sfx->button_press, 0);
            SDL_Delay(100);
            gState.settings = true;
            return;
        }

    }
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && gState.paused == false && gState.settings == false) { //handle gameplay
        Mix_PlayChannel(-1, sfx->player_shoot, 0);
        int mx, my;
        SDL_GetMouseState(&mx, &my);        
        Vec2 mouseWorld{float(mx) + camera.pos.x, float(my) + camera.pos.y};
        //Vec2 dir = mouseWorld - Vec2(player->pos.x + player->w / 2.0f, player->pos.y + player->h / 2.0f);
        Vec2 dir = {player->forward.x, player->forward.y};
        Vec2 spawn = Vec2(player->pos.x + player->w / 2.0f - 4, player->pos.y + player->h / 2.0f - 4);
        entities->create<Bullet>(spawn.x, spawn.y, dir);
    }
}

void GameplayScene::update(float dt) {
    auto& gState = GameState::get();
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
    if (gState.paused == true){

    }
}

void GameplayScene::render(SDL_Renderer* renderer, float dt) {
    auto& gState = GameState::get();
    auto& cfg = Config::get();
    board.renderBoard();
    uiMGR->drawUI();
    entities->renderAll(*this, renderer, dt);
    //player->render(*this, renderer, 0.0f);

    int mx, my;
    SDL_GetMouseState(&mx, &my);
    renderCrosshair(mx, my);
    if (gState.paused == true){
        SDL_Rect bck = {0,0,cfg.windowWidth,cfg.windowHeight};
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer,0,0,0,100);
        SDL_RenderDrawRect(renderer, &bck);
        SDL_RenderFillRect(renderer, &bck);
        if (gState.settings == true){
            stMenu->drawMenu(renderer);
        }
        else{
            psMenu.drawMenu(renderer);
        }
    }
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
    //fonts.reset();
    uiMGR.reset();
}
