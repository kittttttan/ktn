file(GLOB SRC
     "${CMAKE_SOURCE_DIR}/src/*.cc")

file(GLOB HEADERS
     "${CMAKE_SOURCE_DIR}/include/**/*.inl"
     "${CMAKE_SOURCE_DIR}/include/**/*.h")

file(GLOB ALL_SRC
     "${CMAKE_SOURCE_DIR}/src/*.cc"
     "${CMAKE_SOURCE_DIR}/include/**/*.inl"
     "${CMAKE_SOURCE_DIR}/include/**/*.h")

add_executable(testcc test.cc ${ALL_SRC})
add_executable(main main.cc math-test.cc math-test.h ${ALL_SRC})

source_group("src" FILES ${SRC})
source_group("header" FILES ${HEADERS})

include_directories(${CMAKE_SOURCE_DIR}/include)
