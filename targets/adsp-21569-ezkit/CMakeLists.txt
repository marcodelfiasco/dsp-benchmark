cmake_minimum_required(VERSION 3.28)

project(benchmark-adi LANGUAGES C)

set(SOURCES
	src/main.c
)

add_executable(benchmark-adi ${SOURCES})
