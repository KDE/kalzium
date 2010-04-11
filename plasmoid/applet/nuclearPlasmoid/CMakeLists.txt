project(nuclearCalculator)

include_directories(
   ${CMAKE_SOURCE_DIR}/libkdeedu/libscience/
   )

set(nuclearCalculator_SRCS
   nuclearCalculator.cpp
   kalziumdataobject.cpp
   )
kde4_add_ui_files(nuclearCalculator_SRCS nuclearCalculatorConfig.ui)
kde4_add_plugin(nuclearCalculator ${nuclearCalculator_SRCS})
target_link_libraries(nuclearCalculator
                      ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS} ${KDE4_KHTML_LIBRARY}
                      science ${KDE4_KUNITCONVERSION_LIBRARY}
                      )

install(TARGETS nuclearCalculator DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES nuclearCalculator.desktop DESTINATION ${SERVICES_INSTALL_DIR})
