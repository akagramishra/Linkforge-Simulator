cmake_minimum_required(VERSION 3.16)
project(linkforge CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

add_library(lf_core src/solver.cpp)
target_include_directories(lf_core PUBLIC include)

add_executable(linkforge src/main.cpp src/render.cpp src/ioui.cpp src/theme.cpp src/raygui_impl.cpp)
target_link_libraries(linkforge PRIVATE lf_core raylib)