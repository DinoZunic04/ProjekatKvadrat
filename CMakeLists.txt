cmake_minimum_required(VERSION 3.24)
project(ProjekatKvadrat)

set(CMAKE_CXX_STANDARD 17)

add_executable(ProjekatKvadrat main.cpp Language/Numerical.cpp Language/Numerical.h Language/Instruction.cpp Language/Instruction.h Universe.cpp Universe.h Language/Boolean.cpp Language/Boolean.h)
