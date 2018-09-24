cmake_minimum_required(VERSION 3.10)
project(BasicInterpretator C)

set(CMAKE_C_STANDARD 99)

add_executable(BasicInterpretator main.c executor.c analyzer.c headers/main.h headers/executor.h headers/analyzer.h headers/constants.h)