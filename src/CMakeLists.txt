include_directories(
   ${CMAKE_SOURCE_DIR}/libkdeedu/kdeeduplot/
   ${CMAKE_SOURCE_DIR}/libkdeedu/libscience/
   ${CMAKE_SOURCE_DIR}/libkdeedu/kdeeduui/
   ${CMAKE_CURRENT_BINARY_DIR}/..
)

########### next target ###############

set(kalzium_SRCS
   kalzium.cpp
   main.cpp
   periodictableview.cpp
   detailinfodlg.cpp
   tablesdialog.cpp
   orbitswidget.cpp
   elementdataviewer.cpp
   detailedgraphicaloverview.cpp
   molcalcwidget.cpp
   somwidget_impl.cpp
   timewidget_impl.cpp
   kalziumdataobject.cpp
   spectrumwidget.cpp
   nuclideboard.cpp
   exporter.cpp
   spectrumviewimpl.cpp
   kalziumutils.cpp
   kalziumgradienttype.cpp
   kalziumnumerationtype.cpp
   kalziumpainter.cpp
   kalziumschemetype.cpp
   kalziumtabletype.cpp
   search.cpp
   searchwidget.cpp
   rsdialog.cpp
)

if(LIBFACILE_FOUND)
  set( kalzium_SRCS
    ${kalzium_SRCS}
    eqresult.cpp
    eqchemview.cpp
    )
  kde4_add_ui_files(kalzium_SRCS
    equationview.ui
    )
  add_subdirectory(solver)
endif(LIBFACILE_FOUND)

if(OPENBABEL2_FOUND AND OPENGL_FOUND)
  set( kalzium_SRCS
    ${kalzium_SRCS}
    moleculeview.cpp
    latticedialog.cpp
    kalziumglhelperclasses.cpp
    kalziumglwidget.cpp
    openbabel2wrapper.cpp
    )
  kde4_add_ui_files(kalzium_SRCS
    latticeviewer.ui
    )
  include_directories( ${OPENBABEL2_INCLUDE_DIR} )
  add_subdirectory(widgets)
endif(OPENBABEL2_FOUND AND OPENGL_FOUND)

kde4_automoc(kalzium ${kalzium_SRCS})

kde4_add_ui_files(kalzium_SRCS
   settings_colors.ui
   settings_elementsymbol.ui
   plotsetupwidget.ui
   molcalcwidgetbase.ui
   somwidget.ui
   spectrumview.ui
   settings_misc.ui
   settings_units.ui
   rswidget.ui
   moleculeviewerwidget.ui
   timewidget.ui
)

kde4_add_kcfg_files(kalzium kalzium_SRCS prefs.kcfgc )

kde4_add_executable(kalzium ${kalzium_SRCS})

target_link_libraries(kalzium ${KDE4_KDEUI_LIBS} ${KDE4_KHTML_LIBRARY} ${KDE4_KUTILS_LIBRARY} science kdeeduplot kdeeduui)

if (OPENBABEL2_FOUND AND OPENGL_FOUND)
   target_link_libraries(kalzium ${OPENBABEL2_LIBRARIES} ${QT_QTOPENGL_LIBRARY})
endif (OPENBABEL2_FOUND AND OPENGL_FOUND)

install(TARGETS kalzium  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES kalzium.desktop  DESTINATION  ${XDG_APPS_DIR} )
install( FILES kalzium.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kalziumui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kalzium )

kde4_install_icons( ${ICON_INSTALL_DIR}   )
