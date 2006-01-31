INCLUDES=  $(all_includes)

bin_PROGRAMS = games

METASOURCES = AUTO 

KDE_ICON = AUTO

# the library search path. 
games_LDFLAGS = $(all_libraries)

games_SOURCES = main.cpp \
				games.cpp \
				gamecontrols.ui \
				gamecontrols_impl.cpp \
				gamesdialog.cpp \
				gamefieldwidget.cpp \
				stone.cpp \
				field.cpp \
				gamesimplementation.cpp

EXTRA_DIST = gamecontrols.ui

games_LDADD   = $(LIB_KDEUI)
