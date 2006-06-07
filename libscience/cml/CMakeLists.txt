include_directories( ${CMAKE_SOURCE_DIR}/libkdeedu/libscience  )

set(cml_LIB_SRCS 
	cmlclasses.cpp 
#	xml_cml.cpp
)

kde4_automoc(${cml_LIB_SRCS})

kde4_add_library(cml SHARED ${cml_LIB_SRCS})

target_link_libraries(cml ${QT_QTXML_LIBRARY} ${QT_QTCORE_LIBRARY} )

install_targets(${LIB_INSTALL_DIR} cml )

