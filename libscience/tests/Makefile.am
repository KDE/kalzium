INCLUDES = -I$(top_srcdir)/libkdeedu/libscience $(all_includes)

AM_LDFLAGS = $(QT_LDFLAGS) $(X_LDFLAGS) $(KDE_RPATH)

check_PROGRAMS = xmlreadingtest

xmlreadingtest_SOURCES = xmlreadingtest.cpp
xmlreadingtest_LDFLAGS = $(all_libraries)
xmlreadingtest_LDADD = ../libscience.la

METASOURCES = AUTO
