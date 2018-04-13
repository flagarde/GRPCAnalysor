SET_OPTION(CXX_STANDARD 17) # C++14...
SET_OPTION(CXX_STANDARD_REQUIRED ON) #...is required...
SET_OPTION(CXX_EXTENSIONS ON) #...without compiler extensions like gnu++11
# add the binary tree to the search path for include files
include_directories("${PROJECT_BINARY_DIR}")
file(COPY ./include/ DESTINATION ${PROJECT_INCLUDE_DIR})


# -------------------------------------------------
# build the test library

FILE (GLOB_RECURSE TDRSTYLE_SRC_FILES ./source/*)
ADD_LIBRARY( tdrstyle STATIC ${TDRSTYLE_SRC_FILES} )
TARGET_LINK_LIBRARIES( tdrstyle ${ROOT_LIBRARIES} )
INSTALL( TARGETS tdrstyle LIBRARY DESTINATION ${LIBRARY_OUTPUT_PATH} ARCHIVE DESTINATION ${LIBRARY_OUTPUT_PATH})

