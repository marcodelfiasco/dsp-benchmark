cmake_minimum_required(VERSION 3.22)

project(benchmark-rpi4 LANGUAGES C)

set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCES_DIR ${PROJECT_DIR}/src)
set(COMMON_SOURCES_DIR ${PROJECT_DIR}/../../src)

set(SOURCES
	${COMMON_SOURCES_DIR}/main.c
	${COMMON_SOURCES_DIR}/measure.c
	${COMMON_SOURCES_DIR}/mem.c
	${COMMON_SOURCES_DIR}/test.c
	${COMMON_SOURCES_DIR}/test_vectors.c
	${SOURCES_DIR}/target.c
	${SOURCES_DIR}/tp.c
)

set(INCLUDES
	${COMMON_SOURCES_DIR}
	${SOURCES_DIR}
)

add_executable(benchmark-rpi4 ${SOURCES})
target_link_libraries(benchmark-rpi4 PRIVATE "-levl")
target_link_options(benchmark-rpi4 PRIVATE "-Ofast")
target_include_directories(benchmark-rpi4 PRIVATE ${INCLUDES})
