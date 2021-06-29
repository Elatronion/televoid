#include "ioutils.h"

void binwrite(const char* file_name, struct_pointer data, size_t size) {
	FILE* fp = fopen(file_name, "wb");

	if (fp == NULL)  /* If an error occurs during the file creation */
	{
		fprintf(stderr, "fopen() failed for '%s'\n", file_name);
	}
	else
	{
		size_t element_size = size;
		size_t elements_to_write = 1;

		size_t elements_written = fwrite(data, element_size, elements_to_write, fp);
		if (elements_written != elements_to_write)
		{
			fprintf(stderr, "fwrite() failed: wrote only %zu out of %zu elements.\n",
				elements_written, elements_to_write);
		}

		fclose(fp);
	}
}

void binread(const char* file_name, struct_pointer data, size_t size) {
	FILE* fp = fopen(file_name, "rb");

	if (fp == NULL)
	{
		fprintf(stderr, "fopen() failed for '%s'\n", file_name);
	}
	else
	{
		fread(data, size, 1, fp);
		fclose(fp);
	}
}
