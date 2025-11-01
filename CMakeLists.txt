cmake_minimum_required(VERSION 3.15)
project(MiniGit)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/core
        ${PROJECT_SOURCE_DIR}/include/cli
        ${PROJECT_SOURCE_DIR}/include/diff
        ${PROJECT_SOURCE_DIR}/include/observer
        ${PROJECT_SOURCE_DIR}/include/branching
        ${PROJECT_SOURCE_DIR}/include/repository
        ${PROJECT_SOURCE_DIR}/include/command
)

file(GLOB_RECURSE CORE_SOURCES src/core/*.cpp)
file(GLOB_RECURSE CLI_SOURCES src/cli/*.cpp)
file(GLOB_RECURSE DIFF_SOURCES src/diff/*.cpp)
file(GLOB_RECURSE OBSERVER_SOURCES src/observer/*.cpp)
file(GLOB_RECURSE BRANCHING_SOURCES src/branching/*.cpp)
file(GLOB_RECURSE REPOSITORY_SOURCES src/repository/*.cpp)
file(GLOB_RECURSE COMMAND_SOURCES src/command/*.cpp)

set(MINIGIT_LIB_SOURCES
        ${CORE_SOURCES}
        ${CLI_SOURCES}
        ${DIFF_SOURCES}
        ${OBSERVER_SOURCES}
        ${BRANCHING_SOURCES}
        ${REPOSITORY_SOURCES}
        ${COMMAND_SOURCES}
)

add_library(MiniGitCore STATIC ${MINIGIT_LIB_SOURCES})
target_compile_options(MiniGitCore PRIVATE -Wall -Wextra -Wpedantic -g)

add_executable(MiniGit src/main.cpp)
target_link_libraries(MiniGit PRIVATE MiniGitCore)
target_compile_options(MiniGit PRIVATE -Wall -Wextra -Wpedantic -g)

enable_testing()

file(GLOB TEST_SOURCES tests/*.cpp)
if (TEST_SOURCES)
    add_executable(MiniGitTests ${TEST_SOURCES})
    target_link_libraries(MiniGitTests PRIVATE MiniGitCore)
    target_compile_options(MiniGitTests PRIVATE -Wall -Wextra -Wpedantic -g)
    add_test(NAME MiniGitTests COMMAND MiniGitTests)
endif ()
