SUBDIRS = .

INCLUDES= $(all_includes)

lib_LTLIBRARIES = libscience.la

libscience_la_SOURCES = \
	element.cpp \
	spectrum.cpp \
	isotope.cpp \
	spectrumparser.cpp \
	tempunit.cpp

libscience_la_LDFLAGS = $(all_libraries) -no-undefined -version-info 4:0:0
libscience_la_LIBADD = $(LIB_KDEUI) 

METASOURCES = AUTO
