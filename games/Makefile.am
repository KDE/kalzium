# set the include path for X, qt and KDE
INCLUDES= -I$(srcdir)/../../libkdeedu/kdeeduplot -I$(srcdir)/../../libkdeedu/kdeeduui -I$(srcdir)/../../libkdeedu/libscience/ $(all_includes)

bin_PROGRAMS = games

KDE_CXXFLAGS    = -D_ISOC99_SOURCE

METASOURCES = AUTO 

KDE_ICON = AUTO

# the library search path. 
games_LDFLAGS = $(all_libraries) $(KDE_RPATH)

games_SOURCES = main.cpp \
				games.cpp \
				gamecontrols.ui \
				gamecontrols_impl.cpp \
				gamesdialog.cpp \
				gamefieldwidget.cpp \
				stone.cpp \
				field.cpp \
				gamesimplementation.cpp \
				statisticwidget.cpp

EXTRA_DIST = gamecontrols.ui

games_LDADD   = $(LIB_KDEUI)
