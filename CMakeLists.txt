################################################################################
# CMakeLists.txt
#
# Created on: Nov 23, 2010
# Copyright Martin Wojtczyk <martin.wojtczyk@gmail.com>
################################################################################
cmake_minimum_required(VERSION 2.8)

# Set the Project root name
get_filename_component(PROJECT_ROOT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_ROOT_NAME} CXX C)

# Set Package Base Parameters
set(PACKAGE ${PROJECT_NAME})
set(VERSION 0.2.0)

set(PACKAGE_NAME ${PACKAGE})
set(PACKAGE_VERSION ${VERSION})
set(PACKAGE_BUGREPORT martin.wojtczyk@gmail.com)
set(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION} - ${PROJECT_NAME} application.")
set(PACKAGE_TARNAME ${PACKAGE})

add_subdirectory(src)

include(CPack)
