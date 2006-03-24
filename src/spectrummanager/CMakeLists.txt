kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############

set(spectrummanager_SRCS 
   main.cpp 
   spectrumeditor.cpp 
   spectrummanager.cpp 
   smprintdialogpage.cpp )

kde4_automoc(${spectrummanager_SRCS})

kde4_add_ui_files(spectrummanager_SRCS bandeditor.ui )

kde4_add_executable(spectrummanager ${spectrummanager_SRCS})

target_link_libraries(spectrummanager  ${KDE4_KDEUI_LIBS} kdeprint science )

install_targets(/bin spectrummanager )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kalzium FILES spectrummanagerui.rc )

kde4_install_icons( ${ICON_INSTALL_DIR}  hicolor )

kde4_footer()



#original Makefile.am contents follow:

#INCLUDES= -I$(srcdir)/../../../libkdeedu/libscience/ $(all_includes)
#
#bin_PROGRAMS = spectrummanager
#
#spectrummanager_SOURCES = main.cpp \
#						  spectrumeditor.cpp spectrummanager.cpp \
#						  smprintdialogpage.cpp \
#						  bandeditor.ui
#
#spectrummanager_LDADD = $(LIB_KIO) $(LIB_KDEUI) $(LIB_KDEPRINT) ../spectrumwidget.o ../../../libkdeedu/libscience/libscience.la
#
#EXTRA_DIST = main.cpp \
#			 spectrumeditor.cpp spectrumeditor.h \
#			 spectrummanager.cpp spectrummanager.h \
#			 smprintdialogpage.cpp smprintdialogpage.h \
#			 bandeditor.ui
#
#METASOURCES = AUTO
#
#rcuidir = $(kde_datadir)/kalzium
#rcui_DATA = spectrummanagerui.rc
#
#KDE_ICON = AUTO
#
#spectrummanager_LDFLAGS = $(all_libraries) $(KDE_RPATH)
