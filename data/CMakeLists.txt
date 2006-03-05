kde4_header()

message(STATUS "${CMAKE_CURRENT_SOURCE_DIR}: skipped subdir $(AUTODIRS)")
include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kalzium/data FILES bg.jpg data.xml knowledge.xml tools.xml )

kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = $(AUTODIRS)
#
#xml_DATA = bg.jpg data.xml knowledge.xml tools.xml
#
#xmldir = $(kde_datadir)/kalzium/data
#
