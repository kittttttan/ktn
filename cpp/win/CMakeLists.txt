cmake_minimum_required(VERSION 2.8)
project(ktn)

add_definitions(-DUNICODE -D_UNICODE)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(test)
add_subdirectory(samples)
