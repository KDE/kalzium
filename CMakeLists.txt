project(kalzium)

include(KalziumConfigureChecks.cmake)

if(OPENBABEL2_FOUND AND OPENGL_FOUND AND EIGEN_FOUND)
	add_subdirectory(compoundviewer)
	add_subdirectory(libavogadro-kalzium)
endif(OPENBABEL2_FOUND AND OPENGL_FOUND AND EIGEN_FOUND)


add_subdirectory(src)
add_subdirectory(data)
#add_subdirectory(simulations)
