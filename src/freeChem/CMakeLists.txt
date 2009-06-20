project(freeChem)

set(freeChem_LIB_SRCS
    atom.cpp
    bond.cpp
    chain.cpp
    chemistry.cpp
    cycle.cpp
    document.cpp
    formula.cpp   
    element.cpp
    isotope.cpp
    matrix2d.cpp
    molecule.cpp
    object.cpp
    residue.cpp
    value.cpp  
    vector.cpp
    xml-utils.cpp
)

set(freeChem_HEADERS
    atom.h
    bond.h
    chain.h
    chemistry.h
    cycle.h
    document.h
    element.h
    formula.h
    isotope.h
    macros.h
    matrix2d.h
    molecule.h
    object.h
    residue.h
    objprops.h
    value.h
    vector.h
    xml-utils.h
)

kde4_add_library(freeChem SHARED ${freeChem_LIB_SRCS})

find_package(LibXml2)
target_link_libraries(freeChem ${LIBXML2_LIBRARIES} ${KDE4_KDECORE_LIBS} )
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}")
set_target_properties(freeChem PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION})
install(TARGETS freeChem ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES ${freeChem_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR}/freeChem COMPONENT Devel)
install(FILES FindFreeChem.cmake DESTINATION ${DATA_INSTALL_DIR}/cmake/modules)
