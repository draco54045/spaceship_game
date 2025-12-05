#include "UIManager.h"

UIManager::UIManager(){}
UIManager::~UIManager(){}

void UIManager::Set(SDL_Renderer* rend, FontManager* fo, UI* u){
    renderer = rend;
    fonts = fo;
    ui = u;
}

void UIManager::drawUI(){
    auto& cfg = Config::get();
    auto playerData = ui->getPlayerData();
    drawElement(cfg.windowWidth/20, cfg.windowHeight/20, "speed : " + playerData.speed, UIColor, fonts->UIFont);
    drawElement(cfg.windowWidth/20, cfg.windowHeight/20 + 50, "direction : " + playerData.direction, UIColor, fonts->UIFont);
    drawElement(cfg.windowWidth/20, cfg.windowHeight/20 + 100, "mouse : " + playerData.mouseVector, UIColor, fonts->UIFont);
    drawElement(cfg.windowWidth/20, cfg.windowHeight/20 + 150, "health : " + playerData.health, UIColor, fonts->UIFont);
    auto gameData = ui->getGameData();
    drawElement(cfg.windowWidth/20, cfg.windowHeight/20 + 200, "FPS : " + gameData.fps, UIColor, fonts->UIFont);
    drawElement(cfg.windowWidth/20, cfg.windowHeight/20 + 250, "Score : " + gameData.score, UIColor, fonts->UIFont);
}

void UIManager::drawElement(int x, int y, std::string content, SDL_Color color, TTF_Font *font){
    auto& cfg = Config::get();
    int top = 0;
    int left = 0;
    int right = cfg.windowWidth;
    int bottom = cfg.windowHeight;
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