project(kalzium)

find_package(KDE4 REQUIRED)
include (KDE4Defaults)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

include(KalziumConfigureChecks.cmake)

if(OPENBABEL2_FOUND AND OPENGL_FOUND AND EIGEN2_FOUND)
  # avoid compilerwarnings about redefinitions
  # todo: use check_function_exits() ?
  message(STATUS "Kalzium molecular editor enabled")
  if(WIN32)
    add_definitions(-DHAVE_SNPRINTF -DHAVE_STRCASECMP -DHAVE_STRNCASECMP)
  endif(WIN32)
  add_subdirectory(libavogadro-kalzium)
  add_subdirectory(compoundviewer)
else(OPENBABEL2_FOUND AND OPENGL_FOUND AND EIGEN2_FOUND)
  message(STATUS "Kalzium molecular editor disabled")
endif(OPENBABEL2_FOUND AND OPENGL_FOUND AND EIGEN2_FOUND)

check_include_files(ieeefp.h HAVE_IEEEFP_H)

add_subdirectory(src)
add_subdirectory(data)
add_subdirectory(plasmoid)
