# set the include path for X, qt and KDE
INCLUDES= -I$(srcdir)/../../libkdeedu/kdeeduplot -I$(srcdir)/../../libkdeedu/kdeeduui $(all_includes)

bin_PROGRAMS = kalzium
kalzium_SOURCES = eqresult.cpp eqchemview.cpp kalzium.cpp \
			main.cpp \
			periodictableview.cpp \
			element.cpp \
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
			timewidget.ui \
			somwidget.ui \
			somwidget_impl.cpp \
			kalziumtip.cpp \
			spectrum.cpp \
			isotope.cpp \
			kalziumdataobject.cpp \
			spectrumwidget.cpp \
			nuclideboard.cpp \
			exporter.cpp \
			elementbox.cpp \
			spectrumeditor.cpp \
			bandeditor.ui spectrumview.ui \
			spectrumviewimpl.cpp
			
			
kalzium_LDADD   = ../../libkdeedu/kdeeduplot/libkdeeduplot.la ../../libkdeedu/kdeeduui/libkdeeduui.la $(LIB_KIO) $(LIB_KDEUI) $(LIB_KHTML) -L$(OCAMLLIB) -L$(FACILELIB) $(EQCHEM_LDADD)

SUBDIRS = icons elementpics data $(EQCHEM_SUB)

EXTRA_DIST = main.cpp \
		kalzium.cpp kalzium.h \
		periodictableview.cpp periodictableview.h \
		element.cpp element.h \
		settings_colors.ui \
		settings_misc.ui \
		detailinfodlg.cpp detailinfodlg.h \
		orbitswidget.cpp orbitswidget.h \
		plotwidget.cpp plotwidget.h \
		elementdataviewer.cpp elementdataviewer.h \
		detailedgraphicaloverview.cpp detailedgraphicaloverview.h \
		molcalcwidget.cpp molcalcwidget.h \
		timewidget.ui \
		somwidget.ui \
		somwidget_impl.cpp somwidget_impl.h \
		spectrum.cpp spectrum.h \
		isotope.cpp isotope.h \
		kalziumdataobject.cpp kalziumdataobject.h \
		spectrumwidget.cpp spectrumwidget.h \
		nuclideboard.cpp nuclideboar.h \
		exporter.cpp exporter.h \
		elementbox.cpp elementbox.h \
		spectrumeditor.cpp spectrumeditor.h \
		bandeditor.ui spectrumview.ui \
		spectrumviewimpl.cpp spectrumviewimpl.h	
		

METASOURCES = AUTO 

xdg_apps_DATA = kalzium.desktop
kde_kcfg_DATA = kalzium.kcfg

rcuidir = $(kde_datadir)/kalzium
rcui_DATA = kalziumui.rc

KDE_ICON = AUTO

# the library search path. 
kalzium_LDFLAGS = $(all_libraries) $(KDE_RPATH)

#this RegExp-monster grep for <name origin=foo">bar</name> and puts the i18n() around foo and bar
messages: rc.cpp
	cat data/data.xml | grep name| sed -e 's/< *name *origin *= *"\([^\"]*\)\">\([^<]*\)<\/ *name *>/I18N_NOOP(\"\1\"); I18N_NOOP(\"\2\");/;s/I18N_NOOP("");//g' | grep I18N_NOOP >> rc.cpp
	cat data/knowledge.xml | grep name| sed -e 's/< *name *>\([^<]*\)<\/ *name *>/I18N_NOOP(\"\1\");/' >> rc.cpp
	cat data/knowledge.xml | grep desc | sed -e 's/< *desc *>\([^<]*\)<\/ *desc *>/I18N_NOOP(\"\1\");/' >> rc.cpp
	cat data/knowledge.xml | grep refitem | sed -e 's/< *refitem *>\([^<]*\)<\/ *refitem *>/I18N_NOOP(\"\1\");/' >> rc.cpp
	cat data/tools.xml | grep name | sed -e 's/< *name *>\([^<]*\)<\/ *name *>/I18N_NOOP(\"\1\");/' >> rc.cpp
	cat data/tools.xml | grep desc | sed -e 's/< *desc *>\([^<]*\)<\/ *desc *>/I18N_NOOP(\"\1\");/' >> rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kalzium.pot
	rm -f rc.cpp # for testing
