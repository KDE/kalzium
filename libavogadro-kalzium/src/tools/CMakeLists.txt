set(QT_USE_QTOPENGL true)

include(${QT_USE_FILE})

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

set(DESTINATION_DIR ${LIB_INSTALL_DIR}/avogadro-kalzium/tools)

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
)

link_directories(${CMAKE_CURRENT_BINARY_DIR}/..)

### drawtool
set(drawtool_SRCS drawtool.cpp  directorytreemodel.cpp
                                insertfragmentdialog.cpp
                                )
qt4_add_resources(drawtool_RC_SRCS drawtool.qrc)
qt4_automoc(${drawtool_SRCS})
qt4_wrap_ui(drawtool_UIS_H insertfragmentdialog.ui)
ADD_LIBRARY(drawtool SHARED drawcommand.cpp
                    ${drawtool_SRCS}
                    ${drawtool_UIS_H}
                    ${drawtool_RC_SRCS} )
TARGET_LINK_LIBRARIES(drawtool
    ${OPENBABEL2_LIBRARIES}
    ${QT_LIBRARIES}
    ${OPENGL_LIBRARIES}
    avogadro-kalzium)
INSTALL(TARGETS drawtool DESTINATION ${DESTINATION_DIR})

### clickmeasuretool
set(clickmeasuretool_SRCS clickmeasuretool.cpp)
qt4_add_resources(clickmeasuretool_RC_SRCS clickmeasuretool.qrc)
qt4_automoc(${clickmeasuretool_SRCS})
ADD_LIBRARY(clickmeasuretool SHARED
    ${clickmeasuretool_SRCS} ${clickmeasuretool_RC_SRCS} )
TARGET_LINK_LIBRARIES(clickmeasuretool
    ${OPENBABEL2_LIBRARIES}
    ${QT_LIBRARIES}
    ${OPENGL_LIBRARIES}
    avogadro-kalzium)
INSTALL(TARGETS clickmeasuretool DESTINATION ${DESTINATION_DIR})

### navigatetool
set(navigatetool_SRCS navigatetool.cpp)
qt4_add_resources(navigatetool_RC_SRCS navigatetool.qrc)
qt4_automoc(${navigatetool_SRCS})
ADD_LIBRARY(navigatetool SHARED eyecandy.cpp
	${navigatetool_SRCS} ${navigatetool_RC_SRCS})
TARGET_LINK_LIBRARIES(navigatetool
	${OPENBABEL2_LIBRARIES}
	${QT_LIBRARIES}
	${OPENGL_LIBRARIES}
	avogadro-kalzium
)
INSTALL(TARGETS navigatetool DESTINATION ${DESTINATION_DIR})

