#include "Board.h"

Board::Board(){}
Board::~Board(){}

void Board::Set(SDL_Renderer* rend, Camera* cam, FontManager* fo){
    renderer = rend;
    camera = cam;
    fonts = fo;
}

void Board::renderBoard(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    renderGrid();
    SDL_SetRenderDrawColor(renderer, 255,255,0,255);
    auto yZ = camera->worldToScreen(yellowZone);
    SDL_RenderDrawRect(renderer, &yZ);
    SDL_SetRenderDrawColor(renderer, 255,0,0,255);
    auto rZ = camera->worldToScreen(redZone);
    SDL_RenderDrawRect(renderer, &rZ);
    //SDL_RenderClear(renderer);
}

void Board::renderGrid() {
    auto& cfg = Config::get();
    int left = int(camera->pos.x);
    int top = int(camera->pos.y);
    int right = left + cfg.windowWidth;
    int bottom = top + cfg.windowHeight;
    int i;
    for(int gx = (left / cell) * cell; gx <= right; gx += cell) {
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_Rect line = {gx, top, 1, cfg.windowHeight};
        auto s = camera->worldToScreen(line);
        SDL_RenderFillRect(renderer, &s);
    }
    for(int gy = (top / cell) * cell; gy <= bottom; gy += cell) {
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_Rect line = {left, gy, cfg.windowWidth, 1};
        auto s = camera->worldToScreen(line);
        SDL_RenderFillRect(renderer, &s);
    }
    //renderGridText(cell, 0, 0, VIRTUAL_WORLD_W, VIRTUAL_WORLD_H);
}

void Board::renderGridText(int cell, int left, int top, int right, int bottom) {
    if (!fonts || !fonts->GridFont) return;
    SDL_Color color = { 200, 200, 200, 255 };
    int i = 0, j = 0;
    for (int gx = (left / cell) * cell; gx <= right; gx += cell) {
        for (int gy = (top / cell) * cell; gy <= bottom; gy += cell) {
            std::string str = "[" + std::to_string(i) + "." + std::to_string(j) + "]";
            SDL_Surface * surface = TTF_RenderText_Solid(fonts->GridFont, str.c_str(), color);
            SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
            SDL_Rect txt = { gx + (cell/8), gy + (cell/8), 0, 0 };
            SDL_QueryTexture(texture, nullptr, nullptr, &txt.w, &txt.h);
            auto s = camera->worldToScreen(txt);
            SDL_RenderCopy(renderer, texture, nullptr, &s);
            SDL_DestroyTexture(texture);
            i++;
        }
        i = 0;
        j++;
    }
}

bool Board::entityInYellowZone(Entity entity){
    SDL_Rect hb = entity.hitbox;
    int hbLeft = hb.x;
    int hbRight = hb.x + hb.w;
    int hbTop = hb.y;
    int hbBottom = hb.y + hb.h;

    int yZLeft = yellowZone.x;
    int yZRight = yellowZone.x + yellowZone.w;
    int yZTop = yellowZone.y;
    int yZBottom = yellowZone.y + yellowZone.h;
    if (hbLeft <= yZLeft ||
        hbRight >= yZRight ||
        hbTop <= yZTop ||
        hbBottom >= yZBottom){
            std::cout << "DANGER" << std::endl;
            return true;
    }
    else {
        return false;
    }
}
bool Board::entityInRedZone(Entity entity){
    SDL_Rect hb = entity.hitbox;
    int hbLeft = hb.x;
    int hbRight = hb.x + hb.w;
    int hbTop = hb.y;
    int hbBottom = hb.y + hb.h;

    int rZLeft = redZone.x;
    int rZRight = redZone.x + redZone.w;
    int rZTop = redZone.y;
    int rZBottom = redZone.y + redZone.h;
    if (hbLeft <= rZLeft ||
        hbRight >= rZRight ||
        hbTop <= rZTop ||
        hbBottom >= rZBottom){
            std::cout << "MEGA DANGER" << std::endl;
            return true;
    }
    else {
        return false;
    }
}