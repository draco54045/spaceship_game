#include "SettingsMenu.h"
#include "MainMenu.h"

SettingsMenu::SettingsMenu(FontManager* f){
    auto& cfg = Config::get();
    fonts = f;
    Label settings = Label(640, 60, 0, 0, cfg.black, cfg.green, "Settings", fonts->MMFont_huge);
    labels.push_back(settings);
    Label resolution = Label(640, 160, 0, 0, cfg.black, cfg.green, "Resolution", fonts->MMFont_medium);
    labels.push_back(resolution);
    Label fullScreenLabel = Label(540, 210, 50, 40, cfg.none, cfg.green, "Auto Full Screen", fonts->MMFont_small);
    labels.push_back(fullScreenLabel);
    Button fullScreenButton = Button(740, 210, 50, 40, cfg.green, cfg.black, " ", fonts->MMFont_small);
    buttons.push_back(fullScreenButton);
    Label fullScreenWindowedLabel = Label(540, 260, 50, 40, cfg.none, cfg.green, "Full Screen Window", fonts->MMFont_small);
    labels.push_back(fullScreenWindowedLabel);
    Button fullScreenWindowedButton = Button(740, 260, 50, 40, cfg.green, cfg.black, " ", fonts->MMFont_small);
    buttons.push_back(fullScreenWindowedButton);
    Label windowedLabel = Label(540, 310, 50, 40, cfg.none, cfg.green, "Windowed", fonts->MMFont_small);
    labels.push_back(windowedLabel);
    Button windowedButton = Button(740, 310, 50, 40, cfg.green, cfg.black, " ", fonts->MMFont_small);
    buttons.push_back(windowedButton);
    Button r1280x720Button = Button(430, 360, 120, 40, cfg.green, cfg.black, "1280x720", fonts->MMFont_small);
    buttons.push_back(r1280x720Button);
    Button r1920x1080Button = Button(570, 360, 120, 40, cfg.green, cfg.black, "1920x1080", fonts->MMFont_small);
    buttons.push_back(r1920x1080Button);
    Button r2560x1440Button = Button(710, 360, 120, 40, cfg.green, cfg.black, "2560x1440", fonts->MMFont_small);
    buttons.push_back(r2560x1440Button);
    Label currentmode = Label(640, 420, 0, 0, cfg.black, cfg.green, "Current mode : Full Screen Auto / 1920x1080", fonts->MMFont_small);
    labels.push_back(currentmode);
    Label sound = Label(640, 485, 0, 0, cfg.black, cfg.green, "Sound", fonts->MMFont_medium);
    labels.push_back(sound);
    Label masterLabel = Label(440, 520, 0, 30, cfg.none, cfg.green, "Master", fonts->MMFont_small);
    labels.push_back(masterLabel);
    Label musicLabel = Label(440, 560, 0, 30, cfg.none, cfg.green, "Music", fonts->MMFont_small);
    labels.push_back(musicLabel);
    Label sfxLabel = Label(440, 600, 0, 30, cfg.none, cfg.green, "SFX", fonts->MMFont_small);
    labels.push_back(sfxLabel);
    Label masterLabelval = Label(840, 520, 0, 30, cfg.none, cfg.green, "50%", fonts->MMFont_small);
    labels.push_back(masterLabelval);
    Label musicLabelval = Label(840, 560, 0, 30, cfg.none, cfg.green, "50%", fonts->MMFont_small);
    labels.push_back(musicLabelval);
    Label sfxLabelval = Label(840, 600, 0, 30, cfg.none, cfg.green, "50%", fonts->MMFont_small);
    labels.push_back(sfxLabelval);
    Slider masterVol = Slider(490, 520, 300, 30, cfg.green, cfg.red, float(cfg.master_volume / 128.0f));
    sliders.push_back(masterVol);
    Slider musicVol = Slider(490, 560, 300, 30, cfg.green, cfg.red, float(cfg.music_volume / 128.0f));
    sliders.push_back(musicVol);
    Slider sfxVol = Slider(490, 600, 300, 30, cfg.green, cfg.red, float(cfg.sfx_volume / 128.0f));
    sliders.push_back(sfxVol);
    Button back = Button(590, 650, 100, 50, cfg.green, cfg.black, "Back", fonts->MMFont_small);
    buttons.push_back(back);
    for (auto& b : buttons) b.updateLayout();
    for (auto& l : labels) l.updateLayout();
    for (auto& s : sliders) s.updateLayout();
}

