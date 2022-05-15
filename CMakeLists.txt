cmake_minimum_required(VERSION 3.20)

project(space-evader)

add_executable(space-evader)

target_sources(space-evader
  PUBLIC src/main.cpp
  PUBLIC src/Game.cpp
  PUBLIC src/Player.cpp
  PUBLIC src/Asteroid.cpp
  PUBLIC src/Timer.cpp)

add_compile_options(-g -O3)

target_include_directories(space-evader PUBLIC include/)

target_link_libraries(space-evader -lSDL2 -lSDL2_image -lSDL2_ttf)
