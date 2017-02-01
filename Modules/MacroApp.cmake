################################################################################
# CMakeLists.txt
#
# Created on: Nov 3, 2010
# Copyright Martin Wojtczyk <martin.wojtczyk@gmail.com>
################################################################################
macro(APP_MACRO)

# set APP_NAME to enclosing directory name or Parameter 1
if(${ARGC} GREATER 0)
	set(APP_NAME "${ARGV0}")
else(${ARGC} GREATER 0)
	get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
	set(APP_NAME "${DIR_NAME}")
endif(${ARGC} GREATER 0)

message(STATUS "Application: ${APP_NAME}")

project(${APP_NAME})

find_package(PostgreSQL)
find_package(Boost 1.36.0 COMPONENTS date_time)
#find_package(libosal)

include_directories(
	${src_BINARY_DIR}
	${POSTGRESQL_INCLUDE_DIR}
	${Boost_INCLUDE_DIRS}
#	${LIBOSAL_INCLUDES}
)

# if SRCS is not defined, set SRCS to main.cpp
if(NOT DEFINED SRCS)
	set(SRCS main.cpp)
endif(NOT DEFINED SRCS)

# if APP_VERSION is not defined, set APP_VERSION to VERSION
# if VERSION is not defined, set APP_VERSION to 0.1
if(NOT DEFINED APP_VERSION)
	if(NOT DEFINED VERSION)
		set(APP_VERSION 0.1)
	else(NOT DEFINED VERSION)
		set(APP_VERSION ${VERSION})
	endif(NOT DEFINED VERSION)
endif(NOT DEFINED APP_VERSION)

add_executable(${APP_NAME} ${HDRS} ${SRCS})
target_link_libraries(${APP_NAME}
	${POSTGRESQL_LIBRARY}
	${Boost_LIBRARIES}
#	${LIBOSAL_LIBRARIES}
)

set_target_properties("${APP_NAME}" PROPERTIES DEBUG_POSTFIX "_d")
set_target_properties("${APP_NAME}" PROPERTIES VERSION ${APP_VERSION})

install(TARGETS "${APP_NAME}"
	RUNTIME DESTINATION bin
)

endmacro(APP_MACRO)
