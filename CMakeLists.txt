project(kalzium)

include(MacroBoolTo01)
macro_bool_to_01(OPENBABEL2_FOUND HAVE_OPENBABEL2)
macro_bool_to_1(OPENGL_FOUND HAVE_OPENGL)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config-kalzium.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kalzium.h )



add_subdirectory(src)
add_subdirectory(data)
add_subdirectory(simulations)


#original Makefile.am contents follow:

#SUBDIRS = src data
#
#AUTOMAKE_OPTIONS = foreign 
#
#DOXYGEN_REFERENCES = kdeui kdecore kfile libkdeedu
#include ../admin/Doxyfile.am
#
#messages: rc.cpp
#	$(EXTRACTRC) src/*.ui src/*.rc src/*.kcfg >> rc.cpp
#	$(EXTRACTATTR) --attr=name,origin data/data.xml >> rc.cpp
#	$(EXTRACTRC) --tag-group=none --tag=name data/data.xml >> rc.cpp
#	$(EXTRACTRC) --tag-group=none --tag=name --tag=desc --tag=refitem data/knowledge.xml >> rc.cpp
#	$(EXTRACTRC) --tag-group=none --tag=name --tag=desc data/tools.xml >> rc.cpp
#	$(XGETTEXT) src/*.cpp -o $(podir)/kalzium.pot
