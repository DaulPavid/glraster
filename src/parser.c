///
/// This file is part of glraster.
///
/// \file parser.c
/// \brief File parser for sample captures
///

#include <stdlib.h>
#include <string.h>

#include "parser.h"

struct file_parser*
file_parser_init (const char* file_path, size_t block_size)
{
    struct file_parser *parser = malloc(sizeof(struct file_parser));

    strncpy(parser->file_path, file_path, MAX_FILE_LEN);

    parser->fp = fopen(file_path, "rb");

    if (parser->fp == NULL)
    {
        fprintf(stderr, "[FAIL] - Unable to read the file: %s\n",
                parser->file_path);

        free(parser);
        return NULL;
    }

    fseek(parser->fp, 0, SEEK_END);

    long int file_size = ftell(parser->fp);

    fclose(parser->fp);

    if (file_size < 0)
    {
        fprintf(stderr, "[FAIL] - Unable to determine file size\n");

        free(parser);
        return NULL;
    }

    parser->block_size = block_size;

    parser->file_size = file_size;
    parser->symb_size = MAX_SYMB_LEN;

    parser->offset = 0;

    return parser;
}

void
file_parser_free (struct file_parser* parser)
{
    free(parser);
}

void
file_parser_tick (struct file_parser* parser)
{
}

int
file_parser_load (struct file_parser* parser, const char* file_path)
{
}
