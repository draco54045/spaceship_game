#include "MainMenu.h"

MainMenuScene::MainMenuScene(SDL_Renderer* r, SDL_Window* w, FontManager* f, SFXManager* s, MusicManager* m, SettingsMenu* stm) : 
renderer(r), 
window(w), 
startClicked(false), 
fonts(f), 
sfx(s), 
music(m),
stMenu(stm) {
    auto& cfg = Config::get();
    Button Start = Button(490, 150, 300, 120, cfg.green, cfg.black, "Start", fonts->MMFont_big/*, test*/);
    buttons.push_back(Start);
    Button Load = Button(510, 290, 260, 120, cfg.green, cfg.black, "Load", fonts->MMFont_big/*, test*/);
    buttons.push_back(Load);
    Button SetFS = Button(490, 500, 300, 100, cfg.green, cfg.black, "Settings", fonts->MMFont_medium);
    buttons.push_back(SetFS);
    Button Quit = Button(590, 650, 100, 50, cfg.green, cfg.black, "QUIT", fonts->MMFont_small);
    buttons.push_back(Quit);
    Label Title = Label(640, 64, 0, 0, cfg.black, cfg.green, "Game name", fonts->MMFont_huge);
    labels.push_back(Title);
    for(auto& b : buttons) b.updateLayout();
    for(auto& l : labels) l.updateLayout();
    auto& gState = GameState::get();
    int windowFlag = SDL_GetWindowFlags(window);
    std::string windowFlagString;
    if( (windowFlag & SDL_WINDOW_FULLSCREEN_DESKTOP) == SDL_WINDOW_FULLSCREEN_DESKTOP){
        //std::cout << " using fullscreen " << std::endl;
        windowFlagString = "Auto Fullscreen";
    }
    else if(windowFlag & SDL_WINDOW_FULLSCREEN){
        //std::cout << " using fullscreen windowed" << std::endl;
        windowFlagString = "Fullscreen Windowed";
    }
    else{
        //std::cout << " using windowed" << std::endl;
        windowFlagString = "Windowed";
    }
    stMenu->labels[5].updateText("Current Mode : " + std::to_string(cfg.windowWidth) + " x " + std::to_string(cfg.windowHeight) + " / " + windowFlagString);
}


void MainMenuScene::handleEvent(const SDL_Event& e) {
    auto& cfg = Config::get();
    auto& gState = GameState::get();
    if (e.type == SDL_MOUSEBUTTONDOWN && gState.settings == false) {
        int x = e.button.x, y = e.button.y;
        if (buttons[0].wasClicked(x,y)){ startClicked = true; Mix_PlayChannel(-1, sfx->button_press, 0); SDL_Delay(100); }
        if (buttons[1].wasClicked(x,y)){ Mix_PlayChannel(-1, sfx->button_press, 0); SDL_Delay(100);/*load previously saved game here*/ }
        if (buttons[2].wasClicked(x,y)){ //settings button
            Mix_PlayChannel(-1, sfx->button_press, 0);
            SDL_Delay(100);
            gState.settings = true;
        }
        if (buttons[3].wasClicked(x,y)){ 
            Mix_PlayChannel(-1, sfx->button_press, 0);
            SDL_Delay(500);
            SDL_Event quitEvent;
            quitEvent.type = SDL_QUIT;
            SDL_PushEvent(&quitEvent); 
        }
    }
    if (gState.settings == true){
        stMenu->handleEventOther(e);
    }
    if (e.type == SDL_MOUSEBUTTONDOWN && gState.settings == true) {
        stMenu->handleEventClick(e, window, sfx, NULL, NULL, this);
        int width, height;
        SDL_GetWindowSize(window, &width, &height);
        forceFullRedraw(renderer, width, height); 
    }
    if (gState.settings == true){ //handle settings non clicks
        //stMenu.sliders[0].handleSliderEvent(e);
        for (auto& s : stMenu->sliders) s.handleSliderEvent(e);
        float masterVolume = stMenu->sliders[0].value;
        int musicVolume = 128 * stMenu->sliders[1].value * masterVolume;
        int sfxVolume = 128 * stMenu->sliders[2].value * masterVolume;
        Mix_VolumeMusic(musicVolume);
        Mix_Volume(-1, sfxVolume);
    }

}

void MainMenuScene::render(SDL_Renderer* r, float dt){
    auto& cfg = Config::get();
    auto& gState = GameState::get();
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    SDL_RenderClear(r);
    SDL_Rect left = {0,0,10*cfg.scaleX,cfg.windowHeight};
    SDL_Rect right = {cfg.windowWidth - 10*cfg.scaleX,0,10*cfg.windowWidth,cfg.windowHeight};
    SDL_Rect top = {0,0,cfg.windowWidth,10*cfg.scaleY};
    SDL_Rect bottom = {0,cfg.windowHeight-10*cfg.scaleY,cfg.windowWidth,10*cfg.windowHeight};
    SDL_Rect border[4];
    border[0] = left;
    border[1] = right;
    border[2] = top;
    border[3] = bottom;    
    SDL_SetRenderDrawColor(r,0,255,0,255);
    SDL_RenderDrawRects(r, border, 4);
    SDL_RenderFillRects(r, border, 4);
    if (gState.settings == true){
        stMenu->drawMenu(r);
        return;
    }
    for (auto& b : buttons) b.render(r);
    for (auto& l : labels) l.render(r);
}

bool MainMenuScene::isFinished() const {
    return startClicked;
}

std::unique_ptr<Scene> MainMenuScene::nextScene(){
    if (startClicked){
        std::unique_ptr<GameplayScene> gameplay = std::make_unique<GameplayScene>(renderer, window, fonts, sfx, music, stMenu);
        gameplay->init();
        return gameplay;
    }
    return nullptr;
}