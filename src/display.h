/*!
 *  This file is part of glraster.

 *  \file display.h
 *  \brief Symbol raster display and options dialog
 */

#pragma once

#include "GL/gl3w.h"
#include <GLFW/glfw3.h>

struct shader
{
    GLuint vert;
    GLuint frag;

    GLuint prog;
};

struct texture
{
    GLuint id;
};

struct quad
{
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
};

struct raster_display
{
    struct nk_context* ctx;

    struct shader shader_info;
    struct texture tex_info;
    struct quad quad_info;

    int buffer_size;
    int file_size;
    int file_offset;

    int frame_count;
    int frame_length;
    int frame_offset;

    int unpack;

    int w;
    int h;
};

/* Raster display methods */
struct raster_display* raster_display_init (struct nk_context* ctx,
                                            int w, int h,
                                            size_t file_size,
                                            size_t buffer_size);

void raster_display_free (struct raster_display* display);

void raster_display_draw_dialog (struct raster_display* display);
void raster_display_draw (struct raster_display* display, const char* data);

void raster_display_render (struct raster_display* display);
