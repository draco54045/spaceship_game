#pragma once
#include <SDL.h>
#include <memory>

class Scene {
public:
    virtual ~Scene() {}
    virtual void handleEvent(const SDL_Event& e) = 0;
    virtual void update(float dt) = 0;
    virtual void render(SDL_Renderer* renderer, float dt) = 0;
    virtual bool isFinished() const { return false; }
    virtual std::unique_ptr<Scene> nextScene() { return nullptr; }
};
