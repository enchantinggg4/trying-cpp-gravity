cmake_minimum_required(VERSION 3.19)
project(gravity)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/local/include)

set(SOURCE_FILES main.cpp)
add_executable(gravity ${SOURCE_FILES})


find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(gravity sfml-system sfml-window sfml-graphics sfml-audio sfml-network)