########################################################
# cmake file for building Marlino
# @author Lagarde François, IPNL
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
########################################################

macro(SET_OPTION option value)
  set(${option} ${value} CACHE INTERNAL "" FORCE)
endmacro()

# LCIO options
SET_OPTION(BUILD_LCIO_EXAMPLES  OFF)
SET_OPTION(BUILD_TESTING  OFF)



set(PROJECT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/include/")
set(LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build/lib/")
set(MARLINO_LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin/libMarlino/")
SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin")


SET_OPTION(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/)
add_compile_options(-std=c++11)
SET_OPTION(CMAKE_CXX_STANDARD 14) # C++14...
SET_OPTION(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
SET_OPTION(CMAKE_CXX_EXTENSIONS ON) #...without compiler extensions like gnu++11


# project name
project(GRPC_Analysor)
# project version
SET( GRPC_Analysor_VERSION_MAJOR 2 )
SET( GRPC_Analysor_VERSION_MINOR 7 )
SET( GRPC_Analysor_VERSION_PATCH 1 )
include(CheckIncludeFiles)
include(CheckCXXCompilerFlag)

#CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
#if(COMPILER_SUPPORTS_CXX14)
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
#else()
#  CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX0X)
#  if(COMPILER_SUPPORTS_CXX11)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#  else()
#    if(COMPILER_SUPPORTS_CXX0X)
#      CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
#      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
#    else()
#      message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please update your *$!%+£ C++ compiler.")
#    endif() 
#  endif()
#endif()


file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/SlowControl ${CMAKE_CURRENT_SOURCE_DIR}/bin/ElogXML)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/DetectorGeometry ${CMAKE_CURRENT_SOURCE_DIR}/src/xml DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/LCIO/src/cpp/include/ DESTINATION ${PROJECT_INCLUDE_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/build/include/)
file(COPY ${PROJECT_INCLUDE_DIR}/pre-generated/ DESTINATION ${PROJECT_INCLUDE_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -ansi -fno-strict-aliasing -lpthread -ldl -lm -fPIC" )
#list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/Modules/)
#list(APPEND CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake/)
#find_package(ROOT REQUIRED)
#include_directories(${ROOT_INCLUDE_DIR})


# Load Macro to locate ROOT and needed libraries
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/src/Modules/)
#set(ROOT_REQUIRED_COMPONENTS Core RIO Net Hist Graf Graf3d Gpad Tree Rint Postscript Matrix Physics MathCore Thread Gui TreePlayer Minuit TMVA)
#set(ROOT_OPTIONAL_COMPONENTS Proof)
find_package(ROOT REQUIRED #${ROOT_REQUIRED_COMPONENTS}
#OPTIONAL_COMPONENTS ${ROOT_OPTIONAL_COMPONENTS}
)

# Set preprocessor flags for optional components
#foreach(opt ${ROOT_OPTIONAL_COMPONENTS})
#  if(ROOT_${opt}_LIBRARY)
#    add_definitions(-Duse_${opt})
#  endif()
#endforeach()
include_directories(${ROOT_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")


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
#target_compile_options(LCIO PRIVATE --std=c++11)
ADD_SUBDIRECTORY( src/LCIO )
ADD_SUBDIRECTORY( src/Elog )
ADD_SUBDIRECTORY( src/HistoGraphs )
if(HaveOCCI)
ADD_SUBDIRECTORY( src/Database )
endif()
#ADD_SUBDIRECTORY( src/sio )
ADD_SUBDIRECTORY( src/Config )
#ADD_SUBDIRECTORY( src/lcio )
ADD_SUBDIRECTORY( src/Geometry )
ADD_SUBDIRECTORY( src/Marlino )
ADD_SUBDIRECTORY( src/OutFileRoot )
ADD_SUBDIRECTORY( src/Processors )
ADD_SUBDIRECTORY( src/Reader )
ADD_SUBDIRECTORY( src/tdrstyle )
ADD_SUBDIRECTORY( src/Utils )
ADD_SUBDIRECTORY( src/tinyxml )
ADD_SUBDIRECTORY( src/Converter )
<<<<<<< HEAD
=======
#ADD_SUBDIRECTORY( src/jsoncpp )
>>>>>>> beta

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()
    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/src/Modules/Doxyfile.in)
    set(doxyfile ${CMAKE_CURRENT_SOURCE_DIR}/build/Doxyfile)
    configure_file(${doxyfile_in} ${doxyfile} @ONLY)
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/doc/html DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/doc)
endif()

#ADD_SUBDIRECTORY( Simulation )
#ADD_SUBDIRECTORY( Plot )
