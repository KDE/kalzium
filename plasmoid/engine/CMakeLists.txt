set(kalzium_engine_SRCS
    kalzium_engine.cpp
)

include_directories(
   ${CMAKE_SOURCE_DIR}/libkdeedu/libscience/
   )

add_library(plasma_engine_kalzium ${kalzium_engine_SRCS})
target_link_libraries(plasma_engine_kalzium
   KF5::Plasma
   Qt5::Xml
   science
)

install(TARGETS plasma_engine_kalzium DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-dataengine-kalzium.desktop DESTINATION ${SERVICES_INSTALL_DIR})
