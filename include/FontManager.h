#pragma once
#include <SDL_ttf.h>
#include <iostream>
#include <vector>

class Game;

class FontManager {
public:
    FontManager();
    ~FontManager();
    void closeFonts();
    void openFonts();
    std::vector<TTF_Font*> fonts;
    TTF_Font* GridFont = nullptr;
    TTF_Font* UIFont = nullptr;
    Game* game = nullptr;
};