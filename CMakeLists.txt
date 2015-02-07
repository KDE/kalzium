project(kalzium)
cmake_minimum_required(VERSION 2.8.12)

find_package(ECM 0.0.11 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${OPENBABEL2_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/cmake/modules)
find_package(OpenBabel2 REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(KF5 REQUIRED COMPONENTS CoreAddons Solid Config)
find_package(KF5DocTools REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(KF5CoreAddons REQUIRED)
find_package(KF5Config REQUIRED)
find_package(KF5Solid REQUIRED)
find_package(ECM REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Qt4 REQUIRED)
find_package(KF5Archive REQUIRED)
find_package(KF5I18n NO_MODULE)
find_package(KF5Parts NO_MODULE)
find_package(Gettext REQUIRED)
find_package(PythonInterp REQUIRED)
find_package(Avogadro REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(OpenGL REQUIRED)

include(KDEInstallDirs)
include(KDECompilerSettings)
include(FeatureSummary)
include(ECMInstallIcons)
include(ECMMarkNonGuiExecutable)
include_directories(${OPENBABEL2_INCLUDE_DIR} ${AVOGADRO_INCLUDE_DIR} ${OPENGL_INCLUDE_DIRS})

set(CMAKE_MODULE_PATH ${kalzium_SOURCE_DIR}/cmake/modules )

# search packages used by KDE 
if(WIN32)
  find_package(KDEWIN32 REQUIRED)
  # detect oxygen icon dir at configure time based on KDEDIRS - there may be different package installation locations
 #execute_process(COMMAND "${KDE4_KDECONFIG_EXECUTABLE}" --path icon OUTPUT_VARIABLE _dir #ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
  #file(TO_CMAKE_PATH "${_dir}" __dir)
  #find_path(KDE4_ICON_DIR oxygen PATHS
   #  ${__dir})
 # message(STATUS "using oxygen application icons from ${KDE4_ICON_DIR}")
else(WIN32)
 # set (KDE4_ICON_DIR  ${CMAKE_INSTALL_PREFIX}/share/icons)
endif(WIN32)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
#include_directories (${QDBUS_INCLUDE_DIRS}  ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES} libscience)
if(MSVC)
# for correctly linking openbabel:
# see http://www.mail-archive.com/openbabel-devel@lists.sourceforge.net/msg01975.html
add_definitions(-DUSING_DYNAMIC_LIBS)
endif(MSVC)


include(KalziumConfigureChecks.cmake)

if (OPENBABEL2_FOUND AND Avogadro_FOUND AND EIGEN3_FOUND)
  # avoid compilerwarnings about redefinitions
  # todo: use check_function_exits() ?
  message(STATUS "Kalzium molecular editor enabled")
  if (WIN32)
    add_definitions(-DHAVE_SNPRINTF -DHAVE_STRCASECMP -DHAVE_STRNCASECMP)
  endif (WIN32)
  add_subdirectory(compoundviewer)
else (OPENBABEL2_FOUND AND Avogadro_FOUND AND EIGEN3_FOUND)
  message(STATUS "Kalzium molecular editor disabled")
endif (OPENBABEL2_FOUND AND Avogadro_FOUND AND EIGEN3_FOUND)

add_subdirectory(doc)
add_subdirectory(src)
add_subdirectory(data)
add_subdirectory(plasmoid)
add_subdirectory(libscience)
add_subdirectory(qml)


install(FILES kalzium.appdata.xml DESTINATION ${SHARE_INSTALL_PREFIX}/appdata/)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
