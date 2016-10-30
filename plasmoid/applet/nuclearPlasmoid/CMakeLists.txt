project(nuclearCalculator)

#include_directories(   ${CMAKE_SOURCE_DIR}/libkdeedu/libscience/)

set(nuclearCalculator_SRCS
   nuclearCalculator.cpp
   kalziumdataobject.cpp
   )
ki18n_wrap_ui(nuclearCalculator_SRCS nuclearCalculatorConfig.ui)
add_library(nuclearCalculator ${nuclearCalculator_SRCS})
target_link_libraries(nuclearCalculator
                      KF5::Plasma KF5::Ui KF5::KHtml
                      science KF5::UnitConversion
                      )

install(TARGETS nuclearCalculator DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES nuclearCalculator.desktop DESTINATION ${SERVICES_INSTALL_DIR})
