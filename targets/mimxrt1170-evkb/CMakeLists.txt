set(CMAKE_SYSTEM_NAME Generic)
cmake_minimum_required(VERSION 3.22)

project(benchmark-imxrt)

set(MCUXPRESSO_CMAKE_FORMAT_MAJOR_VERSION 2)
set(MCUXPRESSO_CMAKE_FORMAT_MINOR_VERSION 0)

include(ide_overrides.cmake OPTIONAL)

if(CMAKE_SCRIPT_MODE_FILE)
  message("${MCUXPRESSO_CMAKE_FORMAT_MAJOR_VERSION}")
  return()
endif()

enable_language(ASM)

set(CMAKE_STATIC_LIBRARY_PREFIX)
set(CMAKE_STATIC_LIBRARY_SUFFIX)

set(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
set(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCES_DIR ${PROJECT_DIR}/src)
set(COMMON_SOURCES_DIR ${PROJECT_DIR}/../../src)

set(MCUX_SDK_DIR "${PROJECT_DIR}/mcuxsdk")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BUILD_TYPE})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BUILD_TYPE})

set(MCUX_BUILD_TYPES release)
set(MCUX_SDK_PROJECT_NAME benchmark-imxrt.elf)

include(${PROJECT_DIR}/flags.cmake)
include(${PROJECT_DIR}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
	${COMMON_SOURCES_DIR}/main.c
	${COMMON_SOURCES_DIR}/measure.c
	${COMMON_SOURCES_DIR}/test.c
	${SOURCES_DIR}/target.c
	"${MCUX_SDK_DIR}/core/boards/evkbmimxrt1170/board.c"
	"${MCUX_SDK_DIR}/core/boards/evkbmimxrt1170/board.h"
	"${MCUX_SDK_DIR}/core/boards/evkbmimxrt1170/clock_config.c"
	"${MCUX_SDK_DIR}/core/boards/evkbmimxrt1170/clock_config.h"
	"${MCUX_SDK_DIR}/core/boards/evkbmimxrt1170/dcd.c"
	"${MCUX_SDK_DIR}/core/boards/evkbmimxrt1170/dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${MCUX_SDK_DIR}/core/boards/evkbmimxrt1170
)

include(${MCUX_SDK_DIR}/core/devices/MIMXRT1176/all_lib_device.cmake)

if(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    set(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
endif()  

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)
set_target_properties(${MCUX_SDK_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES "output.map")
