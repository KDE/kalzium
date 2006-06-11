
include_directories(
   ${CMAKE_SOURCE_DIR}/libkdeedu/kdeeduplot/
   ${CMAKE_SOURCE_DIR}/libkdeedu/libscience/
   ${CMAKE_SOURCE_DIR}/libkdeedu/kdeeduui/
   ${CMAKE_CURRENT_BINARY_DIR}/..
)

add_definitions(-D_ISOC99_SOURCE)

########### next target ###############

set(kalzium_SRCS
   kalzium.cpp
   main.cpp
   periodictableview.cpp
   detailinfodlg.cpp
   tablesdialog.cpp
   orbitswidget.cpp
   plotwidget.cpp
   elementdataviewer.cpp
   detailedgraphicaloverview.cpp
   molcalcwidget.cpp
   somwidget_impl.cpp
   kalziumdataobject.cpp
   spectrumwidget.cpp
   nuclideboard.cpp
   exporter.cpp
   spectrumviewimpl.cpp
   kalziumutils.cpp
   simplecrystalviewer.cpp
   kalziumgradienttype.cpp
   kalziumnumerationtype.cpp
   kalziumpainter.cpp
   kalziumschemetype.cpp
   kalziumtabletype.cpp
   propertysliderwidget.cpp
   printwidget.cpp
   search.cpp
   searchwidget.cpp
   rsdialog.cpp
   moleculeview.cpp
   kalziumglwidget.cpp
)

kde4_automoc(${kalzium_SRCS})

kde4_add_ui_files(kalzium_SRCS
   settings_colors.ui
   settings_elementsymbol.ui
   plotsetupwidget.ui
   molcalcwidgetbase.ui
   somwidget.ui
   spectrumview.ui
   printwidgetbase.ui
   propertysliderwidgetbase.ui
   settings_misc.ui
   settings_units.ui
   rswidget.ui
)

kde4_add_kcfg_files(kalzium_SRCS prefs.kcfgc )

kde4_add_executable(kalzium ${kalzium_SRCS})

target_link_libraries(kalzium ${KDE4_KDEUI_LIBS} ${KDE4_KHTML_LIBRARY} ${KDE4_KUTILS_LIBRARY} ${QT_QTOPENGL_LIBRARY} science kdeeduplot kdeeduui)

install_targets(/bin kalzium )


########### install files ###############

install_files( ${XDG_APPS_DIR} FILES kalzium.desktop )
install_files( ${KCFG_INSTALL_DIR} FILES kalzium.kcfg )
install_files( ${DATA_INSTALL_DIR}/kalzium FILES kalziumui.rc )

kde4_install_icons( ${ICON_INSTALL_DIR}  hicolor )

