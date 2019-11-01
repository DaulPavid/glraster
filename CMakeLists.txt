#
# This file is part of glraster.
#

###############################
# Project / compiler settings #
###############################
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(glraster C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/packages")

# Set the preferred OpenGL version
set(OpenGL_GL_PREFERENCE GLVND)

####################
# Project packages #
####################
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

###########################
# Project sub-directories #
###########################
add_subdirectory(src)
