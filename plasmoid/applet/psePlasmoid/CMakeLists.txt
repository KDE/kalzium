include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   ${CMAKE_SOURCE_DIR}/libkdeedu/libscience
   )

# We add our source code here
set(periodic_SRCS Molmasscalculator.cpp Periodictable.cpp)

ki18n_wrap_ui(periodic_SRCS Molmassconfig.ui)

# Now make sure all files get to the right place
add_library(plasma_applet_molmassCalculator ${periodic_SRCS})

target_link_libraries(plasma_applet_molmassCalculator
                      KF5::Plasma KF5::WidgetAddons science)

install(TARGETS plasma_applet_molmassCalculator
        DESTINATION ${KDE_INSTALL_PLUGINDIR})

install(FILES plasma-applet-Molmasscalculator.desktop
        DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
