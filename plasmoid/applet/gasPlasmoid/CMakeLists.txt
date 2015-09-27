project(gasCalculator)

set(gasCalculator_SRCS gasCalculator.cpp)
ki18n_wrap_ui(gasCalculator_SRCS gasCalculatorConfig.ui)
add_library(gasCalculator ${gasCalculator_SRCS})
target_link_libraries(gasCalculator
                      KF5::Plasma KF5::Ui KF5::WidgetsAddons
                      KF5::UnitConversion
                      )

install(TARGETS gasCalculator DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES gasCalculator.desktop DESTINATION ${SERVICES_INSTALL_DIR})
