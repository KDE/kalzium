include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/..
    ${OPENBABEL_INCLUDE_DIR}
    ${AvogadroLibs_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/compoundviewer
)

set(compoundviewer_SRCS
    iowrapper.cpp
    kalziumglwidget.cpp
)

#add_subdirectory(widgets)

add_library(compoundviewer ${compoundviewer_SRCS})

target_link_libraries(compoundviewer
    KF5::I18n
    KF5::WidgetsAddons
    KF5::CoreAddons
    Qt::OpenGL
    Qt::Gui
    Qt::Widgets
    ${OPENBABEL_LIBRARIES}
    AvogadroQtGui
    AvogadroQtOpenGL
    AvogadroQtPlugins
    Eigen3::Eigen
)

set_target_properties(compoundviewer PROPERTIES
    VERSION ${KALZIUMLIB_VERSION}
    SOVERSION ${KALZIUMLIB_SOVERSION}
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

generate_export_header(compoundviewer)

install(TARGETS compoundviewer ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
