///
/// This file is part of glraster.
///
/// \file display.c
/// \brief Symbol raster display and dialog
///

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "display.h"
#include "nuklear.h"

static const char* raster_vert_shader =
    "#version 330 core\n"
    "layout (location = 0) in vec3 i_pos;\n\n"
    "layout (location = 1) in vec2 i_uv;\n"
    "out vec2 o_uv;\n"
    "void main() {\n"
    "gl_Position = vec4(i_pos, 1.0);\n"
    "o_uv = i_uv;\n"
    "}";

static const char* raster_frag_shader =
    "#version 330 core\n"
    "in vec2 o_uv;\n"
    "uniform sampler2D g_tex;\n"
    "void main() {\n"
    "gl_FragColor = texture2D(g_tex, o_uv);\n"
    "}";

static int
raster_display_shader_init (struct raster_display* display)
{
    display->shader_info.vert = glCreateShader(GL_VERTEX_SHADER);
    display->shader_info.frag = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(display->shader_info.vert, 1, &raster_vert_shader, NULL);
    glShaderSource(display->shader_info.frag, 1, &raster_frag_shader, NULL);

    glCompileShader(display->shader_info.vert);
    glCompileShader(display->shader_info.frag);

    GLint vert_compiled;
    GLint frag_compiled;

    glGetShaderiv(display->shader_info.vert, GL_COMPILE_STATUS, &vert_compiled);
    glGetShaderiv(display->shader_info.frag, GL_COMPILE_STATUS, &frag_compiled);

    if (vert_compiled != GL_TRUE)
    {
        fprintf(stderr, "[FAIL] - Failed to compile vertex shader\n");
        return -1;
    }

    if (frag_compiled != GL_TRUE)
    {
        fprintf(stderr, "[FAIL] - Failed to compile fragment shader\n");
        return -1;
    }

    display->shader_info.prog = glCreateProgram();
    glAttachShader(display->shader_info.prog, display->shader_info.vert);
    glAttachShader(display->shader_info.prog, display->shader_info.frag);
    glLinkProgram(display->shader_info.prog);

    GLint prog_linked;
    glGetProgramiv(display->shader_info.prog, GL_LINK_STATUS, &prog_linked);
    if (prog_linked != GL_TRUE)
    {
        fprintf(stderr, "[FAIL] - Shader program failed to link\n");
        return -1;
    }

    fprintf(stdout, "[INFO] - Shader program linked\n");
}

struct raster_display*
raster_display_init (struct nk_context* ctx, int w, int h)
{
    struct raster_display *display = malloc(sizeof(struct raster_display));

    display->ctx = ctx;

    display->frame_length = 32;
    display->frame_offset = 0;
    display->frame_bps = 1;

    display->zoom_factor = 1.2f;

    display->w = w;
    display->h = h;

    if (!raster_display_shader_init(display))
    {
        return NULL;
    }

    return display;
}

void
raster_display_free (struct raster_display* display)
{
    glDeleteProgram(display->shader_info.prog);

    free(display);
}

void
raster_display_draw_dialog (struct raster_display* display)
{
    struct nk_context *ctx = display->ctx;

    nk_flags flags = 0
        | NK_WINDOW_BORDER
        | NK_WINDOW_MOVABLE
        | NK_WINDOW_SCALABLE
        | NK_WINDOW_MINIMIZABLE
        | NK_WINDOW_TITLE;

    if (nk_begin(ctx, "Options", nk_rect(50, 50, 300, 300), flags))
    {
        nk_layout_row_dynamic(ctx, 40, 1);
        nk_property_int(ctx, "Frame length:", 1, &display->frame_length,
                        MAX_FRAME_LEN, 1, 1);

        nk_layout_row_dynamic(ctx, 40, 1);
        nk_property_int(ctx, "Frame offset:", 0, &display->frame_offset,
                        display->frame_length, 1, 1);

        nk_layout_row_dynamic(ctx, 40, 1);
        nk_property_int(ctx, "Frame bps:", 1, &display->frame_bps,
                        MAX_FRAME_BPS, 1, 1);
    }
    nk_end(ctx);
}

void
raster_display_draw (struct raster_display* display)
{
}

void
raster_display_tick (struct raster_display* display)
{
    struct nk_context *ctx = display->ctx;
    struct nk_input *input = &ctx->input;
}
