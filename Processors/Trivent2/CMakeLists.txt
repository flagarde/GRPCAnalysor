
# add the binary tree to the search path for include files
include_directories("${CMAKE_SOURCE_DIR}/build/include")
include_directories(include)

# require proper c++
ADD_DEFINITIONS( "-fPIC -pedantic -Wunused-value -O3 -Wno-long-long -Wreturn-type -Wall" )


# -------------------------------------------------
# build the test library

FILE (GLOB_RECURSE Trivent_SRC_FILES ./source/*)
ADD_LIBRARY( Trivent SHARED ${Trivent_SRC_FILES} )
TARGET_LINK_LIBRARIES( Trivent Marlin )
INSTALL( TARGETS Trivent LIBRARY DESTINATION ${MARLINO_LIBRARY_OUTPUT_PATH} ARCHIVE DESTINATION ${MARLINO_LIBRARY_OUTPUT_PATH})

