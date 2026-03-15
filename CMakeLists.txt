cmake_minimum_required(VERSION 3.17)
project(JSnake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB ImGUI_Headers CONFIGURE_DEPENDS ImGUI/*.h)
file(GLOB ImGUI_Sources CONFIGURE_DEPENDS ImGUI/*.cpp)

file(GLOB JSnake_Headers CONFIGURE_DEPENDS include/*.h)
file(GLOB JSnake_Sources CONFIGURE_DEPENDS src/*.cpp)

find_package(Allegro CONFIG QUIET)

if(Allegro_FOUND)
    set(JSNAKE_ALLEGRO_LIBS
        Allegro::allegro
        Allegro::allegro_font
        Allegro::allegro_image
        Allegro::allegro_primitives
        Allegro::allegro_ttf
    )
else()
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

    find_package(Allegro5 REQUIRED)
    find_package(Allegro5_Font REQUIRED)
    find_package(Allegro5_Image REQUIRED)
    find_package(Allegro5_Primitives REQUIRED)
    find_package(Allegro5_TTF REQUIRED)

    include_directories(
        ${ALLEGRO5_INCLUDE_DIR}
    )

    link_directories(${ALLEGRO5_LIBRARY_DIRS})
    set(JSNAKE_ALLEGRO_LIBS ${ALLEGRO5_LIBS})
endif()

add_executable(JSnake ${ImGUI_Sources} ${ImGUI_Headers} ${JSnake_Sources} ${JSnake_Headers} main.cpp)

target_include_directories(JSnake PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(JSnake PRIVATE ${JSNAKE_ALLEGRO_LIBS})

add_custom_command(TARGET JSnake POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/Assets
        $<TARGET_FILE_DIR:JSnake>/Assets
)
