#include <iostream>
#include<SFML/Network.hpp>
#include "Application.h"

int main() {
    Application app;
    app.initializeSocket();
    app.waitForClients();
    app.initializeGame();
    app.run();
//    app.communicate();
    return 0;

}
