kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )

add_subdirectory(bcc)
add_subdirectory(fcc)
add_subdirectory(rbg)
add_subdirectory(rpg)
add_subdirectory(simple-cubic)
add_subdirectory(tpg)
add_subdirectory(trg)


########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = $(AUTODIRS)
