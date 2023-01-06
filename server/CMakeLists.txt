cmake_minimum_required(VERSION 3.18)
project(server)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML COMPONENTS system network REQUIRED)

add_executable(server main.cpp application/Application.cpp application/Application.h client/Client.cpp client/Client.h)

target_link_libraries(server sfml-network pthread)
