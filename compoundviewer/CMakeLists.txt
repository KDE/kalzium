include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/..
    ${EIGEN3_INCLUDE_DIR}
    ${OPENBABEL2_INCLUDE_DIR}
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
    Qt5::OpenGL
    Qt5::Gui
    Qt5::Widgets
    ${OPENBABEL2_LIBRARIES}
    AvogadroQtGui
    AvogadroQtOpenGL
    AvogadroQtPlugins
)

set_target_properties(compoundviewer PROPERTIES
    VERSION ${KALZIUMLIB_VERSION}
    SOVERSION ${KALZIUMLIB_SOVERSION}
)

install(TARGETS compoundviewer ${INSTALL_TARGETS_DEFAULT_ARGS})
