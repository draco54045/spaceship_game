#include "FontManager.h"

FontManager::FontManager(){ openFonts(); }
FontManager::~FontManager(){ closeFonts(); }

void FontManager::openFonts(){
    GridFont = TTF_OpenFont("./assets/fonts/Roboto/static/Roboto-Regular.ttf", 16);
    if (!GridFont) {
        std::cerr << "Failed to load font: " << TTF_GetError() << "\n";
    }
    fonts.push_back(GridFont);
    UIFont = TTF_OpenFont("./assets/fonts/Roboto/static/Roboto-Regular.ttf", 32);
    if (!UIFont) {
        std::cerr << "Failed to load font: " << TTF_GetError() << "\n";
    }
    fonts.push_back(UIFont);
    MMFont_medium = TTF_OpenFont("./assets/fonts/Roboto/static/Roboto-Regular.ttf", 64);
    if (!MMFont_medium) {
        std::cerr << "Failed to load font: " << TTF_GetError() << "\n";
    }
    fonts.push_back(MMFont_medium);
    MMFont_small = TTF_OpenFont("./assets/fonts/Roboto/static/Roboto-Regular.ttf", 24);
    if (!MMFont_small) {
        std::cerr << "Failed to load font: " << TTF_GetError() << "\n";
    }
    fonts.push_back(MMFont_small);
    MMFont_big = TTF_OpenFont("./assets/fonts/Roboto/static/Roboto-Regular.ttf", 96);
    if (!MMFont_big) {
        std::cerr << "Failed to load font: " << TTF_GetError() << "\n";
    }
    fonts.push_back(MMFont_big);
    MMFont_huge = TTF_OpenFont("./assets/fonts/Roboto/static/Roboto-Regular.ttf", 128);
    if (!MMFont_huge) {
        std::cerr << "Failed to load font: " << TTF_GetError() << "\n";
    }
    fonts.push_back(MMFont_huge);
}
void FontManager::closeFonts(){
    for (auto f : fonts) {
            if (f) TTF_CloseFont(f);
        }
        fonts.clear();
        GridFont = nullptr;
        UIFont = nullptr;
}