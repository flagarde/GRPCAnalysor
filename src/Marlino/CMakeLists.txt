

# project name
PROJECT( Marlino )
# project version

# add the binary tree to the search path for include files
include_directories("${PROJECT_BINARY_DIR}")
include_directories(include)
file(COPY ./include/ DESTINATION ${PROJECT_INCLUDE_DIR})

# -------------------------------------------------
AUX_SOURCE_DIRECTORY( ./source library_sources )
LIST( REMOVE_ITEM library_sources ./source/Marlino.cc ./source/ProcessorLoader.cc )

ADD_LIBRARY( Marlin STATIC ${library_sources} )
TARGET_LINK_LIBRARIES( Marlin ${CMAKE_DL_LIBS} lcio)
INSTALL( TARGETS Marlin LIBRARY DESTINATION ${LIBRARY_OUTPUT_PATH} ARCHIVE DESTINATION ${LIBRARY_OUTPUT_PATH})
# ----- Marlin executable ----------------------------------------------------
ADD_EXECUTABLE( Marlino ./source/Marlino.cc ./source/ProcessorLoader.cc )
# avoid name conflict between Marlin library and Marlin application
TARGET_LINK_LIBRARIES( Marlino Marlin tinyxml Histographs ElogInformations ${ROOT_LIBRARIES} outfileroot Reader tdrstyle Config )
# ----------------------------------------------------------------------------
