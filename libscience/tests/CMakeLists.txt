
include_directories( ${CMAKE_SOURCE_DIR}/libkdeedu/libscience  )

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

if (KDE4_BUILD_TESTS)

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






#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/libkdeedu/libscience $(all_includes)
#
#AM_LDFLAGS = $(QT_LDFLAGS) $(X_LDFLAGS) $(KDE_RPATH)
#
#check_PROGRAMS = xmlreadingtest isotopereadingtest #cmlspectest
#
#xmlreadingtest_SOURCES = xmlreadingtest.cpp
#xmlreadingtest_LDFLAGS = $(all_libraries)
#xmlreadingtest_LDADD = ../libscience.la
#
#isotopereadingtest_SOURCES = isotopereadingtest.cpp
#isotopereadingtest_LDFLAGS = $(all_libraries)
#isotopereadingtest_LDADD = ../libscience.la
#
##cmlspectest_SOURCES = cmlspectest.cpp
##cmlspectest_LDFLAGS = $(all_libraries)
##cmlspectest_LDADD = ../libscience.la
#METASOURCES = AUTO
