cmake_minimum_required(VERSION 3.2.2) # CMake version check
project(renderer-project)               # Create project "simple_example"
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") # Enable c++11 standard
include_directories( ${MY_SOURCE_DIR}/src ) #including /src dir
set(SOURCE_FILES src/main.cpp src/tgaimage.cpp src/tgaimage.h src/primitives.cpp src/primitives.h src/geometry.h src/model.cpp src/model.h src/geometry.cpp src/material.cpp src/material.h)            # Add main.cpp file of project root directory as source file
add_executable(renderer ${SOURCE_FILES})       # Add executable target with source files listed in SOURCE_FILES variable