# set the include path for X, qt and KDE
INCLUDES= -I$(top_srcdir)/libkdeedu/kdeeduplot $(all_includes)

bin_PROGRAMS = kalzium
kalzium_SOURCES = kalzium.cpp \
			main.cpp \
			pse.cpp \
			element.cpp \
			prefs.kcfgc \
			settings_colors.ui \
			settings_misc.ui \
			infodialog_small.ui \
			infodialog_small_impl.cpp \
			detailinfodlg.cpp \
			orbitswidget.cpp \
			detail_chemical.ui \
			detail_energy.ui \
			slider_widget.ui \
			plotwidget.cpp \
			elementdataviewer.cpp \
			plotsetupwidget.ui \
			tempslider.ui \
			isotopewidget.cpp \
			overviewwidget.cpp \
			detailedgraphicaloverview.cpp \
			molcalcbase.ui \
			molcalc_impl.cpp \
			overviewbase.ui
			
kalzium_LDADD   = $(LIB_KIO) $(LIB_KDEUI)\
		  $(top_builddir)/libkdeedu/kdeeduplot/libkdeeduplot.la
		  
SUBDIRS = icons elementpics data

EXTRA_DIST = main.cpp \
		kalzium.cpp kalzium.h \
		pse.cpp pse.h \
		element.cpp element.h \
		settings_colors.ui \
		settings_misc.ui \
		infodialog_small.ui \
		infodialog_small_impl.cpp \
		infodialog_small_impl.h \
		detailinfodlg.cpp detailinfodlg.h \
		orbitswidget.cpp orbitswidget.h \
		detail_chemical.ui \
		detail_energy.ui \
		plotwidget.cpp plotwidget.h \
		elementdataviewer.cpp elementdataviewer.h \
		isotopewidget.h isotopewidget.cpp \
		overviewwidget.cpp overviewwidget.h \
		detailedgraphicaloverview.cpp detailedgraphicaloverview.h \
		molcalc_impl.cpp molcalc_impl.h


METASOURCES = AUTO 

xdg_apps_DATA = kalzium.desktop
kde_kcfg_DATA = kalzium.kcfg

rcdir = $(kde_datadir)/kalzium
rc_DATA = kalziumrc 

rcuidir = $(kde_datadir)/kalzium
rcui_DATA = kalziumui.rc

KDE_ICON = AUTO

# the library search path. 
kalzium_LDFLAGS = $(all_libraries) $(KDE_RPATH)

elementnames:
	  rm -f kalzium_i18n.cpp
	  echo "#if 0" >> kalzium_i18n.cpp
	  echo "^Name=" > keys
	  grep -f keys kalziumrc | sed -e 's#Name=#N=#' | sed -e 's#^[NQA12]=\(.*\)#i18n(\"\1\"); // xgettext:no-c-format#'  >> kalzium_i18n.cpp
	  rm -f keys
	  echo "#endif" >> kalzium_i18n.cpp

messages: elementnames rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kalzium.pot

