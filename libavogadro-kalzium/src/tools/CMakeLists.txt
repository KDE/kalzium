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

### navigatetool
set(navigatetool_SRCS navigatetool.cpp)
kde4_automoc(${navigatetool_SRCS})
KDE4_ADD_LIBRARY(navigatetool-kalzium SHARED ${navigatetool_SRCS} ${navigatetool_RC_SRCS})
TARGET_LINK_LIBRARIES(navigatetool-kalzium 
	${OPENBABEL2_LIBRARIES} 
	${QT_LIBRARIES} 
    ${OPENGL_LIBRARIES}
	avogadro-kalzium
)
INSTALL(TARGETS navigatetool-kalzium DESTINATION ${DESTINATION_DIR})
