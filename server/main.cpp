#include <iostream>
#include<SFML/Network.hpp>
#include "Application.h"

int main() {
    Application app;
//    app.run();
    app.initializeSocket();
    app.waitForClients();
    return 0;

}
