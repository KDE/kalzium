# set the include path for X, qt and KDE
INCLUDES= $(all_includes) 

bin_PROGRAMS = kalzium
kalzium_SOURCES = kalzium.cpp \
			main.cpp \
			pse.cpp \
			element.cpp \
			elementbutton.cpp \
			prefs.kcfgc \
			settings_colorschemes.ui \
			settings_colors.ui \
			settings_quiz.ui \
			settings_misc.ui \
			questionadddialog.ui \
			questionadddialog_impl.cpp \
			questioneditor.ui \
			questioneditor_impl.cpp \
			infodialog_small.ui \
			infodialog_small_impl.cpp \
			detailinfodlg.cpp \
			orbitswidget.cpp \
			detail_chemical.ui \
			detail_energy.ui \
			slider_widget.ui \
			quiz.cpp
kalzium_LDADD   = $(LIB_KDEUI)

SUBDIRS = icons elementpics

EXTRA_DIST = main.cpp \
		kalzium.cpp kalzium.h \
		pse.cpp pse.h \
		element.cpp element.h \
		elementbutton.cpp elementbutton.h \
		settings_colorschemes.ui \
		settings_colors.ui \
		settings_quiz.ui \
		settings_misc.ui \
		questionadddialog.ui \
		questionadddialog_impl.cpp \
		questionadddialog_impl.h \
		questioneditor.ui \
		questioneditor_impl.cpp \
		questioneditor_impl.h \
		infodialog_small.ui \
		infodialog_small_impl.cpp \
		infodialog_small_impl.h \
		detailinfodlg.cpp \
		detailinfodlg.h \
		orbitswidget.cpp \
		orbitswidget.h \
		detail_chemical.ui \
		detail_energy.ui \
		quiz.cpp \
		quiz.h

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

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kalzium.pot

