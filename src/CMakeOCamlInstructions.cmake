link_directories (${OCAMLC_DIR})

macro(OCAML_MLI_TO_CMI _cmi _mli)
   add_custom_command(OUTPUT ${_cmi}
                      COMMAND ${OCAML_OCAMLC_EXECUTABLE} ARGS -I +facile -c ${_mli}
                      DEPENDS ${ARGN}
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/solver/)
endmacro(OCAML_MLI_TO_CMI)

macro(OCAML_ML_TO_CMX _cmx _ml)
   add_custom_command(OUTPUT ${_cmx}
                   COMMAND ${OCAML_OCAMLOPT_EXECUTABLE} ARGS -I +facile -c ${_ml}
                   DEPENDS ${ARGN}
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/solver/)
endmacro(OCAML_ML_TO_CMX)

ocaml_mli_to_cmi(${CMAKE_CURRENT_SOURCE_DIR}/solver/chemset.cmi ${CMAKE_CURRENT_SOURCE_DIR}/solver/chemset.mli
                 ${CMAKE_CURRENT_SOURCE_DIR}/solver/calc.mli  )

ocaml_ml_to_cmx(${CMAKE_CURRENT_SOURCE_DIR}/solver/chemset.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/chemset.ml
                ${CMAKE_CURRENT_SOURCE_DIR}/solver/chemset.cmi  )

ocaml_mli_to_cmi(${CMAKE_CURRENT_SOURCE_DIR}/solver/datastruct.cmi ${CMAKE_CURRENT_SOURCE_DIR}/solver/datastruct.mli
                 ${CMAKE_CURRENT_SOURCE_DIR}/solver/datastruct.mli )

ocaml_ml_to_cmx(${CMAKE_CURRENT_SOURCE_DIR}/solver/datastruct.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/datastruct.ml
                ${CMAKE_CURRENT_SOURCE_DIR}/solver/datastruct.cmi ${CMAKE_CURRENT_SOURCE_DIR}/solver/chemset.cmx  )


ocaml_mli_to_cmi(${CMAKE_CURRENT_SOURCE_DIR}/solver/chem.cmi ${CMAKE_CURRENT_SOURCE_DIR}/solver/chem.mli
                 ${CMAKE_CURRENT_SOURCE_DIR}/solver/chem.mli  )

ocaml_ml_to_cmx(${CMAKE_CURRENT_SOURCE_DIR}/solver/chem.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/chem.ml
                ${CMAKE_CURRENT_SOURCE_DIR}/solver/chem.cmi ${CMAKE_CURRENT_SOURCE_DIR}/solver/chemset.cmx  )


ocaml_mli_to_cmi(${CMAKE_CURRENT_SOURCE_DIR}/solver/parser.cmi ${CMAKE_CURRENT_SOURCE_DIR}/solver/parser.mli
                 ${CMAKE_CURRENT_SOURCE_DIR}/solver/parser.mli  )

ocaml_ml_to_cmx(${CMAKE_CURRENT_SOURCE_DIR}/solver/parser.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/parser.ml
                ${CMAKE_CURRENT_SOURCE_DIR}/solver/parser.cmi ${CMAKE_CURRENT_SOURCE_DIR}/solver/chemset.cmx )

ocaml_ml_to_cmx(${CMAKE_CURRENT_SOURCE_DIR}/solver/lexer.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/lexer.ml
                ${CMAKE_CURRENT_SOURCE_DIR}/solver/chemset.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/parser.cmx  )


ocaml_mli_to_cmi(${CMAKE_CURRENT_SOURCE_DIR}/solver/calc.cmi ${CMAKE_CURRENT_SOURCE_DIR}/solver/calc.mli
                 ${CMAKE_CURRENT_SOURCE_DIR}/solver/chemset.cmi ${CMAKE_CURRENT_SOURCE_DIR}/solver/chemset.cmx  ${CMAKE_CURRENT_SOURCE_DIR}/solver/calc.mli ${CMAKE_CURRENT_SOURCE_DIR}/solver/datastruct.mli ${CMAKE_CURRENT_SOURCE_DIR}/solver/datastruct.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/chem.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/chem.mli ${CMAKE_CURRENT_SOURCE_DIR}/solver/parser.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/parser.mli ${CMAKE_CURRENT_SOURCE_DIR}/solver/lexer.cmx)

ocaml_ml_to_cmx(${CMAKE_CURRENT_SOURCE_DIR}/solver/calc.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/calc.ml
                ${CMAKE_CURRENT_SOURCE_DIR}/solver/calc.cmi )

ocaml_ml_to_cmx(${CMAKE_CURRENT_BINARY_DIR}/solver/lexer.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/lexer.ml
                ${CMAKE_CURRENT_SOURCE_DIR}/solver/parser.ml ${CMAKE_CURRENT_SOURCE_DIR}/solver/lexer.ml )

# object files

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/solver/modwrap.o
                   COMMAND ${OCAMLOPT_EXECUTABLE} -I +facile -c ${CMAKE_CURRENT_SOURCE_DIR}/solver/modwrap.c
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/solver/modwrap.c ${CMAKE_CURRENT_SOURCE_DIR}/solver/solver.o
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/solver)

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/solver/solver.o
                   COMMAND ${OCAMLOPT_EXECUTABLE} -output-obj -o ${CMAKE_CURRENT_SOURCE_DIR}/solver/solver.o ${LIBFACILE_INCLUDE_DIR}/facile.cmxa  ${CMAKE_CURRENT_SOURCE_DIR}/solver/chemset.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/parser.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/lexer.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/datastruct.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/chem.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/calc.cmx
                   DEPENDS ${LIBFACILE_INCLUDE_DIR}/facile.cmxa  ${CMAKE_CURRENT_SOURCE_DIR}/solver/chemset.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/parser.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/lexer.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/datastruct.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/chem.cmx ${CMAKE_CURRENT_SOURCE_DIR}/solver/calc.cmx
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})



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

