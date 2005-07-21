## Makefile.am for eqchem

SUBDIRS = eqchemsolver

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

# TODO: someone write the kalzium/configure.in.in please
OCAMLLIB=`ocamlc -where`

lib_LTLIBRARIES = libsolver.la

libsolver_la_SOURCES = eqresult.cpp eqchemview.cpp

#libsolverincludedir = $(all_includes) 
#libsolverinclude_HEADERS = kdeeduglossary.h

libsolver_la_LDFLAGS = $(all_libraries) -no-undefined -avoid-version
#libsolver_la_LIBADD = $(LIB_KDECORE) $(LIB_KIO) $(LIB_KDEUI) $(LIB_KHTML)

libsolver_la_LIBADD   = $(LIB_KIO) $(LIB_KDEPRINT) -L$(OCAMLLIB) -L$(OCAMLLIB)/facile -lasmrun -lstr -lnums -lm  -ldl solver.o modwrap.o


### OCAMLLIB is defined in configure.in.in
### if that does not work, uncomment the following lines :
# OCAMLLIB=`ocamlc -where`

# the library search path. 
#libsolver_la_LDFLAGS = $(KDE_RPATH) $(all_libraries)

# the libraries to link against.
#
# OCAMLLIB is user-defined : see ../configure.in.in
# 
#libsolver_la_LDADD   = $(LIB_KIO) $(LIB_KDEPRINT) \
#	-L$(OCAMLLIB) -L$(OCAMLLIB)/facile \
#	-lasmrun -lstr -lnums -lm  -ldl solver.o modwrap.o


# which sources should be compiled for eqchem
#libsolverla_SOURCES = eqresult.cpp eqchemview.cpp

# these are the headers for your project
noinst_HEADERS = eqchemview.h eqresult.h