SettingsMenu::~SettingsMenu(){}

void SettingsMenu::Set(SDL_Renderer* r, FontManager* f){
    renderer = r;
    fonts = f;
}

void SettingsMenu::drawMenu(SDL_Renderer* r){
for (auto& b : buttons) b.render(r);
for (auto& l : labels) l.render(r);
for (auto& s : sliders) s.render(r);
}


void SettingsMenu::handleEventClick(const SDL_Event& e, SDL_Window* window, SFXManager* sfx, PauseMenu* psMenu, Camera* camera, MainMenuScene* mMenu)
{
    auto& cfg = Config::get();
    auto& gState = GameState::get();

    int x = e.button.x, y = e.button.y;

    auto updateAllLayouts = [&]() {
        cfg.updateScale();
        for (auto& b : buttons) b.updateLayout();
        for (auto& l : labels)  l.updateLayout();
        for (auto& s : sliders) s.updateLayout();
        if (psMenu) {
            for (auto& b : psMenu->buttons) b.updateLayout();
            for (auto& l : psMenu->labels)  l.updateLayout();
        }
        if (mMenu) {
            for (auto& b : mMenu->buttons) b.updateLayout();
            for (auto& l : mMenu->labels)  l.updateLayout();
        }
        if (camera) {
            int w, h;
            SDL_GetWindowSize(window, &w, &h);
            *camera = Camera(w, h);
        }
    };

// --- NEW: helper that sets resolution safely in windowed or fullscreen ---
    auto applyNewWindowSize = [&](int w, int h) {
        cfg.windowWidth  = w;
        cfg.windowHeight = h;
        int flags = SDL_GetWindowFlags(window);
        bool isFullscreen = (flags & SDL_WINDOW_FULLSCREEN) || (flags & SDL_WINDOW_FULLSCREEN_DESKTOP);
        if (isFullscreen) {
            SDL_DisplayMode mode;
            mode.w = w;
            mode.h = h;
            mode.format = 0;         // keep current pixel format
            mode.refresh_rate = 0;   // keep current refresh rate
            mode.driverdata = nullptr;
            SDL_SetWindowDisplayMode(window, &mode);
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
        } 
        else {
            SDL_SetWindowFullscreen(window, 0);
            SDL_SetWindowSize(window, w, h);
        }
        updateAllLayouts();
    };

    // --- Back button ---
    if (buttons.back().wasClicked(x, y)) {
        Mix_PlayChannel(-1, sfx->button_press, 0);
        cfg.writeToFile("./settings.ini");
        gState.settings = false;
        return;
    }

    // --- Fullscreen desktop (auto fullscreen) ---
    if (buttons[0].wasClicked(x, y)) {
        Mix_PlayChannel(-1, sfx->button_press, 0);
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        SDL_DisplayMode dm;
        SDL_GetCurrentDisplayMode(0, &dm);
        applyNewWindowSize(dm.w, dm.h);
        cfg.isFullScreened = true;
    }

    // --- Fullscreen windowed ---
    else if (buttons[1].wasClicked(x, y)) {
        Mix_PlayChannel(-1, sfx->button_press, 0);
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
        SDL_DisplayMode dm;
        SDL_GetCurrentDisplayMode(0, &dm);
        applyNewWindowSize(dm.w, dm.h);
        cfg.isFullScreened = true;
    }

    // --- Windowed mode ---
    else if (buttons[2].wasClicked(x, y)) {
        Mix_PlayChannel(-1, sfx->button_press, 0);
        SDL_SetWindowFullscreen(window, 0);
        applyNewWindowSize(cfg.windowWidth, cfg.windowHeight);
        cfg.isFullScreened = false;
    }

    if (buttons[3].wasClicked(x, y)) { // 1280x720
        Mix_PlayChannel(-1, sfx->button_press, 0);
        SDL_Delay(100);
        applyNewWindowSize(1280, 720);
    }
    if (buttons[4].wasClicked(x, y)) { // 1920x1080
        Mix_PlayChannel(-1, sfx->button_press, 0);
        SDL_Delay(100);
        applyNewWindowSize(1920, 1080);
    }
    if (buttons[5].wasClicked(x, y)) { // 2560x1440
        Mix_PlayChannel(-1, sfx->button_press, 0);
        SDL_Delay(100);
        applyNewWindowSize(2560, 1440);
    }

    // --- Update display mode label ---
    int windowFlags = SDL_GetWindowFlags(window);
    std::string windowMode;

    if (windowFlags & SDL_WINDOW_FULLSCREEN_DESKTOP)
        windowMode = "Auto Fullscreen";
    else if (windowFlags & SDL_WINDOW_FULLSCREEN)
        windowMode = "Fullscreen Windowed";
    else
        windowMode = "Windowed";

    labels[5].updateText(
        "Current Mode : " + std::to_string(cfg.windowWidth) +
        " x " + std::to_string(cfg.windowHeight) +
        " / " + windowMode
    );
}

