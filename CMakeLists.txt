cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(conways_game_of_life VERSION 1.0.0 LANGUAGES C CXX)

add_subdirectory("dependencies/tex")

add_executable(cgol "sources/main.cpp")

target_include_directories(cgol PRIVATE "sources")
target_link_libraries(cgol PRIVATE tex::tex)
