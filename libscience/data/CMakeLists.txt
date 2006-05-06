kde4_header()

install_files(${DATA_INSTALL_DIR}/libkdeedu/data/ FILES
   elements.xml
   isotopes.xml
   spectra.xml
)

kde4_footer()

#original Makefile.am contents follow:

#xml_DATA = elements.xml isotopes.xml
#
#xmldir = $(kde_datadir)/libkdeedu/data/