/*void SettingsMenu::handleEventClick(const SDL_Event& e, SDL_Window* window, SFXManager* sfx, PauseMenu* psMenu, Camera* camera, MainMenuScene* mMenu){
    auto& cfg = Config::get();
    auto& gState = GameState::get();
    int x = e.button.x, y = e.button.y;
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
    //std::cout << windowFlag << std::endl;
    int n = buttons.size();
    if(buttons[n-1].wasClicked(x,y)){ //back button
        Mix_PlayChannel(-1, sfx->button_press, 0);
        cfg.writeToFile("./settings.ini");
        gState.settings = false;
    }
    if (buttons[0].wasClicked(x,y)){ //fullscreen
        Mix_PlayChannel(-1, sfx->button_press, 0);
        SDL_Delay(100);
        if (cfg.isFullScreened == true && ((windowFlag & SDL_WINDOW_FULLSCREEN_DESKTOP) == SDL_WINDOW_FULLSCREEN_DESKTOP)){
            //already fullscreened, do nothing
        }
        else if(cfg.isFullScreened == true && (windowFlag & SDL_WINDOW_FULLSCREEN)) {
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            SDL_DisplayMode fullScreen;
            SDL_GetCurrentDisplayMode(0, &fullScreen);
            cfg.windowWidth = fullScreen.w;
            cfg.windowHeight = fullScreen.h;
            cfg.updateScale();
            for (auto& b : buttons) b.updateLayout();
            for (auto& l : labels) l.updateLayout();       
            for (auto& s : sliders) s.updateLayout();
            if (psMenu){
                for (auto& b : psMenu->buttons) b.updateLayout();
                for (auto& l : psMenu->labels) l.updateLayout();       
            }
            if (mMenu){
                for (auto& b : mMenu->buttons) b.updateLayout();
                for (auto& l : mMenu->labels) l.updateLayout();
            }
            cfg.isFullScreened = true;
        }
        else if(cfg.isFullScreened == false){
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            SDL_DisplayMode fullScreen;
            SDL_GetCurrentDisplayMode(0, &fullScreen);
            cfg.windowWidth = fullScreen.w;
            cfg.windowHeight = fullScreen.h;
            cfg.updateScale();
            for (auto& b : buttons) b.updateLayout();
            for (auto& l : labels) l.updateLayout();       
            for (auto& s : sliders) s.updateLayout();
            if (psMenu){
                for (auto& b : psMenu->buttons) b.updateLayout();
                for (auto& l : psMenu->labels) l.updateLayout();       
            }
            if (mMenu){
                for (auto& b : mMenu->buttons) b.updateLayout();
                for (auto& l : mMenu->labels) l.updateLayout();
            }
            cfg.isFullScreened = true;
        }
        if (camera){
            *camera = Camera(cfg.windowWidth, cfg.windowHeight);
        }
    }
    if (buttons[1].wasClicked(x,y)){ //fullscreen windowed
        Mix_PlayChannel(-1, sfx->button_press, 0);
        SDL_Delay(100);
        if(cfg.isFullScreened == true && ((windowFlag & SDL_WINDOW_FULLSCREEN_DESKTOP) == SDL_WINDOW_FULLSCREEN_DESKTOP)){
            SDL_SetWindowFullscreen(window, 0);
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
            SDL_DisplayMode fullScreen;
            SDL_GetCurrentDisplayMode(0, &fullScreen);
            cfg.windowWidth = fullScreen.w;
            cfg.windowHeight = fullScreen.h;
            cfg.updateScale();
            for (auto& b : buttons) b.updateLayout();
            for (auto& l : labels) l.updateLayout();       
            for (auto& s : sliders) s.updateLayout();
            if (psMenu){
                for (auto& b : psMenu->buttons) b.updateLayout();
                for (auto& l : psMenu->labels) l.updateLayout();       
            }
            if (mMenu){
                for (auto& b : mMenu->buttons) b.updateLayout();
                for (auto& l : mMenu->labels) l.updateLayout();
            }
            cfg.isFullScreened = true;
        }
        else if (cfg.isFullScreened == true && (windowFlag & SDL_WINDOW_FULLSCREEN)){
            //already fullscreen windowed, do nothing
        }
        else if(cfg.isFullScreened == false){
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
            SDL_DisplayMode fullScreen;
            SDL_GetCurrentDisplayMode(0, &fullScreen);
            cfg.windowWidth = fullScreen.w;
            cfg.windowHeight = fullScreen.h;
            cfg.updateScale();
            for (auto& b : buttons) b.updateLayout();
            for (auto& l : labels) l.updateLayout();       
            for (auto& s : sliders) s.updateLayout();
            if (psMenu){
                for (auto& b : psMenu->buttons) b.updateLayout();
                for (auto& l : psMenu->labels) l.updateLayout();       
            }
            if (mMenu){
                for (auto& b : mMenu->buttons) b.updateLayout();
                for (auto& l : mMenu->labels) l.updateLayout();
            }
            cfg.isFullScreened = true;
        }
        if (camera){
            *camera = Camera(cfg.windowWidth, cfg.windowHeight);
        }
    }
    if (buttons[2].wasClicked(x,y)){ // windowed
        Mix_PlayChannel(-1, sfx->button_press, 0);
        SDL_Delay(100);
        //std::cout << "windowing now" << std::endl;
        if (cfg.isFullScreened == true){
            SDL_SetWindowFullscreen(window, 0);
            SDL_SetWindowSize(window, cfg.windowWidth, cfg.windowHeight);
            //SDL_SetWindowPosition(window, 100, 100);
            cfg.updateScale();
            for (auto& b : buttons) b.updateLayout();
            for (auto& l : labels) l.updateLayout();       
            for (auto& s : sliders) s.updateLayout();
            if (psMenu){
                for (auto& b : psMenu->buttons) b.updateLayout();
                for (auto& l : psMenu->labels) l.updateLayout();       
            }
            if (mMenu){
                for (auto& b : mMenu->buttons) b.updateLayout();
                for (auto& l : mMenu->labels) l.updateLayout();
            }
            cfg.isFullScreened = false;
        }
        if (camera){
            *camera = Camera(cfg.windowWidth, cfg.windowHeight);
        }
    }
    if (buttons[3].wasClicked(x,y)){ // 1280x720
        Mix_PlayChannel(-1, sfx->button_press, 0);
        SDL_Delay(100);
        cfg.windowWidth = 1280;
        cfg.windowHeight = 720;
        SDL_SetWindowSize(window, cfg.windowWidth, cfg.windowHeight);
        if (cfg.isFullScreened == true && ((windowFlag & SDL_WINDOW_FULLSCREEN_DESKTOP) == SDL_WINDOW_FULLSCREEN_DESKTOP)){
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
            //SDL_SetWindowPosition(window, 100, 100);
            //cfg.isFullScreened = false;
        }
        cfg.updateScale();
        for (auto& b : buttons) b.updateLayout();
        for (auto& l : labels) l.updateLayout();       
        for (auto& s : sliders) s.updateLayout();
        if (psMenu){
            for (auto& b : psMenu->buttons) b.updateLayout();
            for (auto& l : psMenu->labels) l.updateLayout();       
        }
            if (mMenu){
                for (auto& b : mMenu->buttons) b.updateLayout();
                for (auto& l : mMenu->labels) l.updateLayout();
            }
        if (camera){
            *camera = Camera(cfg.windowWidth, cfg.windowHeight);
        }
    }
    if (buttons[4].wasClicked(x,y)){ // 1920x1080
        Mix_PlayChannel(-1, sfx->button_press, 0);
        SDL_Delay(100);
        cfg.windowWidth = 1920;
        cfg.windowHeight = 1080;
        SDL_SetWindowSize(window, cfg.windowWidth, cfg.windowHeight);
        if (cfg.isFullScreened == true && ((windowFlag & SDL_WINDOW_FULLSCREEN_DESKTOP) == SDL_WINDOW_FULLSCREEN_DESKTOP)){
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
            //SDL_SetWindowPosition(window, 100, 100);
            //cfg.isFullScreened = false;
        }
        cfg.updateScale();
        for (auto& b : buttons) b.updateLayout();
        for (auto& l : labels) l.updateLayout();       
        for (auto& s : sliders) s.updateLayout();
        if (psMenu){
            for (auto& b : psMenu->buttons) b.updateLayout();
            for (auto& l : psMenu->labels) l.updateLayout();       
        }
            if (mMenu){
                for (auto& b : mMenu->buttons) b.updateLayout();
                for (auto& l : mMenu->labels) l.updateLayout();
            }
        if (camera){
            *camera = Camera(cfg.windowWidth, cfg.windowHeight);
        }
    }
    if (buttons[5].wasClicked(x,y)){ // 2560x1440
        Mix_PlayChannel(-1, sfx->button_press, 0);
        SDL_Delay(100);
        cfg.windowWidth = 2560;
        cfg.windowHeight = 1440;
        SDL_SetWindowSize(window, cfg.windowWidth, cfg.windowHeight);
        if (cfg.isFullScreened == true && ((windowFlag & SDL_WINDOW_FULLSCREEN_DESKTOP) == SDL_WINDOW_FULLSCREEN_DESKTOP)){
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
            //SDL_SetWindowPosition(window, 100, 100);
            //cfg.isFullScreened = false;
        }
        cfg.updateScale();
        for (auto& b : buttons) b.updateLayout();
        for (auto& l : labels) l.updateLayout();       
        for (auto& s : sliders) s.updateLayout();
        if (psMenu){
            for (auto& b : psMenu->buttons) b.updateLayout();
            for (auto& l : psMenu->labels) l.updateLayout();       
        }
            if (mMenu){
                for (auto& b : mMenu->buttons) b.updateLayout();
                for (auto& l : mMenu->labels) l.updateLayout();
            }
        if (camera){
            *camera = Camera(cfg.windowWidth, cfg.windowHeight);
        }
    }
    windowFlag = SDL_GetWindowFlags(window);
    if( (windowFlag & SDL_WINDOW_FULLSCREEN_DESKTOP) == SDL_WINDOW_FULLSCREEN_DESKTOP){
        windowFlagString = "Auto Fullscreen";
    }
    else if(windowFlag & SDL_WINDOW_FULLSCREEN){
        windowFlagString = "Fullscreen Windowed";
    }
    else{
        windowFlagString = "Windowed";
    }
    labels[5].updateText("Current Mode : " + std::to_string(cfg.windowWidth) + " x " + std::to_string(cfg.windowHeight) + " / " + windowFlagString);
    //std::cout << "Current Mode : " << std::to_string(cfg.windowWidth) << " x " << std::to_string(cfg.windowHeight) << " / " << windowFlagString << std::endl;
    //labels[5].render(renderer);
    SDL_RenderClear(renderer);
}
*/
void SettingsMenu::handleEventOther(const SDL_Event& e){
    auto& cfg = Config::get();
    for (auto& s : sliders) s.handleSliderEvent(e);
    float masterVolume = sliders[0].value;
    cfg.master_volume = masterVolume * 128;
    cfg.music_volume = 128 * sliders[1].value;
    int musicVolumeReadable = int(sliders[1].value * 100);
    cfg.sfx_volume = 128 * sliders[2].value;
    int sfxVolumeReadable = int(sliders[2].value * 100);
    Mix_VolumeMusic(cfg.music_volume * masterVolume);
    Mix_Volume(-1, cfg.sfx_volume * masterVolume);
    labels[10].updateText(std::to_string(int(masterVolume*100)) + "%");
    labels[11].updateText(std::to_string(musicVolumeReadable) + "%");
    labels[12].updateText(std::to_string(sfxVolumeReadable) + "%");

}