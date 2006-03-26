kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


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
			stone.cpp
			)




kde4_add_ui_files(simulations_SRCS 
		boltzmannform.ui
		crystalform.ui
		decompositionform.ui
		gamecontrols.ui
		lightabsorptionform.ui
		maindialog.ui
		raform.ui
		settings.ui
		volterraform.ui
   )

kde4_automoc(${simulations_SRCS})

kde4_add_executable(kalzium-simulations ${simulations_SRCS})

install_targets(/bin kalzium-simulations )
target_link_libraries(kalzium-simulations )


########### install files ###############

install_files( ${XDG_APPS_DIR} FILES kalzium-simulations.desktop )

kde4_footer()

