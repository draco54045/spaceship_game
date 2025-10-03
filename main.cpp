#include "Game.h"

int main(int /*argc*/, char** /*argv*/) {
    Game g;
    if(!g.init()) return 1;
    g.run();
    return 0;
}