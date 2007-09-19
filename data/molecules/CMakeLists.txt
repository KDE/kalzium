FILE(GLOB CML_EXAMPLE_FILES "*.cml")

install( FILES 
   ${CML_EXAMPLE_FILES}
	DESTINATION  ${DATA_INSTALL_DIR}/kalzium/data/molecules )
