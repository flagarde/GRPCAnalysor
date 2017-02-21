########################################################
# cmake file for building Marlino
# @author Lagarde François, IPNL
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
########################################################
# project name
project(GRPC_Analysor)
# project version
SET( GRPC_Analysor_VERSION_MAJOR 2 )
SET( GRPC_Analysor_VERSION_MINOR 7 )
SET( GRPC_Analysor_VERSION_PATCH 1 )
include(CheckIncludeFiles)
include(CheckCXXCompilerFlag)

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
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/SlowControl ${CMAKE_CURRENT_SOURCE_DIR}/bin/ElogXML)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/DetectorGeometry ${CMAKE_CURRENT_SOURCE_DIR}/src/xml DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(PROJECT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/include/")
set(LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build/lib/")
set(MARLINO_LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin/libMarlino/")
SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/build/include/)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -ansi -fno-strict-aliasing -lpthread -ldl -lm -fPIC" )
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/Modules/)
list( APPEND CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake/)
find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIR})
configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/src/Utils/Version.h.in" "${PROJECT_INCLUDE_DIR}/Version.h")

find_library(LOCCI_SUPPORT NAMES occi)
if(LOCCI_SUPPORT)
  message("-- LOCCI found")
  SET(HaveOCCI 1)
  SET(GCC_COVERAGE_LINK_FLAGS "-locci -lclntsh")
  add_definitions(-DOCCI_SUPPORT)
else()
  message("-- No LOCCI support")
  SET(HaveOCCI 0)
endif()

ADD_SUBDIRECTORY( src/Elog )
ADD_SUBDIRECTORY( src/HistoGraphs )
if(HaveOCCI)
ADD_SUBDIRECTORY( src/Database )
endif()
ADD_SUBDIRECTORY( src/sio )
ADD_SUBDIRECTORY( src/Config )
ADD_SUBDIRECTORY( src/lcio )
ADD_SUBDIRECTORY( src/Geometry )
ADD_SUBDIRECTORY( src/Marlino )
ADD_SUBDIRECTORY( src/OutFileRoot )
ADD_SUBDIRECTORY( src/Processors )
ADD_SUBDIRECTORY( src/Reader )
ADD_SUBDIRECTORY( src/tdrstyle )
ADD_SUBDIRECTORY( src/Utils )
ADD_SUBDIRECTORY( src/tinyxml )
ADD_SUBDIRECTORY( src/Converter )
ADD_SUBDIRECTORY( src/jsoncpp )

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/src/Modules/Doxyfile.in)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/build/Doxyfile)

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/html DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/doc)
endif()

#ADD_SUBDIRECTORY( Simulation )
#ADD_SUBDIRECTORY( Plot )
