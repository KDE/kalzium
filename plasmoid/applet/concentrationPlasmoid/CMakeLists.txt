project(concentrationCalculator)

set(concentrationCalculator_SRCS concentrationCalculator.cpp)
ki18n_wrap_ui(concentrationCalculator_SRCS concentrationCalculatorConfig.ui)
add_library(concentrationCalculator ${concentrationCalculator_SRCS})
target_link_libraries(concentrationCalculator
                      KF5::Plasma KF5::WidgetsAddons
                      KF5::UnitConversion
                      )

install(TARGETS concentrationCalculator DESTINATION ${KDE_INSTALL_PLUGINDIR})

install(FILES concentrationCalculator.desktop DESTINATION ${SERVICES_INSTALL_DIR})
