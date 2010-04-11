project(concentrationCalculator)

set(concentrationCalculator_SRCS concentrationCalculator.cpp)
kde4_add_ui_files(concentrationCalculator_SRCS concentrationCalculatorConfig.ui)
kde4_add_plugin(concentrationCalculator ${concentrationCalculator_SRCS})
target_link_libraries(concentrationCalculator
                      ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS}
                      ${KDE4_KUNITCONVERSION_LIBRARY}
                      )

install(TARGETS concentrationCalculator DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES concentrationCalculator.desktop DESTINATION ${SERVICES_INSTALL_DIR})
