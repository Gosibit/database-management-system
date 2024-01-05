cmake_minimum_required(VERSION 3.27)
project(cpp-db-mvp)

set(CMAKE_CXX_STANDARD 20)

add_executable(cpp-db-mvp main.cpp includes/Table.cpp includes/tokenizer.cpp includes/Keyword.cpp includes/KeywordDerivaties.cpp includes/Column.cpp includes/stringUtilities.cpp
        includes/Type.h
        includes/Type.cpp
        includes/TypeDerivaties.cpp
        includes/TypeDerivaties.h
        init.cpp
)


include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 10.1.1
)

FetchContent_MakeAvailable(fmt)

target_link_libraries(cpp-db-mvp fmt)

add_executable(tests tests.cpp includes/Table.cpp includes/tokenizer.cpp includes/Keyword.cpp includes/KeywordDerivaties.cpp init.cpp includes/Column.cpp includes/stringUtilities.cpp
        includes/Type.h
        includes/Type.cpp
        includes/TypeDerivaties.cpp
        includes/TypeDerivaties.h)

target_link_libraries(tests fmt)
