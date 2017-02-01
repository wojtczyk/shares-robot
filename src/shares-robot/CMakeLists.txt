################################################################################
# CMakeLists.txt
#
# Created on: Nov 24, 2010
# Copyright Martin Wojtczyk <martin.wojtczyk@gmail.com>
################################################################################
include(MacroApp)

set(HDRS
	Database.h
	Stock.h
	Robot.h
)

set(SRCS
	Database.cpp
	Stock.cpp
	Robot.cpp
	main.cpp
)

APP_MACRO()
