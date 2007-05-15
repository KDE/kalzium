add_subdirectory(tools)

include_directories(
   ${CMAKE_SOURCE_DIR}/libkdeedu/libscience/
   ${CMAKE_SOURCE_DIR}/kalzium/compoundviewer/
   ${CMAKE_SOURCE_DIR}/libkdeedu/kdeeduui/
   ${CMAKE_SOURCE_DIR}/kalzium/src/tools/
   ${CMAKE_CURRENT_BINARY_DIR}/..
)

########### next target ###############

#the tools can only be build (right now) when OpenBabel2 is
#found. The moleculeviewer needs not only OpenBabel2 but
#also Eigen and OpenGL, so there is a nested if-check
if(OPENBABEL2_FOUND)
	set(kalziumtools_SRCS
		tools/obconverter.cpp
		tools/obsupportedformat.cpp
	)
	
	kde4_add_ui_files(kalziumtools_SRCS
		tools/obconverterwidget.ui
	)
	include_directories( ${OPENBABEL2_INCLUDE_DIR} )
	
	if(OPENGL_FOUND AND EIGEN_FOUND)
	
# to enable libavogadro building, uncomment the following line:
	        add_subdirectory(libavogadro-kalzium)
	
		set(kalziumtools_SRCS
			${kalziumtools_SRCS}
			tools/moleculeview.cpp
			)
		kde4_add_ui_files(kalziumtools_SRCS
			tools/moleculeviewerwidget.ui
			)
		include_directories( ${EIGEN_INCLUDE_DIR} )
	endif(OPENGL_FOUND AND EIGEN_FOUND)
endif(OPENBABEL2_FOUND)

set(kalzium_SRCS
	${kalziumtools_SRCS}
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
   legendwidget.cpp
   tableinfowidget.cpp
)

if(LIBFACILE_FOUND)
  include(CMakeOCamlInstructions.cmake)

  set( kalzium_SRCS
    ${kalzium_SRCS}
    eqchemview.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/solver.o
    ${CMAKE_CURRENT_BINARY_DIR}/modwrap.o
    )
  kde4_add_ui_files(kalzium_SRCS
    equationview.ui
    )
endif(LIBFACILE_FOUND)

kde4_automoc(${kalzium_SRCS})

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
   timewidget.ui
)

kde4_add_kcfg_files(kalzium_SRCS prefs.kcfgc )

kde4_add_executable(kalzium ${kalzium_SRCS} )

target_link_libraries(kalzium ${KDE4_KDEUI_LIBS} ${KDE4_KHTML_LIBRARY} ${KDE4_KUTILS_LIBRARY} science kdeeduui )


if (OPENBABEL2_FOUND AND OPENGL_FOUND AND EIGEN_FOUND)
    target_link_libraries(kalzium ${OPENBABEL2_LIBRARIES} ${QT_QTOPENGL_LIBRARY} compoundviewer)
endif (OPENBABEL2_FOUND AND OPENGL_FOUND AND EIGEN_FOUND)

if(LIBFACILE_FOUND)
   link_directories (${OCAMLC_DIR})
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

   target_link_libraries(kalzium ${kalzium_EXTRA_LIBS} m dl)
endif(LIBFACILE_FOUND)	

install(TARGETS kalzium  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES kalzium.desktop  DESTINATION  ${XDG_APPS_DIR} )
install( FILES kalzium.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kalziumui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kalzium )

kde4_install_icons( ${ICON_INSTALL_DIR}   )
