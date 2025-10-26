#pragma once
#include "FontManager.h"
#include "Scene.h"
#include "Game.h"
#include <memory>
#include "Button.h"
#include "Label.h"

class MainMenuScene : public Scene {
public:
    MainMenuScene(SDL_Renderer* r, FontManager* f) : renderer(r), startClicked(false), fonts(f) {
        SDL_Color red = {255,0,0,255};
        SDL_Color white = {255,255,255,255};
        Start = Button(64, 200, 500, 100, white, red, "Start", fonts->MMFont/*, test*/);
        Title = Label(300, 64, 0, 0, white, white, "Game name", fonts->MMFont);
    }

    void handleEvent(const SDL_Event& e) override;

    void update(float dt) override {}

    void render(SDL_Renderer* r, float dt) override;

    bool isFinished() const override;

    std::unique_ptr<Scene> nextScene() override; 

    FontManager* fonts = nullptr;

private:
    SDL_Renderer* renderer;
    bool startClicked;
    Button Start;
    Label Title;
};
