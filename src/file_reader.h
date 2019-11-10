///
/// This file is part of glraster.
///
/// \file file_reader.h
/// \brief File reader for sample captures
///

#pragma once

#include <stddef.h>
#include <stdint.h>

#define MAX_FILE_LEN 4096

#define MAX_SYMBOL_SIZE 64
#define MAX_BUFFER_SIZE 128000000L

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

struct file_parser
{
    char file_path[MAX_FILE_LEN];

    size_t file_size;
    size_t file_offset;

    size_t buffer_size;

    char* buffer;
};

struct file_parser* file_parser_init (const char* file_path, size_t buffer_size);

void file_parser_free (struct file_parser* parser);
void file_parser_tick (struct file_parser* parser);
