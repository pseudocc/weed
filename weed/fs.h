/**
 * vim: ts=4:noet
**/

#ifndef WEED_FILE_SYSTEM_H
#define WEED_FILE_SYSTEM_H

#include <weed/stream.h>

#ifdef __linux__
typedef i32 fd_t;
typedef fd_t file_t;
typedef fd_t dir_t;

#define PATH_MAX 4096
#endif

typedef struct {
	usize flags;
	usize mode;
} file_open_t;

file_t file_open(string path, file_open_t options);
usize file_close(file_t file);

writer_t file_writer(const file_t* file);
reader_t file_reader(const file_t* file);

usize __file_write(void* ctx, opaque data);
usize __file_read(void* ctx, opaque_mut buffer);

#endif // WEED_FILE_SYSTEM_H
