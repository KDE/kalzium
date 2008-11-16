include(CheckIncludeFiles)
include(CheckFunctionExists)

macro_optional_find_package(OCaml)
macro_optional_find_package(Libfacile)
macro_optional_find_package(OpenBabel2)
macro_optional_find_package(OpenGL)

# Since Kalzium's old snapshot of libavogadro is the last remaining piece of code using eigen1,
# we remove eigen1 from kdesupport and instead add a copy of it in our snapshot of libavogadro.
# Yes, this is very ugly. But eigen1 is much smaller than libavogadro anyway. So this doesn't make
# things much worse. The real big problem is that we use a snapshot of libavogadro in the first place.
# I really want this to go away in 4.3, normally by relying directly on libavogadro (which should at last
# make a stable release), or else we just move the 3D molecular editor to playground/.
# macro_optional_find_package(Eigen)
set(EIGEN_FOUND TRUE)

macro_bool_to_01(LIBFACILE_FOUND HAVE_FACILE)
macro_bool_to_01(OPENBABEL2_FOUND HAVE_OPENBABEL2)
macro_bool_to_01(OPENGL_FOUND HAVE_OPENGL)
macro_bool_to_01(EIGEN_FOUND HAVE_EIGEN)

# at the end, output the configuration
configure_file(
   ${CMAKE_CURRENT_SOURCE_DIR}/config-kalzium.h.cmake
   ${CMAKE_CURRENT_BINARY_DIR}/config-kalzium.h
)

macro_log_feature(OCAML_FOUND "OCaml" "OCaml is needed by Kalzium for the Equation Solver" "http://caml.inria.fr/" FALSE "" "")
macro_log_feature(LIBFACILE_FOUND "LibFacile" "libfacile is needed by Kalzium for the Equation Solver." "http://www.recherche.enac.fr/log/facile/" FALSE "" "")
macro_log_feature(OPENGL_FOUND "OpenGL" "OpenGL is needed by Kalzium for the 3D-molecule viewer" "http://www.opengl.org" FALSE "" "")
macro_log_feature(OPENBABEL2_FOUND "OpenBabel2" "OpenBabel is needed by Kalzium for the 3D-molecule viewer" "http://openbabel.sourceforge.net/" FALSE "2.2" "")
# macro_log_feature(EIGEN_FOUND "Eigen" "Eigen is needed by Kalzium for the 3D-molecule viewer" "http://eigen.tuxfamily.org" FALSE "1.0.5")

