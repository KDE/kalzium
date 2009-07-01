# Project Needs a name ofcourse
project(gasCalculator)
 
# Find the required Libaries
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
 
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   ${CMAKE_SOURCE_DIR}/kalzium/src/conversion/
   )
 
# We add our source code here
set(gasCalculator_SRCS gasCalculator.cpp)
# Now make sure all files get to the right place
kde4_add_plugin(gasCalculator ${gasCalculator_SRCS})
target_link_libraries(gasCalculator 
                      ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS}
                      conversion
                      )

install(TARGETS gasCalculator
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES gasCalculator.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
