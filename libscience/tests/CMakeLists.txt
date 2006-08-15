if (KDE4_BUILD_TESTS)
include_directories( ${CMAKE_SOURCE_DIR}/libkdeedu/libscience  )

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})


########### next target ###############

set(xmlreadingtest_SRCS xmlreadingtest.cpp )

kde4_automoc(${xmlreadingtest_SRCS})

kde4_add_executable(xmlreadingtest ${xmlreadingtest_SRCS})

target_link_libraries(xmlreadingtest  ${KDE4_KDECORE_LIBS} science )

########### next target ###############

#set(isotopereadingtest_SRCS isotopereadingtest.cpp )

#kde4_automoc(${isotopereadingtest_SRCS})

#kde4_add_executable(isotopereadingtest ${isotopereadingtest_SRCS})

#target_link_libraries(isotopereadingtest  ${KDE4_KDECORE_LIBS} science )

endif(KDE4_BUILD_TESTS)

