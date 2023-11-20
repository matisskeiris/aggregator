cmake_minimum_required(VERSION 3.28)

project(aggregator)

add_library(aggregator INTERFACE)
target_include_directories(aggregator INTERFACE "${PROJECT_SOURCE_DIR}/src")
