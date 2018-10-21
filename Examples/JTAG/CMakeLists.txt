cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(JTAG)

add_executable(jtag main.c)

target_link_libraries(jtag PJ_RPI)
