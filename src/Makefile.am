# set the include path for X, qt and KDE
INCLUDES= $(all_includes) 

bin_PROGRAMS = kalzium
kalzium_SOURCES = kalzium.cpp main.cpp pse.cpp element.cpp elementbutton.cpp prefs.kcfgc settings_colorschemes.ui settings_colors.ui
kalzium_LDADD   = $(LIB_KDEUI)

SUBDIRS = icons elementpics

EXTRA_DIST = main.cpp kalzium.cpp kalzium.h pse.cpp pse.h element.cpp element.h elementbutton.cpp elementbutton.h settings_colorschemes.ui settings_colors.ui

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
	  echo "^Q=" >> keys
	  echo "^A=" >> keys
	  echo "^Alternative1=" >> keys
	  echo "^Alternative2=" >> keys
	  grep -f keys kalziumrc | sed -e 's#Name=#N=#' | sed -e 's#Alternative1=#1=#' | sed -e 's#Alternative2=#2=#' | sed -e 's#^[NQA12]=\(.*\)#i18n(\"\1\"); // xgettext:no-c-format#' >> kalzium_i18n.cpp
	  echo "#endif" >> kalzium_i18n.cpp
	  rm -f keys

messages: elementnames rc.cpp
	$(XGETTEXT) *.cpp ../calculations/*.cpp -o $(podir)/kalzium.pot

