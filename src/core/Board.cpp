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
    SDL_RenderClear(renderer);
    renderGrid();
}

void Board::renderGrid() {
    int left = int(camera->pos.x);
    int top = int(camera->pos.y);
    int right = left + WINDOW_W;
    int bottom = top + WINDOW_H;
    int i;
    for(int gx = (left / cell) * cell; gx <= right; gx += cell) {
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_Rect line = {gx, top, 1, WINDOW_H};
        auto s = camera->worldToScreen(line);
        SDL_RenderFillRect(renderer, &s);
    }
    for(int gy = (top / cell) * cell; gy <= bottom; gy += cell) {
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_Rect line = {left, gy, WINDOW_W, 1};
        auto s = camera->worldToScreen(line);
        SDL_RenderFillRect(renderer, &s);
    }
    renderGridText(cell, 0, 0, VIRTUAL_WORLD_W, VIRTUAL_WORLD_H);
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