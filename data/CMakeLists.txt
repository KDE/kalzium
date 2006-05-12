

add_subdirectory(elementpics)
add_subdirectory(htmlview)
add_subdirectory(icons)
add_subdirectory(iconsets)
add_subdirectory(lattice)
add_subdirectory(toolpics)



########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kalzium/data FILES bg.jpg knowledge.xml tools.xml )




#original Makefile.am contents follow:

#SUBDIRS = $(AUTODIRS)
#
#xml_DATA = bg.jpg data.xml knowledge.xml tools.xml
#
#xmldir = $(kde_datadir)/kalzium/data
#
