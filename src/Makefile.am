# set the include path for X, qt and KDE
INCLUDES= -I$(srcdir)/../../libkdeedu/kdeeduplot -I$(srcdir)/../../libkdeedu/kdeeduui -I$(srcdir)/../../libkdeedu/libscience/ $(all_includes)

bin_PROGRAMS = kalzium
kalzium_SOURCES = kalzium.cpp \
			main.cpp \
			periodictableview.cpp \
			prefs.kcfgc \
			settings_colors.ui \
			settings_misc.ui \
			settings_units.ui \
			detailinfodlg.cpp \
			orbitswidget.cpp \
			plotwidget.cpp \
			elementdataviewer.cpp \
			plotsetupwidget.ui \
			detailedgraphicaloverview.cpp \
			molcalcwidgetbase.ui \
			molcalcwidget.cpp \
			somwidget.ui \
			somwidget_impl.cpp \
			kalziumtip.cpp \
			kalziumdataobject.cpp \
			spectrumwidget.cpp \
			nuclideboard.cpp \
			exporter.cpp \
			spectrumview.ui \
			spectrumviewimpl.cpp \
			kalziumutils.cpp \
			animatedmoviewidget.cpp \
			simplecrystalviewer.cpp \
			kalziumgradienttype.cpp \
			kalziumnumerationtype.cpp \
			kalziumpainter.cpp \
			kalziumschemetype.cpp \
			kalziumtabletype.cpp \
			propertysliderwidget.cpp \
			printwidget.cpp \
			printwidgetbase.ui \
			propertysliderwidgetbase.ui \

kalzium_LDADD   = ../../libkdeedu/libscience/libscience.la \
				  ../../libkdeedu/kdeeduplot/libkdeeduplot.la \
				  ../../libkdeedu/kdeeduui/libkdeeduui.la \
				  $(LIB_KIO) $(LIB_KDEUI) $(LIB_KHTML)
KDE_CXXFLAGS    = -D_ISOC99_SOURCE

#SUBDIRS = solver spectrummanager

EXTRA_DIST = main.cpp \
		kalzium.cpp kalzium.h \
		periodictableview.cpp periodictableview.h \
		settings_colors.ui \
		settings_misc.ui \
		detailinfodlg.cpp detailinfodlg.h \
		orbitswidget.cpp orbitswidget.h \
		plotwidget.cpp plotwidget.h \
		elementdataviewer.cpp elementdataviewer.h \
		detailedgraphicaloverview.cpp detailedgraphicaloverview.h \
		molcalcwidget.cpp molcalcwidget.h \
		somwidget.ui \
		propertysliderwidgetbase.ui \
		somwidget_impl.cpp somwidget_impl.h \
		kalziumdataobject.cpp kalziumdataobject.h \
		spectrumwidget.cpp spectrumwidget.h \
		nuclideboard.cpp nuclideboar.h \
		exporter.cpp exporter.h \
		spectrumview.ui \
		spectrumviewimpl.cpp spectrumviewimpl.h	\
		kalziumutils.cpp kalziumutils.h \
		animatedmoviewidget.cpp animatedmoviewidget.h \
		simplecrystalviewer.cpp simplecrystalviewer.h \
		kalziumgradienttype.cpp kalziumgradienttype.h \
		kalziumnumerationtype.cpp kalziumnumerationtype.h \
		kalziumpainter.cpp kalziumpainter.h \
		kalziumschemetype.cpp kalziumschemetype.h \
		kalziumtabletype.cpp kalziumtabletype.h \
		propertysliderwidget.cpp propertysliderwidget.h \
		printwidgetbase.ui \
		printwidget.cpp printwidget.h

METASOURCES = AUTO 

xdg_apps_DATA = kalzium.desktop
kde_kcfg_DATA = kalzium.kcfg

rcuidir = $(kde_datadir)/kalzium
rcui_DATA = kalziumui.rc

KDE_ICON = AUTO

# the library search path. 
kalzium_LDFLAGS = $(all_libraries) $(KDE_RPATH)

#this RegExp-monster grep for <name origin=foo">bar</name> and puts the i18n() around foo and bar
messages-old: rc.cpp
	cat data/knowledge.xml | grep name| sed -e 's/< *name *>\([^<]*\)<\/ *name *>/I18N_NOOP(\"\1\");/' >> rc.cpp
	cat data/knowledge.xml | grep desc | sed -e 's/< *desc *>\([^<]*\)<\/ *desc *>/I18N_NOOP(\"\1\");/' >> rc.cpp
	cat data/knowledge.xml | grep refitem | sed -e 's/< *refitem *>\([^<]*\)<\/ *refitem *>/I18N_NOOP(\"\1\");/' >> rc.cpp
	cat data/tools.xml | grep name | sed -e 's/< *name *>\([^<]*\)<\/ *name *>/I18N_NOOP(\"\1\");/' >> rc.cpp
	cat data/tools.xml | grep desc | sed -e 's/< *desc *>\([^<]*\)<\/ *desc *>/I18N_NOOP(\"\1\");/' >> rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kalzium.pot
	rm -f rc.cpp # for testing

