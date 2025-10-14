#include "UI.h"

UI::UI(){}
UI::~UI(){}

void UI::Set(Player* plr){
    player = plr;
}

UIData UI::getData(){
    int mx,my;
    SDL_GetMouseState(&mx,&my);
    return { 
        std::to_string(player->currentSpeed),
        "[" + std::to_string(player->dir.x) + ", " + std::to_string(player->dir.y) + "]",
        "[" + std::to_string(mx) + ", " + std::to_string(my) + "]"
    };
}