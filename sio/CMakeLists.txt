#################################
# cmake file for building SIO
# @author Frank Gaede, DESY
# @author Jan Engels, DESY
#################################

# project name
PROJECT( SIO )
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

# project version
SET( SIO_VERSION_MAJOR ${LCIO_VERSION_MAJOR} )
SET( SIO_VERSION_MINOR ${LCIO_VERSION_MINOR} )
SET( SIO_VERSION_PATCH ${LCIO_VERSION_PATCH} )

# library *nix style versioning
SET( SIO_SOVERSION "${SIO_VERSION_MAJOR}.${SIO_VERSION_MINOR}" )
SET( SIO_VERSION "${SIO_SOVERSION}.${SIO_VERSION_PATCH}" )

FIND_PACKAGE( ZLIB REQUIRED )

INCLUDE( TestBigEndian )
TEST_BIG_ENDIAN( BIG_ENDIAN )

IF( NOT BIG_ENDIAN )
    ADD_DEFINITIONS( "-D__LITTLE_ENDIAN__" )
ENDIF()

file(COPY ${SIO_SOURCE_DIR}/include/ DESTINATION ${PROJECT_INCLUDE_DIR})

INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR})

SET (SIO_SRCS
    ${SIO_SOURCE_DIR}/src/SIO_block.cc
    ${SIO_SOURCE_DIR}/src/SIO_blockManager.cc
    ${SIO_SOURCE_DIR}/src/SIO_functions.cc
    ${SIO_SOURCE_DIR}/src/SIO_record.cc
    ${SIO_SOURCE_DIR}/src/SIO_recordManager.cc
    ${SIO_SOURCE_DIR}/src/SIO_stream.cc
    ${SIO_SOURCE_DIR}/src/SIO_streamManager.cc
)

ADD_LIBRARY( sio STATIC ${SIO_SRCS} )
TARGET_LINK_LIBRARIES( sio ${ZLIB_LIBRARIES} )
INSTALL( TARGETS sio DESTINATION ${LIBRARY_OUTPUT_PATH} )
