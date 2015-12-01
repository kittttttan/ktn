set(CMAKE_CXX_FLAGS_DEBUG "/MTd")

file(GLOB MATH_SOURCES
     "${CMAKE_SOURCE_DIR}/include/ktn/math/*.h")

source_group("math" FILES ${MATH_SOURCES})

include_directories(${CMAKE_SOURCE_DIR}/include)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(unittest unittest.cc ${MATH_SOURCES})
target_link_libraries(unittest ktn ${GTEST_BOTH_LIBRARIES})

# add_test(Test1 unittest)
