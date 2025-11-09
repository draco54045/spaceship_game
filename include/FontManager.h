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
    TTF_Font* MMFont_medium = nullptr;
    TTF_Font* MMFont_small = nullptr;
    TTF_Font* MMFont_big = nullptr;
    TTF_Font* MMFont_huge = nullptr;
    Game* game = nullptr;
};