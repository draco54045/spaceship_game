#include "TextureManager.h"

TextureManager::TextureManager(SDL_Renderer* r): renderer(r) {}

TextureManager::~TextureManager() {
    for(auto &p: textures) if(p.second) SDL_DestroyTexture(p.second);
}

SDL_Texture* TextureManager::load(const std::string &path) {
    if(textures.count(path)) return textures[path];
    SDL_Surface* surf = IMG_Load(path.c_str());
    if(!surf) {
        std::cerr << "IMG_Load failed for "<<path<<" -> "<<IMG_GetError()<<"\n";
        textures[path] = nullptr;
        return nullptr;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    if(!tex) std::cerr<<"CreateTextureFromSurface failed: "<<SDL_GetError()<<"\n";
    textures[path] = tex;
    return tex;
}
