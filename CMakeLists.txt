cmake_minimum_required(VERSION 3.23)

project(ChessGame)

set(CMAKE_CXX_STANDARD 20)

set(SDL2_INCLUDE_DIR C:/libs/SDL/x86_64-w64-mingw32/include)
set(SDL2_LIB_DIR C:/libs/SDL/x86_64-w64-mingw32/lib/)

set(SDL2_DIR C:/libs/SDL/cmake)
find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

add_executable(ChessGame main.cpp LinkedList.h Input.cpp Input.h Frame.cpp Frame.h Menu.cpp Menu.h Chess.cpp Chess.h Enum.h Location.cpp Location.h)

target_link_libraries(${PROJECT_NAME} SDL2main SDL2)
