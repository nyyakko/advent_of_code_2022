cmake_minimum_required(VERSION 3.1)
project(advent_of_code LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

include(static_analyzers)

find_package(FMT CONFIG REQUIRED)

set(advent_of_code_ExternalLibraries
    fmt::fmt
)

add_subdirectory(advent_of_code)
