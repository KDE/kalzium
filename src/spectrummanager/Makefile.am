INCLUDES= $(all_includes)

bin_PROGRAMS = spectrummanager

spectrummanager_SOURCES = main.cpp \
						  spectrumeditor.cpp spectrummanager.cpp \
						  smprintdialogpage.cpp \
						  spectrumparser.cpp \
						  bandeditor.ui

spectrummanager_LDADD = $(LIB_KIO) $(LIB_KDEUI) $(LIB_KDEPRINT) ../spectrumwidget.o

EXTRA_DIST = main.cpp \
			 spectrumeditor.cpp spectrumeditor.h \
			 spectrummanager.cpp spectrummanager.h \
			 smprintdialogpage.cpp smprintdialogpage.h \
			 spectrumparser.cpp spectrumparser.h \
			 bandeditor.ui

METASOURCES = AUTO

rcuidir = $(kde_datadir)/kalzium
rcui_DATA = spectrummanagerui.rc

KDE_ICON = AUTO

spectrummanager_LDFLAGS = $(all_libraries) $(KDE_RPATH)
