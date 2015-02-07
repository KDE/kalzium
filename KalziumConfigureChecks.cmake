include(CheckIncludeFiles)
include(CheckFunctionExists)

find_package(OCaml)
find_package(Libfacile)
find_package(OpenBabel2)
find_package(Eigen3)
find_package(Avogadro 1.0.0 NO_MODULE)

find_package(PkgConfig)
pkg_check_modules(CHEMICAL_MIME_DATA chemical-mime-data)
include(CheckIncludeFiles)
check_include_files(ieeefp.h HAVE_IEEEFP_H)
set(HAVE_FACILE ${LIBFACILE_FOUND})
set(HAVE_OPENBABEL2 ${OPENBABEL2_FOUND})
set(HAVE_EIGEN ${EIGEN3_FOUND})
set(HAVE_AVOGADRO ${Avogadro_FOUND})
#macro_bool_to_01(LIBFACILE_FOUND HAVE_FACILE)
#macro_bool_to_01(OPENBABEL2_FOUND HAVE_OPENBABEL2)
#macro_bool_to_01(EIGEN3_FOUND HAVE_EIGEN)
#macro_bool_to_01(Avogadro_FOUND HAVE_AVOGADRO)

# at the end, output the configuration
configure_file(
   ${CMAKE_CURRENT_SOURCE_DIR}/config-kalzium.h.cmake
   ${CMAKE_CURRENT_BINARY_DIR}/config-kalzium.h
)

#set_package_properties(OCAML_FOUND "OCaml" "http://caml.inria.fr/"  FALSE "OCaml is needed by Kalzium for the Equation Solver")
#(OCAML_FOUND "OCaml" "OCaml is needed by Kalzium for the Equation Solver" "http://caml.inria.fr/" FALSE "" "")
#set_package_properties(LIBFACILE_FOUND "LibFacile" "libfacile is needed by Kalzium for the Equation Solver." "http://www.recherche.enac.fr/log/facile/" FALSE "" "")
#set_package_properties(OPENBABEL2_FOUND "OpenBabel2" "OpenBabel is needed by Kalzium for the 3D-molecule viewer" "http://openbabel.sourceforge.net/" FALSE "2.2" "")
#set_package_properties(EIGEN3_FOUND "Eigen3" "A generic C++ template library for dense and sparse matrices" "http://eigen.tuxfamily.org" FALSE "3.0.0" "")
#set_package_properties(Avogadro_FOUND "Avogadro" "Advanced molecular editor" "http://avogadro.openmolecules.net" FALSE "1.0" "")
#set_package_properties(CHEMICAL_MIME_DATA_FOUND "Chemical mime data" "Needed to open CML molecules from the file manager" "http://chemical-mime.sourceforge.net/" FALSE "" "")