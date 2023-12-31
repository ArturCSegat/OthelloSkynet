cmake_minimum_required(VERSION 3.0) 
 
project(OthelloSkynet) 
 
# Define source files 
file(GLOB MAIN_SOURCES "src/main.cpp")  
file(GLOB MODULE_SOURCES "src/modules/*.cpp") 
 
# Include directories 
include_directories(includes) 
 
link_libraries(pthread) 
find_package(Threads) 
 
# Set compile and link flags for AddressSanitizer 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address") 
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address") 
set(CMAKE_CXX_FLAGS "-g")
add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY) 
# Define the executable 
add_executable(Skynet ${MAIN_SOURCES} ${MODULE_SOURCES}) 

