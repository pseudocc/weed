/**
 * vim: ts=4:noet
**/

#ifndef WEED_STDIO_H
#define WEED_STDIO_H

#include <weed/fs.h>

#ifdef __linux__

const file_t file_stdin = 0;
const file_t file_stdout = 1;
const file_t file_stderr = 2;

const reader_t stdin = {
	.ctx = (void*)(usize)&file_stdin,
	.fn = __file_read
};

const writer_t stdout = {
	.ctx = (void*)(usize)&file_stdout,
	.fn = __file_write,
};

const writer_t stderr = {
	.ctx = (void*)(usize)&file_stderr,
	.fn = __file_write,
};

#endif // __linux__

#endif // WEED_STDIO_H
