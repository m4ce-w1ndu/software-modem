cmake_minimum_required(VERSION 3.15)

# Project name
project(SoftwareModem VERSION 1.0 LANGUAGES CXX)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include subdirectories
add_subdirectory(src/modem)
add_subdirectory(src/example)
