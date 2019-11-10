///
/// This file is part of glraster.
///
/// \file parser.c
/// \brief File parser for sample captures
///

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "file_reader.h"

static int
file_parser_size (struct file_parser* parser)
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

    parser->file_size = file_size;
}

static int
file_parser_read (struct file_parser* parser)
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
}

struct file_parser*
file_parser_init (const char* file_path, size_t buffer_size)
{
    struct file_parser *parser = malloc(sizeof(struct file_parser));

    strncpy(parser->file_path, file_path, MAX_FILE_LEN);

    if (file_parser_size(parser) < 0)
    {
        free(parser);
        return NULL;
    }

    parser->file_offset = 0;

    parser->buffer_size = MIN(buffer_size, MAX_BUFFER_SIZE);
    parser->buffer_size = MIN(parser->buffer_size, parser->file_size);
    parser->buffer = malloc(buffer_size);

    return parser;
}

void
file_parser_free (struct file_parser* parser)
{
    free(parser->buffer);
    free(parser);
}

void
file_parser_tick (struct file_parser* parser)
{
    if ((parser->file_offset + parser->buffer_size) > parser->file_size)
    {
        parser->file_offset = parser->file_size - parser->buffer_size - 1;
    }

    file_parser_read(parser);
}
