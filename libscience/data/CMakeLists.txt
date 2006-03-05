kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/libkdeedu/data/ FILES elements.xml isotopes.xml )

kde4_footer()



#original Makefile.am contents follow:

#xml_DATA = elements.xml isotopes.xml
#
#xmldir = $(kde_datadir)/libkdeedu/data/
