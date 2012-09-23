 
install(DIRECTORY periodsystem2/
        DESTINATION ${DATA_INSTALL_DIR}/plasma/plasmoids/org.kde.plasma.applet.periodsystem2)
 
install(FILES periodsystem2/metadata.desktop
        DESTINATION ${SERVICES_INSTALL_DIR} RENAME plasma-applet-periodsystem2.desktop)