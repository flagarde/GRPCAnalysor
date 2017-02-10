# ----- Simulation executable ----------------------------------------------------
ADD_EXECUTABLE( Plot ./Plot.cpp)
INSTALL( TARGETS Plot DESTINATION ${EXECUTABLE_OUTPUT_PATH} )  
