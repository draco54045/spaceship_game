#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <functional>
#include <string>
#include "Config.h"

struct Slider{
    int bar_x;
    int bar_y;
    int bar_w;
    int bar_h;
    int base_bar_x;
    int base_bar_y;
    int base_bar_w;
    int base_bar_h;
    SDL_Rect bar;
    SDL_Rect handle;
    SDL_Color bar_clr;
    SDL_Color handle_clr;
    float value;
    bool dragging;

    Slider(){}
    Slider(int bar_pos_x, int bar_pos_y, int bar_width, int bar_height, SDL_Color bar_color, SDL_Color handle_color, float val){
        bar_x = bar_pos_x;
        base_bar_x = bar_x;
        bar_y = bar_pos_y;
        base_bar_y = bar_y;
        bar_w = bar_width;
        base_bar_w = bar_w;
        bar_h = bar_height;
        base_bar_h = bar_h;
        bar = {bar_x, bar_y, bar_w, bar_h};
        value = val;
        handle = {bar_x + int(bar_w*value) - bar_w/20 , bar_y, bar_w/10, bar_h};
        bar_clr = bar_color;
        handle_clr = handle_color;
        dragging = false;
    }
    void render(SDL_Renderer* r){
        SDL_SetRenderDrawColor(r, bar_clr.r, bar_clr.g, bar_clr.b, bar_clr.a);
        SDL_RenderFillRect(r, &bar);
        SDL_SetRenderDrawColor(r, handle_clr.r, handle_clr.g, handle_clr.b, handle_clr.a);
        SDL_RenderFillRect(r, &handle);
    }
    void handleSliderEvent(const SDL_Event& e) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mx = e.button.x, my = e.button.y;
            SDL_Point point = {mx, my};
            if (SDL_PointInRect(&point, &handle)) {
                dragging = true;
            }
        } 
        else if (e.type == SDL_MOUSEBUTTONUP) {
            dragging = false;
        } 
        else if (e.type == SDL_MOUSEMOTION && dragging) {
            int mx = e.motion.x;
            int minX = bar.x;
            int maxX = bar.x + bar.w - handle.w;
            handle.x = std::clamp(mx - handle.w / 2, minX, maxX);
            value = float(handle.x - bar.x) / float(bar.w - handle.w);
            int hx;
            hx = std::clamp(int(bar_x + (bar_w * value)) - bar.w / 20, minX, maxX);
            handle = {hx,bar_y,bar_w / 10,bar_h};
        }
    }
    void updateLayout(){
        auto& cfg = Config::get();
        bar_x = base_bar_x * cfg.scaleX;
        bar_y = base_bar_y * cfg.scaleY;
        bar_w = base_bar_w * cfg.scaleX;
        bar_h = base_bar_h * cfg.scaleY;
        bar = { bar_x, bar_y, bar_w, bar_h };
        int hx;
        int minX = bar.x;
        int maxX = bar.x + bar.w - bar.w/10;
        hx = std::clamp(int(bar_x + (bar_w * value)) - bar.w / 20, minX, maxX);
        handle = {hx,bar_y,bar_w / 10,bar_h};
    }
};

