project(solver)

#create depend file need to generate .o file
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/.depend
                   COMMAND ${OCAMLDEP_EXECUTABLE}  ARGS -I +facile *.mli *.ml
                   DEPENDS calc.mli
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})


macro(GENERATE_OCALM_FILE _file_name)
	add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${_file_name}.cmi
                   COMMAND ${OCAMLC_EXECUTABLE}  ARGS -I +facile -c ${_file_name}.mli
                   DEPENDS ${_file_name}.mli
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

        add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${_file_name}.cmx
                   COMMAND ${OCAMLOPT_EXECUTABLE}  ARGS -I +facile -c ${_file_name}.ml
                   DEPENDS ${_file_name}.cmi
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endmacro(GENERATE_OCALM_FILE _file_name)       	


set(OCAML_FILE chemset parser datastruct chem calc)

foreach (_ocaml_file ${OCAML_FILE})
	GENERATE_OCALM_FILE(${_ocaml_file})
endforeach(_ocaml_file ${OCAML_FILE})

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lexer.cmx
                   COMMAND ${OCAMLOPT_EXECUTABLE}  ARGS -I +facile -c lexer.ml
                   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/parser.ml lexer.ml
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/modwrap.o
                   COMMAND ${OCAMLOPT_EXECUTABLE}  ARGS -I +facile -c modwrap.c
                   DEPENDS modwrap.c
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/solver.o
                   COMMAND ${OCAMLOPT_EXECUTABLE}  ARGS -output-obj -o ${CMAKE_CURRENT_BINARY_DIR}/solver.o ${LIBFACILE_INCLUDE_DIR}/facile.cmxa  ${CMAKE_CURRENT_BINARY_DIR}/chemset.cmx ${CMAKE_CURRENT_BINARY_DIR}/parser.cmx ${CMAKE_CURRENT_BINARY_DIR}/lexer.cmx ${CMAKE_CURRENT_BINARY_DIR}/datastruct.cmx ${CMAKE_CURRENT_BINARY_DIR}/chem.cmx ${CMAKE_CURRENT_BINARY_DIR}/calc.cmx
                   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/solver.o ${LIBFACILE_INCLUDE_DIR}/facile.cmxa  ${CMAKE_CURRENT_BINARY_DIR}/chemset.cmx ${CMAKE_CURRENT_BINARY_DIR}/parser.cmx ${CMAKE_CURRENT_BINARY_DIR}/lexer.cmx ${CMAKE_CURRENT_BINARY_DIR}/datastruct.cmx ${CMAKE_CURRENT_BINARY_DIR}/chem.cmx ${CMAKE_CURRENT_BINARY_DIR}/calc.cmx
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})


if(KDE4_BUILD_TESTS)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/main.cmx
                   COMMAND ${OCAMLOPT_EXECUTABLE}  ARGS -I +facile -c main.ml
                   DEPENDS main.ml
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set(atestprog.opt_SRCS main.cmx ${CMAKE_CURRENT_BINARY_DIR}/chemset.cmx ${CMAKE_CURRENT_BINARY_DIR}/parser.cmx ${CMAKE_CURRENT_BINARY_DIR}/lexer.cmx ${CMAKE_CURRENT_BINARY_DIR}/datastruct.cmx ${CMAKE_CURRENT_BINARY_DIR}/chem.cmx ${CMAKE_CURRENT_BINARY_DIR}/calc.cmx  )

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/atestprog.opt
                   COMMAND ${OCAMLOPT_EXECUTABLE}  ARGS -o ${atestprog.opt_SRCS}
                   DEPENDS ${atestprog.opt_SRCS}
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

	   #install( PROGRAMS atestprog.opt DESTINATION  ${BIN_INSTALL_DIR} )
endif(KDE4_BUILD_TESTS)
