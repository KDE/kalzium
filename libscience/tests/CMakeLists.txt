include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

########### next target ###############

set(elementparsertest_SRCS xmlreadingtest.cpp)

kde4_add_test_executable(elementparsertest ${elementparsertest_SRCS})

target_link_libraries(elementparsertest science)

########### next target ###############

set(isotopeparsertest_SRCS isotopereadingtest.cpp)

kde4_add_test_executable(isotopeparsertest ${isotopeparsertest_SRCS})

target_link_libraries(isotopeparsertest science)


