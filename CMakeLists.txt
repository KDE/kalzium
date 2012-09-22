project(kalzium)

set(CMAKE_MODULE_PATH ${kalzium_SOURCE_DIR}/cmake/modules )

# search packages used by KDE
find_package(KDE4 4.6.80 REQUIRED)
if(WIN32)
  find_package(KDEWIN32 REQUIRED)
  # detect oxygen icon dir at configure time based on KDEDIRS - there may be different package installation locations
  execute_process(COMMAND "${KDE4_KDECONFIG_EXECUTABLE}" --path icon OUTPUT_VARIABLE _dir ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
  file(TO_CMAKE_PATH "${_dir}" __dir)
  find_path(KDE4_ICON_DIR oxygen PATHS
      ${__dir}
  )
  message(STATUS "using oxygen application icons from ${KDE4_ICON_DIR}")
else(WIN32)
    set (KDE4_ICON_DIR  ${CMAKE_INSTALL_PREFIX}/share/icons)
endif(WIN32)

include(KDE4Defaults)
include(MacroLibrary)
include(MacroOptionalAddSubdirectory)
include(MacroBoolTo01)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
include_directories (${QDBUS_INCLUDE_DIRS}  ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES} libscience)

kde4_no_enable_final(kalzium)

include(KalziumConfigureChecks.cmake)

if (OPENBABEL2_FOUND AND Avogadro_FOUND AND EIGEN2_FOUND)
  # avoid compilerwarnings about redefinitions
  # todo: use check_function_exits() ?
  message(STATUS "Kalzium molecular editor enabled")
  if (WIN32)
    add_definitions(-DHAVE_SNPRINTF -DHAVE_STRCASECMP -DHAVE_STRNCASECMP)
  endif (WIN32)
  add_subdirectory(compoundviewer)
else (OPENBABEL2_FOUND AND Avogadro_FOUND AND EIGEN2_FOUND)
  message(STATUS "Kalzium molecular editor disabled")
endif (OPENBABEL2_FOUND AND Avogadro_FOUND AND EIGEN2_FOUND)

add_subdirectory(doc)
add_subdirectory(src)
add_subdirectory(data)
add_subdirectory(plasmoid)
add_subdirectory(libscience)
add_subdirectory(qml)

macro_display_feature_log()

