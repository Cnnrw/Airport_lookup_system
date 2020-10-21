MACRO(CHECK_PKGCONFIG)
	IF (LINUX OR FREEBSD)
		# Do a sanity check, before bailing out in FIND_PACKAGE below.
		FIND_PROGRAM(PKG_CONFIG_EXECUTABLE NAMES pkg-config
			DOC "pkg-config executable")
		IF (NOT PKG_CONFIG_EXECUTABLE)
			MESSAGE(WARNING "Cannot find pkg-config. You need to "
				"install the required package:\n"
				"   Debian/Ubuntu:              apt install pkg-config\n"
				"   RedHat/Fedora/Oracle Linux: yum install pkg-config\n"
				"   SuSe:                       zypper install pkg-config\n"
				)
		ENDIF()
		FIND_PACKAGE(PkgConfig REQUIRED)
	ENDIF()
ENDMACRO()