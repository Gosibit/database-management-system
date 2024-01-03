cmake_minimum_required(VERSION 3.27)
project(db_mvp)

set(CMAKE_CXX_STANDARD 20)

add_executable(db_mvp main.cpp)

include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 10.1.1
)

FetchContent_MakeAvailable(fmt)

add_executable(db_mvp main.cpp)
target_link_libraries(db_mvp fmt)
