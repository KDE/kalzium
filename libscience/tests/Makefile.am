INCLUDES = -I$(top_srcdir)/libkdeedu/libscience $(all_includes)

AM_LDFLAGS = $(QT_LDFLAGS) $(X_LDFLAGS) $(KDE_RPATH)

check_PROGRAMS = xmlreadingtest isotopereadingtest cmlspectest

xmlreadingtest_SOURCES = xmlreadingtest.cpp
xmlreadingtest_LDFLAGS = $(all_libraries)
xmlreadingtest_LDADD = ../libscience.la

isotopereadingtest_SOURCES = isotopereadingtest.cpp
isotopereadingtest_LDFLAGS = $(all_libraries)
isotopereadingtest_LDADD = ../libscience.la

cmlspectest_SOURCES = cmlspectest.cpp
cmlspectest_LDFLAGS = $(all_libraries)
cmlspectest_LDADD = ../libscience.la
METASOURCES = AUTO
