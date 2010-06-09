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
   psetables.cpp
)


kde4_add_library(science SHARED ${science_LIB_SRCS})

target_link_libraries(science  ${KDE4_KDECORE_LIBS} ${QT_QTXML_LIBRARY} )

set_target_properties(science PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )
install(TARGETS science  ${INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
   chemicaldataobject.h
   element.h
   elementparser.h
   isotope.h
   isotopeparser.h
   libkdeedu_science_export.h
   moleculeparser.h
   parser.h
   spectrum.h
   spectrumparser.h
   tempunit.h
   psetables.h
   DESTINATION ${INCLUDE_INSTALL_DIR}/libkdeedu  COMPONENT Devel
)
