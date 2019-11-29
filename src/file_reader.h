/*!
 *  This file is part of glraster.
 *
 *  \file file_reader.h
 *  \brief File reader for sample captures
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

#define MAX_FILE_LEN 4096

#define MAX_SYMBOL_SIZE 64
#define MAX_BUFFER_SIZE 128000000L

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

struct file_reader
{
    char file_path[MAX_FILE_LEN];

    size_t file_size;
    size_t file_offset;

    size_t buffer_size;

    char* buffer;
};

struct file_reader* file_reader_init (const char* file_path, size_t buffer_size);

void file_reader_free (struct file_reader* parser);
void file_reader_tick (struct file_reader* parser, size_t offset);
