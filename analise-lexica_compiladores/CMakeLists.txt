cmake_minimum_required(VERSION 3.29)
project(analise_lexica_compiladores C)

set(CMAKE_C_STANDARD 99)

add_executable(analise_lexica_compiladores main.c
        globals.h
        util.h
        util.c
        scan.h
        scan.c
)
