include_directories(
   ${CMAKE_SOURCE_DIR}/libkdeedu/kdeeduplot/
   ${CMAKE_SOURCE_DIR}/libkdeedu/libscience/
   ${CMAKE_SOURCE_DIR}/libkdeedu/kdeeduui/
   ${CMAKE_CURRENT_BINARY_DIR}/..
)
if(LIBFACILE_FOUND)
link_directories (${OCAMLC_DIR})
#create depend file need to generate .o file
add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/solver/.depend
                   COMMAND ${OCAMLDEP_EXECUTABLE}  ARGS -I +facile ${CMAKE_CURRENT_SOURCE_DIR}/solver/*.mli ${CMAKE_CURRENT_SOURCE_DIR}/solver/*.ml > ${CMAKE_CURRENT_SOURCE_DIR}/solver/.depend
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/solver)

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/solver/chemset.cmi
                   COMMAND ${OCAMLC_EXECUTABLE}  ARGS -I +facile -c ${CMAKE_CURRENT_SOURCE_DIR}/solver/chemset.mli
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/solver/calc.mli ${CMAKE_CURRENT_SOURCE_DIR}/solver/.depend
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/solver/)

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/solver/chemset.cmx
                   COMMAND ${OCAMLOPT_EXECUTABLE}  ARGS -I +facile -c ${CMAKE_CURRENT_SOURCE_DIR}/solver/chemset.ml
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/solver/chemset.cmi ${CMAKE_CURRENT_SOURCE_DIR}/solver/.depend
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/solver/)

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/solver/datastruct.cmi
                   COMMAND ${OCAMLC_EXECUTABLE}  ARGS -I +facile -c ${CMAKE_CURRENT_SOURCE_DIR}/solver/datastruct.mli
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/solver/datastruct.mli ${CMAKE_CURRENT_SOURCE_DIR}/solver/.depend
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/solver/)

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/solver/datastruct.cmx
                   COMMAND ${OCAMLOPT_EXECUTABLE}  ARGS -I +facile -c ${CMAKE_CURRENT_SOURCE_DIR}/solver/datastruct.ml
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/solver/datastruct.cmi ${CMAKE_CURRENT_SOURCE_DIR}/solver/chemset.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/.depend
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/solver/)


add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/solver/chem.cmi
                   COMMAND ${OCAMLC_EXECUTABLE}  ARGS -I +facile -c ${CMAKE_CURRENT_SOURCE_DIR}/solver/chem.mli
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/solver/chem.mli ${CMAKE_CURRENT_SOURCE_DIR}/solver/.depend
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/solver/)

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/solver/chem.cmx
                   COMMAND ${OCAMLOPT_EXECUTABLE}  ARGS -I +facile -c ${CMAKE_CURRENT_SOURCE_DIR}/solver/chem.ml
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/solver/chem.cmi ${CMAKE_CURRENT_SOURCE_DIR}/solver/chemset.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/.depend
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/solver/)


add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/solver/parser.cmi
                   COMMAND ${OCAMLC_EXECUTABLE}  ARGS -I +facile -c ${CMAKE_CURRENT_SOURCE_DIR}/solver/parser.mli
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/solver/parser.mli ${CMAKE_CURRENT_SOURCE_DIR}/solver/.depend
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/solver/)

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/solver/parser.cmx
                   COMMAND ${OCAMLOPT_EXECUTABLE}  ARGS -I +facile -c ${CMAKE_CURRENT_SOURCE_DIR}/solver/parser.ml
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/solver/parser.cmi ${CMAKE_CURRENT_SOURCE_DIR}/solver/chemset.cmx
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/solver/)

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/solver/lexer.cmx
                   COMMAND ${OCAMLOPT_EXECUTABLE}  ARGS -I +facile -c ${CMAKE_CURRENT_SOURCE_DIR}/solver/lexer.ml
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/solver/chemset.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/parser.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/.depend
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/solver/)


add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/solver/calc.cmi
                   COMMAND ${OCAMLC_EXECUTABLE}  ARGS -I +facile -c ${CMAKE_CURRENT_SOURCE_DIR}/solver/calc.mli
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/solver/chemset.cmi ${CMAKE_CURRENT_SOURCE_DIR}/solver/chemset.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/.depend ${CMAKE_CURRENT_SOURCE_DIR}/solver/calc.mli ${CMAKE_CURRENT_SOURCE_DIR}/solver/datastruct.mli ${CMAKE_CURRENT_SOURCE_DIR}/solver/datastruct.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/chem.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/chem.mli ${CMAKE_CURRENT_SOURCE_DIR}/solver/parser.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/parser.mli ${CMAKE_CURRENT_SOURCE_DIR}/solver/lexer.cmx
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/solver)

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/solver/calc.cmx
                   COMMAND ${OCAMLOPT_EXECUTABLE}  ARGS -I +facile -c ${CMAKE_CURRENT_SOURCE_DIR}/solver/calc.ml
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/solver/calc.cmi
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/solver)



add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/solver/lexer.cmx
                   COMMAND ${OCAMLOPT_EXECUTABLE}  ARGS -I +facile -c lexer.ml
                   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/solver/parser.ml solver/lexer.ml
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/solver/modwrap.o
                   COMMAND ${OCAMLOPT_EXECUTABLE}  ARGS -I +facile -c modwrap.c
                   DEPENDS solver/modwrap.c
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/solver/solver.o
                   COMMAND ${OCAMLOPT_EXECUTABLE}  ARGS -output-obj -o ${CMAKE_CURRENT_SOURCE_DIR}/solver/solver.o ${LIBFACILE_INCLUDE_DIR}/facile.cmxa  ${CMAKE_CURRENT_SOURCE_DIR}/solver/chemset.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/parser.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/lexer.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/datastruct.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/chem.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/calc.cmx
                   DEPENDS ${LIBFACILE_INCLUDE_DIR}/facile.cmxa  ${CMAKE_CURRENT_SOURCE_DIR}/solver/chemset.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/parser.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/lexer.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/datastruct.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/chem.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/calc.cmx
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})


if(KDE4_BUILD_TESTS)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/solver/main.cmx
                   COMMAND ${OCAMLOPT_EXECUTABLE}  ARGS -I +facile -c main.ml
                   DEPENDS solver/main.ml
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set(atestprog.opt_SRCS main.cmx ${CMAKE_CURRENT_BINARY_DIR}/chemset.cmx ${CMAKE_CURRENT_BINARY_DIR}/parser.cmx ${CMAKE_CURRENT_BINARY_DIR}/lexer.cmx ${CMAKE_CURRENT_BINARY_DIR}/datastruct.cmx ${CMAKE_CURRENT_BINARY_DIR}/chem.cmx ${CMAKE_CURRENT_BINARY_DIR}/calc.cmx  )

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/solver/atestprog.opt
                   COMMAND ${OCAMLOPT_EXECUTABLE}  ARGS -o ${atestprog.opt_SRCS}
                   DEPENDS ${atestprog.opt_SRCS}
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

	   #install( PROGRAMS atestprog.opt DESTINATION  ${BIN_INSTALL_DIR} )
endif(KDE4_BUILD_TESTS)
endif(LIBFACILE_FOUND)

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
    ${CMAKE_CURRENT_SOURCE_DIR}/solver/solver.o
    )
  kde4_add_ui_files(kalzium_SRCS
    equationview.ui
    )
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

kde4_add_executable(kalzium ${kalzium_SRCS} )

target_link_libraries(kalzium ${KDE4_KDEUI_LIBS} ${KDE4_KHTML_LIBRARY} ${KDE4_KUTILS_LIBRARY} science kdeeduplot kdeeduui )

if(LIBFACILE_FOUND)
   target_link_libraries(kalzium asmrun str nums m dl)
endif(LIBFACILE_FOUND)	

if (OPENBABEL2_FOUND AND OPENGL_FOUND)
   target_link_libraries(kalzium ${OPENBABEL2_LIBRARIES} ${QT_QTOPENGL_LIBRARY})
endif (OPENBABEL2_FOUND AND OPENGL_FOUND)

install(TARGETS kalzium  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES kalzium.desktop  DESTINATION  ${XDG_APPS_DIR} )
install( FILES kalzium.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kalziumui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kalzium )

kde4_install_icons( ${ICON_INSTALL_DIR}   )
