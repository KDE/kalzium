## Makefile.am for eqchem

SUBDIRS = eqchemsolver

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

### OCAMLLIB is defined in configure.in.in
### if that does not work, uncomment the following lines :
# OCAMLLIB=`ocamlc -where`

# the library search path. 
libsolver_la_LDFLAGS = $(KDE_RPATH) $(all_libraries)

# the libraries to link against.
#
# OCAMLLIB is user-defined : see ../configure.in.in
# 
libsolver_la_LDADD   = $(LIB_KIO) $(LIB_KDEPRINT) \
	-L$(OCAMLLIB) -L$(OCAMLLIB)/facile \
	-lasmrun -lstr -lnums -lm  -ldl solver.o modwrap.o


# which sources should be compiled for eqchem
libsolverla_SOURCES = settings.kcfgc main.cpp eqchem.cpp eqresult.cpp \
	eqchemview.cpp prefs-base.ui prefs.cpp

xdg_apps_DATA = eqchem.desktop
kde_kcfg_DATA = eqchem.kcfg

# these are the headers for your project
noinst_HEADERS = eqchem.h eqchemview.h prefs.h

