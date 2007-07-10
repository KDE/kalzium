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
KDE4_ADD_LIBRARY(bsdyengine-kalzium SHARED ${bsdyengine_SRCS} ${bsdyengine_UIS})
TARGET_LINK_LIBRARIES(bsdyengine-kalzium avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)

INSTALL(TARGETS bsdyengine-kalzium DESTINATION ${DESTINATION_DIR})

# label engine
set(labelengine_SRCS labelengine.cpp)
KDE4_ADD_LIBRARY(labelengine-kalzium SHARED ${labelengine_SRCS} )
TARGET_LINK_LIBRARIES(labelengine-kalzium avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)
INSTALL(TARGETS labelengine-kalzium DESTINATION ${DESTINATION_DIR})

# wireframe
set(wireengine_SRCS wireengine.cpp)
KDE4_ADD_LIBRARY(wireengine-kalzium SHARED ${wireengine_SRCS} )
TARGET_LINK_LIBRARIES(wireengine-kalzium avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)
INSTALL(TARGETS wireengine-kalzium DESTINATION ${DESTINATION_DIR})

# sphere
set(sphereengine_SRCS sphereengine.cpp)
KDE4_ADD_LIBRARY(sphereengine-kalzium SHARED ${sphereengine_SRCS} )
TARGET_LINK_LIBRARIES(sphereengine-kalzium avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)

INSTALL(TARGETS sphereengine-kalzium DESTINATION ${DESTINATION_DIR})

# stick (cylinder) engine
set(stickengine_SRCS stickengine.cpp)
KDE4_ADD_LIBRARY(stickengine-kalzium SHARED ${stickengine_SRCS} )
TARGET_LINK_LIBRARIES(stickengine-kalzium avogadro-kalzium
                                ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES}
                                ${OPENGL_LIBRARIES}
)

INSTALL(TARGETS stickengine-kalzium DESTINATION ${DESTINATION_DIR})
