#pragma once
#include <SDL.h>
#include "Camera.h"
#include "FontManager.h"
#include "Config.h"

class Board{
public:
    Board();
    ~Board();
    void renderBoard();
    void renderGrid();
    void renderGridText(int cell, int left, int top, int right, int bottom);
    void Set(SDL_Renderer* rend, Camera* cam, FontManager* fo);
private:
    SDL_Renderer* renderer = nullptr;
    Camera* camera = nullptr;
    FontManager* fonts = nullptr;
    int gridDimensions=16;
    int cell=VIRTUAL_WORLD_H/gridDimensions;
};