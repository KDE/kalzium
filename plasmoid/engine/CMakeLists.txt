set(kalzium_engine_SRCS
    kalzium_engine.cpp
)

include_directories(
   ${CMAKE_SOURCE_DIR}/libkdeedu/libscience/
   )

kde4_add_plugin(plasma_engine_kalzium ${kalzium_engine_SRCS})
target_link_libraries(plasma_engine_kalzium
   ${KDE4_PLASMA_LIBS}
   ${QT_QTXML_LIBRARY}
   science
)

install(TARGETS plasma_engine_kalzium DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-dataengine-kalzium.desktop DESTINATION ${SERVICES_INSTALL_DIR})
