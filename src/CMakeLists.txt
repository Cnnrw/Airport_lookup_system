INCLUDE(rpcgen)

#ADD_CUSTOM_COMMAND (
#	OUTPUT  place_airport_common.h place_airport_common_xdr.c
#	COMMAND rpcgen -C "${CMAKE_CURRENT_BINARY_DIR}/place_airport_common.x"
#	COMMENT "Generating place_airport_common rpc protocol header and stubs..."
#)

CONFIGURE_FILE(protocol/place_airport_common.x
	${CMAKE_CURRENT_BINARY_DIR}/place_airport_common.x COPYONLY)

ADD_CUSTOM_COMMAND(
	OUTPUT place_airport_common.h
	COMMAND ${RPCGEN_EXECUTABLE} -C -h -o place_airport_common.h "${CMAKE_CURRENT_BINARY_DIR}/place_airport_common.x"
	COMMENT "Generating place_airport_common rpc protocol header and stubs..."
)

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

SET (data_source "${CMAKE_SOURCE_DIR}/data")
SET (data_dest "${CMAKE_BINARY_DIR}/data")
ADD_CUSTOM_COMMAND(
	TARGET client POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E create_symlink ${data_source} ${data_dest}
	DEPENDS ${data_dest}
	COMMENT "symbolic link resources folder from ${data_source} => ${data_dest}"
)
