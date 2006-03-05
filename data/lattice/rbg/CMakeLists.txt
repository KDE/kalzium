kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kalzium/data/lattice/rbg FILES 01.jpg 02.jpg 03.jpg 04.jpg 05.jpg 06.jpg 07.jpg 08.jpg 09.jpg 10.jpg 11.jpg 12.jpg 13.jpg 14.jpg 15.jpg 16.jpg 17.jpg )

kde4_footer()



#original Makefile.am contents follow:

#lattice_DATA = 01.jpg 02.jpg 03.jpg 04.jpg 05.jpg 06.jpg 07.jpg 08.jpg 09.jpg 10.jpg 11.jpg 12.jpg 13.jpg 14.jpg 15.jpg 16.jpg 17.jpg
#
#latticedir = $(kde_datadir)/kalzium/data/lattice/rbg
