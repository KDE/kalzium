# Main libavogadro build file
set(QT_USE_QTOPENGL true)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

include(${QT_USE_FILE})

include_directories(
  ${libavogadro-kalzium_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/..
  ${OPENBABEL2_INCLUDE_DIR}
  ${EIGEN_INCLUDE_DIR}
)

FILE(GLOB libavogadro_SRCS "*.cpp")
FILE(GLOB libavogadro_HDRS "*.h")

# Statically link the core engines and tools so that users have a working
# system even if no plugins can be found.
set(bsdyengine_SRCS engines/bsdyengine.cpp)
qt4_wrap_ui(bsdyengine_UIS engines/bsdysettingswidget.ui)
set(libavogadro_SRCS ${libavogadro_SRCS} ${bsdyengine_SRCS} ${bsdyengine_UIS})

qt4_automoc(${libavogadro_SRCS})

KDE4_ADD_LIBRARY(avogadro-kalzium SHARED ${libavogadro_SRCS})
SET_TARGET_PROPERTIES(avogadro-kalzium PROPERTIES OUTPUT_NAME avogadro-kalzium)
set_target_properties(avogadro-kalzium PROPERTIES VERSION 0.6.1 SOVERSION 0 )

TARGET_LINK_LIBRARIES(avogadro-kalzium ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                   ${OPENGL_LIBRARIES}
    )

INSTALL(TARGETS avogadro-kalzium DESTINATION ${LIB_INSTALL_DIR})

add_subdirectory(engines)
add_subdirectory(tools)
