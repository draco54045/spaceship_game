#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <functional>
#include <string>
#include "Config.h"

struct Button {
    int pos_x;
    int pos_y;
    int w;
    int h;
    int base_pos_x;
    int base_pos_y;
    int base_w;
    int base_h;
    SDL_Rect rect;
    SDL_Color color = {0,0,0,255};
    SDL_Color text_color = {255,255,255,255};
    std::string text;
    TTF_Font* font = TTF_OpenFont("./assets/fonts/Roboto/static/Roboto-Regular.ttf", 32);
    std::function<void()> onClick;
    Button(){}
    Button(int b_pos_x, int b_pos_y, int b_w, int b_h, SDL_Color b_color, SDL_Color b_text_color, std::string b_text, TTF_Font* b_font /*, std::function<void()> b_onClick*/){
        pos_x = b_pos_x;
        base_pos_x = pos_x;
        pos_y = b_pos_y;
        base_pos_y = pos_y;
        w = b_w;
        base_w = w;
        h = b_h;
        base_h = h;
        color = b_color;
        text_color = b_text_color;
        text = b_text;
        font = b_font;
        //onClick = b_onClick;
    }
    void render(SDL_Renderer* renderer){
        int texW = 0;
        int texH = 0;
        SDL_Surface * surface = TTF_RenderText_Solid(font, text.c_str(), text_color);
        SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
        SDL_Rect button_text = { pos_x+((w-texW)/2), pos_y+((h-texH)/2), texW, texH };
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);

        SDL_Rect button = {pos_x, pos_y, w, h};
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &button);

        SDL_RenderCopy(renderer, texture, NULL, &button_text);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
    bool wasClicked(int m_x, int m_y){
        if (m_x > this->pos_x && m_x < (this->pos_x + this->w) && m_y > this->pos_y && m_y < (this->pos_y + this->h)){
            return true;
        }
        return false;
    }
    void updateLayout(){
        auto& cfg = Config::get();
        pos_x = base_pos_x * cfg.scaleX;
        pos_y = base_pos_y * cfg.scaleY;
        w = base_w * cfg.scaleX;
        h = base_h * cfg.scaleY;
    }
    void updateText(std::string new_text){
        text = new_text;
    }
    void updateFont(TTF_Font* new_font){
        font = new_font;
    }
};
