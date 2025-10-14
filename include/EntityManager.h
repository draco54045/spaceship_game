#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include "Entity.h"

class EntityManager {
public:
    template<typename T, typename...Args>
    T* create(Args&&... a) {
        auto e = std::make_unique<T>(std::forward<Args>(a)...);
        T* ptr = e.get();
        entities.push_back(std::move(e));
        return ptr;
    }
    void updateAll(Game& g, float dt);
    void renderAll(Game& g, SDL_Renderer* renderer, float dt);

private:
    std::vector<std::unique_ptr<Entity>> entities;
};
