#include "App.h"

bool App::init() {
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n"; return false;
    }
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "IMG_Init failed: " << IMG_GetError() << "\n";
    }
    if (TTF_Init() == -1) {
        std::cout << "Error initializing SDL_ttf: "<< SDL_GetError();
        return false;
    }
    auto& cfg = Config::get();
    window = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, cfg.windowWidth, cfg.windowHeight, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer) { std::cerr<<"Renderer failed: "<<SDL_GetError()<<"\n"; return false; }

    fonts = std::make_unique<FontManager>();
    currentScene = std::make_unique<MainMenuScene>(renderer, fonts.get());
    return true;
}

void App::run() {
    bool running = true;
    Uint64 lastTicks = SDL_GetPerformanceCounter();
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            currentScene->handleEvent(e);
        }

        Uint64 now = SDL_GetPerformanceCounter();
        float dt = (now - lastTicks) / static_cast<float>(SDL_GetPerformanceFrequency());
        lastTicks = now;
        currentScene->update(dt);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        currentScene->render(renderer, dt);
        SDL_RenderPresent(renderer);

        if (currentScene->isFinished()) {
            currentScene = currentScene->nextScene();
            if (!currentScene) running = false;
        }
    }
}

void App::cleanup() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}
