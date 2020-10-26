CMAKE_MINIMUM_REQUIRED (VERSION 3.11)

PROJECT (Airport_Lookup_System
	VERSION 0.1
	DESCRIPTION "A three tiered airport lookup system"
	LANGUAGES C CXX)

# Path to project cmake modules
SET (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

SET (CMAKE_CXX_STANDARD 11)
SET (CMAKE_CXX_STANDARD_REQUIRED ON)
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

### Add macros ###
INCLUDE(pkg-config)
INCLUDE(rpc)

# Only do these if this is the main project, and not it it is included through
# add_subdirectory
IF (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	# Optionally set things like CMAKE_CXX_STANDARD,
	# CMAKE_POSITION_INDEPENDENT_CODE here

	# Let's ensure -std=c++xx instead of -std=g++xx
	SET (CMAKE_CXX_EXTENSIONS OFF)

	# Let's nicely support folders in IDE's
	SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

	ENABLE_TESTING() # Must be in root CMakeLists.txt
ENDIF()

# The compiled library code is here
ADD_SUBDIRECTORY(src)

#MACRO (INSTALL_HEADERS_WITH_DIRECTORY HEADER_LIST)
#	FOREACH(HEADER ${${HEADER_LIST}})
#		STRING(REGEX MATCH "(.\\\*)\\\[/\\\]" DIR ${HEADER})
#		INSTALL(FILES ${HEADER} DESTINATION include/${DIR})
#	ENDFOREACH(HEADER)
#ENDMACRO(INSTALL_HEADERS_WITH_DIRECTORY)
#
#INSTALL_HEADERS_WITH_DIRECTORY(src/protocol/prog_limits.h)

# Testing only available if this is the main app
IF (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
	INCLUDE(FetchContent)
	ADD_SUBDIRECTORY(tests)
ENDIF()