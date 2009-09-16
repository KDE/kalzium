#This is the CMakeLists.txt file for the nuclearCalculator project
project(nuclearCalculator)

include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   ${CMAKE_SOURCE_DIR}/libkdeedu/libscience/
   )
 
# We add our source code here
set(nuclearCalculator_SRCS 
   nuclearCalculator.cpp
   kalziumdataobject.cpp
   )
kde4_add_ui_files(nuclearCalculator_SRCS nuclearCalculatorConfig.ui )
# Now make sure all files get to the right place
kde4_add_plugin(nuclearCalculator ${nuclearCalculator_SRCS})
target_link_libraries(nuclearCalculator 
                      ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS} ${KDE4_KHTML_LIBRARY}
                      science ${KDE4_KUNITCONVERSION_LIBS}
                      )

install(TARGETS nuclearCalculator
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES nuclearCalculator.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
