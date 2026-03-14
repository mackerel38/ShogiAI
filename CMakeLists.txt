cmake_minimum_required(VERSION 3.16)
project(ShogiAI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(shogi_ai
    src/main.cpp
    src/board/board.cpp
    src/engine/usi_engine.cpp
    src/move/move.cpp
    src/nn/board_encoder.cpp
)

target_include_directories(shogi_ai PRIVATE src)

enable_testing()
