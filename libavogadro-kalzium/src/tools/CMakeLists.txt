set(QT_USE_QTOPENGL TRUE)

include(${QT_USE_FILE})

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

# Install to avogadro-kalzium so as not to collide with avogadro.
set(DESTINATION_DIR ${LIB_INSTALL_DIR}/avogadro-kalzium/tools)

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
)

link_directories(${CMAKE_CURRENT_BINARY_DIR}/.. ${CMAKE_BINARY_DIR}/lib)

### drawtool
set(drawtool_SRCS drawtool.cpp)
qt4_add_resources(drawtool_RC_SRCS draw.qrc)
kde4_automoc(${drawtool_SRCS})
KDE4_ADD_LIBRARY(drawtool SHARED drawcommand.cpp drawtool.cpp ${drawtool_RC_SRCS} )
TARGET_LINK_LIBRARIES(drawtool 
    ${OPENBABEL2_LIBRARIES} 
    ${QT_LIBRARIES}
    ${OPENGL_LIBRARIES} 
    avogadro-kalzium)
INSTALL(TARGETS drawtool DESTINATION ${DESTINATION_DIR})

### clickmeasuretool
set(clickmeasuretool_SRCS clickmeasuretool.cpp)
qt4_add_resources(clickmeasuretool_RC_SRCS measure.qrc)
kde4_automoc(${clickmeasuretool_SRCS})
KDE4_ADD_LIBRARY(clickmeasuretool SHARED ${clickmeasuretool_SRCS} ${clickmeasuretool_RC_SRCS} )
TARGET_LINK_LIBRARIES(clickmeasuretool 
    ${OPENBABEL2_LIBRARIES} 
    ${QT_LIBRARIES}
    ${OPENGL_LIBRARIES} 
    avogadro-kalzium)
INSTALL(TARGETS clickmeasuretool DESTINATION ${DESTINATION_DIR})

### selectrotatetool
set(selectrotatetool_SRCS selectrotatetool.cpp)
qt4_add_resources(selectrotatetool_RC_SRCS select.qrc)
kde4_automoc(${selectrotatetool_SRCS})
KDE4_ADD_LIBRARY(selectrotatetool SHARED ${selectrotatetool_SRCS} ${selectrotatetool_RC_SRCS})
TARGET_LINK_LIBRARIES(selectrotatetool 
	${OPENBABEL2_LIBRARIES} 
	${QT_LIBRARIES} 
    ${OPENGL_LIBRARIES}
	avogadro-kalzium
)
INSTALL(TARGETS selectrotatetool DESTINATION ${DESTINATION_DIR})

### navigatetool
set(navigatetool_SRCS navigatetool.cpp)
qt4_add_resources(navigatetool_RC_SRCS navigate.qrc)
kde4_automoc(${navigatetool_SRCS})
KDE4_ADD_LIBRARY(navigatetool SHARED ${navigatetool_SRCS} ${navigatetool_RC_SRCS})
TARGET_LINK_LIBRARIES(navigatetool 
	${OPENBABEL2_LIBRARIES} 
	${QT_LIBRARIES} 
    ${OPENGL_LIBRARIES}
	avogadro-kalzium
)
INSTALL(TARGETS navigatetool DESTINATION ${DESTINATION_DIR})

### manipulatetool
set(manipulatetool_SRCS manipulatetool.cpp)
qt4_add_resources(manipulatetool_RC_SRCS manipulate.qrc)
kde4_automoc(${manipulatetool_SRCS})
KDE4_ADD_LIBRARY(manipulatetool SHARED ${manipulatetool_SRCS} ${manipulatetool_RC_SRCS})
TARGET_LINK_LIBRARIES(manipulatetool 
	${OPENBABEL2_LIBRARIES} 
	${QT_LIBRARIES} 
    ${OPENGL_LIBRARIES}
	avogadro-kalzium
)
INSTALL(TARGETS manipulatetool DESTINATION ${DESTINATION_DIR})
