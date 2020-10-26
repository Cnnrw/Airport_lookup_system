### Locate `rpcgen` executable
FIND_PROGRAM(RPCGEN_EXECUTABLE rpcgen DOC "path to the rpcgen executable")
MARK_AS_ADVANCED(RPCGEN_EXECUTABLE)

IF(NOT RPCGEN_EXECUTABLE)
	MESSAGE(FATAL_ERROR "Could not find rpcgen")
ENDIF()

### Find tirpc and set cache vars
CHECK_RPC()
MESSAGE(STATUS "RPC_INCLUDE_DIRS ${RPC_INCLUDE_DIRS}")

### Gen func
FUNCTION(GEN_HEADER file out)

ENDFUNCTION()

FUNCTION(GEN_C file out)
ENDFUNCTION()

FUNCTION(GEN_XDR file out)
ENDFUNCTION()