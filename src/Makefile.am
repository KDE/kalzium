# set the include path for X, qt and KDE
INCLUDES= -I../calculations $(all_includes) 

SUBDIRS = icons

bin_PROGRAMS = kalzium
kalzium_SOURCES = eleminfo.cpp elementkp.cpp kalzium.cpp main.cpp quizdlg.cpp stateofmatterdlg.cpp calcdlg.cpp calcdialog.cpp settingsdialog.cpp kalziumconverter.cpp legend.cpp fastinfo.cpp KalziumGraph.cpp infodlg.ui quizdlgui.ui quiz_result.ui kmolui.ui
kalzium_LDADD   = $(LIB_KHTML)  ../calculations/libkmol.la


EXTRA_DIST = main.cpp kalzium.cpp kalzium.h kalzium.desktop elementkp.cpp elementkp.h eleminfo.cpp eleminfo.h kalziumrc TODO ChangeLog quizdlg.h quizdlg.cpp stateofmatterdlg.cpp tempdlg.h calcdlg.cpp calcdlg.h calcdialog.cpp calcdialog.cpp settingsdialog.cpp settingsdialog.h kalziumconverter.h kalziumconverter.cpp legend.cpp legend.h fastinfo.cpp fastinfo.h KalziumGraph.cpp KalziumGraph.h infodlg.ui quizdlgui.ui quiz_result.ui kmolui.ui

METASOURCES = AUTO 

appsdir = $(kde_appsdir)/Edutainment/Science
apps_DATA = kalzium.desktop

rcdir = $(kde_datadir)/kalzium
rc_DATA = kalziumrc kmolweights

rcuidir = $(kde_datadir)/kalzium
rcui_DATA = kalziumui.rc

KDE_ICON = kalzium


# the library search path. 
kalzium_LDFLAGS = $(all_libraries) $(KDE_RPATH)


elementnames:
	  rm -f kalzium_i18n.cpp; \
	  echo "#if 0" >> kalzium_i18n.cpp ;\
          grep 'Name=' kalziumrc | sed -e 's#^Name=\(.*\)#i18n(\"\1\")#' >> kalzium_i18n.cpp ;\
          grep 'Q=' kalziumrc | sed -e 's#^Q=\(.*\)#i18n(\"\1\")#' >> kalzium_i18n.cpp ;\
          grep 'A=' kalziumrc | sed -e 's#^A=\(.*\)#i18n(\"\1\")#' >> kalzium_i18n.cpp ;\
          grep 'Alternative1=' kalziumrc | sed -e 's#^Alternative1=\(.*\)#i18n(\"\1\")#' >> kalzium_i18n.cpp ;\
          grep 'Alternative2=' kalziumrc | sed -e 's#^Alternative2=\(.*\)#i18n(\"\1\")#' >> kalzium_i18n.cpp ;\
	  echo "#endif" >> kalzium_i18n.cpp

messages: elementnames rc.cpp
	$(XGETTEXT) *.cpp ../calculations/*.cpp -o $(podir)/kalzium.pot

