///
/// This file is part of glraster.
///
/// \file display.h
/// \brief Symbol raster display and options dialog
///

#pragma once

#include <stddef.h>

#define MAX_FILE_LEN 1024

struct raster_display
{
    char file_path[MAX_FILE_LEN];

    size_t width;
    size_t offset;
    size_t num_bits;
};

