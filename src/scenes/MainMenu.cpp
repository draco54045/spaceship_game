#include "MainMenu.h"

void MainMenuScene::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        // Detect button click (hardcoded example)
        int x = e.button.x, y = e.button.y;
        if (x > 100 && x < 300 && y > 200 && y < 250){

            startClicked = true;
        }
    }
}

void MainMenuScene::render(SDL_Renderer* r, float dt){
    SDL_SetRenderDrawColor(r, 0, 0, 128, 255);
    SDL_RenderClear(r);
    SDL_Rect startButton = {100, 200, 200, 50};
    SDL_SetRenderDrawColor(r, 0, 255, 0, 255);
    SDL_RenderFillRect(r, &startButton);
}

bool MainMenuScene::isFinished() const {
    return startClicked;
}

std::unique_ptr<Scene> MainMenuScene::nextScene(){
    if (startClicked){
        std::unique_ptr<GameplayScene> gameplay = std::make_unique<GameplayScene>(renderer);
        gameplay->init();
        return gameplay;
    }
    return nullptr;
}