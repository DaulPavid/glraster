#
# This file is part of glraster.
#

###########################
# Add executable includes #
###########################
list(APPEND GLRASTER_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/deps/nuklear
    ${CMAKE_SOURCE_DIR}/deps/nuklear/demo/glfw_opengl3
    ${GLFW_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
)

############################
# Add executable libraries #
############################
list(APPEND GLRASTER_LIBRARIES
    ${CMAKE_DL_LIBS}
    ${OPENGL_LIBRARIES}
    ${GLFW_LIBRARIES}
    m
)

###############################
# Add executable source files #
###############################
list(APPEND GLRASTER_SOURCES
    display.c
    file_reader.c
    main.c
)

# Add the gl3w sources
list(APPEND GLRASTER_SOURCES GL/gl3w.c)

###########################
# Add glraster executable #
###########################
add_executable(glraster ${GLRASTER_SOURCES})

target_include_directories(glraster PRIVATE ${GLRASTER_INCLUDE_DIRS})
target_link_libraries(glraster ${GLRASTER_LIBRARIES})
