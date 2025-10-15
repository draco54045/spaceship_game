#include "FontManager.h"

FontManager::FontManager(){}
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
}
void FontManager::closeFonts(){
    for (auto f : fonts) {
            if (f) TTF_CloseFont(f);
        }
        fonts.clear();
        GridFont = nullptr;
        UIFont = nullptr;
}