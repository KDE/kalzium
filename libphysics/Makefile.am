SUBDIRS = .

INCLUDES= $(all_includes)

lib_LTLIBRARIES = libphysics.la

libphysics_la_SOURCES = element.cpp \
						spectrum.cpp \
						isotope.cpp \
						spectrumparser.cpp \
						tempunit.cpp

libphysics_la_LDFLAGS = $(all_libraries) -no-undefined -version-info 4:0:0
libphysics_la_LIBADD = $(LIB_KDEUI) 

METASOURCES = AUTO
