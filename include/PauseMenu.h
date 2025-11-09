#pragma once
#include "FontManager.h"
#include "Scene.h"
#include <memory>
#include "Config.h"
#include "Button.h"
#include "Label.h"

class PauseMenu {
public:
    PauseMenu(FontManager* f);
    ~PauseMenu();
    void Set(SDL_Renderer* r, FontManager* f);
    void drawMenu(SDL_Renderer* r);
    std::vector<Button> buttons;
    std::vector<Label> labels;

private:
    SDL_Renderer* renderer = nullptr;
    FontManager* fonts = nullptr;
    bool startClicked;
    bool isFullScreened;
};
