cmake_minimum_required(VERSION 3.17)
project(JSnake)

set(CMAKE_CXX_STANDARD 20)

FILE(GLOB ImGUI_Headers ImGUI/*.h)
FILE(GLOB ImGUI_Sources ImGUI/*.cpp)

FILE(GLOB JSnake_Headers *.h)
FILE(GLOB JSnake_Sources *.cpp)

INCLUDE_DIRECTORIES(/usr/include/allegro5)

LINK_DIRECTORIES(/usr/lib64/)

add_executable(JSnake ${ImGUI_Sources} ${ImGUI_Headers} ${JSnake_Sources} ${JSnake_Headers})

TARGET_LINK_LIBRARIES(JSnake liballegro.so liballegro_primitives.so liballegro_font.so liballegro_ttf.so)
