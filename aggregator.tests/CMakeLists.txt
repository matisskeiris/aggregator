cmake_minimum_required(VERSION 3.28)

project(aggregator.tests)

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
FetchContent_MakeAvailable(googletest)

file( GLOB LIB_SOURCES src/*.cpp )

add_executable(aggregator.tests ${LIB_SOURCES})
target_link_libraries(aggregator.tests gtest_main aggregator)
