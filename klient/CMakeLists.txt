cmake_minimum_required(VERSION 3.18)
project(klient)

set(CMAKE_CXX_STANDARD 20)

set(SDL2_INCLUDE_DIR C:/Users/vrsp/Documents/UNIZA/5semester/POS/SDL2-2.26.1-VCbackup/include)
set(SDL2_LIB_DIR C:/Users/vrsp/Documents/UNIZA/5semester/POS/SDL2-2.26.1-VCbackup/lib/x64)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

add_executable(klient main.cpp)

target_link_libraries(klient SDL2main SDL2)