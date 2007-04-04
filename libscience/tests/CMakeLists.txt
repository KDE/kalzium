if(KDE4_BUILD_TESTS)

# Tests don't need to go into toplevel/bin, they are fine in the current dir.
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

########### next target ###############

set(parsertest_SRCS xmlreadingtest.cpp)

kde4_add_executable(parsertest ${parsertest_SRCS})

target_link_libraries(parsertest science)


endif(KDE4_BUILD_TESTS)

