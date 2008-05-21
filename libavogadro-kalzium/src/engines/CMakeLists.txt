set(QT_USE_QTOPENGL true)

include(${QT_USE_FILE})

include_directories(${OPENBABEL_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)

set(DESTINATION_DIR ${LIB_INSTALL_DIR}/avogadro-kalzium/engines)

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

link_directories(${CMAKE_CURRENT_BINARY_DIR}/..)

# label engine
set(labelengine_SRCS labelengine.cpp)
qt4_wrap_ui(labelengine_UIS labelsettingswidget.ui)
qt4_automoc(${labelengine_SRCS})
ADD_LIBRARY(labelengine SHARED ${labelengine_SRCS} ${labelengine_UIS} )
TARGET_LINK_LIBRARIES(labelengine avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)
INSTALL(TARGETS labelengine DESTINATION ${DESTINATION_DIR})

# wireframe
set(wireengine_SRCS wireengine.cpp)
qt4_automoc(${wireengine_SRCS})
qt4_wrap_ui(wireengine_UIS wiresettingswidget.ui)
ADD_LIBRARY(wireengine SHARED ${wireengine_SRCS} ${wireengine_UIS})
TARGET_LINK_LIBRARIES(wireengine avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)
INSTALL(TARGETS wireengine DESTINATION ${DESTINATION_DIR})

# sphere
set(sphereengine_SRCS sphereengine.cpp)
qt4_wrap_ui(sphereengine_UIS spheresettingswidget.ui)
qt4_automoc(${sphereengine_SRCS})
ADD_LIBRARY(sphereengine SHARED ${sphereengine_SRCS} ${sphereengine_UIS} )
TARGET_LINK_LIBRARIES(sphereengine avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)

INSTALL(TARGETS sphereengine DESTINATION ${DESTINATION_DIR})

# stick (cylinder) engine
set(stickengine_SRCS stickengine.cpp)
qt4_wrap_ui(stickengine_UIS sticksettingswidget.ui)
qt4_automoc(${stickengine_SRCS})
ADD_LIBRARY(stickengine SHARED ${stickengine_SRCS} ${stickengine_UIS})
TARGET_LINK_LIBRARIES(stickengine avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)
INSTALL(TARGETS stickengine DESTINATION ${DESTINATION_DIR})

# Ribbon engine (biological structure)
set(ribbonengine_SRCS ribbonengine.cpp)
qt4_wrap_ui(ribbonengine_UIS ribbonsettingswidget.ui)
qt4_automoc(${ribbonengine_SRCS})
ADD_LIBRARY(ribbonengine SHARED ${ribbonengine_SRCS} ${ribbonengine_UIS} )
TARGET_LINK_LIBRARIES(ribbonengine avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)

INSTALL(TARGETS ribbonengine DESTINATION ${DESTINATION_DIR})

# Ring engine - display cyclic rings
set(ringengine_SRCS ringengine.cpp)
qt4_wrap_ui(ringengine_UIS ringsettingswidget.ui)
qt4_automoc(${ringengine_SRCS})
ADD_LIBRARY(ringengine SHARED ${ringengine_SRCS} ${ringengine_UIS} )
TARGET_LINK_LIBRARIES(ringengine avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)

INSTALL(TARGETS ringengine DESTINATION ${DESTINATION_DIR})

# Orbital engine - molecular orbitals
set(orbitalengine_SRCS iso.cpp orbitalengine.cpp)
qt4_wrap_ui(orbitalengine_UIS orbitalsettingswidget.ui)
qt4_automoc(${orbitalengine_SRCS})
ADD_LIBRARY(orbitalengine SHARED ${orbitalengine_SRCS} ${orbitalengine_UIS})
TARGET_LINK_LIBRARIES(orbitalengine avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)

INSTALL(TARGETS orbitalengine DESTINATION ${DESTINATION_DIR})

