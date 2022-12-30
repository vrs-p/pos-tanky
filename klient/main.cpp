#include <iostream>

#include "Application.h"
#include<SFML/Network.hpp>

//const int SCREEN_WIDTH = 800;
//const int SCREEN_HEIGHT = 800;

int main(int argc, char* args[]) {
//    sf::UdpSocket socket;
//    sf::Packet packetSend = sf::Packet{};
//    sf::Packet packetRecieve = sf::Packet{};
//
//    std::string sprava = "Hey server";
//
//    sf::IpAddress ipAddress = sf::IpAddress::LocalHost;
//    std::cout << "Packet will be send on IP address: " << ipAddress.toString() << "\n";
//    std::cout << "Packet will be send on PORT: 55000" << "\n";
//
//    packetSend << sprava;
//    if (socket.send(packetSend, ipAddress, 55000) != sf::Socket::Done) {
//        std::cout << "Sending failed" << "\n";
//        return 1;
//    }
//    std::cout << "Message was sent sucesfully" << "\n";

    Application app;
    app.run();
    return 0;
}