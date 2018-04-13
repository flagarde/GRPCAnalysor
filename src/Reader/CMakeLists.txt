SET_OPTION(CXX_STANDARD 17) # C++14...
SET_OPTION(CXX_STANDARD_REQUIRED ON) #...is required...
SET_OPTION(CXX_EXTENSIONS ON) #...without compiler extensions like gnu++11
include_directories(include)
file(COPY ./include/ DESTINATION ${PROJECT_INCLUDE_DIR})
ADD_DEFINITIONS( "-fPIC -pedantic -Wunused-value -O3 -Wno-long-long -Wreturn-type -Wall -locci -lclntsh " )

FILE (GLOB_RECURSE READER_SRC_FILES ./source/*)
ADD_LIBRARY( Reader STATIC ${READER_SRC_FILES} )
if(HaveOCCI)
TARGET_LINK_LIBRARIES( Reader Database ElogInformations Geometry Config)
else()
TARGET_LINK_LIBRARIES( Reader ElogInformations Geometry Config)
endif()

INSTALL( TARGETS Reader LIBRARY DESTINATION ${LIBRARY_OUTPUT_PATH} ARCHIVE DESTINATION ${LIBRARY_OUTPUT_PATH})
