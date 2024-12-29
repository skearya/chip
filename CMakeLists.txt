cmake_minimum_required(VERSION 3.30)
project(chip)

set(RAYLIB_VERSION 5.5)
set(RAYLIB_CPP_VERSION v5.5.0)

find_package(raylib QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            raylib
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_MakeAvailable(raylib)
endif ()

find_package(raylib_cpp QUIET)
if (NOT raylib_cpp_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            raylib_cpp
            URL https://github.com/RobLoach/raylib-cpp/archive/refs/tags/${RAYLIB_CPP_VERSION}.tar.gz
    )
    FetchContent_MakeAvailable(raylib_cpp)
endif ()

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} main.cpp
        cpu.cpp
        cpu.hpp)

target_link_libraries(${PROJECT_NAME} raylib)
target_link_libraries(${PROJECT_NAME} raylib_cpp)

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif ()
