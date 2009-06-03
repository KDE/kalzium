# Project Needs a name ofcourse
project(nuclearCalculator)
 
# Find the required Libaries
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
 
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   )
 
# We add our source code here
set(nuclearCalculator_SRCS nuclearCalculator.cpp)
# Now make sure all files get to the right place
kde4_add_plugin(nuclearCalculator ${nuclearCalculator_SRCS})
target_link_libraries(nuclearCalculator 
                      ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS})

install(TARGETS nuclearCalculator
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES nuclearCalculator.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
