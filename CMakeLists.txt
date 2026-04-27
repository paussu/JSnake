cmake_minimum_required(VERSION 3.17)

# Bootstrap vcpkg toolchain if not provided on the command line.
# Must be set before the first project() call.
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "vcpkg toolchain file")
endif()
if(NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "vcpkg target triplet")
endif()

project(JSnake)

# Ensure vcpkg installed packages are findable even if the toolchain file
# was not injected before project() (e.g. when VS Code omits it).
list(APPEND CMAKE_PREFIX_PATH "C:/vcpkg/installed/x64-windows")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB ImGUI_Headers CONFIGURE_DEPENDS ImGUI/*.h)
file(GLOB ImGUI_Sources CONFIGURE_DEPENDS ImGUI/*.cpp)

file(GLOB JSnake_Headers CONFIGURE_DEPENDS include/*.h)
file(GLOB JSnake_Sources CONFIGURE_DEPENDS src/*.cpp)

find_package(Allegro CONFIG REQUIRED)

add_executable(JSnake
    ${ImGUI_Sources}
    ${ImGUI_Headers}
    ${JSnake_Sources}
    ${JSnake_Headers}
    main.cpp
)

target_include_directories(JSnake PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(JSnake PRIVATE
    Allegro::allegro
    Allegro::allegro_font
    Allegro::allegro_image
    Allegro::allegro_primitives
    Allegro::allegro_ttf
)

# Copy Allegro DLLs next to the executable after each build.
if(DEFINED VCPKG_INSTALLED_DIR AND DEFINED VCPKG_TARGET_TRIPLET)
    set(_ALLEGRO_BIN_DIR "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin")
else()
    set(_ALLEGRO_BIN_DIR "C:/vcpkg/installed/x64-windows/bin")
endif()
add_custom_command(TARGET JSnake POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${_ALLEGRO_BIN_DIR}/allegro-5.2.dll"
        "${_ALLEGRO_BIN_DIR}/allegro_font-5.2.dll"
        "${_ALLEGRO_BIN_DIR}/allegro_image-5.2.dll"
        "${_ALLEGRO_BIN_DIR}/allegro_primitives-5.2.dll"
        "${_ALLEGRO_BIN_DIR}/allegro_ttf-5.2.dll"
        $<TARGET_FILE_DIR:JSnake>
    COMMENT "Copying Allegro DLLs..."
)

# Copy game assets next to the executable.
add_custom_command(TARGET JSnake POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/Assets
        $<TARGET_FILE_DIR:JSnake>/Assets
    COMMENT "Copying Assets..."
)
