#include_directories(${CMAKE_SOURCE_DIR}/libkdeedu/libscience/)

set(didyouknow_plasma_SRCS
    didyouknow.cpp
    )

add_library(plasma_applet_didyouknow ${didyouknow_plasma_SRCS})

target_link_libraries(plasma_applet_didyouknow
    KF5::Plasma KF5::KIOWidgests
    science
    )

install(TARGETS plasma_applet_didyouknow DESTINATION ${KDE_INSTALL_PLUGINDIR})

install(FILES plasma_didyouknow.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})

install(
    FILES chalkboard.svg
    DESTINATION ${KDE_INSTALL_DATADIR}/desktoptheme/default/widgets/
    )
