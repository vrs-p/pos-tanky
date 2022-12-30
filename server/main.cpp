#include <iostream>
#include<SFML/Network.hpp>

enum DIRECTION {UP,
    DOWN,
    LEFT,
    RIGHT};

int main() {
    float x, y;
    int dir;
    bool isRunning = false;


    sf::UdpSocket socket;
    sf::Packet packetSend = sf::Packet{};
    sf::Packet packetRecieve = sf::Packet{};

    std::string sprava;
    std::size_t received;

    sf::IpAddress ipAddress = sf::IpAddress::Any;
    std::cout << "IP address: " << ipAddress.toString() << "\n";

    if (socket.bind(55000, ipAddress) != sf::Socket::Done) {
        std::cout << "Unable to bind PORT 55000"<< "\n";
    }

    unsigned short port;
    if (socket.receive(packetRecieve, ipAddress, port) == sf::Socket::Done) {
        std::cout << "Message was received\n";
        packetRecieve >> x;
        packetRecieve >> y;
        packetRecieve >> dir;
        packetRecieve >> isRunning;
    }

    while (isRunning) {
        if (socket.receive(packetRecieve, ipAddress, port) == sf::Socket::Done) {
            packetRecieve >> x;
            packetRecieve >> y;
            packetRecieve >> dir;
            packetRecieve >> isRunning;
        }

        std::cout << "X: " << x << " Y: " << y << " Direction: " << dir << "\n";
    }

    return 0;
}
