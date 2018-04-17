########################################################
# cmake file for building Marlin example Package
# @author Jan Engels, Desy IT
CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)
########################################################


# project name
PROJECT( streamlog )


# project version
SET( ${PROJECT_NAME}_VERSION_MAJOR 0 )
SET( ${PROJECT_NAME}_VERSION_MINOR 1 )
SET( ${PROJECT_NAME}_VERSION_PATCH 0 )

# WARNING: Don't forget to update the svn:externals property of cmake4hep in
# your branches/pre-tag before creating the final tag !!!
# to check current externals use: svn propget svn:externals -R
# to change the externals use (top-level dir of your project):
# svn propset svn:externals 'cmake4hep https://svnsrv.desy.de/public/aidasoft/cmake4hep/trunk' .

# load cmake4hep modules and default settings
FIND_PACKAGE( cmake4hep QUIET )
IF( NOT cmake4hep_FOUND )
    MESSAGE( STATUS "using built-in cmake4hep modules" )
    # use built-in cmake4hep if package not found!!
    LIST( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake4hep )
ELSE()
    MESSAGE( STATUS "using cmake4hep modules from: ${cmake4hep_ROOT}" )
ENDIF()
INCLUDE( set_default_settings )


### DOCUMENTATION ###########################################################

#OPTION( INSTALL_DOC "Set to OFF to skip build/install Documentation" ON )

#IF( INSTALL_DOC )
#    ADD_SUBDIRECTORY( ./doc )
#ELSE()
#    MESSAGE( STATUS "INSTALL_DOC set to OFF" )
#ENDIF()


### LIBRARY #################################################################

# definitions to pass to the compiler
ADD_DEFINITIONS( "-Wall -ansi -pedantic" )

# include directories
INCLUDE_DIRECTORIES( BEFORE ./source/include )
INSTALL_DIRECTORY( ./source/include DESTINATION . FILES_MATCHING PATTERN "*.h" )

# add library
AUX_SOURCE_DIRECTORY( ./source/src library_sources )
ADD_SHARED_LIBRARY( ${PROJECT_NAME} ${library_sources} )
INSTALL_SHARED_LIBRARY( ${PROJECT_NAME} DESTINATION lib )

# display some variables and write them to cache
DISPLAY_STD_VARIABLES()

# generate and install following configuration files
GENERATE_PACKAGE_CONFIGURATION_FILES( streamlogConfig.cmake streamlogConfigVersion.cmake )

