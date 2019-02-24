cmake_minimum_required(VERSION 3.10)
project(dlvm)

set(CMAKE_CXX_STANDARD 17)

IF (WIN32)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
ELSE()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lrt")
ENDIF()

include_directories(include)
add_subdirectory(src)
#add_subdirectory(test)