cmake_minimum_required(VERSION 2.8)
project(ktn)

set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
add_definitions(-DUNICODE -D_UNICODE)
include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB ALL_SRC
     "${CMAKE_SOURCE_DIR}/src/*.c"
     "${CMAKE_SOURCE_DIR}/src/math/*.c"
     "${CMAKE_SOURCE_DIR}/include/ktn/*.h"
     "${CMAKE_SOURCE_DIR}/include/ktn/math/*.h")

add_library(ktn ${ALL_SRC})

add_subdirectory(test)
add_subdirectory(samples)
