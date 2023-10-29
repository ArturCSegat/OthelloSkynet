cmake_minimum_required(VERSION 3.0)

project(OthelloSkynet)

# Define source files
file(GLOB MAIN_SOURCES "src/main.cpp")
file(GLOB MODULE_SOURCES "src/modules/*.cpp")

# Include directories
include_directories(includes)

# Set compile and link flags for AddressSanitizer
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

# Define the executable
add_executable(Skinet ${MAIN_SOURCES} ${MODULE_SOURCES})

