project(kalzium)

kde4_no_enable_final(kalzium)

include(KalziumConfigureChecks.cmake)

macro_optional_find_package(Avogadro 1.0.0)

if(OPENBABEL2_FOUND AND Avogadro_FOUND AND EIGEN2_FOUND)
  # avoid compilerwarnings about redefinitions
  # todo: use check_function_exits() ?
  message(STATUS "Kalzium molecular editor enabled")
  if(WIN32)
    add_definitions(-DHAVE_SNPRINTF -DHAVE_STRCASECMP -DHAVE_STRNCASECMP)
  endif(WIN32)
  add_subdirectory(compoundviewer)
else(OPENBABEL2_FOUND AND Avogadro_FOUND AND EIGEN2_FOUND)
  message(STATUS "Kalzium molecular editor disabled")
endif(OPENBABEL2_FOUND AND Avogadro_FOUND AND EIGEN2_FOUND)

check_include_files(ieeefp.h HAVE_IEEEFP_H)

add_subdirectory(src)
add_subdirectory(data)
add_subdirectory(plasmoid)
