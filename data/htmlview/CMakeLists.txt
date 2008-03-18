FILE(GLOB HTMLVIEW_ICONS "*.png")

install( FILES 
   ${HTMLVIEW_ICONS}
   icons.svg 
   style.css 
   DESTINATION  ${DATA_INSTALL_DIR}/kalzium/data/htmlview )


