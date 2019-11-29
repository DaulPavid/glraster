/*!
 *  This file is part of glraster.

 *  \file display.h
 *  \brief Symbol raster display and options dialog
 */

#pragma once

#include "GL/gl3w.h"
#include <GLFW/glfw3.h>

#define MAX_FRAME_LEN (1 << 20)
#define MAX_FRAME_CNT (1 << 10)

struct shader
{
    GLuint vert;
    GLuint frag;

    GLuint prog;
};

struct texture
{
    GLuint id;

    int w;
    int h;
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

    int frame_length;
    int frame_offset;
    int frame_count;

    int w;
    int h;

    int x_0, x_1;
    int y_0, y_1;
};

/* Raster display methods */
struct raster_display* raster_display_init (struct nk_context* ctx, int w, int h);

void raster_display_free (struct raster_display* display);

void raster_display_draw_dialog (struct raster_display* display);
void raster_display_draw (struct raster_display* display, const char* data);

void raster_display_render (struct raster_display* display);

void raster_display_tick (struct raster_display* display);
