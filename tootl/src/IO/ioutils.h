#ifndef IO_UTILS_H
#define IO_UTILS_H
#include <HGE/HGE_FileUtility.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t *struct_pointer;

void binwrite(const char* file_name, struct_pointer data, size_t size);
void binread(const char* file_name, struct_pointer data, size_t size);

#endif
