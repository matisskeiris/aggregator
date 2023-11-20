cmake_minimum_required(VERSION 3.28)

project(aggregator.runner)

add_executable(aggregator.runner src/main.cpp)
target_link_libraries(aggregator.runner PRIVATE aggregator)

