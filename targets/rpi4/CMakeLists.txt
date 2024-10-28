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

# Enable compiler feedback
add_compile_options(-fverbose-asm)
add_compile_options(-save-temps=cwd)
add_compile_options(-fdump-tree-vect-details)

# Add CMSIS-DSP
add_compile_options(-D__GNUC_PYTHON__)
add_compile_options(-DARM_MATH_NEON)
add_compile_options(-Ofast)

set(NEON ON CACHE BOOL "" FORCE)
set(NEONEXPERIMENTAL ON CACHE BOOL "" FORCE)
set(HELIUMEXPERIMENTAL OFF CACHE BOOL "" FORCE)
set(LOOPUNROLL ON CACHE BOOL "" FORCE)
set(ROUNDING OFF CACHE BOOL "" FORCE)
set(MATRIXCHECK OFF CACHE BOOL "" FORCE)
set(HELIUM OFF CACHE BOOL "" FORCE)
set(MVEF OFF CACHE BOOL "" FORCE)
set(MVEI OFF CACHE BOOL "" FORCE)
set(MVEFLOAT16 OFF CACHE BOOL "" FORCE)
set(DISABLEFLOAT16 ON CACHE BOOL "" FORCE)
set(HOST ON CACHE BOOL "" FORCE)
set(AUTOVECTORIZE ON CACHE BOOL "" FORCE)
set(LAXVECTORCONVERSIONS ON CACHE BOOL "" FORCE)
add_subdirectory(CMSIS-DSP)

add_executable(benchmark-rpi4 ${SOURCES})
target_link_libraries(benchmark-rpi4 PRIVATE CMSISDSP evl)
target_include_directories(benchmark-rpi4 PRIVATE ${INCLUDES})
