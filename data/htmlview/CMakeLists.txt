FILE(GLOB HTMLVIEW_ICONS "*.png")

install( FILES
   ${HTMLVIEW_ICONS}
   icons.svg
   DESTINATION  ${DATA_INSTALL_DIR}/kalzium/data/htmlview )


