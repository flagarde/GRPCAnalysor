CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
if(COMPILER_SUPPORTS_CXX14)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
else()
  CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX0X)
  if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  else()
    if(COMPILER_SUPPORTS_CXX0X)
      CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    else()
      message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please update your *$!%+£ C++ compiler.")
    endif() 
  endif()
endif()
# add the binary tree to the search path for include files
include_directories("${PROJECT_BINARY_DIR}")
file(COPY ./include/ DESTINATION ${PROJECT_INCLUDE_DIR})


# -------------------------------------------------
# build the test library

FILE (GLOB_RECURSE TDRSTYLE_SRC_FILES ./source/*)
ADD_LIBRARY( tdrstyle STATIC ${TDRSTYLE_SRC_FILES} )
TARGET_LINK_LIBRARIES( tdrstyle ${ROOT_LIBRARIES} )
INSTALL( TARGETS tdrstyle LIBRARY DESTINATION ${LIBRARY_OUTPUT_PATH} ARCHIVE DESTINATION ${LIBRARY_OUTPUT_PATH})

