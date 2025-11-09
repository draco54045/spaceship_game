#pragma once
#include "FontManager.h"
#include "Scene.h"
#include <memory>
#include "Button.h"
#include "Label.h"
#include "Config.h"
#include "GameState.h"
#include "Slider.h"
#include <SDL_mixer.h>
#include "SFXManager.h"
#include "PauseMenu.h"
#include "Camera.h"

class MainMenuScene;

class SettingsMenu {
public :
    SettingsMenu(FontManager* f);
    ~SettingsMenu();
    void Set(SDL_Renderer* r, FontManager* f);
    void drawMenu(SDL_Renderer* r);
    void handleEventClick(const SDL_Event& e, SDL_Window* window, SFXManager* sfx, PauseMenu* psMenu, Camera* camera, MainMenuScene* mMenu);
    void handleEventOther(const SDL_Event& e);
    std::vector<Button> buttons;
    std::vector<Label> labels;
    std::vector<Slider> sliders;
    MainMenuScene* mMenu;
    bool resolutionDropdown;
private :
    SDL_Renderer* renderer = nullptr;
    FontManager* fonts = nullptr;
};