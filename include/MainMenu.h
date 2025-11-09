#pragma once
#include "FontManager.h"
#include "Scene.h"
#include "Game.h"
#include <memory>
#include "Button.h"
#include "Label.h"
#include "Config.h"
#include <SDL_ttf.h>
#include "SettingsMenu.h"

class MainMenuScene : public Scene {
public:
    MainMenuScene(SDL_Renderer* r, SDL_Window* window, FontManager* f, SFXManager* s, MusicManager* m, SettingsMenu* stm);
    
    void handleEvent(const SDL_Event& e) override;

    void update(float dt) override {}

    void render(SDL_Renderer* r, float dt) override;

    bool isFinished() const override;

    std::unique_ptr<Scene> nextScene() override; 

    FontManager* fonts = nullptr;
    SFXManager* sfx = nullptr;
    MusicManager* music = nullptr;
    SettingsMenu* stMenu = nullptr;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    std::vector<Button> buttons;
    std::vector<Label> labels;

private:
    bool startClicked;
    //bool isFullScreened = false;
    bool settings = false;
};
