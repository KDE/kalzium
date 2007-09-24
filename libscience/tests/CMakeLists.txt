set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

########### next target ###############

set(elementparsertest_SRCS xmlreadingtest.cpp)

kde4_add_executable(elementparsertest TEST ${elementparsertest_SRCS})

target_link_libraries(elementparsertest science)

########### next target ###############

set(isotopeparsertest_SRCS isotopereadingtest.cpp)

kde4_add_executable(isotopeparsertest TEST ${isotopeparsertest_SRCS})

target_link_libraries(isotopeparsertest science)


