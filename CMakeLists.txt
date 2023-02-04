cmake_minimum_required(VERSION 3.24)
project(ProjekatKvadrat)

set(CMAKE_CXX_STANDARD 17)

add_executable(ProjekatKvadrat main.cpp Language/Term.cpp Language/Term.h Language/Instruction.cpp Language/Instruction.h)
