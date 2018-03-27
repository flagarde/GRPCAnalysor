
# ----- Simulation executable ----------------------------------------------------

include_directories(include ${PROJECT_INCLUDE_DIR})
file(COPY ./include/ DESTINATION ${PROJECT_INCLUDE_DIR})
FILE (GLOB_RECURSE ELOGINFOS_SRC_FILES ./source/*)
ADD_LIBRARY( ElogInformations STATIC ${ELOGINFOS_SRC_FILES} )
INSTALL( TARGETS ElogInformations LIBRARY DESTINATION ${LIBRARY_OUTPUT_PATH} ARCHIVE DESTINATION ${LIBRARY_OUTPUT_PATH})
ADD_EXECUTABLE( ElogFiller ./source/ElogFillerNewDAQ.cc )
TARGET_LINK_LIBRARIES( ElogFiller tinyxml)
INSTALL( TARGETS ElogFiller DESTINATION ${EXECUTABLE_OUTPUT_PATH} )  
