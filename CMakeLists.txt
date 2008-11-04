project(kalzium)

include(KalziumConfigureChecks.cmake)


if(OPENBABEL2_FOUND AND OPENGL_FOUND AND EIGEN_FOUND)
  # avoid compilerwarnings about redefinitions
  # todo: use check_function_exits() ?
  if(WIN32)
    add_definitions(-DHAVE_SNPRINTF -DHAVE_STRCASECMP -DHAVE_STRNCASECMP)
  endif(WIN32)
	add_subdirectory(libavogadro-kalzium)
	add_subdirectory(compoundviewer)
endif(OPENBABEL2_FOUND AND OPENGL_FOUND AND EIGEN_FOUND)

add_subdirectory(src)
add_subdirectory(data)
add_subdirectory(plasmoid)
