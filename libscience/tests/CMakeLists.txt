set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

########### next target ###############

set(elementparsertest_SRCS xmlreadingtest.cpp)

add_executable(elementparsertest ${elementparsertest_SRCS})

target_link_libraries(elementparsertest science Qt5::Core Qt5::Xml)
ecm_mark_nongui_executable(elementparsertest)

########### next target ###############

set(isotopeparsertest_SRCS isotopereadingtest.cpp)

add_executable(isotopeparsertest ${isotopeparsertest_SRCS})
ecm_mark_nongui_executable(isotopeparsertest)

target_link_libraries(isotopeparsertest science Qt5::Core Qt5::Xml)

########### next target ###############

set(spectrumtest_SRCS spectrumreadingtests.cpp)

add_executable(spectrumtest ${spectrumtest_SRCS})
ecm_mark_nongui_executable(spectrumtest)

target_link_libraries(spectrumtest science Qt5::Core Qt5::Xml)


