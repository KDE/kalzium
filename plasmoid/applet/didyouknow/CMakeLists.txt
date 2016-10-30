#include_directories(${CMAKE_SOURCE_DIR}/libkdeedu/libscience/)

set(didyouknow_plasma_SRCS
    didyouknow.cpp
    )

add_library(plasma_applet_didyouknow ${didyouknow_plasma_SRCS})

target_link_libraries(plasma_applet_didyouknow
    KF5::Plasma KF5::KIOWidgests
    science
    )

install(TARGETS plasma_applet_didyouknow DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES plasma_didyouknow.desktop DESTINATION ${SERVICES_INSTALL_DIR})

install(
    FILES chalkboard.svg
    DESTINATION ${DATA_INSTALL_DIR}/desktoptheme/default/widgets/
    )
