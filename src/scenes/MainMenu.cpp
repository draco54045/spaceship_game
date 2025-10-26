#include "MainMenu.h"

void MainMenuScene::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int x = e.button.x, y = e.button.y;
        if (Start.wasClicked(x,y)){ startClicked = true; }
    }
}

void MainMenuScene::render(SDL_Renderer* r, float dt){
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    SDL_RenderClear(r);
    Start.render(r);
    Title.render(r);
}

bool MainMenuScene::isFinished() const {
    return startClicked;
}

std::unique_ptr<Scene> MainMenuScene::nextScene(){
    if (startClicked){
        std::unique_ptr<GameplayScene> gameplay = std::make_unique<GameplayScene>(renderer, fonts);
        gameplay->init();
        return gameplay;
    }
    return nullptr;
}