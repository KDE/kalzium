include_directories(
   ${CMAKE_SOURCE_DIR}/..
   ${CMAKE_CURRENT_BINARY_DIR}/..
)

if(OPENBABEL2_FOUND AND OPENGL_FOUND AND EIGEN_FOUND)
  set( compoundviewer_SRCS
    kalziumglhelperclasses.cpp
    kalziumglwidget.cpp
    openbabel2wrapper.cpp
    kalziumglpart.cpp
    )
  include_directories( ${OPENBABEL2_INCLUDE_DIR} )

  kde4_automoc(${compoundviewer_SRCS})
  add_subdirectory(widgets)

  kde4_add_library(compoundviewer SHARED ${compoundviewer_SRCS})

  target_link_libraries(compoundviewer ${OPENBABEL2_LIBRARIES} ${QT_QTOPENGL_LIBRARY} ${KDE4_KPARTS_LIBS} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} )
endif(OPENBABEL2_FOUND AND OPENGL_FOUND AND EIGEN_FOUND)

set_target_properties(compoundviewer PROPERTIES VERSION 4.0.0 SOVERSION 4 )

install(TARGETS compoundviewer  DESTINATION ${LIB_INSTALL_DIR} )

