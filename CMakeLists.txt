CMAKE_MINIMUM_REQUIRED (VERSION 3.11)

PROJECT (Airport_Lookup_System
	VERSION 0.1
	DESCRIPTION "A three tiered airport lookup system"
	LANGUAGES C CXX)

# Path to project cmake modules
SET (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

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

# The executable code is here
# ADD_SUBDIRECTORY(apps)

# Testing only available if this is the main app
IF (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
	INCLUDE(FetchContent)
	ADD_SUBDIRECTORY(tests)
ENDIF()