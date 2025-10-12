#include "UIManager.h"

UIManager::UIManager(){}
UIManager::~UIManager(){}

void UIManager::Set(SDL_Renderer* rend, FontManager* fo, UI* u){
    renderer = rend;
    fonts = fo;
    ui = u;
}

void UIManager::drawUI(){
    auto data = ui->getData();
    drawElement(16, 16, "speed : " + data.speed, UIColor, fonts->UIFont);
    drawElement(16, 64, "direction : " + data.direction, UIColor, fonts->UIFont);
}

void UIManager::drawElement(int x, int y, std::string content, SDL_Color color, TTF_Font *font){
    int top = 0;
    int left = 0;
    int right = WINDOW_W;
    int bottom = WINDOW_H;
    int texW = 0;
    int texH = 0;

    SDL_Surface * surface = TTF_RenderText_Solid(font, content.c_str(), color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = { x, y, texW, texH };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}