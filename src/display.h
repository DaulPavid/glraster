///
/// This file is part of glraster.
///
/// \file display.h
/// \brief Symbol raster display and options dialog
///

#pragma once

#include "GL/gl3w.h"
#include <GLFW/glfw3.h>

#define MAX_FRAME_LEN 1048576
#define MAX_FRAME_BPS 1024

#define SYM_BLOCK_SIZE 32

struct shaders
{
    GLuint vert;
    GLuint frag;

    GLuint prog;
};

struct raster_display
{
    struct nk_context* ctx;

    struct shaders shader_info;

    int frame_length;
    int frame_offset;
    int frame_bps;

    float zoom_factor;

    int w;
    int h;

    int x_0, x_1;
    int y_0, y_1;
};

struct raster_display* raster_display_init (struct nk_context* ctx, int w, int h);

void raster_display_free (struct raster_display* display);

void raster_display_draw_dialog (struct raster_display* display);
void raster_display_draw (struct raster_display* display);

void raster_display_tick (struct raster_display* display);
