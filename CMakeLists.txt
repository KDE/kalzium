kde4_header()

project(kalzium)

add_subdirectory( src ) 
add_subdirectory( data ) 

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############


kde4_create_doxygen_docs( REFERENCES kdeui kdecore kfile libkdeedu  )

kde4_footer()



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
