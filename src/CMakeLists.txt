include_directories(
   ${CMAKE_SOURCE_DIR}/libkdeedu/libscience/
   ${CMAKE_SOURCE_DIR}/libkdeedu/kdeeduui/
   ../compoundviewer
   tools
   calculator
   isotopetable
   ${CMAKE_CURRENT_BINARY_DIR}/..
   )

########### next target ###############

# The tools can only be built when OpenBabel2 is found.
# The moleculeviewer needs not only OpenBabel2 but also Eigen2 and Avogadro,
# so there is a nested if-check.
if (OPENBABEL2_FOUND)
   set(kalziumtools_SRCS
      tools/obconverter.cpp
      )

   kde4_add_ui_files(kalziumtools_SRCS
      tools/obconverterwidget.ui
      )
   include_directories(${OPENBABEL2_INCLUDE_DIR})

   if (EIGEN2_FOUND AND Avogadro_FOUND)

      include(${Avogadro_USE_FILE})

      set(kalziumtools_SRCS
         ${kalziumtools_SRCS}
         tools/moleculeview.cpp
         ../compoundviewer/kalziumglwidget.cpp
         )
      kde4_add_ui_files(kalziumtools_SRCS
         tools/moleculeviewerwidget.ui
         )
      include_directories(${EIGEN2_INCLUDE_DIR})
   endif (EIGEN2_FOUND AND Avogadro_FOUND)
endif (OPENBABEL2_FOUND)

## Kalzium calculator files
set(kalziumtools_SRCS
   ${kalziumtools_SRCS}
   calculator/calculator.cpp
   calculator/nuclearCalculator.cpp
   calculator/gasCalculator.cpp
   calculator/concCalculator.cpp
   calculator/titrationCalculator.cpp
#   calculator/massCalculator.cpp
   )
kde4_add_ui_files(kalziumtools_SRCS
   calculator/calculator.ui
   calculator/nuclearCalculator.ui
   calculator/gasCalculator.ui
   calculator/concCalculator.ui
   calculator/settings_calc.ui
   calculator/titrationCalculator.ui
#   calculator/massCalculator.ui
   )

set(kalzium_SRCS
   ${kalziumtools_SRCS}
   isotopetable/informationitem.cpp
   isotopetable/isotopeguideview.cpp
   isotopetable/isotopeitem.cpp
   isotopetable/isotopescene.cpp
   isotopetable/isotopetabledialog.cpp
   isotopetable/isotopeview.cpp
   kalzium.cpp
   main.cpp
   detailinfodlg.cpp
   tablesdialog.cpp
   detailedgraphicaloverview.cpp
   orbitswidget.cpp
   elementdataviewer.cpp
   molcalcwidget.cpp
   gradientwidget_impl.cpp
   kalziumdataobject.cpp
   kalziumutils.cpp
   kalziumgradienttype.cpp
   kalziumnumerationtype.cpp
   kalziumelementproperty.cpp
   kalziumschemetype.cpp
   search.cpp
   searchwidget.cpp
   rsdialog.cpp
   legendwidget.cpp
   exportdialog.cpp
   tableinfowidget.cpp
   spectrumviewimpl.cpp
   spectrumwidget.cpp
   periodsystembase.cpp
   periodictablescene_p.cpp
   elementitem.cpp
   statemachine.cpp
   numerationitem.cpp
   )

if (LIBFACILE_FOUND)
   include(CMakeOCamlInstructions.cmake)

   set(kalzium_SRCS
      ${kalzium_SRCS}
      eqchemview.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/solver.o
      ${CMAKE_CURRENT_BINARY_DIR}/modwrap.o
      )
   kde4_add_ui_files(kalzium_SRCS
      equationview.ui
      )
endif (LIBFACILE_FOUND)

kde4_add_ui_files(kalzium_SRCS
   isotopetable/isotopedialog.ui
   settings_colors.ui
   plotsetupwidget.ui
   molcalcwidgetbase.ui
   spectrumview.ui
#    settings_misc.ui
#    settings_web.ui
   settings_units.ui
   settings_gradients.ui
   rswidget.ui
   gradientwidget.ui
   exportdialog.ui
   )

kde4_add_kcfg_files(kalzium_SRCS prefs.kcfgc )

kde4_add_app_icon(kalzium_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/hi*-app-kalzium.png")

kde4_add_executable(kalzium ${kalzium_SRCS})

target_link_libraries(kalzium ${KDE4_KHTML_LIBRARY} ${KDE4_KNEWSTUFF3_LIBS} ${KDE4_KUNITCONVERSION_LIBS} ${QT_QTSCRIPT_LIBRARIES} science kdeeduui)

if (OPENBABEL2_FOUND)
   target_link_libraries(kalzium ${OPENBABEL2_LIBRARIES})
   if (EIGEN2_FOUND AND Avogadro_FOUND)
      target_link_libraries(kalzium compoundviewer avogadro)
   endif (EIGEN2_FOUND AND Avogadro_FOUND)
endif (OPENBABEL2_FOUND)

if (LIBFACILE_FOUND)
   link_directories(${OCAMLC_DIR})
   set(CMAKE_LIBRARY_PATH ${OCAMLC_DIR})
   set(kalzium_EXTRA_LIBS)

   find_library(OCAML_ASMRUN_LIBRARY NAMES asmrun)
   if (OCAML_ASMRUN_LIBRARY)
      set(kalzium_EXTRA_LIBS ${kalzium_EXTRA_LIBS} ${OCAML_ASMRUN_LIBRARY})
   endif (OCAML_ASMRUN_LIBRARY)

   find_library(OCAML_STR_LIBRARY NAMES str)
   if (OCAML_STR_LIBRARY)
      set(kalzium_EXTRA_LIBS ${kalzium_EXTRA_LIBS} ${OCAML_STR_LIBRARY})
   endif (OCAML_STR_LIBRARY)

   find_library(OCAML_NUMS_LIBRARY NAMES nums)
   if (OCAML_NUMS_LIBRARY)
      set(kalzium_EXTRA_LIBS ${kalzium_EXTRA_LIBS} ${OCAML_NUMS_LIBRARY})
   endif (OCAML_NUMS_LIBRARY)

   target_link_libraries(kalzium ${kalzium_EXTRA_LIBS} m ${CMAKE_DL_LIBS})
endif (LIBFACILE_FOUND)

install(TARGETS kalzium ${INSTALL_TARGETS_DEFAULT_ARGS})

########### install files ###############

install(PROGRAMS kalzium.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES kalzium.kcfg DESTINATION ${KCFG_INSTALL_DIR})
install(FILES kalziumui.rc DESTINATION ${DATA_INSTALL_DIR}/kalzium)
install(FILES kalzium.knsrc DESTINATION ${CONFIG_INSTALL_DIR})

kde4_install_icons(${ICON_INSTALL_DIR})
