include_directories("${CMAKE_SOURCE_DIR}/build/include")
include_directories(include)

FILE (GLOB_RECURSE Streamout_SRC_FILES ./source/*)
ADD_LIBRARY( Streamout SHARED ${Streamout_SRC_FILES} )
TARGET_LINK_LIBRARIES( Streamout Marlin lcio)
INSTALL( TARGETS Streamout LIBRARY DESTINATION ${MARLINO_LIBRARY_OUTPUT_PATH} ARCHIVE DESTINATION ${MARLINO_LIBRARY_OUTPUT_PATH})
