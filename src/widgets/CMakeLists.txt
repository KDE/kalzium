if(OPENBABEL2_FOUND)
	include_directories( ${OPENBABEL2_INCLUDE_DIR} )
endif(OPENBABEL2_FOUND)

set(kalziumui_PART_SRCS
		kalziumglplugin.cpp
		kalziumglwidget.cpp
   )

KDE4_ADD_WIDGET_FILES(${kalziumui_PART_SRCS} kalzium.widgets)

kde4_add_plugin(kalziumuiwidgets ${kalziumui_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} kalziumuiwidgets )

target_link_libraries(kalziumuiwidgets ${OPENBABEL2_LIBRARIES} ${QT_QTOPENGL_LIBRARY} ${QT_QTDESIGNER_LIBRARY} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})

install_targets(${PLUGIN_INSTALL_DIR}/plugins/designer kalziumuiwidgets)

kde4_automoc(${kalziumui_PART_SRCS})
