project(gasCalculator)

set(gasCalculator_SRCS gasCalculator.cpp)
ki18n_wrap_ui(gasCalculator_SRCS gasCalculatorConfig.ui)
add_library(gasCalculator ${gasCalculator_SRCS})
target_link_libraries(gasCalculator
                      KF5::Plasma KF5::Ui KF5::WidgetsAddons
                      KF5::UnitConversion
                      )

install(TARGETS gasCalculator DESTINATION ${KDE_INSTALL_PLUGINDIR})

install(FILES gasCalculator.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
