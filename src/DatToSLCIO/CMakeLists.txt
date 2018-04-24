# ----- Simulation executable ----------------------------------------------------
cmake_minimum_required(VERSION 2.8)
project(DatToSLCIO)

ADD_EXECUTABLE( DatToSLCIO ./DatToSLCIO.cc )
TARGET_LINK_LIBRARIES(DatToSLCIO lcio  )
INSTALL(TARGETS DatToSLCIO DESTINATION bin )  
