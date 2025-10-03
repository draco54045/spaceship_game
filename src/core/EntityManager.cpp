#include "EntityManager.h"
#include "Game.h"

void EntityManager::updateAll(Game& g, float dt) {
    for(auto &e: entities) if(e->alive) e->update(g, dt);
    entities.erase(std::remove_if(entities.begin(), entities.end(),
        [](const std::unique_ptr<Entity>& p){ return !p->alive; }),
        entities.end());
}

void EntityManager::renderAll(Game& g, SDL_Renderer* renderer) {
    for(auto &e: entities) if(e->alive) e->render(g, renderer);
}
