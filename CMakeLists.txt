CMAKE_MINIMUM_REQUIRED (VERSION 3.11)
PROJECT (airport_lookup_system)

SET (CMAKE_CXX_STANDARD 14)

INCLUDE_DIRECTORIES (.)
INCLUDE_DIRECTORIES (include)
INCLUDE_DIRECTORIES (libs)
INCLUDE_DIRECTORIES (libs/catch2)

ADD_EXECUTABLE (airport_lookup_system
		docs/hw4.pdf
		libs/catch2/catch.hpp
		places/airport-locations.txt
		places/places2k.txt
		scripts/airports.x
		scripts/places.x
		README.md)
