#include "Camera.h"

Camera::Camera() {}
Camera::Camera(int w, int h): screenW(w), screenH(h) {}

void Camera::follow(const Vec2 &target, float /*dt*/) {
    Vec2 center = {target.x - screenW/2.0f, target.y - screenH/2.0f};
    pos.x += (center.x - pos.x) * 0.15f;
    pos.y += (center.y - pos.y) * 0.15f;
    clamp();
}

SDL_Rect Camera::worldToScreen(const SDL_Rect &r) const {
    SDL_Rect s = r;
    s.x = int(r.x - pos.x);
    s.y = int(r.y - pos.y);
    return s;
}

void Camera::clamp() {
    if(pos.x < 0) pos.x = 0;
    if(pos.y < 0) pos.y = 0;
    if(pos.x > VIRTUAL_WORLD_W - screenW) pos.x = VIRTUAL_WORLD_W - screenW;
    if(pos.y > VIRTUAL_WORLD_H - screenH) pos.y = VIRTUAL_WORLD_H - screenH;
}

void Camera::Set(int w, int h){
    screenW = w;
    screenH = h;
}