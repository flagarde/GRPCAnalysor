# ----- Simulation executable ----------------------------------------------------
ADD_EXECUTABLE( ElogFiller ElogFiller.cc )
TARGET_LINK_LIBRARIES( ElogFiller tinyxml)
INSTALL( TARGETS ElogFiller DESTINATION ${EXECUTABLE_OUTPUT_PATH} )  
