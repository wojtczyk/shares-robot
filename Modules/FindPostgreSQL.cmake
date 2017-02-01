################################################################################
# CMakeLists.txt
#
# Created on: Feb 28, 2011
# Copyright Martin Wojtczyk <martin.wojtczyk@gmail.com>
################################################################################
FIND_PATH(POSTGRESQL_INCLUDE_DIR libpq-fe.h
	${CMAKE_INCLUDE_PATH}
	${CMAKE_FRAMEWORK_PATH}
	$ENV{POSTGRESQLDIR}/include
	/usr/local/include/postgres
	/usr/local/include
	/usr/include/postgres
	/usr/include
	"$ENV{ProgramFiles}/PostgreSQL/include"
)

IF (NOT POSTGRESQL_INCLUDE_DIR_FOUND)
	IF (POSTGRESQL_INCLUDE_DIR)
		MESSAGE(STATUS "Looking for PostgreSQL headers -- found " ${POSTGRESQL_INCLUDE_DIR}/libpq-fe.h)
		SET(POSTGRESQL_INCLUDE_DIR_FOUND 1 CACHE INTERNAL "PostgreSQL headers found")
	ELSE (POSTGRESQL_INCLUDE_DIR)
		MESSAGE(FATAL_ERROR 
			"-- Looking for PostgreSQL headers -- not found\n"
			"Please install PostgreSQL http://www.postgresql.org/ or adjust CMAKE_INCLUDE_PATH\n"
			"e.g. cmake -DCMAKE_INCLUDE_PATH=/path-to-PostgreSQL/include ..."
		)
	ENDIF (POSTGRESQL_INCLUDE_DIR)
ENDIF (NOT POSTGRESQL_INCLUDE_DIR_FOUND)

FIND_LIBRARY(POSTGRESQL_LIBRARY
	NAMES pq
	PATHS
	${CMAKE_LIBRARY_PATH}
	${CMAKE_FRAMEWORK_PATH}
	$ENV{POSTGRESQLDIR}/lib
	/usr/local/lib
	/usr/lib
	"$ENV{ProgramFiles}/PostgreSQL/lib"
)

IF (NOT POSTGRESQL_LIBRARY_FOUND)
	IF (POSTGRESQL_LIBRARY)
		MESSAGE(STATUS "Looking for PostgreSQL library -- found " ${POSTGRESQL_LIBRARY})
		SET(POSTGRESQL_LIBRARY_FOUND 1 CACHE INTERNAL "PostgreSQL library found")
	ELSE (POSTGRESQL_LIBRARY)
		MESSAGE(FATAL_ERROR 
			"-- Looking for PostgreSQL library -- not found\n"
			"Please install PostgreSQL http://www.postgresql.org/ or adjust CMAKE_LIBRARY_PATH\n"
			"e.g. cmake -DCMAKE_LIBRARY_PATH=/path-to-PostgreSQL/lib ..."
		)
	ENDIF (POSTGRESQL_LIBRARY)
ENDIF (NOT POSTGRESQL_LIBRARY_FOUND)

MARK_AS_ADVANCED(
	POSTGRESQL_FOUND
	POSTGRESQL_INCLUDE_DIR
	POSTGRESQL_LIBRARY
) 
