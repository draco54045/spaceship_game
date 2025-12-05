#pragma once
#include <SDL.h>
#include "Camera.h"
#include "FontManager.h"
#include "Config.h"
#include "Entity.h"

class Board{
public:
    Board();
    ~Board();
    void renderBoard();
    void renderGrid();
    void renderGridText(int cell, int left, int top, int right, int bottom);
    void Set(SDL_Renderer* rend, Camera* cam, FontManager* fo);
    bool entityInYellowZone(Entity entity);
    bool entityInRedZone(Entity entity);
    SDL_Rect yellowZone = {1024, 1024, 6144, 6144};
    SDL_Rect redZone = {512, 512, 7168, 7168};
private:
    SDL_Renderer* renderer = nullptr;
    Camera* camera = nullptr;
    FontManager* fonts = nullptr;
    int gridDimensions=64;
    int cell=VIRTUAL_WORLD_H/gridDimensions;
};