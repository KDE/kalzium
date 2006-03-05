kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kalzium/data/htmlview FILES characteristics.png density.png header.png icons.svg radius.png structure.png style.css mass.png book.png boilingpoint.png meltingpoint.png ionisation.png electronaffinity.png discovery.png abundance.png )

kde4_footer()



#original Makefile.am contents follow:

#appdatadir = $(kde_datadir)/kalzium/data/htmlview
#appdata_DATA = characteristics.png density.png header.png icons.svg radius.png structure.png style.css mass.png book.png boilingpoint.png meltingpoint.png ionisation.png electronaffinity.png discovery.png abundance.png
#
