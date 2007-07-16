add_subdirectory(data)
add_subdirectory(tests)

set(science_LIB_SRCS
   element.cpp
   spectrum.cpp
   isotope.cpp
   spectrumparser.cpp
   elementparser.cpp
   isotopeparser.cpp
   tempunit.cpp
   chemicaldataobject.cpp
   moleculeparser.cpp
   parser.cpp
)


kde4_add_library(science SHARED ${science_LIB_SRCS})

target_link_libraries(science  ${KDE4_KDECORE_LIBS} ${QT_QTXML_LIBRARY} )

set_target_properties(science PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )
install(TARGETS science  DESTINATION ${LIB_INSTALL_DIR} )

