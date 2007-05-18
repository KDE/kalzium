set(QT_USE_QTOPENGL TRUE)

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

# ball and stick dynamic engine
set(bsdyengine_SRCS bsdyengine.cpp)
qt4_wrap_ui(bsdyengine_UIS bsdysettingswidget.ui)
kde4_automoc(${bsdyengine_SRCS})
KDE4_ADD_LIBRARY(bsdyengine SHARED ${bsdyengine_SRCS} ${bsdyengine_UIS})
TARGET_LINK_LIBRARIES(bsdyengine avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)

INSTALL(TARGETS bsdyengine DESTINATION ${DESTINATION_DIR})

# label engine
set(labelengine_SRCS labelengine.cpp)
kde4_automoc(${labelengine_SRCS})
KDE4_ADD_LIBRARY(labelengine SHARED ${labelengine_SRCS} )
TARGET_LINK_LIBRARIES(labelengine avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)
INSTALL(TARGETS labelengine DESTINATION ${DESTINATION_DIR})

# wireframe
set(wireengine_SRCS wireengine.cpp)
kde4_automoc(${wireengine_SRCS})
KDE4_ADD_LIBRARY(wireengine SHARED ${wireengine_SRCS} )
TARGET_LINK_LIBRARIES(wireengine avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)
INSTALL(TARGETS wireengine DESTINATION ${DESTINATION_DIR})

# debug engine
set(debugengine_SRCS debugengine.cpp)
kde4_automoc(${debugengine_SRCS})
KDE4_ADD_LIBRARY(debugengine SHARED ${debugengine_SRCS} )
TARGET_LINK_LIBRARIES(debugengine avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)
INSTALL(TARGETS debugengine DESTINATION ${DESTINATION_DIR})

# sphere
set(sphereengine_SRCS sphereengine.cpp)
kde4_automoc(${sphereengine_SRCS})
KDE4_ADD_LIBRARY(sphereengine SHARED ${sphereengine_SRCS} )
TARGET_LINK_LIBRARIES(sphereengine avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)

INSTALL(TARGETS sphereengine DESTINATION ${DESTINATION_DIR})

# stick (cylinder) engine
set(stickengine_SRCS stickengine.cpp)
kde4_automoc(${stickengine_SRCS})
KDE4_ADD_LIBRARY(stickengine SHARED ${stickengine_SRCS} )
TARGET_LINK_LIBRARIES(stickengine avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)

INSTALL(TARGETS stickengine DESTINATION ${DESTINATION_DIR})
