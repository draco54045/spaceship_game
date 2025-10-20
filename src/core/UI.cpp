#include "UI.h"
#include "Game.h"

UI::UI(){}
UI::~UI(){}

void UI::Set(Player* plr, GameplayScene& g){
    player = plr;
    game = &g;
}

UIPlayerData UI::getPlayerData(){
    int mx,my;
    SDL_GetMouseState(&mx,&my);
    return { 
        std::to_string(player->currentSpeed),
        "[" + std::to_string(player->dir.x) + ", " + std::to_string(player->dir.y) + "]",
        "[" + std::to_string(mx) + ", " + std::to_string(my) + "]"
    };
}

UIGameData UI::getGameData(){
    return { 
        std::to_string((int)game->fps)
    };
}