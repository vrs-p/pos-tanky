cmake_minimum_required(VERSION 3.18)
project(klient)

set(CMAKE_CXX_STANDARD 20)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/Users/filip/Desktop/POS/sfml/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(C:/Users/filip/Desktop/POS/sfml/include)


add_executable(klient main.cpp Tank.cpp Tank.h Application.cpp Application.h Bullet.h Bullet.cpp Menu.cpp Menu.h)

target_link_libraries(klient sfml-system sfml-window sfml-graphics sfml-audio sfml-network)