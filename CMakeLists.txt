cmake_minimum_required(VERSION 3.15)
project(besturings2 C)

set(CMAKE_C_STANDARD 11)

add_executable(
        threading_extra c/threading/parallel-merge.c
)
