cmake_minimum_required(VERSION 3.25)

project(BspDowngradeTool CXX)

set(CMAKE_BUILD_RPATH_USE_ORIGIN ON)
set(CMAKE_CXX_STANDARD 23)

## App src.
set(SOURCES
        src/main.cpp src/ValveBsp.h src/StreamUtils.h)
add_executable(${PROJECT_NAME} ${SOURCES})