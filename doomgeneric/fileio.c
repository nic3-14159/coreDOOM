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

#include "fileio.h"
#include <cbfs.h>

struct _FILE {
    unsigned char *buf;
    long int pos;
    size_t buf_len;
};

typedef unsigned int mode_t;

// Ignore mode for now, assuming behavior for binary mode
FILE* fopen(const char* filename, const char* mode) {
    FILE* file = malloc(sizeof(FILE));

    file->buf = cbfs_map(filename, &file->buf_len);
    if (file->buf == NULL) {
	printf("Error!\n");
	free(file);
        return NULL;
    }
    file->pos = 0;
    return file;
}

// Just return success, no need to flush with a memory buffer
int fflush(FILE* stream) {
    return 0;
}

int fclose(FILE* stream) {
    cbfs_unmap(stream->buf);
    free(stream);
    return 0;
}

// Not implemented, just indicate a failure
int remove(const char* filename) {
    return -1;
}

// Not implemented, just indicate a failure
int rename(const char* oldname, const char* newname) {
    return -1;
}

// Not implemented
size_t fwrite(const void* ptr, size_t size, size_t nobj, FILE* stream) {
    return 0;
}

// Copied from memio.c
size_t fread(void* ptr, size_t size, size_t nobj, FILE* stream) {
    size_t items;

    // Trying to read more bytes than we have left?

    items = nobj;

    if (items * size > stream->buf_len - stream->pos)
    {
            items = (stream->buf_len - stream->pos) / size;
    }

    // Copy bytes to buffer

    memcpy(ptr, stream->buf + stream->pos, items * size);

    // Update position

    stream->pos += items * size;

    return items;

}

int fseek(FILE* stream, long offset, int origin) {
    long newpos;
    switch (origin) {
    case SEEK_SET:
        newpos = offset;
        break;
    case SEEK_CUR:
        newpos = stream->pos + offset;
        break;
    case SEEK_END:
        newpos = stream->buf_len + offset;
        break;
    default:
        return -1;
    }
    if (newpos <= stream->buf_len) {
        stream->pos = newpos;
        return 0;
    }
    return -1;
}

long int ftell(FILE* stream) {
    return stream->pos;
}

int fputs(const char *str, FILE *stream) {
    return -1;
}
