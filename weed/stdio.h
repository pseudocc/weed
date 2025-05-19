/**
 * vim: ts=4:noet
**/

#ifndef WEED_STDIO_H
#define WEED_STDIO_H

#include <weed/fs.h>

#ifdef __linux__

extern const file_t file_stdin;
extern const file_t file_stdout;
extern const file_t file_stderr;

extern const reader_t stdin;
extern const writer_t stdout;
extern const writer_t stderr;

#endif // __linux__

#endif // WEED_STDIO_H
