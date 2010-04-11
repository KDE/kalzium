include_directories(
   ${CMAKE_SOURCE_DIR}/..
   ${CMAKE_CURRENT_BINARY_DIR}/..
   ${CMAKE_CURRENT_SOURCE_DIR}/..
)

include_directories( ${OPENBABEL2_INCLUDE_DIR} ..)

set(kalziumui_PART_SRCS 
# nothing left here!
   )

KDE4_ADD_WIDGET_FILES(kalziumui_PART_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/kalziumui.widgets)

kde4_add_plugin(kalziumuiwidgets ${kalziumui_PART_SRCS})

target_link_libraries(kalziumuiwidgets ${OPENBABEL2_LIBRARIES} ${QT_QTDESIGNER_LIBRARY} ${KDE4_KDECORE_LIBS})

install(TARGETS kalziumuiwidgets DESTINATION ${PLUGIN_INSTALL_DIR}/plugins/designer )

