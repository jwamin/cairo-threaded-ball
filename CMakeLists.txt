cmake_minimum_required(VERSION 3.17)
set(CMAKE_VERBOSE_MAKEFILE on)

project(cairoball C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Cairo)
message(STATUS ${CAIRO_LIBRARIES})
link_directories(/usr/local/Cellar/cairo/1.16.0_3/lib)
include_directories(${CAIRO_INCLUDE_DIRS})

add_executable(cairoball main.c)

target_link_libraries(cairoball ${CAIRO_LIBRARIES})

