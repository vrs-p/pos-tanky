cmake_minimum_required(VERSION 3.18)
project(klient)

set(CMAKE_CXX_STANDARD 20)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR src/lib/cmake/SFML)
find_package(SFML COMPONENTS window graphics network REQUIRED)

include_directories(src/include)


add_executable(klient main.cpp tank/Tank.cpp tank/Tank.h application/Application.cpp application/Application.h bullet/Bullet.h bullet/Bullet.cpp windows/Menu.cpp windows/Menu.h windows/ShowScore.cpp windows/ShowScore.h windows/elements/Textbox.cpp windows/elements/Textbox.h windows/elements/Button.cpp windows/elements/Button.h map/Map.cpp map/Map.h)

target_link_libraries(klient sfml-window sfml-graphics sfml-network)