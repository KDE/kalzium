## Makefile.am for eqchem

SUBDIRS = eqchemsolver


# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = eqchem

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

### OCAMLLIB is defined in configure.in.in
### if that does not work, uncomment the following lines :
#
# OCAMLLIB=`ocamlc -where`

# the library search path. 
eqchem_LDFLAGS = $(KDE_RPATH) $(all_libraries)

# the libraries to link against.
#
# OCAMLLIB is user-defined : see ../configure.in.in
# 
eqchem_LDADD   = $(LIB_KFILE) $(LIB_KDEPRINT) \
	-L$(OCAMLLIB) -L$(OCAMLLIB)/facile \
	-lasmrun -lstr -lnums -lm  -ldl solver.o modwrap.o


# which sources should be compiled for eqchem
eqchem_SOURCES = settings.kcfgc main.cpp eqchem.cpp eqresult.cpp \
	eqchemview.cpp prefs-base.ui prefs.cpp

xdg_apps_DATA = eqchem.desktop
kde_kcfg_DATA = eqchem.kcfg

# these are the headers for your project
noinst_HEADERS = eqchem.h eqchemview.h prefs.h

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/eqchem.pot

KDE_ICON = eqchem

# this is where the kdelnk file will go 
kdelnkdir   = $(kde_appsdir)/Utilities
kdelnk_DATA = eqchem.desktop

# this is where the XML-GUI resource file goes
rcdir = $(kde_datadir)/eqchem
rc_DATA = eqchemui.rc



