INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

FIND_PROGRAM(RPCGEN_EXECUTABLE rpcgen DOC  "path to the rpcgen executable")
MARK_AS_ADVANCED(RPCGEN_EXECUTABLE)

CONFIGURE_FILE("protocol/place_airport_common.x" ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

ADD_CUSTOM_COMMAND (
	OUTPUT  place_airport_common.h place_airport_common_xdr.c
	COMMAND rpcgen -C "${CMAKE_CURRENT_BINARY_DIR}/place_airport_common.x"
	COMMENT "Generating place_airport_common rpc protocol header and stubs..."
)

################################################################################
# Common
################################################################################
SET(COMMON_HEADER_LIST
	"${PROJECT_SOURCE_DIR}/include/common.h")

ADD_LIBRARY(common common.cpp ${COMMON_HEADER_LIST})
TARGET_COMPILE_FEATURES(common PUBLIC cxx_std_11)

################################################################################
# Airport
################################################################################
SET (AIRPORT_HEADER_LIST
	${PROJECT_SOURCE_DIR}/include/airports/KDTree.h)

ADD_LIBRARY(airports
	KDTree.cpp
	${AIRPORT_HEADER_LIST})
TARGET_LINK_LIBRARIES(airports common)
TARGET_COMPILE_FEATURES(airports PUBLIC cxx_std_11)

################################################################################
# Places
################################################################################
SET (PLACES_HEADER_LIST
	${PROJECT_SOURCE_DIR}/include/places/trie.h)

ADD_LIBRARY(places
	trie.cpp
	${PLACES_HEADER_LIST})
TARGET_LINK_LIBRARIES(places common)
TARGET_COMPILE_FEATURES(places PUBLIC cxx_std_11)
