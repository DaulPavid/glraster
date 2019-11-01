#
# This file is part of cdsp.
#

#
# GLFW_FOUND
# GLFW_INCLUDE_DIR
# GLFW_LIBRARIES
#

find_path(GLFW_INCLUDE_DIR
    NAMES GLFW/glfw3.h
    PATHS /usr/include
          /usr/local/include
         "C:/Program Files (x86)/GLFW/include"
         "C:/Program Files/GLFW/include"
)

find_library(GLFW_LIBRARIES
    NAMES glfw glfw3
    PATHS /usr/lib
          /usr/local/lib
          /usr/lib/x86_64-linux-gnu
          "C:/Program Files (x86)/GLFW/lib"
          "C:/Program Files/GLFW/lib"
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(glfw3 GLFW_LIBRARIES GLFW_INCLUDE_DIR)
mark_as_advanced(GLFW_INCLUDE_DIR GLFW_LIBRARIES)
