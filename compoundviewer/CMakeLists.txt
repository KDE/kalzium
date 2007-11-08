include_directories(
  ${CMAKE_SOURCE_DIR}/..
  ${CMAKE_CURRENT_BINARY_DIR}/..
  ${CMAKE_SOURCE_DIR}/kalzium/libavogadro-kalzium/include/
  )

set( compoundviewer_SRCS
  openbabel2wrapper.cpp
  kalziumglpart.cpp
  )
include_directories( ${OPENBABEL2_INCLUDE_DIR} ${EIGEN_INCLUDE_DIR} )

add_subdirectory(widgets)

kde4_add_library(compoundviewer SHARED ${compoundviewer_SRCS})

target_link_libraries(compoundviewer ${OPENBABEL2_LIBRARIES}
${QT_QTOPENGL_LIBRARY} ${KDE4_KPARTS_LIBS} ${OPENGL_gl_LIBRARY}
${OPENGL_glu_LIBRARY} avogadro-kalzium)

set_target_properties(compoundviewer PROPERTIES VERSION 4.0.0 SOVERSION 4 )

install(TARGETS compoundviewer 
        RUNTIME DESTINATION ${BIN_INSTALL_DIR}
        LIBRARY DESTINATION ${LIB_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)

