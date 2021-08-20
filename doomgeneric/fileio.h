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
