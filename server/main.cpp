#include <iostream>
#include<SFML/Network.hpp>
#include "Application.h"

int main() {
    Application app;
//    app.run();
    app.initializeSocket();
    app.waitForClients();
    app.initializeGame();
    app.communicate();
    return 0;

}
