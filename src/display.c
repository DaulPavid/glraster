/*!
 *  This file is part of glraster.
 *
 *  \file display.c
 *  \brief Symbol raster display and dialog
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "display.h"
#include "nuklear.h"

static const char* raster_vert_shader =
    "#version 330 core\n"
    "layout (location = 0) in vec2 i_pos;\n\n"
    "layout (location = 1) in float i_u;\n"
    "out float o_u;\n"
    "void main() {\n"
    "gl_Position = vec4(i_pos, 0.0, 1.0);\n"
    "o_u = i_u;\n"
    "}";

static const char* raster_frag_shader =
    "#version 330 core\n"
    "in float o_u;\n"
    "uniform float total_size;\n"
    "uniform float frame_size;\n"
    "uniform float offset;\n"
    "uniform sampler1D g_tex;\n"
    "void main() {\n"
    "float x = (o_u * frame_size + offset) / total_size;\n"
    "vec4 color = texture(g_tex, x);\n"
    "gl_FragColor = vec4(color.x, color.x, color.x, 1.0);\n"
    "}";

static void
quad_create (struct raster_display* display)
{
    struct quad *quad_info = &display->quad_info;

    glGenVertexArrays(1, &quad_info->vao);
    glBindVertexArray(quad_info->vao);

    glGenBuffers(1, &quad_info->vbo);
    GLfloat verts[] =
    {
        -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 1.0f,
         1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,
    };

    glBindBuffer(GL_ARRAY_BUFFER, quad_info->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glGenBuffers(1, &quad_info->ebo);
    GLuint elems[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad_info->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elems), elems, GL_STATIC_DRAW);

    GLint pos_attrib = glGetAttribLocation(display->shader_info.prog, "i_pos");
    glEnableVertexAttribArray(pos_attrib);
    glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE,
                          3*sizeof(GLfloat), 0);

    GLint uv_attrib = glGetAttribLocation(display->shader_info.prog, "i_u");
    glEnableVertexAttribArray(uv_attrib);
    glVertexAttribPointer(uv_attrib, 1, GL_FLOAT, GL_FALSE,
                          3*sizeof(GLfloat), (GLvoid *) (2*sizeof(GLfloat)));

    glBindVertexArray(0);
}

static void
quad_delete (struct raster_display* display)
{
    struct quad *quad_info = &display->quad_info;

    glDeleteBuffers(1, &quad_info->ebo);
    glDeleteBuffers(1, &quad_info->vbo);
    glDeleteVertexArrays(1, &quad_info->vao);
}

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
    return 0;
}

static void
raster_display_tex_init (struct raster_display* display, const char* data)
{
    glGenTextures(1, &display->tex_info.id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, display->tex_info.id);

    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB,
                 display->frame_length,
                 0, GL_RED, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_1D, 0);
}

static void
raster_display_tex_tick (struct raster_display* display, const char* data)
{
    glBindTexture(GL_TEXTURE_1D, display->tex_info.id);
    glTexSubImage1D(GL_TEXTURE_1D, 0, 0, display->frame_length,
                    GL_RED, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_1D, 0);
}

struct raster_display*
raster_display_init (struct nk_context* ctx,
                     int w, int h,
                     size_t file_size,
                     size_t buffer_size)
{
    struct raster_display *display = malloc(sizeof(struct raster_display));

    display->ctx = ctx;

    display->file_size = file_size;
    display->file_offset = 0;
    display->buffer_size = buffer_size;

    display->frame_length = buffer_size;
    display->frame_offset = 0;

    display->w = w;
    display->h = h;

    if (raster_display_shader_init(display) < 0)
    {
        return NULL;
    }

    raster_display_tex_init(display, NULL);

    quad_create(display);

    return display;
}

void
raster_display_free (struct raster_display* display)
{
    quad_delete(display);

    glDeleteProgram(display->shader_info.prog);
    glDeleteTextures(1, &display->tex_info.id);

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
                        display->buffer_size, 1, 1);

        nk_layout_row_dynamic(ctx, 40, 1);
        nk_property_int(ctx, "Frame offset:", 1, &display->frame_offset,
                        display->buffer_size - display->frame_length, 1, 1);

        nk_layout_row_dynamic(ctx, 40, 1);
        nk_property_int(ctx, "File offset:", 0, &display->file_offset,
                        display->file_size, 1, 1);
    }
    nk_end(ctx);
}

void
raster_display_draw (struct raster_display* display, const char* data)
{
    raster_display_tex_tick(display, data);
}


static void
raster_display_tick (struct raster_display* display)
{
    GLint total_size = glGetUniformLocation(display->shader_info.prog,
                                            "total_size");
    GLint frame_size = glGetUniformLocation(display->shader_info.prog,
                                            "frame_size");

    GLint offset = glGetUniformLocation(display->shader_info.prog, "offset");

    glUniform1f(total_size, display->buffer_size);
    glUniform1f(frame_size, display->frame_length);
    glUniform1f(offset, display->frame_offset);
}

void
raster_display_render (struct raster_display* display)
{
    glViewport(0, 0, display->w, display->h);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(display->shader_info.prog);

    raster_display_tick(display);

    glBindTexture(GL_TEXTURE_1D, display->tex_info.id);

    glBindVertexArray(display->quad_info.vao);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_1D, 0);
    glBindVertexArray(0);
}
