include_directories(
   ${CMAKE_SOURCE_DIR}/kalzium/src
   ${CMAKE_BINARY_DIR}/kalzium/src
)
if(OPENBABEL2_FOUND)
	include_directories( ${OPENBABEL2_INCLUDE_DIR} )
endif(OPENBABEL2_FOUND)

set(kalziumui_PART_SRCS 
   ${CMAKE_SOURCE_DIR}/kalzium/src/kalziumglwidget.cpp
   )

KDE4_ADD_WIDGET_FILES(kalziumui_PART_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/kalziumui.widgets)


kde4_add_plugin(kalziumuiwidgets ${kalziumui_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} kalziumuiwidgets )

target_link_libraries(kalziumuiwidgets ${OPENBABEL2_LIBRARIES} ${QT_QTOPENGL_LIBRARY} ${QT_QTDESIGNER_LIBRARY} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${KDE4_KDEUI_LIBS} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})

install_targets(${PLUGIN_INSTALL_DIR}/plugins/designer kalziumuiwidgets)

