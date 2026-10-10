cmake_minimum_required(VERSION 3.16)

project(bouncing_balls LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SFML 3 REQUIRED COMPONENTS Graphics)

add_executable(bouncing_balls src/main.cpp)
target_compile_features(bouncing_balls PRIVATE cxx_std_17)
target_link_libraries(bouncing_balls PRIVATE SFML::Graphics)
