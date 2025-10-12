#pragma once
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include "Config.h"
#include "FontManager.h"
#include "UI.h"

class UI;

class UIManager {
public:
    UIManager();
    ~UIManager();
    void drawUI();
    void drawElement(int x, int y, std::string content, SDL_Color color, TTF_Font *font);
    void Set(SDL_Renderer* rend, FontManager* fo, UI* u);
    SDL_Color UIColor = {255,255,0,255};

private:
    SDL_Renderer* renderer = nullptr;
    FontManager* fonts = nullptr;
    UI* ui = nullptr;
};