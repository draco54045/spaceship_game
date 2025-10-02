// sdl2_topdown_game.cpp
// Extendable 2D top-down game skeleton using SDL2 + SDL2_image
// Fixes incomplete type issues by moving Game class definition above Entity classes

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <string>
#include <unordered_map>
#include <algorithm>

// ---------------------- Config ----------------------
static const int WINDOW_W = 800;
static const int WINDOW_H = 600;
static const int VIRTUAL_WORLD_W = 2000; // world size (for demo)
static const int VIRTUAL_WORLD_H = 2000;

// ---------------------- Utility Math ----------------------
struct Vec2 {
    float x = 0;
    float y = 0;
    Vec2() = default;
    Vec2(float _x, float _y): x(_x), y(_y) {}
    Vec2 operator+(const Vec2&o) const { return {x+o.x, y+o.y}; }
    Vec2 operator-(const Vec2&o) const { return {x-o.x, y-o.y}; }
    Vec2 operator*(float s) const { return {x*s, y*s}; }
};

float length(const Vec2 &v) { return std::sqrt(v.x*v.x + v.y*v.y); }
Vec2 normalize(const Vec2 &v) { float l = length(v); return l==0?Vec2(0,0):Vec2(v.x/l, v.y/l); }

// ---------------------- Camera ----------------------
class Camera {
public:
    Camera(int w, int h): screenW(w), screenH(h) {}
    void follow(const Vec2 &target, float /*dt*/) {
        Vec2 center = {target.x - screenW/2.0f, target.y - screenH/2.0f};
        pos.x += (center.x - pos.x) * (0.15f);
        pos.y += (center.y - pos.y) * (0.15f);
        clamp();
    }
    SDL_Rect worldToScreen(const SDL_Rect &r) const {
        SDL_Rect s = r;
        s.x = int(r.x - pos.x);
        s.y = int(r.y - pos.y);
        return s;
    }
    void clamp() {
        if(pos.x < 0) pos.x = 0;
        if(pos.y < 0) pos.y = 0;
        if(pos.x > VIRTUAL_WORLD_W - screenW) pos.x = VIRTUAL_WORLD_W - screenW;
        if(pos.y > VIRTUAL_WORLD_H - screenH) pos.y = VIRTUAL_WORLD_H - screenH;
    }
    Vec2 pos{0,0};
private:
    int screenW, screenH;
};

// Forward declarations
class Entity;
class Player;
class TextureManager;
class EntityManager;

// ---------------------- Game ----------------------
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

// ---------------------- TextureManager ----------------------
class TextureManager {
public:
    TextureManager(SDL_Renderer* r): renderer(r) {}
    ~TextureManager() { for(auto &p: textures) if(p.second) SDL_DestroyTexture(p.second); }

    SDL_Texture* load(const std::string &path) {
        if(textures.count(path)) return textures[path];
        SDL_Surface* surf = IMG_Load(path.c_str());
        if(!surf) {
            std::cerr << "IMG_Load failed for "<<path<<" -> "<<IMG_GetError()<<"\n";
            textures[path] = nullptr;
            return nullptr;
        }
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_FreeSurface(surf);
        if(!tex) std::cerr<<"CreateTextureFromSurface failed: "<<SDL_GetError()<<"\n";
        textures[path] = tex;
        return tex;
    }
private:
    SDL_Renderer* renderer;
    std::unordered_map<std::string, SDL_Texture*> textures;
};

// ---------------------- Entity ----------------------
enum class Team { Neutral, Player, Enemy, Bullet };

class Entity {
public:
    Entity(float x, float y, int w, int h): pos{x,y}, w(w), h(h) {}
    virtual ~Entity() = default;
    virtual void update(Game& /*g*/, float /*dt*/) {}
    virtual void render(Game& g, SDL_Renderer* renderer) {
        SDL_Rect r{int(pos.x), int(pos.y), w, h};
        auto screen = g.camera.worldToScreen(r);
        SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
        SDL_RenderFillRect(renderer, &screen);
    }
    SDL_Rect aabb() const { return SDL_Rect{int(pos.x), int(pos.y), w, h}; }
    Vec2 pos;
    int w, h;
    bool alive = true;
    Team team = Team::Neutral;
};

