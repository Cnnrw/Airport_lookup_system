# FineTIRPC
# ---------
#
# Find the native TIRPC includes and library
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module will set the following variables in your project:
#
# ``TIRPC_INCLUDE_DIRS``
#   where to find rpc.h, etc.
# ``TIRPC_LIBRARIES``
#   the libraries to link against to use TIRPC.
# ``TIRPC_VERSION``
#   the version of TIRPC found.
# ``TIRPC_FOUND``
#   true if the TIRPC headers and libraries were found.

FIND_PACKAGE(PkgConfig QUIET)
pkg_check_modules(TIRPC libtirpc)

FIND_PATH(TIRPC_INCLUDE_DIRS
	NAMES rpc/rpc.h
	PATH_SUFFIXES tirpc
	HINTS ${TIRPC_INCLUDE_DIRS}
	HINTS /usr/include/tirpc
)

FIND_LIBRARY(TIRPC_LIBRARIES
	NAMES tirpc
	HINTS ${TIRPC_LIBRARY_DIRS}
	HINTS /usr/include/tirpc
)

SET(TIRPC_VERSION ${TIRPC_VERSION})

INCLUDE(FindPackageHandleStandardArgs)

find_package_handle_standard_args(TIRPC
	REQUIRED_VARS TIRPC_LIBRARIES TIRPC_INCLUDE_DIRS
	VERSION_VAR TIRPC_VERSION
)

MARK_AS_ADVANCED(TIRPC_INCLUDE_DIRS TIRPC_LIBRARIES)