///
/// This file is part of glraster.
///
/// \file parser.h
/// \brief File parser for sample captures
///

#pragma once

#include <stddef.h>
#include <stdint.h>

#define MAX_FILE_LEN 1024

struct file_parser
{
    char* file_path[MAX_FILE_LEN];

    size_t total_size;
    size_t block_size;

    size_t offset;
};
