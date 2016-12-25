cmake_minimum_required(VERSION 3.6)
project(simple_bytecode_interpreter)

set(CMAKE_C_STANDARD 11)

add_library(vm SHARED src/vm.c)