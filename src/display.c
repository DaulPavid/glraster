///
/// This file is part of glraster.
///
/// \file display.c
/// \brief Symbol raster display and dialog
///

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "display.h"
#include "nuklear.h"

static const char* raster_vert_shader =
    ""
    "";

static const char* raster_frag_shader =
    "";

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

    return display;
}

void
raster_display_free (struct raster_display* display)
{
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
        nk_layout_row_begin(ctx, NK_STATIC, 40, 1);
        {
            nk_layout_row_push(ctx, 40);
            nk_property_int(ctx, "Frame length:", 1,
                            &display->frame_length,
                            MAX_FRAME_LEN, 1, 1);

            nk_layout_row_push(ctx, 40);
            nk_property_int(ctx, "Frame offset:", 0,
                            &display->frame_offset,
                            display->frame_length, 1, 1);

            nk_layout_row_push(ctx, 40);
            nk_property_int(ctx, "Frame bps:", 1,
                            &display->frame_bps,
                            MAX_FRAME_BPS, 1, 1);
        }
        nk_layout_row_end(ctx);
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
}
