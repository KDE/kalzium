set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

########### next target ###############

set(elementparsertest_SRCS xmlreadingtest.cpp)

kde4_add_executable(elementparsertest NOGUI ${elementparsertest_SRCS})

target_link_libraries(elementparsertest science ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY})

########### next target ###############

set(isotopeparsertest_SRCS isotopereadingtest.cpp)

kde4_add_executable(isotopeparsertest NOGUI ${isotopeparsertest_SRCS})

target_link_libraries(isotopeparsertest science ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY})

########### next target ###############

set(spectrumtest_SRCS spectrumreadingtests.cpp)

kde4_add_executable(spectrumtest NOGUI ${spectrumtest_SRCS})

target_link_libraries(spectrumtest science ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY})


