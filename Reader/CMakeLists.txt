
# add the binary tree to the search path for include files
include_directories(include)
file(COPY ./include/ DESTINATION ${PROJECT_INCLUDE_DIR})
# require proper c++
ADD_DEFINITIONS( "-fPIC -pedantic -Wunused-value -O3 -Wno-long-long -Wreturn-type -Wall -locci -lclntsh " )


# -------------------------------------------------
# build the test library

FILE (GLOB_RECURSE READER_SRC_FILES ./source/*)
ADD_LIBRARY( Reader STATIC ${READER_SRC_FILES} )
TARGET_LINK_LIBRARIES( Reader Database Geometry )
INSTALL( TARGETS Reader LIBRARY DESTINATION ${LIBRARY_OUTPUT_PATH} ARCHIVE DESTINATION ${LIBRARY_OUTPUT_PATH})

