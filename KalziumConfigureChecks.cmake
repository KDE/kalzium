include(CheckIncludeFiles)
include(CheckFunctionExists)

macro_optional_find_package(OCaml)
macro_optional_find_package(Libfacile)

macro_bool_to_01(LIBFACILE_FOUND HAVE_FACILE)
macro_bool_to_01(OPENBABEL2_FOUND HAVE_OPENBABEL2)
macro_bool_to_01(OPENGL_FOUND HAVE_OPENGL)
macro_bool_to_01(EIGEN_FOUND HAVE_EIGEN)

# at the end, output the configuration
configure_file(
   ${CMAKE_CURRENT_SOURCE_DIR}/config-kalzium.h.cmake
   ${CMAKE_CURRENT_BINARY_DIR}/config-kalzium.h
)

macro_log_feature(OCAML_FOUND "OCaml" "OCaml is needed by Kalzium" "http://caml.inria.fr/" FALSE "" "")
macro_log_feature(LIBFACILE_FOUND "LibFacile" "libfacile is needed by Kalzium" "http://www.recherche.enac.fr/log/facile/" FALSE "" "")

