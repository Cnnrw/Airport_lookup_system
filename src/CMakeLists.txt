FIND_PROGRAM(RPCGEN_EXECUTABLE rpcgen DOC "path to the rpcgen executable")
MARK_AS_ADVANCED(RPCGEN_EXECUTABLE)

IF(NOT RPCGEN_EXECUTABLE)
	MESSAGE(FATAL_ERROR "Could not find rpcgen")
ENDIF()

### Find tirpc and set cache vars
CHECK_RPC()
MESSAGE(STATUS "RPC_INCLUDE_DIRS ${RPC_INCLUDE_DIRS}")

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

################################################################################
# Common
################################################################################
SET(COMMON_HEADER_LIST
	${PROJECT_SOURCE_DIR}/include/place_airport_common.h
	${PROJECT_SOURCE_DIR}/include/common.h)

ADD_LIBRARY(common
	common.cpp
	places_airports_clnt.c
	place_airport_common_xdr.c
	${COMMON_HEADER_LIST})
TARGET_COMPILE_FEATURES(common PUBLIC cxx_std_11)
TARGET_LINK_LIBRARIES(common ${TIRPC_LIBRARIES})

################################################################################
# Airport
################################################################################
SET (AIRPORT_HEADER_LIST
	${PROJECT_SOURCE_DIR}/include/airports/airports.h
	${PROJECT_SOURCE_DIR}/include/airports/KDTree.h)

ADD_EXECUTABLE(airport_server
	airports_server.cpp
	KDTree.cpp
	${AIRPORT_HEADER_LIST})
TARGET_LINK_LIBRARIES(airport_server common)

################################################################################
# Places
################################################################################
SET (PLACES_HEADER_LIST
	${PROJECT_SOURCE_DIR}/include/places/places.h
	${PROJECT_SOURCE_DIR}/include/places/trie.h)

ADD_EXECUTABLE(places_server
	places_server.cpp
	trie.cpp
	${PLACES_HEADER_LIST})
TARGET_LINK_LIBRARIES(places_server common)

################################################################################
# Client
################################################################################
ADD_EXECUTABLE(client places_client.cpp)
TARGET_LINK_LIBRARIES(client common)
