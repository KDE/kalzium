set(QT_USE_QTOPENGL true)

# Eigen 1 's performance degrades much when asserts are turned on.
# So, turn them off in libavogadro, unless we are in debugfull mode.
STRING(TOLOWER ${CMAKE_BUILD_TYPE} MY_CMAKE_BUILD_TYPE_TOLOWER)
if(MY_CMAKE_BUILD_TYPE_TOLOWER MATCHES debugfull)
  message("-- debugfull mode, so keeping asserts enabled in libavogadro")
else(MY_CMAKE_BUILD_TYPE_TOLOWER MATCHES debugfull)
  message("-- not debugfull mode, so disabling asserts in libavogadro")
  add_definitions(-DNDEBUG)
endif(MY_CMAKE_BUILD_TYPE_TOLOWER MATCHES debugfull)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

include(${QT_USE_FILE})

include_directories(
  ${libavogadro-kalzium_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/..
  ${OPENBABEL2_INCLUDE_DIR}
  ${EIGEN_INCLUDE_DIR}
)

# Need to build with visibility enabled if KDE is
if(__KDE_HAVE_GCC_VISIBILITY)
  add_definitions(-DHAVE_GCC_VISIBILITY)
endif(__KDE_HAVE_GCC_VISIBILITY)

# I think this is necessary now in order to tell the linker where openbabel is
link_directories(${OPENBABEL2_LIBRARY_DIRS})

FILE(GLOB libavogadro_SRCS "*.cpp")
FILE(GLOB libavogadro_HDRS "*.h")

# Statically link the core engines and tools so that users have a working
# system even if no plugins can be found.
set(bsdyengine_SRCS engines/bsdyengine.cpp)
qt4_wrap_ui(bsdyengine_UIS engines/bsdysettingswidget.ui)
set(libavogadro_SRCS ${libavogadro_SRCS} ${bsdyengine_SRCS} ${bsdyengine_UIS})

qt4_automoc(${libavogadro_SRCS})

KDE4_ADD_LIBRARY(avogadro-kalzium SHARED ${libavogadro_SRCS})
SET_TARGET_PROPERTIES(avogadro-kalzium PROPERTIES OUTPUT_NAME avogadro-kalzium)
set_target_properties(avogadro-kalzium PROPERTIES VERSION 0.8.0 SOVERSION 0 )

TARGET_LINK_LIBRARIES(avogadro-kalzium ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
  ${OPENGL_LIBRARIES})

INSTALL(TARGETS avogadro-kalzium DESTINATION ${LIB_INSTALL_DIR})

# Add the engines and tools directories
add_subdirectory(engines)
add_subdirectory(tools)
