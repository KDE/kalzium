cmake_minimum_required(VERSION 3.24)

# KDE Application Version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "25")
set(RELEASE_SERVICE_VERSION_MINOR "12")
set(RELEASE_SERVICE_VERSION_MICRO "3")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(kalzium VERSION ${RELEASE_SERVICE_VERSION})

set(QT_MIN_VERSION "6.6.2")
set(KF_MIN_VERSION "6.7.0")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${kalzium_SOURCE_DIR}/cmake/modules)

set(KDE_COMPILERSETTINGS_LEVEL "5.82")

include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(FeatureSummary)
include(ECMInstallIcons)
include(ECMMarkNonGuiExecutable)
include(CheckIncludeFiles)
include(ECMAddAppIcon)
include(ECMSetupVersion)
include(ECMQtDeclareLoggingCategory)
include(GenerateExportHeader)
include(KDEGitCommitHooks)
include(KDEClangFormat)
include(ECMDeprecationSettings)
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h *.c)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})


find_package(Gettext REQUIRED)
find_package(PkgConfig)
find_package(OpenBabel3 3.1)
if(NOT ${OpenBabel3_FOUND})
    find_package(OpenBabel2)
endif()
find_package(AvogadroLibs)
find_package(Eigen3 CONFIG)
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
set(HAVE_EIGEN ${Eigen3_FOUND})
set(HAVE_AVOGADRO ${AvogadroLibs_FOUND})

if(HAVE_OPENBABEL3)
    set(OPENBABEL_INCLUDE_DIR ${OpenBabel3_INCLUDE_DIRS})
    set(OPENBABEL_LIBRARIES ${OpenBabel3_LIBRARIES})
elseif(HAVE_OPENBABEL2)
    set(OPENBABEL_INCLUDE_DIR ${OPENBABEL2_INCLUDE_DIR})
    set(OPENBABEL_LIBRARIES ${OPENBABEL2_LIBRARIES})
endif()

configure_file(
   ${CMAKE_CURRENT_SOURCE_DIR}/config-kalzium.h.in
   ${CMAKE_CURRENT_BINARY_DIR}/config-kalzium.h
)

include_directories(${AvogadroLibs_INCLUDE_DIRS})

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS
    Core
    Gui
    Svg
    Widgets
    Xml
    Core5Compat
    StateMachine
    Qml
    OpenGLWidgets
)

find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS
    Archive
    Config
    ConfigWidgets
    CoreAddons
    DocTools
    I18n
    ItemViews
    KIO
    Plotting
    UnitConversion
    WidgetsAddons
    TextWidgets
    XmlGui
    Crash
)

ecm_setup_version("5.0.0"
    VARIABLE_PREFIX KALZIUMLIB
)

add_definitions(
    -DQT_NO_URL_CAST_FROM_STRING
)
if(MSVC)
    # for correctly linking openbabel:
    # see https://www.mail-archive.com/openbabel-devel@lists.sourceforge.net/msg01975.html
    add_definitions(-DUSING_DYNAMIC_LIBS)
endif()

ecm_set_disabled_deprecation_versions(QT 5.14.0
    KF 6.13
)


if (HAVE_OPENBABEL AND AvogadroLibs_FOUND AND Eigen3_FOUND)
  find_package(Qt6OpenGL ${QT_MIN_VERSION} REQUIRED)
  find_package(KF6NewStuff ${KF_MIN_VERSION} REQUIRED)
  # avoid compilerwarnings about redefinitions
  # todo: use check_function_exits() ?
  message(STATUS "Kalzium molecular editor enabled")
  if (WIN32)
    add_definitions(-DHAVE_SNPRINTF -DHAVE_STRCASECMP -DHAVE_STRNCASECMP)
  endif ()
  add_subdirectory(compoundviewer)
else ()
  message(STATUS "Kalzium molecular editor disabled")
endif ()

add_subdirectory(src)
add_subdirectory(data)
add_subdirectory(libscience)

ki18n_install(po)
if (KF6DocTools_FOUND)
    kdoctools_install(po)
    add_subdirectory(doc)
endif()
install(FILES org.kde.kalzium.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

ecm_qt_install_logging_categories(
        EXPORT KALZIUM
        FILE kalzium.categories
        DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
        )

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
