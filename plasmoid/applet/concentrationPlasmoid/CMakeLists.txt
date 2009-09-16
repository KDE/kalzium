# Project Needs a name ofcourse
project(concentrationCalculator)
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   )
 
# We add our source code here
set(concentrationCalculator_SRCS concentrationCalculator.cpp)
kde4_add_ui_files(concentrationCalculator_SRCS concentrationCalculatorConfig.ui )
# Now make sure all files get to the right place
kde4_add_plugin(concentrationCalculator ${concentrationCalculator_SRCS})
target_link_libraries(concentrationCalculator 
                      ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS}
                      ${KDE4_KUNITCONVERSION_LIBS}
                      )

install(TARGETS concentrationCalculator
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES concentrationCalculator.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
