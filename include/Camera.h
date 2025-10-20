#pragma once
#include <SDL.h>
#include "Vec2.h"
#include "Config.h"

class Camera {
public:
    Camera();
    Camera(int w, int h);
    void Set(int w, int h);
    void follow(const Vec2 &target, float dt);
    SDL_Rect worldToScreen(const SDL_Rect &r) const;
    void clamp();
    Vec2 pos{0,0};
private:
    int screenW, screenH;
};
