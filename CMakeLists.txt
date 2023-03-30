cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS -pthread)

project(maze_runner)


set(SOURCE maze_runner_threaded.cpp)

add_executable(${PROJECT_NAME} ${SOURCE})
