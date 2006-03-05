kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kalzium/data/iconsets/school FILES 1.jpg 2.jpg 3.jpg )

kde4_footer()



#original Makefile.am contents follow:

#school_DATA = 1.jpg 2.jpg 3.jpg
#
#schooldir = $(kde_datadir)/kalzium/data/iconsets/school
