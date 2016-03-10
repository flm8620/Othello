cmake_minimum_required(VERSION 3.3)
project(Othello)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_definitions(-DsrcPath="${CMAKE_CURRENT_SOURCE_DIR}")
set(SOURCE_FILES main.cpp GameState.cpp GameState.h OthelloAI.cpp OthelloAI.h GameJudge.cpp GameJudge.h genetichost.cpp tools.cpp)
add_executable(Othello ${SOURCE_FILES})
include_directories("${PROJECT_SOURCE_DIR}")
add_executable(UnitTest unit_test.cpp GameState.cpp GameState.h OthelloAI.cpp OthelloAI.h GameJudge.cpp GameJudge.h genetichost.cpp tools.cpp)
