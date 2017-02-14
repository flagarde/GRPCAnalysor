
# add the binary tree to the search path for include files
include_directories("${CMAKE_SOURCE_DIR}/build/include")
include_directories(include)

# require proper c++
ADD_DEFINITIONS( "-fPIC -pedantic -Wunused-value -O5 -Wno-long-long -Wreturn-type -Wall" )


# -------------------------------------------------
# build the test library

FILE (GLOB_RECURSE Progress_SRC_FILES ./source/*)
ADD_LIBRARY( Progress SHARED ${Progress_SRC_FILES} )
TARGET_LINK_LIBRARIES( Progress ${ROOT_LIBRARIES} Marlin)
INSTALL( TARGETS Progress LIBRARY DESTINATION ${MARLINO_LIBRARY_OUTPUT_PATH} ARCHIVE DESTINATION ${MARLINO_LIBRARY_OUTPUT_PATH})

