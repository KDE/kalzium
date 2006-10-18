project(kalzium)

include(MacroBoolTo01)
macro_bool_to_01(OPENBABEL2_FOUND HAVE_OPENBABEL2)
macro_bool_to_01(OPENGL_FOUND HAVE_OPENGL)
macro_bool_to_01(LIBFACILE_FOUND HAVE_FACILE)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config-kalzium.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kalzium.h )

add_subdirectory(src)
add_subdirectory(data)
#add_subdirectory(simulations)
