cmake_minimum_required(VERSION 3.5)

project(BuddyMemAlloc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILE_TYPE DEBUG) 
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

add_executable(buddyMemAlloc example/main.cpp)
target_include_directories(buddyMemAlloc PUBLIC include/)