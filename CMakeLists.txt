cmake_minimum_required(VERSION 3.21)
project(api2022 C)

set(CMAKE_C_STANDARD 11)

add_executable(api2022 main.c)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -O2 -g3 -fsanitize=address -static-libasan")