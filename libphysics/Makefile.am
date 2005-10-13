SUBDIRS = .

INCLUDES= $(all_includes)

lib_LTLIBRARIES = libphysics.la

libextdate_la_SOURCES = element.cpp \
						spectrum.cpp \
						isotope.cpp \
						spectrumparser.cpp

EXTRA_DIST = element.cpp element.h \
			 spectrum.cpp spectrum.h \
			 isotope.cpp isotope.h \
			 spectrumparser.cpp spectrumparser.h

libextdate_la_LDFLAGS = $(all_libraries) -no-undefined -version-info 4:0:0
libextdate_la_LIBADD = $(LIB_KDEUI) 

METASOURCES = AUTO
