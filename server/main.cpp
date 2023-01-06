#include <iostream>
#include<SFML/Network.hpp>
#include "application/Application.h"

int main() {
    Application app;
    app.initializeSocket();
    app.waitForClients();
    app.updateOfTanksPositions();
    app.initializeGame();
    app.run();
//    app.communicate();
    return 0;

}
