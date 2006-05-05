kde4_header()

include_directories(
   ${CMAKE_SOURCE_DIR}/libkdeedu/kdeeduplot/
   ${CMAKE_SOURCE_DIR}/libkdeedu/libscience/
   ${CMAKE_SOURCE_DIR}/libkdeedu/kdeeduui/
   ${KDE4_INCLUDE_DIR}
   ${QT_INCLUDES}
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_BINARY_DIR}
)

add_definitions(-D_ISOC99_SOURCE)

########### next target ###############

set(kalzium_SRCS
   kalzium.cpp
   main.cpp
   periodictableview.cpp
   detailinfodlg.cpp
   orbitswidget.cpp
   plotwidget.cpp
   elementdataviewer.cpp
   detailedgraphicaloverview.cpp
   molcalcwidget.cpp
   somwidget_impl.cpp
   kalziumdataobject.cpp
   spectrumwidget.cpp
   nuclideboard.cpp
   exporter.cpp
   spectrumviewimpl.cpp
   kalziumutils.cpp
   animatedmoviewidget.cpp
   simplecrystalviewer.cpp
   kalziumgradienttype.cpp
   kalziumnumerationtype.cpp
   kalziumpainter.cpp
   kalziumschemetype.cpp
   kalziumtabletype.cpp
   propertysliderwidget.cpp
   printwidget.cpp
)

kde4_automoc(${kalzium_SRCS})

kde4_add_ui_files(kalzium_SRCS
   settings_colors.ui
   settings_elementsymbol.ui
   plotsetupwidget.ui
   molcalcwidgetbase.ui
   somwidget.ui
   spectrumview.ui
   printwidgetbase.ui
   propertysliderwidgetbase.ui
   settings_misc.ui
   settings_units.ui
)

kde4_add_kcfg_files(kalzium_SRCS prefs.kcfgc )

kde4_add_executable(kalzium ${kalzium_SRCS})

target_link_libraries(kalzium ${KDE4_KDEUI_LIBS} ${KDE4_KHTML_LIBRARY} ${KDE4_KUTILS_LIBRARY} science kdeeduplot kdeeduui)

install_targets(/bin kalzium )


########### install files ###############

install_files( ${XDG_APPS_DIR} FILES kalzium.desktop )
install_files( ${KCFG_INSTALL_DIR} FILES kalzium.kcfg )
install_files( ${DATA_INSTALL_DIR}/kalzium FILES kalziumui.rc )

kde4_install_icons( ${ICON_INSTALL_DIR}  hicolor )

kde4_footer()



#original Makefile.am contents follow:

