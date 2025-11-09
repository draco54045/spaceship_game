#include "PauseMenu.h"

PauseMenu::PauseMenu(FontManager* f){
    auto& cfg = Config::get();
    fonts = f;
    Label pause = Label(640, 150, 0, 0, cfg.black, cfg.green, "Game paused", fonts->MMFont_huge);
    labels.push_back(pause);
    Label resume = Label(640, 250, 0, 0, cfg.black, cfg.green, "Press ESC to resume", fonts->MMFont_medium);
    labels.push_back(resume);
    Button Quit = Button(440, 600, 400, 100, cfg.green, cfg.black, "Quit", fonts->MMFont_small);
    buttons.push_back(Quit);
    Button Settings = Button(440, 300, 400, 100, cfg.green, cfg.black, "Settings", fonts->MMFont_small);
    buttons.push_back(Settings);
    Button SaveQuit = Button(440, 450, 400, 100, cfg.green, cfg.black, "Save and Quit", fonts->MMFont_small);
    buttons.push_back(SaveQuit);
    for (auto& b : buttons) b.updateLayout();
    for (auto& l : labels) l.updateLayout();       

}

PauseMenu::~PauseMenu(){}

void PauseMenu::Set(SDL_Renderer* r, FontManager* f){
    renderer = r;
    fonts = f;
}

void PauseMenu::drawMenu(SDL_Renderer* r){
for (auto& b : buttons) b.render(r);
for (auto& l : labels) l.render(r);
}