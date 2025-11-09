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
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL_Init error: " << SDL_GetError() << "\n";
        return 1;
    }
    int mixFlags = MIX_INIT_OGG;
    if ((Mix_Init(mixFlags) & mixFlags) != mixFlags) {
        std::cerr << "Mix_Init error: " << Mix_GetError() << "\n";
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Mix_OpenAudio error: " << Mix_GetError() << "\n";
    }
    Mix_AllocateChannels(16);
    auto& cfg = Config::get();
    if (cfg.isFullScreened == true) { window = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, cfg.windowWidth, cfg.windowHeight, SDL_WINDOW_FULLSCREEN); }
    else { window = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, cfg.windowWidth, cfg.windowHeight, 0); }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer) { std::cerr<<"Renderer failed: "<<SDL_GetError()<<"\n"; return false; }

    fonts = std::make_unique<FontManager>();
    sfx = std::make_unique<SFXManager>();
    music = std::make_unique<MusicManager>();
    stMenu = std::make_unique<SettingsMenu>(&*fonts);
    currentScene = std::make_unique<MainMenuScene>(renderer, window, fonts.get(), sfx.get(), music.get(), stMenu.get());

    //monitor testing
    int numModes = SDL_GetNumDisplayModes(0);
    SDL_DisplayMode mode;
    for (int i = 0; i < numModes; ++i) {
        SDL_GetDisplayMode(0, i, &mode);
    //    std::cout << "Mode " << i << ": " << mode.w << "x" << mode.h << " @ " << mode.refresh_rate << "Hz\n";
    }
    return true;
}

void App::run() {
    bool running = true;
    auto& cfg = Config::get();
    auto& gState = GameState::get();
    Uint64 lastTicks = SDL_GetPerformanceCounter();
    SDL_Event e;
    float masterVolume = cfg.master_volume / 128.0f;
    Mix_VolumeMusic(cfg.music_volume * masterVolume);
    Mix_Volume(-1, cfg.sfx_volume * masterVolume);
    while (running) {
        while (SDL_PollEvent(&e)) {
            std::string sceneType = typeid(*currentScene).name();
            sceneType = sceneType.erase(0,2);
            //std::cout << sceneType << std::endl;
            if (e.type == SDL_QUIT) running = false;
            currentScene->handleEvent(e);
            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                if (sceneType == "MainMenuScene"){
                    if (gState.settings == true) { cfg.writeToFile("./settings.ini"); gState.settings = false; }
                }
                else{
                    if (gState.settings == true) { cfg.writeToFile("./settings.ini"); gState.settings = false; break; }
                    if (gState.paused == false) { gState.paused = true; Mix_PauseMusic(); }
                    else { gState.paused = false; Mix_ResumeMusic(); }
                }
            }
        }

        Uint64 now = SDL_GetPerformanceCounter();
        float dt = (now - lastTicks) / static_cast<float>(SDL_GetPerformanceFrequency());
        lastTicks = now;
        if (gState.paused == false){
            currentScene->update(dt);
        }
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
