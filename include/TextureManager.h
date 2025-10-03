#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <unordered_map>
#include <iostream>

class TextureManager {
public:
    TextureManager(SDL_Renderer* r);
    ~TextureManager();
    SDL_Texture* load(const std::string &path);

private:
    SDL_Renderer* renderer;
    std::unordered_map<std::string, SDL_Texture*> textures;
};
