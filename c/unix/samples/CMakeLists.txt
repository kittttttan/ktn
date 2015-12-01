file(GLOB HEADERS
     "${CMAKE_SOURCE_DIR}/include/**/*.h")

add_executable(testc test.c ${HEADERS})
target_link_libraries(testc ktn)

add_executable(main main.c math-test.c math-test.h ${HEADERS})
target_link_libraries(main ktn)

source_group("ktn" FILES ${HEADERS})

include_directories(${CMAKE_SOURCE_DIR}/include)
