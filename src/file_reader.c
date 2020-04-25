/*!
 *  This file is part of glraster.
 *
 *   \file parser.c
 *   \brief File parser for sample captures
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "file_reader.h"

static int
file_reader_size (struct file_reader* parser)
{
    FILE *fp = fopen(parser->file_path, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "[FAIL] - Unable to read the file: %s\n",
                parser->file_path);

        return -1;
    }

    fseek(fp, 0, SEEK_END);

    long int file_size = ftell(fp);

    fclose(fp);

    if (file_size < 0)
    {
        fprintf(stderr, "[FAIL] - Unable to determine file size\n");
        return -1;
    }

    parser->file_size = file_size - 1;
    return file_size;
}

static int
file_reader_read (struct file_reader* parser)
{
    FILE *fp = fopen(parser->file_path, "rb");

    if (fp == NULL)
    {
        fprintf(stderr, "[FAIL] - Unable to read the file: %s\n",
                parser->file_path);

        return -1;
    }

    fseek(fp, parser->file_offset, SEEK_SET);
    fread(parser->buffer, 1, parser->buffer_size, fp);

    fclose(fp);

    return 0;
}

struct file_reader*
file_reader_init (const char* file_path, size_t buffer_size)
{
    struct file_reader *parser = malloc(sizeof(struct file_reader));

    strncpy(parser->file_path, file_path, MAX_FILE_LEN);

    if (file_reader_size(parser) < 0)
    {
        free(parser);
        return NULL;
    }

    parser->file_offset = 0;

    parser->buffer_size = MIN(buffer_size, MAX_BUFFER_SIZE);
    parser->buffer_size = MIN(parser->buffer_size, parser->file_size);
    parser->buffer = malloc(buffer_size);

    file_reader_read(parser);

    return parser;
}

void
file_reader_free (struct file_reader* parser)
{
    free(parser->buffer);
    free(parser);
}

void
file_reader_tick (struct file_reader* parser, size_t offset)
{
    offset += parser->buffer_size;
    if (offset >= parser->file_size)
    {
        offset = parser->file_size - parser->buffer_size;
    }

    if (parser->file_offset != offset)
    {
        parser->file_offset = offset;
        file_reader_read(parser);
    }
}
