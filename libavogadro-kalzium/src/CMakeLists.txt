set(QT_USE_QTOPENGL TRUE)

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

set(libavogadro_SRCS
  camera.cpp
  color.cpp
  engine.cpp
  glwidget.cpp
  moleculetreeview.cpp
  primitive.cpp
  tool.cpp
  toolgroup.cpp
  undosequence.cpp
  painter.cpp
  textrenderer.cpp
  sphere.cpp
  cylinder.cpp
)

#Don't install headers since Kalzium is the only app using this snapshot
#set(libavogadro_HDRS
#  color.h
#  primitive.h
#  engine.h
#  glwidget.h
#  toolgroup.h
#  moleculetreeview.h
#  global.h
#  camera.h
#  undosequence.h
#  painter.h
#)


#set(libavogadro_MOC_HDRS
  #primitive.h
  #glwidget.h
  #moleculetreeview.h
#)
#qt4_wrap_cpp(libavogadro_MOC_SRCS ${libavogadro_MOC_HDRS})

# Install the library to avogadro-kalzium so that it does not collide with avo
KDE4_ADD_LIBRARY(avogadro-kalzium SHARED ${libavogadro_SRCS})
SET_TARGET_PROPERTIES(avogadro-kalzium PROPERTIES OUTPUT_NAME avogadro-kalzium)
set_target_properties(avogadro-kalzium PROPERTIES VERSION 0.0.1 SOVERSION 0 )

TARGET_LINK_LIBRARIES(avogadro-kalzium ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                   ${OPENGL_LIBRARIES} ${KDE4_KDECORE_LIBS} )
install(TARGETS avogadro-kalzium  DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})

#Don't install headers since Kalzium is the only app using this snapshot
#INSTALL(FILES ${libavogadro_HDRS} DESTINATION include/avogadro)

add_subdirectory(engines)
add_subdirectory(tools)
