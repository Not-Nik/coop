# coop (c) Nikolas Wipper 2021

cmake_minimum_required(VERSION 3.17)
project(coop)

enable_language(ASM_NASM)

set(CMAKE_C_STANDARD 11)

set_source_files_properties(lib/jmp.s PROPERTIES LANGUAGE ASM_NASM)
add_library(coop STATIC lib/jmp.h lib/jmp.c lib/jmp.s lib/coop.c lib/coop.h)

add_executable(coop-test test.c)
target_link_libraries(coop-test coop)