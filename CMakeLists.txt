cmake_minimum_required(VERSION 3.16)

# KDE Application Version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "22")
set(RELEASE_SERVICE_VERSION_MINOR "04")
set(RELEASE_SERVICE_VERSION_MICRO "2")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(kalzium VERSION ${RELEASE_SERVICE_VERSION})

set(QT_MIN_VERSION "5.15.0")
set(KF5_MIN_VERSION "5.90.0")
set(KDE_COMPILERSETTINGS_LEVEL "5.82")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
include(ECMQtDeclareLoggingCategory)

find_package(Gettext REQUIRED)
find_package(PythonInterp REQUIRED)
find_package(PkgConfig)
find_package(OpenBabel3 3.1)
if(NOT ${OpenBabel3_FOUND})
    find_package(OpenBabel2)
endif()
find_package(AvogadroLibs)
find_package(Eigen3)
find_package(OCaml)
find_package(Libfacile)

pkg_check_modules(CHEMICAL_MIME_DATA chemical-mime-data)
check_include_files(ieeefp.h HAVE_IEEEFP_H)

# create configuration file
set(HAVE_FACILE ${LIBFACILE_FOUND})
set(HAVE_OPENBABEL3 ${OpenBabel3_FOUND})
set(HAVE_OPENBABEL2 ${OPENBABEL2_FOUND})
if(HAVE_OPENBABEL2 OR HAVE_OPENBABEL3)
    set(HAVE_OPENBABEL 1)
endif()
set(HAVE_EIGEN ${EIGEN3_FOUND})
set(HAVE_AVOGADRO ${AvogadroLibs_FOUND})

if(HAVE_OPENBABEL3)
    set(OPENBABEL_INCLUDE_DIR ${OpenBabel3_INCLUDE_DIRS})
    set(OPENBABEL_LIBRARIES ${OpenBabel3_LIBRARIES})
elseif(HAVE_OPENBABEL2)
    set(OPENBABEL_INCLUDE_DIR ${OPENBABEL2_INCLUDE_DIR})
    set(OPENBABEL_LIBRARIES ${OPENBABEL2_LIBRARIES})
endif()

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
if(MSVC)
# for correctly linking openbabel:
# see https://www.mail-archive.com/openbabel-devel@lists.sourceforge.net/msg01975.html
add_definitions(-DUSING_DYNAMIC_LIBS)
endif(MSVC)

if (HAVE_OPENBABEL AND AvogadroLibs_FOUND AND EIGEN3_FOUND)
  find_package(Qt5OpenGL ${QT_MIN_VERSION} REQUIRED)
  find_package(KF5NewStuff REQUIRED)
  # avoid compilerwarnings about redefinitions
  # todo: use check_function_exits() ?
  message(STATUS "Kalzium molecular editor enabled")
  if (WIN32)
    add_definitions(-DHAVE_SNPRINTF -DHAVE_STRCASECMP -DHAVE_STRNCASECMP)
  endif (WIN32)
  add_subdirectory(compoundviewer)
else (HAVE_OPENBABEL AND AvogadroLibs_FOUND AND EIGEN3_FOUND)
  message(STATUS "Kalzium molecular editor disabled")
endif (HAVE_OPENBABEL AND AvogadroLibs_FOUND AND EIGEN3_FOUND)

add_subdirectory(doc)
add_subdirectory(src)
add_subdirectory(data)
add_subdirectory(libscience)

ki18n_install(po)
if (KF5DocTools_FOUND)
    kdoctools_install(po)
endif()
install(FILES org.kde.kalzium.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

ecm_qt_install_logging_categories(
        EXPORT KALZIUM
        FILE kalzium.categories
        DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
        )

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
