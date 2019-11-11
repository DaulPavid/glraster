///
/// This file is part of glraster.
///
/// \file main.c
/// \brief Main application and event loop
///

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#include "file_reader.h"
#include "display.h"

// Program options
#include <getopt.h>

// OpenGL / GLFW
#include "GL/gl3w.h"
#include <GLFW/glfw3.h>

// Nuklear UI
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "nuklear.h"
#include "nuklear_glfw_gl3.h"

#define DEFAULT_WINDOW_PREFIX "glraster"

#define DEFAULT_WINDOW_W 1024
#define DEFAULT_WINDOW_H 768

#define DEFAULT_BUFFER_SIZE 512 * 1024;

#define MAX_TITLE_LEN 128
#define MAX_VERTEX_BUFFER 128 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

static void
glfw_error_callback(int error_code, const char* msg)
{
    fprintf(stderr, "[FAIL] - Error code = %d: %s\n", error_code, msg);
}

static void
print_help()
{
    fprintf(stdout, "  glraster usage / options      \n"
                    "--------------------------------\n"
                    " -f  --file    :    Input file  \n"
                    " -s  --size    :    Buffer size \n"
                    " -h  --help    :    Print help  \n");
}

int
main(int argc, char* argv[])
{
    static struct option option_list[] =
    {
        {"file", required_argument, 0, 'f'},
        {"size", required_argument, 0, 's'},
        {"help", no_argument,       0, 'h'}
    };

    //
    // CLI arguments
    //
    char file_path[MAX_FILE_LEN] = "\0";

    long int buffer_size = DEFAULT_BUFFER_SIZE;

    while (1)
    {
        int option_index = 0;
        int opt = getopt_long(argc, argv, "f:s:h", option_list, &option_index);

        if (opt == -1)
        {
            break;
        }

        switch (opt)
        {
            case 'f':
                strncpy(file_path, optarg, MAX_FILE_LEN);
                break;
            case 's':
                buffer_size = strtol(optarg, NULL, 10);
                break;
            case 'h':
            case '?': print_help(); return EXIT_SUCCESS;
            default:
                abort();
        }
    }

    if (strlen(file_path) <= 0)
    {
        fprintf(stderr, "[FAIL] - No input file provided\n");

        print_help();
        return EXIT_FAILURE;
    }

    struct file_reader *reader = file_reader_init(file_path, buffer_size);
    if (!reader)
    {
        fprintf(stderr, "[FAIL] - Failed to read input file in argument\n");
        return EXIT_FAILURE;
    }

    //
    // Platform and GLFW
    //
    int window_w = 0;
    int window_h = 0;

    GLFWwindow *window = NULL;

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        fprintf(stderr, "[FAIL] - GLFW failed to initialize\n");
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    char window_title[MAX_TITLE_LEN];

    snprintf(window_title, sizeof(window_title), "[%s] - %s",
             DEFAULT_WINDOW_PREFIX, "filename");

    window = glfwCreateWindow(DEFAULT_WINDOW_W, DEFAULT_WINDOW_H,
                              window_title, NULL, NULL);

    if (!window)
    {
        fprintf(stderr, "[FAIL] - Unable to create the window\n");
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetWindowSize(window, &window_w, &window_h);

    //
    // OpenGL
    //
    if (gl3wInit())
    {
        fprintf(stderr, "[FAIL] - gl3w failed to initialize\n");
        return EXIT_FAILURE;
    }

    glViewport(0, 0, DEFAULT_WINDOW_W, DEFAULT_WINDOW_H);

    //
    // Nuklear UI
    //
    struct nk_context *ctx = nk_glfw3_init(window, NK_GLFW3_INSTALL_CALLBACKS);
    {
        struct nk_font_atlas *atlas;
        nk_glfw3_font_stash_begin(&atlas);
        nk_glfw3_font_stash_end();
    }

    struct raster_display *display;
    display = raster_display_init(ctx, window_w, window_h);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        nk_glfw3_new_frame();

        display->w = window_w;
        display->h = window_h;

        raster_display_tick(display);

        //
        // Nuklear UI drawing routines
        //
        raster_display_draw_dialog(display);

        //
        // OpenGL Drawing stuff here...
        //

        //
        // Rendering
        //
        glfwGetWindowSize(window, &window_w, &window_h);
        glViewport(0, 0, window_w, window_h);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1, 0, 0, 0);

        nk_glfw3_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
        glfwSwapBuffers(window);
    }

    raster_display_free(display);

    nk_glfw3_shutdown();
    glfwTerminate();

    file_reader_free(reader);

    return EXIT_SUCCESS;
}
