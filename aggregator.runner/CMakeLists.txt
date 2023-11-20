cmake_minimum_required(VERSION 3.28)

project(aggregator.runner)

add_executable(aggregator.runner src/main.cpp)

target_include_directories(aggregator.runner PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../aggregator/src")
