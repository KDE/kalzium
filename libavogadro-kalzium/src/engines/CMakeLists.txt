set(QT_USE_QTOPENGL true)

include(${QT_USE_FILE})

include_directories(${OPENBABEL_INCLUDE_DIR} 
	${CMAKE_CURRENT_BINARY_DIR}
)

# Install to avogadro-kalzium so as not to collide with avogadro.
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
ADD_LIBRARY(labelengine-kalzium SHARED ${labelengine_SRCS} ${labelengine_UIS} )
TARGET_LINK_LIBRARIES(labelengine-kalzium avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)
INSTALL(TARGETS labelengine-kalzium DESTINATION ${DESTINATION_DIR})

# wireframe
set(wireengine_SRCS wireengine.cpp)
qt4_automoc(${wireengine_SRCS})
qt4_wrap_ui(wireengine_UIS wiresettingswidget.ui)
ADD_LIBRARY(wireengine-kalzium SHARED ${wireengine_SRCS} ${wireengine_UIS})
TARGET_LINK_LIBRARIES(wireengine-kalzium avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)
INSTALL(TARGETS wireengine-kalzium DESTINATION ${DESTINATION_DIR})

# hydrogen bond
set(hbondengine_SRCS hbondengine.cpp)
qt4_automoc(${hbondengine_SRCS})
qt4_wrap_ui(hbondengine_UIS hbondsettingswidget.ui)
ADD_LIBRARY(hbondengine-kalzium SHARED ${hbondengine_SRCS} ${hbondengine_UIS})
TARGET_LINK_LIBRARIES(hbondengine-kalzium avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)
INSTALL(TARGETS hbondengine-kalzium DESTINATION ${DESTINATION_DIR})

# sphere
set(sphereengine_SRCS sphereengine.cpp)
qt4_wrap_ui(sphereengine_UIS spheresettingswidget.ui)
qt4_automoc(${sphereengine_SRCS})
ADD_LIBRARY(sphereengine-kalzium SHARED ${sphereengine_SRCS} ${sphereengine_UIS} )
TARGET_LINK_LIBRARIES(sphereengine-kalzium avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)

INSTALL(TARGETS sphereengine-kalzium DESTINATION ${DESTINATION_DIR})

# stick (cylinder) engine
set(stickengine_SRCS stickengine.cpp)
qt4_automoc(${stickengine_SRCS})
ADD_LIBRARY(stickengine-kalzium SHARED ${stickengine_SRCS} )
TARGET_LINK_LIBRARIES(stickengine-kalzium avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)

INSTALL(TARGETS stickengine-kalzium DESTINATION ${DESTINATION_DIR})

# Ribbon engine (biological structure)
set(ribbonengine_SRCS ribbonengine.cpp)
qt4_wrap_ui(ribbonengine_UIS ribbonsettingswidget.ui)
qt4_automoc(${ribbonengine_SRCS})
ADD_LIBRARY(ribbonengine-kalzium SHARED ${ribbonengine_SRCS} ${ribbonengine_UIS} )
TARGET_LINK_LIBRARIES(ribbonengine-kalzium avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)

INSTALL(TARGETS ribbonengine-kalzium DESTINATION ${DESTINATION_DIR})

# Ring engine - display cyclic rings
set(ringengine_SRCS ringengine.cpp)
qt4_wrap_ui(ringengine_UIS ringsettingswidget.ui)
qt4_automoc(${ringengine_SRCS})
ADD_LIBRARY(ringengine-kalzium SHARED ${ringengine_SRCS} ${ringengine_UIS} )
TARGET_LINK_LIBRARIES(ringengine-kalzium avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)

INSTALL(TARGETS ringengine-kalzium DESTINATION ${DESTINATION_DIR})

# Polygon engine
set(polygonengine_SRCS polygonengine.cpp)
qt4_automoc(${polygonengine_SRCS})
ADD_LIBRARY(polygonengine-kalzium SHARED ${polygonengine_SRCS} )
TARGET_LINK_LIBRARIES(polygonengine-kalzium avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)

INSTALL(TARGETS polygonengine-kalzium DESTINATION ${DESTINATION_DIR})

# Surface engine - molecular surfaces and 3D functions
set(surfaceengine_SRCS iso.cpp surfaceengine.cpp)
qt4_wrap_ui(surfaceengine_UIS surfacesettingswidget.ui)
qt4_automoc(${surfaceengine_SRCS})
ADD_LIBRARY(surfaceengine-kalzium SHARED ${surfaceengine_SRCS} ${surfaceengine_UIS})
TARGET_LINK_LIBRARIES(surfaceengine-kalzium avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)

INSTALL(TARGETS surfaceengine-kalzium DESTINATION ${DESTINATION_DIR})

# Orbital engine - molecular orbitals
set(orbitalengine_SRCS iso.cpp orbitalengine.cpp)
qt4_wrap_ui(orbitalengine_UIS orbitalsettingswidget.ui)
qt4_automoc(${orbitalengine_SRCS})
ADD_LIBRARY(orbitalengine-kalzium SHARED ${orbitalengine_SRCS} ${orbitalengine_UIS})
TARGET_LINK_LIBRARIES(orbitalengine-kalzium avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)

INSTALL(TARGETS orbitalengine-kalzium DESTINATION ${DESTINATION_DIR})
