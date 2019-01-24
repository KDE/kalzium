add_subdirectory(data)
add_subdirectory(tests)

set(science_LIB_SRCS
   element.cpp
   spectrum.cpp
   isotope.cpp
   spectrumparser.cpp
   elementparser.cpp
   isotopeparser.cpp
   chemicaldataobject.cpp
   moleculeparser.cpp
   parser.cpp
   psetables.cpp
)

add_library(science SHARED ${science_LIB_SRCS})

target_link_libraries(science Qt5::Xml KF5::UnitConversion KF5::WidgetsAddons KF5::I18n)
set_target_properties(science PROPERTIES VERSION ${KALZIUMLIB_VERSION} SOVERSION ${KALZIUMLIB_SOVERSION} )
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
   psetables.h
   DESTINATION ${INCLUDE_INSTALL_DIR}/libkdeedu  COMPONENT Devel
)
