#include "App.h"

int main(int /*argc*/, char** /*argv*/) {
    Config::get();
    Config::get().loadFromFile("./settings.ini");
    App app;
    if(!app.init()) return 1;
    app.run();
    return 0;
}