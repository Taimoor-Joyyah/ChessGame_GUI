cmake_minimum_required(VERSION 3.23)

project(ChessGame_GUI)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#set(raylib_DIR C:/libs/raylib)
#
#include_directories(${raylib_DIR}/include)
#link_directories(${raylib_DIR}/lib)

find_package(raylib 4.2.0 QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            URL https://github.com/raysan5/raylib/archive/refs/tags/4.0.0.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

add_executable(ChessGame_GUI main.cpp LinkedList.h Menu.cpp Menu.h Chess.cpp Chess.h Enum.h Location.cpp Location.h Piece.cpp Piece.h Chess_Rules.cpp Popup.cpp Popup.h Ending.cpp Ending.h ChessWindow.cpp ChessWindow.h)

target_link_libraries(${PROJECT_NAME} raylib)
