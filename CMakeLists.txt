project(kalzium)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config-kalzium.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kalzium.h )

add_subdirectory(src)
add_subdirectory(data)
#add_subdirectory(simulations)
