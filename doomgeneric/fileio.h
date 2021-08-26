/*
 * Copyright(C) 2021 Nicholas Chin
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Emulates the IO functions in stdio.h using memory buffers,
 * loading files off CBFS
 */

#ifndef FILEIO_H
#define FILEIO_H
#include <stddef.h>

typedef struct _FILE FILE;

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

FILE* fopen(const char* filename, const char* mode);

int fflush(FILE* stream);

int fclose(FILE* stream);

int remove(const char* filename);

int rename(const char* oldname, const char* newname);

size_t fwrite(const void* ptr, size_t size, size_t nobj, FILE* stream);

size_t fread(void* ptr, size_t size, size_t nobj, FILE* stream);

int fseek(FILE* stream, long offset, int origin);

long ftell(FILE* stream);
#endif
