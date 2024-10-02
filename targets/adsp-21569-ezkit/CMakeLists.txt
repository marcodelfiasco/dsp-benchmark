cmake_minimum_required(VERSION 3.28)

project(benchmark-adi LANGUAGES C)

set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCES_DIR ${PROJECT_DIR}/src)
set(COMMON_SOURCES_DIR ${PROJECT_DIR}/../../src)

set(SOURCES
	${COMMON_SOURCES_DIR}/main.c
	${COMMON_SOURCES_DIR}/measure.c
	${COMMON_SOURCES_DIR}/test.c
	${SOURCES_DIR}/target.c
)

add_executable(benchmark-adi ${SOURCES})
