# set the include path for X, qt and KDE
INCLUDES= -I../calculations $(all_includes) 

SUBIRS = calculations

bin_PROGRAMS = kalzium
kalzium_SOURCES = eleminfo.cpp elementkp.cpp kalzium.cpp main.cpp quizdlg.cpp stateofmatterdlg.cpp calcdlg.cpp calcdialog.cpp value_visualisation.cpp settingsdialog.cpp

kalzium_LDADD   =  $(LIB_KDEUI) $(LIB_KDECORE) $(LIB_KHTML) $(LIB_QT) $(LIBSOCKET) ../calculations/libkmol.la


EXTRA_DIST = main.cpp kalzium.cpp kalzium.h kalzium.desktop elementkp.cpp elementkp.h eleminfo.cpp eleminfo.h kalziumrc TODO ChangeLog quizdlg.h quizdlg.cpp stateofmatterdlg.cpp tempdlg.h calcdlg.cpp calcdlg.h calcdialog.cpp calcdialog.cpp value_visualisation.cpp value_visualisation.h settingsdialog.cpp settingsdialog.h

	$(mkinstalldirs) $(kde_appsdir)/Edutainment/
	$(INSTALL_DATA) $(srcdir)/kalzium.desktop $(kde_appsdir)/Edutainment/kalzium.desktop



METASOURCES = AUTO

appsdir = $(kde_appsdir)/Edutainment
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
          egrep 'Name=' kalziumrc | sed -e 's#^Name=\(.*\)#i18n(\"\1\")#' >> kalzium_i18n.cpp ;\
          egrep 'Q=' kalziumrc | sed -e 's#^Q=\(.*\)#i18n(\"\1\")#' >> kalzium_i18n.cpp ;\
          egrep 'A=' kalziumrc | sed -e 's#^A=\(.*\)#i18n(\"\1\")#' >> kalzium_i18n.cpp ;\
          egrep 'Alternative1=' kalziumrc | sed -e 's#^Alternative1=\(.*\)#i18n(\"\1\")#' >> kalzium_i18n.cpp ;\
          egrep 'Alternative2=' kalziumrc | sed -e 's#^Alternative2=\(.*\)#i18n(\"\1\")#' >> kalzium_i18n.cpp ;\
	  echo "#endif" >> kalzium_i18n.p
messages: rc.cpp
    LIST=`find . -name \*.h -o -name \*.hh -o -name \*.H -o -name \*.hxx -o -name \*.hpp -o -name \*.cpp -o -name \*.cc -o -name \*.cxx -o -name \*.ecpp -o -name \*.C`; \
	if test -n "$$LIST"; then \
	 $(XGETTEXT) $$LIST -o $(podir)/kalzium.pot; \
	fi

