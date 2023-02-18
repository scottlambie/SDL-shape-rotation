cmake_minimum_required(VERSION 3.0.0)
project(SDLproject VERSION 0.1.0)

add_executable(rotateShape src/rotateShape.cpp)

target_link_libraries(rotateShape SDL2main SDL2)