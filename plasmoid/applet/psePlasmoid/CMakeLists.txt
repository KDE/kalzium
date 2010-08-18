include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   ${CMAKE_SOURCE_DIR}/libkdeedu/libscience
   )

# We add our source code here
set(periodic_SRCS Molmasscalculator.cpp Periodictable.cpp)

kde4_add_ui_files(periodic_SRCS Molmassconfig.ui)

# Now make sure all files get to the right place
kde4_add_plugin(plasma_applet_molmassCalculator ${periodic_SRCS})

target_link_libraries(plasma_applet_molmassCalculator
                      ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS} science)

install(TARGETS plasma_applet_molmassCalculator
        DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES plasma-applet-Molmasscalculator.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
