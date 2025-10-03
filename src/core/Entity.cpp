#include "Entity.h"
#include "Game.h"

Entity::Entity(float x, float y, int w, int h): pos{x,y}, w(w), h(h) {}

void Entity::render(Game& g, SDL_Renderer* renderer) {
    SDL_Rect r{int(pos.x), int(pos.y), w, h};
    auto screen = g.camera.worldToScreen(r);
    SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
    SDL_RenderFillRect(renderer, &screen);
}

SDL_Rect Entity::aabb() const {
    return SDL_Rect{int(pos.x), int(pos.y), w, h};
}
