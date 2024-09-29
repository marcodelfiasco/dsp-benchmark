cmake_minimum_required(VERSION 3.22)

project(benchmark-rpi4 LANGUAGES C)

set(SOURCES
	src/main.c
)

add_executable(benchmark-rpi4 ${SOURCES})
