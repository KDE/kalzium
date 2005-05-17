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
			plotwidget.cpp \
			elementdataviewer.cpp \
			plotsetupwidget.ui \
			isotopewidget.cpp \
			detailedgraphicaloverview.cpp \
			molcalcbase.ui \
			molcalc_impl.cpp \
			informationdialog.ui \
			informationdialog_impl.cpp
			
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
		detailedgraphicaloverview.cpp detailedgraphicaloverview.h \
		molcalc_impl.cpp molcalc_impl.h \
		informationdialog.ui \
		informationdialog_impl.cpp informationdialog_impl.h


METASOURCES = AUTO 

xdg_apps_DATA = kalzium.desktop
kde_kcfg_DATA = kalzium.kcfg

rcuidir = $(kde_datadir)/kalzium
rcui_DATA = kalziumui.rc

KDE_ICON = AUTO

# the library search path. 
kalzium_LDFLAGS = $(all_libraries) $(KDE_RPATH)

messages: rc.cpp
	extractstrings
	$(XGETTEXT) *.cpp -o $(podir)/kalzium.pot

#This is the code to extract the names of the elements
#from src/data/data.xml. The names are now at the end
#of kalzium.cpp
#grep name data/data.xml | sed s/\<name\>/i18n\(\"/ | sed s/\<\\/name\>/\"\)\;/ > i18n_temp.cpp


