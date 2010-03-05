set(QT_USE_QTOPENGL true)

set(LOCAL_PLUGIN_INSTALL_DIR   "${LIB_INSTALL_DIR}/avogadro-kalzium"
    CACHE PATH "Install dir for avogadro-kalzium plugins")

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

include(${QT_USE_FILE})

# Ensure the Avogadro include directory is always first
get_directory_property(tmp_include_dirs INCLUDE_DIRECTORIES)
set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES
    "${libavogadro-kalzium_SOURCE_DIR}/include;${tmp_include_dirs}")

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${EIGEN2_INCLUDE_DIR}
  ${OPENBABEL2_INCLUDE_DIR}
)

# Need to build with visibility enabled if KDE is
if(__KDE_HAVE_GCC_VISIBILITY)
  add_definitions(-DHAVE_GCC_VISIBILITY)
endif(__KDE_HAVE_GCC_VISIBILITY)

# FindOpenBabel2.cmake uses find_library(), so ${OPENBABEL2_LIBRARIES} should 
# be the full path to the library. So the line below should not be necessary. Alex
#
# I think this is necessary now in order to tell the linker where openbabel is
# link_directories(${OPENBABEL2_LIBRARY_DIRS})

file(GLOB libavogadro_SRCS "*.cpp")

# Only headers with exported symbols should actually be installed
set(libavogadro_HDRS
  animation.h
  atom.h
  bond.h
  camera.h
  color.h
  colorbutton.h
  cube.h
  elementtranslate.h
  engine.h
  extension.h
  filetreeitem.h
  fragment.h
  glgraphicsview.h
  glwidget.h
  global.h
  idlist.h
  plotaxis.h # dlonie: Are these necessary here?
  plotobject.h
  plotpoint.h
  plotwidget.h
  line.h
  mesh.h
  meshgenerator.h
  molecule.h
  navigate.h
  neighborlist.h
  painter.h
  painterdevice.h
  periodictableview.h
  plugin.h
  pluginmanager.h
  point.h
  primitive.h
  primitiveitemmodel.h
  primitivelist.h
  residue.h
  tool.h
  toolgroup.h
  undosequence.h
  zmatrix.h
)

set(libavogadro_SRCS ${libavogadro_SRCS} ${libavogadro_UIS})

# Statically link the core engines and tools so that users have a working
# system even if no plugins can be found.
set(bsdyengine_SRCS engines/bsdyengine.cpp)
qt4_wrap_ui(bsdyengine_UIS engines/bsdysettingswidget.ui)
set(libavogadro_SRCS ${libavogadro_SRCS} ${bsdyengine_SRCS} ${bsdyengine_UIS})

set(elementcolor_SRCS colors/elementcolor.cpp)
set(libavogadro_SRCS ${libavogadro_SRCS} ${elementcolor_SRCS})

set(AVO_LINK_LIBRARIES ${OPENBABEL2_LIBRARIES}
                      ${QT_LIBRARIES}
                      ${OPENGL_LIBRARIES}
                      )

if(GLEW_FOUND)
  include_directories(${GLEW_INCLUDE_DIR})
# If glew is found properly, this link_directories() call shouldn't be necessary. Alex
#  link_directories(${GLEW_LIBRARY_DIR})
  set(AVO_LINK_LIBRARIES ${AVO_LINK_LIBRARIES} ${GLEW_LIBRARY})
endif(GLEW_FOUND)

# If python dependencies aren't found, remove the Avogadro Python source files
if(NOT ENABLE_PYTHON OR NOT ALL_PYTHON_FOUND)
  set(PYTHON_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/pythoninterpreter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pythonscript.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pythonerror.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pythontool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pythonengine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pythonextension.cpp)
  foreach(P_ITEM ${PYTHON_SRCS})
    list(REMOVE_ITEM libavogadro_SRCS "${P_ITEM}")
  endforeach(P_ITEM ${PYTHON_SRCS})
endif(NOT ENABLE_PYTHON OR NOT ALL_PYTHON_FOUND)

qt4_automoc(${libavogadro_SRCS})

kde4_add_library(avogadro-kalzium SHARED ${libavogadro_SRCS})
set_target_properties(avogadro-kalzium PROPERTIES OUTPUT_NAME avogadro-kalzium)
set_target_properties(avogadro-kalzium PROPERTIES VERSION 0.9.3 SOVERSION 0)
set_target_properties(avogadro-kalzium PROPERTIES DEFINE_SYMBOL avogadro_lib_EXPORTS)

target_link_libraries(avogadro-kalzium ${AVO_LINK_LIBRARIES})

install(TARGETS avogadro-kalzium ${INSTALL_TARGETS_DEFAULT_ARGS})

# Add the engiens, tools and extensions directories
add_subdirectory(engines)
add_subdirectory(tools)
add_subdirectory(colors)

