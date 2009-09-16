# Project Needs a name ofcourse
project(gasCalculator)
 
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   )
 
# We add our source code here
set(gasCalculator_SRCS gasCalculator.cpp)
kde4_add_ui_files(gasCalculator_SRCS gasCalculatorConfig.ui )
# Now make sure all files get to the right place
kde4_add_plugin(gasCalculator ${gasCalculator_SRCS})
target_link_libraries(gasCalculator 
                      ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS}
                      ${KDE4_KUNITCONVERSION_LIBS}
                      )

install(TARGETS gasCalculator
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES gasCalculator.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
