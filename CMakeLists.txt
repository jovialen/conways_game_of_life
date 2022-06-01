cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)
project(conways_game_of_life VERSION 1.0.0 LANGUAGES C CXX)

add_subdirectory("dependencies/tex")

if (WIN32)
	add_executable(cgol WIN32 "sources/main.cpp")
	target_compile_definitions(cgol PRIVATE WINDOWS_BUILD)
else()
	add_executable(cgol "sources/main.cpp")
endif()

set_target_properties(cgol PROPERTIES 
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON)

target_include_directories(cgol PRIVATE "sources")
target_link_libraries(cgol PRIVATE tex::tex)
