include_directories( 
   ${CMAKE_SOURCE_DIR}/libkdeedu/libscience/   
   )

set(kalzium_plasma_SRCS
    kalzium_plasma.cpp
    )

kde4_add_ui_files(kalzium_plasma_SRCS config.ui)

kde4_add_plugin(plasma_applet_kalzium ${kalzium_plasma_SRCS})

target_link_libraries(plasma_applet_kalzium
    ${KDE4_PLASMA_LIBS} ${KDE4_KIO_LIBS}
    science
    )

install(
    TARGETS plasma_applet_kalzium
    DESTINATION ${KDE_INSTALL_PLUGINDIR}
    )

install(
    FILES plasma_kalzium.desktop
    DESTINATION ${KDE_INSTALL_KSERVICES5DIR}
    )

install(
    FILES testtube.svg
    DESTINATION ${KDE_INSTALL_DATADIR}/desktoptheme/default/widgets/
    )
