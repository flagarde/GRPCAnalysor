# ----- Simulation executable ----------------------------------------------------
ADD_EXECUTABLE( Simulation ./Simulation.cc )
INSTALL( TARGETS Simulation DESTINATION ${EXECUTABLE_OUTPUT_PATH} ) 
