#pragma once
#include "Scene.h"
#include "Game.h"
#include <memory>

class MainMenuScene : public Scene {
public:
    MainMenuScene(SDL_Renderer* r) : renderer(r), startClicked(false) {
        // Initialize your UI buttons here
    }

    void handleEvent(const SDL_Event& e) override;

    void update(float dt) override {}

    void render(SDL_Renderer* r, float dt) override;

    bool isFinished() const override;

    std::unique_ptr<Scene> nextScene() override; 

private:
    SDL_Renderer* renderer;
    bool startClicked;
};
