# Project Needs a name ofcourse
project(calc)
 
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
set(calc_SRCS calc.cpp)
# Now make sure all files get to the right place
kde4_add_plugin(calc ${calc_SRCS})
target_link_libraries(calc 
                      ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS})
 
install(TARGETS calc
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES calc.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
