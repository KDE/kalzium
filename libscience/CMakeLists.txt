
add_subdirectory(data)
add_subdirectory(tests)


########### next target ###############

set(science_LIB_SRCS
   element.cpp
   spectrum.cpp
   isotope.cpp
   spectrumparser.cpp
   elementparser.cpp
   isotopeparser.cpp
   tempunit.cpp
   chemicaldataobject.cpp
   moleculeparser.cpp
   parser.cpp
)

kde4_automoc(${science_LIB_SRCS})

kde4_add_library(science SHARED ${science_LIB_SRCS})

target_link_libraries(science  ${KDE4_KDEUI_LIBS} ${QT_QTXML_LIBRARY} )

set_target_properties(science PROPERTIES VERSION 4.0.0 SOVERSION 4 )
install_targets(${LIB_INSTALL_DIR} science )


########### install files ###############



#original Makefile.am contents follow:

#SUBDIRS = . data tests
#KDE_CXXFLAGS = -UQT3_SUPPORT
#
#INCLUDES= $(all_includes)
#
#lib_LTLIBRARIES = libscience.la
#
#libscience_la_SOURCES = \
#	element.cpp \
#	spectrum.cpp \
#	isotope.cpp \
#	spectrumparser.cpp \
#	elementparser.cpp \
#	isotopeparser.cpp \
#	tempunit.cpp \
#	chemicaldataobject.cpp \
#	moleculeparser.cpp \
#	parser.cpp
#
#libscience_la_LDFLAGS = $(all_libraries) -no-undefined -version-info 4:0:0
#libscience_la_LIBADD = $(LIB_KDEUI) 
#
#METASOURCES = AUTO
#
#DOXYGEN_SET_WARN_IF_UNDOCUMENTED = YES
#DOXYGEN_SET_INTERNAL_DOCS = YES
#
