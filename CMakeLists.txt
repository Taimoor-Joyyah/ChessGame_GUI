cmake_minimum_required(VERSION 3.23)

project(ChessGame_GUI)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(raylib_DIR C:/libs/raylib)

include_directories(${raylib_DIR}/include)
link_directories(${raylib_DIR}/lib)

add_executable(ChessGame_GUI main.cpp LinkedList.h Input.cpp Input.h Frame.cpp Frame.h Menu.cpp Menu.h Chess.cpp Chess.h Enum.h Location.cpp Location.h Piece.cpp Piece.h Chess_Frame.cpp Chess_Rules.cpp Popup.cpp Popup.h Ending.cpp Ending.h Logo.cpp Logo.h)

target_link_libraries(${PROJECT_NAME} raylib)
