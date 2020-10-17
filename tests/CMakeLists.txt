PROJECT(Airport_tests)

INCLUDE(FetchContent)

FetchContent_Declare(
		googletest
		GIT_REPOSITORY https://github.com/google/googletest.git
		GIT_TAG        release-1.10.0
)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
		FetchContent_Populate(googletest)
		ADD_SUBDIRECTORY(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

INCLUDE_DIRECTORIES(${googletest_SOURCE_DIR}/include ${googletest_SOURCE_DIR})

################################################################################
# Test Runner
################################################################################

