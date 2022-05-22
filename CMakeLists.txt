cmake_minimum_required(VERSION 3.17)
project(JSnake)

set(CMAKE_CXX_STANDARD 20)

FILE(GLOB ImGUI_Headers ImGUI/*.h)
FILE(GLOB ImGUI_Sources ImGUI/*.cpp)

FILE(GLOB JSnake_Headers *.h)
FILE(GLOB JSnake_Sources *.cpp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(Allegro5 REQUIRED)
find_package(Allegro5_Font REQUIRED)
find_package(Allegro5_Image REQUIRED)
find_package(Allegro5_Primitives REQUIRED)
find_package(Allegro5_TTF REQUIRED)

include_directories(
    ${ALLEGRO5_INCLUDE_DIR}
)

LINK_DIRECTORIES (${ALLEGRO5_LIBRARY_DIRS})

add_executable(JSnake ${ImGUI_Sources} ${ImGUI_Headers} ${JSnake_Sources} ${JSnake_Headers})

TARGET_LINK_LIBRARIES(JSnake ${ALLEGRO5_LIBS})
