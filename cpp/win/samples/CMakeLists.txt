file(GLOB HEADERS
     "${CMAKE_SOURCE_DIR}/include/ktn/*.inl"
     "${CMAKE_SOURCE_DIR}/include/ktn/*.h")

file(GLOB MATH_HEADERS
     "${CMAKE_SOURCE_DIR}/include/ktn/math/*.inl"
     "${CMAKE_SOURCE_DIR}/include/ktn/math/*.h")

add_executable(testcc test.cc ${HEADERS} ${MATH_HEADERS})
add_executable(main main.cc math-test.cc math-test.h ${HEADERS} ${MATH_HEADERS})

source_group("src" FILES ${SRC})
source_group("header" FILES ${HEADERS})
source_group("math" FILES ${MATH_HEADERS})

include_directories(${CMAKE_SOURCE_DIR}/include)
