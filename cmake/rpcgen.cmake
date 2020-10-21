MACRO (RPCGEN src)
	# Clean up the generated files
#	FILE (REMOVE ${gen_xdr_dir})
#	FILE (MAKE_DIRECTORY ${gen_xdr_dir})

	# We are generating and/or copying the original files to
	# gen_xdr_dir
#	INCLUDE_DIRECTORIES (${gen_xdr_dir})

	# "copied" files
#	SET (x_tmp_plat_h ${gen_xdr_dir}/${X}.h)
#	SET (x_tmp_x      ${gen_xdr_dir}/${X}.x)
	# We need a canonical name, so that rpcgen generates the
	# C source with relative includes paths
#	SET (x_tmp_x_canonical_name ${X}.x)

	# Generated or copied files
#	SET (x_gen_h      ${gen_xdr_dir}/${X}.h)
#	SET (x_gen_c      ${gen_xdr_dir}/${X}_xdr.c)
	SET (x_gen_h      ${src}.h)
	SET (x_gen_c      ${src}.c)

	# original files that we are copying or generating from
	SET (xfile "${CMAKE_SOURCE_DIR}/src/protocol/${src}.x")
	SET (hfile "include/${src}.h")
	SET (cfile "${src}.c")
	SET (xdrfile "${src}_xdr.c")

	# xdrgen(1) doesn't produce the cleanest code imaginable
	SET_SOURCE_FILES_PROPERTIES ("${cfile}" PROPERTIES
		COMPILE_FLAGS "-Wno-unused-variable")

	ADD_CUSTOM_COMMAND (
		OUTPUT "${cfile}" "${hfile}"
		COMMAND rm -f "${cfile}" "${hfile}"
		COMMAND rpcgen -C -h -o "${hfile}" "${xfile}"
		COMMAND rpcgen -C -c -o "${cfile}" "${xfile}"
		DEPENDS "${xfile}"
	)
ENDMACRO ()