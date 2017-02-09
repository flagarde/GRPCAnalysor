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
ADD_EXECUTABLE( Marlinoo ./source/Marlino.cc ./source/ProcessorLoader.cc )
# avoid name conflict between Marlin library and Marlin application
TARGET_LINK_LIBRARIES( Marlinoo Marlin tinyxml Histographs ElogInformations ${ROOT_LIBRARIES} outfileroot Reader tdrstyle Config )
# ----------------------------------------------------------------------------
