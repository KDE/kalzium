project(kalzium)
cmake_minimum_required(VERSION 2.8.12)


set(QT_MIN_VERSION "5.9.0")
set(KF5_MIN_VERSION "5.42.0")

find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${kalzium_SOURCE_DIR}/cmake/modules)

include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(FeatureSummary)
include(ECMInstallIcons)
include(ECMMarkNonGuiExecutable)
include(CheckIncludeFiles)
include(ECMSetupVersion)

find_package(Gettext REQUIRED)
find_package(PythonInterp REQUIRED)
find_package(PkgConfig)
find_package(OpenBabel2)
find_package(AvogadroLibs)
find_package(Eigen3)
find_package(OCaml)
find_package(Libfacile)

pkg_check_modules(CHEMICAL_MIME_DATA chemical-mime-data)
check_include_files(ieeefp.h HAVE_IEEEFP_H)

# create configuration file
set(HAVE_FACILE ${LIBFACILE_FOUND})
set(HAVE_OPENBABEL2 ${OPENBABEL2_FOUND})
set(HAVE_EIGEN ${EIGEN3_FOUND})
set(HAVE_AVOGADRO ${AvogadroLibs_FOUND})
configure_file(
   ${CMAKE_CURRENT_SOURCE_DIR}/config-kalzium.h.cmake
   ${CMAKE_CURRENT_BINARY_DIR}/config-kalzium.h
)

include_directories(${AvogadroLibs_INCLUDE_DIRS})

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED COMPONENTS
    Core
    Gui
    Script
    Svg
    Widgets
    Xml
)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    Archive
    Config
    CoreAddons
    DocTools
    I18n
    KHtml
    Parts
    Plotting
    Solid
    UnitConversion
    WidgetsAddons
)

ecm_setup_version("5.0.0"
    VARIABLE_PREFIX KALZIUMLIB
)

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

add_definitions(
    -DQT_NO_URL_CAST_FROM_STRING
)
#include_directories (${QDBUS_INCLUDE_DIRS}  ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES} libscience)
if(MSVC)
# for correctly linking openbabel:
# see http://www.mail-archive.com/openbabel-devel@lists.sourceforge.net/msg01975.html
add_definitions(-DUSING_DYNAMIC_LIBS)
endif(MSVC)

if (OPENBABEL2_FOUND AND AvogadroLibs_FOUND AND EIGEN3_FOUND)
  find_package(Qt5OpenGL ${QT_MIN_VERSION} REQUIRED)
  find_package(KF5NewStuff REQUIRED)
  # avoid compilerwarnings about redefinitions
  # todo: use check_function_exits() ?
  message(STATUS "Kalzium molecular editor enabled")
  if (WIN32)
    add_definitions(-DHAVE_SNPRINTF -DHAVE_STRCASECMP -DHAVE_STRNCASECMP)
  endif (WIN32)
  add_subdirectory(compoundviewer)
else (OPENBABEL2_FOUND AND AvogadroLibs_FOUND AND EIGEN3_FOUND)
  message(STATUS "Kalzium molecular editor disabled")
endif (OPENBABEL2_FOUND AND AvogadroLibs_FOUND AND EIGEN3_FOUND)

add_subdirectory(doc)
add_subdirectory(src)
add_subdirectory(data)
# add_subdirectory(plasmoid)
add_subdirectory(libscience)
# add_subdirectory(qml)

install(FILES org.kde.kalzium.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
