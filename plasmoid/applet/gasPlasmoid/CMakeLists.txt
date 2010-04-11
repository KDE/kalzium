project(gasCalculator)

set(gasCalculator_SRCS gasCalculator.cpp)
kde4_add_ui_files(gasCalculator_SRCS gasCalculatorConfig.ui)
kde4_add_plugin(gasCalculator ${gasCalculator_SRCS})
target_link_libraries(gasCalculator
                      ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS}
                      ${KDE4_KUNITCONVERSION_LIBRARY}
                      )

install(TARGETS gasCalculator DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES gasCalculator.desktop DESTINATION ${SERVICES_INSTALL_DIR})
