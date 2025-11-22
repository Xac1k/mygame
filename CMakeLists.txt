cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(myapp)

set(SFML_DIR "/home/xac1k/SFML-3.0.2/lib/cmake/SFML")
find_package(SFML 2.6 COMPONENTS graphics window system audio REQUIRED)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(myapp main.cpp)

file(GLOB_RECURSE SOURCES */*.cpp */*.h)
add_library(lib_utils ${SOURCES})

target_link_libraries(myapp lib_utils sfml-graphics sfml-window sfml-system sfml-audio)

target_include_directories(myapp PRIVATE lib)
target_include_directories(lib_utils PRIVATE lib)