// ---------------------- Player ----------------------
class Player : public Entity {
public:
    Player(float x, float y): Entity(x,y,48,48) { team = Team::Player; }
    void handleInput(const Uint8* k, float dt) {
        Vec2 vel{0,0};
        if(k[SDL_SCANCODE_W] || k[SDL_SCANCODE_UP]) vel.y -= 1;
        if(k[SDL_SCANCODE_S] || k[SDL_SCANCODE_DOWN]) vel.y += 1;
        if(k[SDL_SCANCODE_A] || k[SDL_SCANCODE_LEFT]) vel.x -= 1;
        if(k[SDL_SCANCODE_D] || k[SDL_SCANCODE_RIGHT]) vel.x += 1;
        if(length(vel) > 0) vel = normalize(vel);
        pos = pos + vel * (speed * dt);
        if(pos.x < 0) pos.x = 0;
        if(pos.y < 0) pos.y = 0;
        if(pos.x + w > VIRTUAL_WORLD_W) pos.x = VIRTUAL_WORLD_W - w;
        if(pos.y + h > VIRTUAL_WORLD_H) pos.y = VIRTUAL_WORLD_H - h;
    }
    void update(Game& /*g*/, float /*dt*/) override {}
    void render(Game& g, SDL_Renderer* renderer) override {
        SDL_Rect r{int(pos.x), int(pos.y), w, h};
        auto screen = g.camera.worldToScreen(r);
        SDL_SetRenderDrawColor(renderer, 30, 144, 255, 255);
        SDL_RenderFillRect(renderer, &screen);
    }
    float speed = 300.0f;
};

// ---------------------- Bullet ----------------------
class Bullet : public Entity {
public:
    Bullet(float x, float y, Vec2 dir): Entity(x,y,8,8), dir(normalize(dir)) { team = Team::Bullet; }
    void update(Game& /*g*/, float dt) override {
        pos = pos + dir * (speed * dt);
        life -= dt;
        if(life <= 0) alive = false;
        if(pos.x < -50 || pos.y < -50 || pos.x > VIRTUAL_WORLD_W+50 || pos.y > VIRTUAL_WORLD_H+50) alive = false;
    }
    void render(Game& g, SDL_Renderer* renderer) override {
        SDL_Rect r{int(pos.x), int(pos.y), w, h};
        auto screen = g.camera.worldToScreen(r);
        SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
        SDL_RenderFillRect(renderer, &screen);
    }
    Vec2 dir;
    float speed = 600.0f;
    float life = 2.5f;
};

// ---------------------- EntityManager ----------------------
class EntityManager {
public:
    template<typename T, typename...Args>
    T* create(Args&&... a) {
        auto e = std::make_unique<T>(std::forward<Args>(a)...);
        T* ptr = e.get();
        entities.push_back(std::move(e));
        return ptr;
    }
    void updateAll(Game& g, float dt) {
        for(auto &e: entities) if(e->alive) e->update(g, dt);
        entities.erase(std::remove_if(entities.begin(), entities.end(), [](const std::unique_ptr<Entity>& p){ return !p->alive; }), entities.end());
    }
    void renderAll(Game& g, SDL_Renderer* renderer) {
        for(auto &e: entities) if(e->alive) e->render(g, renderer);
    }
    std::vector<std::unique_ptr<Entity>> entities;
};

// ---------------------- Game Implementation ----------------------
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
            //if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
            if(e.type == SDL_MOUSEBUTTONDOWN && e.button.clicks == SDL_BUTTON_LEFT){
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

        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
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
    const int cell = 64;
    SDL_SetRenderDrawColor(renderer, 40, 40, 50, 255);
    int left = int(camera.pos.x);
    int top = int(camera.pos.y);
    int right = left + WINDOW_W;
    int bottom = top + WINDOW_H;
    for(int gx = (left / cell) * cell; gx <= right; gx += cell) {
        SDL_Rect line = {gx, top, 1, WINDOW_H};
        auto s = camera.worldToScreen(line);
        SDL_RenderFillRect(renderer, &s);
    }
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

// ---------------------- main ----------------------
int main(int /*argc*/, char** /*argv*/) {
    Game g;
    if(!g.init()) return 1;
    g.run();
    return 0;
}