## set the include path for X, qt and KDE
#INCLUDES= -I$(srcdir)/../../libkdeedu/kdeeduplot -I$(srcdir)/../../libkdeedu/kdeeduui -I$(srcdir)/../../libkdeedu/libscience/ $(all_includes)
#
#bin_PROGRAMS = kalzium
#
#kalzium_SOURCES = kalzium.cpp \
#			main.cpp \
#			periodictableview.cpp \
#			prefs.kcfgc \
#			settings_colors.ui \
#			settings_misc.ui \
#			settings_units.ui \
#			detailinfodlg.cpp \
#			orbitswidget.cpp \
#			plotwidget.cpp \
#			elementdataviewer.cpp \
#			plotsetupwidget.ui \
#			detailedgraphicaloverview.cpp \
#			molcalcwidgetbase.ui \
#			molcalcwidget.cpp \
#			somwidget.ui \
#			somwidget_impl.cpp \
#			kalziumdataobject.cpp \
#			spectrumwidget.cpp \
#			nuclideboard.cpp \
#			exporter.cpp \
#			spectrumview.ui \
#			spectrumviewimpl.cpp \
#			kalziumutils.cpp \
#			animatedmoviewidget.cpp \
#			simplecrystalviewer.cpp \
#			kalziumgradienttype.cpp \
#			kalziumnumerationtype.cpp \
#			kalziumpainter.cpp \
#			kalziumschemetype.cpp \
#			kalziumtabletype.cpp \
#			propertysliderwidget.cpp \
#			printwidget.cpp \
#			printwidgetbase.ui \
#			propertysliderwidgetbase.ui
#
#kalzium_LDADD   = ../../libkdeedu/libscience/libscience.la \
#				  ../../libkdeedu/kdeeduplot/libkdeeduplot.la \
#				  ../../libkdeedu/kdeeduui/libkdeeduui.la \
#				  $(LIB_KIO) $(LIB_KDEUI) $(LIB_KHTML)
#KDE_CXXFLAGS    = -D_ISOC99_SOURCE
#
#EXTRA_DIST = main.cpp \
#		kalzium.cpp kalzium.h \
#		periodictableview.cpp periodictableview.h \
#		settings_colors.ui \
#		settings_misc.ui \
#		detailinfodlg.cpp detailinfodlg.h \
#		orbitswidget.cpp orbitswidget.h \
#		plotwidget.cpp plotwidget.h \
#		elementdataviewer.cpp elementdataviewer.h \
#		detailedgraphicaloverview.cpp detailedgraphicaloverview.h \
#		molcalcwidget.cpp molcalcwidget.h \
#		somwidget.ui \
#		propertysliderwidgetbase.ui \
#		somwidget_impl.cpp somwidget_impl.h \
#		kalziumdataobject.cpp kalziumdataobject.h \
#		spectrumwidget.cpp spectrumwidget.h \
#		nuclideboard.cpp nuclideboar.h \
#		exporter.cpp exporter.h \
#		spectrumview.ui \
#		spectrumviewimpl.cpp spectrumviewimpl.h	\
#		kalziumutils.cpp kalziumutils.h \
#		animatedmoviewidget.cpp animatedmoviewidget.h \
#		simplecrystalviewer.cpp simplecrystalviewer.h \
#		kalziumgradienttype.cpp kalziumgradienttype.h \
#		kalziumnumerationtype.cpp kalziumnumerationtype.h \
#		kalziumpainter.cpp kalziumpainter.h \
#		kalziumschemetype.cpp kalziumschemetype.h \
#		kalziumtabletype.cpp kalziumtabletype.h \
#		propertysliderwidget.cpp propertysliderwidget.h \
#		printwidgetbase.ui \
#		printwidget.cpp printwidget.h
#
#METASOURCES = AUTO 
#
#xdg_apps_DATA = kalzium.desktop
#kde_kcfg_DATA = kalzium.kcfg
#
#rcuidir = $(kde_datadir)/kalzium
#rcui_DATA = kalziumui.rc
#
#KDE_ICON = AUTO
#
## the library search path. 
#kalzium_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#
##this RegExp-monster grep for <name origin=foo">bar</name> and puts the i18n() around foo and bar
#messages-old: rc.cpp
#	cat data/knowledge.xml | grep name| sed -e 's/< *name *>\([^<]*\)<\/ *name *>/I18N_NOOP(\"\1\");/' >> rc.cpp
#	cat data/knowledge.xml | grep desc | sed -e 's/< *desc *>\([^<]*\)<\/ *desc *>/I18N_NOOP(\"\1\");/' >> rc.cpp
#	cat data/knowledge.xml | grep refitem | sed -e 's/< *refitem *>\([^<]*\)<\/ *refitem *>/I18N_NOOP(\"\1\");/' >> rc.cpp
#	cat data/tools.xml | grep name | sed -e 's/< *name *>\([^<]*\)<\/ *name *>/I18N_NOOP(\"\1\");/' >> rc.cpp
#	cat data/tools.xml | grep desc | sed -e 's/< *desc *>\([^<]*\)<\/ *desc *>/I18N_NOOP(\"\1\");/' >> rc.cpp
#	$(XGETTEXT) *.cpp -o $(podir)/kalzium.pot
#	rm -f rc.cpp # for testing
#
