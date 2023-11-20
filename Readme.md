# Aggregator

Small classes to generate, process and aggragate data on multiple threads.

Used simple concurrent queue implementation using locks and awaiting condition variable.

## Build

Use CMake to generate build files or use already generated VS solution (`./aggregator.sln`). 

```
cmake .
cmake --build
```

## Running application

For showcase provided `aggregator.runner` application for showcasing running all threads. 

After building sources run the following file:

```
./aggregator.runner/<Debug/Release>/aggregator.runner.exe
```

## Tests

For testing purposes using gtest framework. Just run built test application:

```
./aggregator.tests/Debug/aggregator.tests.exe
```
