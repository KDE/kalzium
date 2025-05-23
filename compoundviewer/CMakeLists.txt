add_library(compoundviewer)

set_target_properties(compoundviewer PROPERTIES
    VERSION ${KALZIUMLIB_VERSION}
    SOVERSION ${KALZIUMLIB_SOVERSION}
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

generate_export_header(compoundviewer)

target_sources(compoundviewer PRIVATE
    iowrapper.cpp
    kalziumglwidget.cpp
    iowrapper.h
    kalziumglwidget.h
)

target_link_libraries(compoundviewer
    KF6::I18n
    KF6::WidgetsAddons
    KF6::CoreAddons
    Qt::OpenGL
    Qt::Gui
    Qt::Widgets
    Qt::OpenGLWidgets
    ${OPENBABEL_LIBRARIES}
    Avogadro::Core
    Avogadro::IO
    Avogadro::QtGui
    Avogadro::QtOpenGL
    Avogadro::QtPlugins
    Eigen3::Eigen
)

target_include_directories(compoundviewer PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/..
    ${OPENBABEL_INCLUDE_DIR}
    ${AvogadroLibs_INCLUDE_DIRS}
)

install(TARGETS compoundviewer ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

#add_subdirectory(widgets)
