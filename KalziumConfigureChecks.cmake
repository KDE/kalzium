include(CheckIncludeFiles)
include(CheckFunctionExists)

macro_optional_find_package(OCaml)
macro_optional_find_package(Libfacile)
macro_optional_find_package(OpenBabel2)
macro_optional_find_package(Eigen2)
macro_optional_find_package(Avogadro 1.0.0 NO_MODULE)

find_package(PkgConfig)
pkg_check_modules(CHEMICAL_MIME_DATA chemical-mime-data)

check_include_files(ieeefp.h HAVE_IEEEFP_H)

macro_bool_to_01(LIBFACILE_FOUND HAVE_FACILE)
macro_bool_to_01(OPENBABEL2_FOUND HAVE_OPENBABEL2)
macro_bool_to_01(EIGEN2_FOUND HAVE_EIGEN)
macro_bool_to_01(Avogadro_FOUND HAVE_AVOGADRO)

# at the end, output the configuration
configure_file(
   ${CMAKE_CURRENT_SOURCE_DIR}/config-kalzium.h.cmake
   ${CMAKE_CURRENT_BINARY_DIR}/config-kalzium.h
)

macro_log_feature(OCAML_FOUND "OCaml" "OCaml is needed by Kalzium for the Equation Solver" "http://caml.inria.fr/" FALSE "" "")
macro_log_feature(LIBFACILE_FOUND "LibFacile" "libfacile is needed by Kalzium for the Equation Solver." "http://www.recherche.enac.fr/log/facile/" FALSE "" "")
macro_log_feature(OPENBABEL2_FOUND "OpenBabel2" "OpenBabel is needed by Kalzium for the 3D-molecule viewer" "http://openbabel.sourceforge.net/" FALSE "2.2" "")
macro_log_feature(EIGEN2_FOUND "Eigen2" "A generic C++ template library for dense and sparse matrices" "http://eigen.tuxfamily.org" FALSE "2.0.0" "")
macro_log_feature(Avogadro_FOUND "Avogadro" "Advanced molecular editor" "http://avogadro.openmolecules.net" FALSE "1.0" "")
macro_log_feature(CHEMICAL_MIME_DATA_FOUND "Chemical mime data" "Needed to open CML molecules from the file manager" "http://chemical-mime.sourceforge.net/" FALSE "" "")

