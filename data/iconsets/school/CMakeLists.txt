FILE(GLOB SVG_SCHOOL_ICONS "*.svg")

install( FILES 
	${SVG_SCHOOL_ICONS}
        iconinformation.txt
	DESTINATION  ${DATA_INSTALL_DIR}/kalzium/data/iconsets/school )
