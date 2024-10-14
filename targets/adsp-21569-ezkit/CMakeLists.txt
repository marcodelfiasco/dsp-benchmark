cmake_minimum_required(VERSION 3.28)

project(benchmark-adi LANGUAGES C ASM)

set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCES_DIR ${PROJECT_DIR}/src)
set(COMMON_SOURCES_DIR ${PROJECT_DIR}/../../src)

set(SOURCES
	${COMMON_SOURCES_DIR}/main.c
	${COMMON_SOURCES_DIR}/measure.c
	${COMMON_SOURCES_DIR}/test.c
	${COMMON_SOURCES_DIR}/test_vectors.c
	${SOURCES_DIR}/target.c
	${SOURCES_DIR}/tp.c
	${SOURCES_DIR}/twi.c
	${SOURCES_DIR}/startup.s
	${SOURCES_DIR}/vectors.s
)

set(INCLUDES
	${COMMON_SOURCES_DIR}
	${SOURCES_DIR}
)

add_executable(benchmark-adi ${SOURCES})
target_compile_options(benchmark-adi PRIVATE -si-revision 0.0 -short-word-code -char-size-8 -save-temps -O1 -Ov 100 -Oa -always-inline)
target_link_options(benchmark-adi PRIVATE -si-revision 0.0 -T${SOURCES_DIR}/linker.ldf -LCMakeFiles/benchmark-adi.dir/src -no-mem -map map.xml -flags-link -e -flags-link)
target_include_directories(benchmark-adi PRIVATE ${INCLUDES})
