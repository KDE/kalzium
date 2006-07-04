
include_directories( 
	${CMAKE_SOURCE_DIR}/kalzium/src/
	${CMAKE_SOURCE_DIR}/libkdeedu/kdeeduplot/ 
	${CMAKE_SOURCE_DIR}/libkdeedu/libscience/ 
	${CMAKE_SOURCE_DIR}/libkdeedu/kdeeduui/ )


########### next target ###############

set(simulations_SRCS 
			main.cpp
			field.cpp
			gamecontrols_impl.cpp
			maindialog_impl.cpp
			simulation.cpp
			simulationfield.cpp
			simulationsdialog.cpp
			simulationsimplementations.cpp
			stone.cpp )

kde4_add_ui_files(simulations_SRCS 
		boltzmannform.ui
		crystalform.ui
		decompositionform.ui
		gamecontrols.ui
		lightabsorptionform.ui
		maindialog.ui
		raform.ui
		settings.ui
		volterraform.ui )

kde4_automoc(${simulations_SRCS})

kde4_add_executable(kalzium-simulations ${simulations_SRCS})

install_targets(/bin kalzium-simulations )
target_link_libraries(kalzium-simulations ${KDE4_KIO_LIBS})


########### install files ###############

install( FILES kalzium-simulations.desktop  DESTINATION  ${XDG_APPS_DIR} )


