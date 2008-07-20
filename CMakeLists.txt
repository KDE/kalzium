project(kalzium)

include(KalziumConfigureChecks.cmake)

macro_optional_find_package(Plasma)
macro_bool_to_01(PLASMA_FOUND HAVE_PLASMA)
macro_log_feature(PLASMA_FOUND "Plasma" "Plasma is the KDE4 desktop" "http://plasma.kde.org" FALSE "" "Provides a Kalzium Plasmoid")

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
if(HAVE_PLASMA)
   add_subdirectory(plasmoid)
endif(HAVE_PLASMA)
