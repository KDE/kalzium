FILE(GLOB SVG_CRYSTAL "*.svgz")

install( FILES 
	${SVG_CRYSTAL}
	DESTINATION  ${DATA_INSTALL_DIR}/kalzium/data/latticeicons
)
