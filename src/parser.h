///
/// This file is part of glraster.
///
/// \file parser.h
/// \brief File parser for sample captures
///

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define MAX_FILE_LEN 1024
#define MAX_SYMB_LEN 32

struct file_parser
{
    char file_path[MAX_FILE_LEN];

    FILE* fp;

    size_t block_size;

    size_t file_size;
    size_t symb_size;

    size_t offset;
};

struct file_parser* file_parser_init (const char* file_path, size_t block_size);

void file_parser_free (struct file_parser* parser);

void file_parser_tick (struct file_parser* parser);

int file_parser_load (struct file_parser* parser, const char* file_path);
