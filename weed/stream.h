/**
 * vim: ts=4:noet
**/

#ifndef WEED_STREAM_H
#define WEED_STREAM_H

#include <weed/builtins.h>

typedef usize (*write_fn)(void* ctx, opaque data);

typedef struct {
	void* ctx;
	write_fn fn;
} writer_t;

typedef usize (*read_fn)(void* ctx, opaque_mut buffer);

typedef struct {
	void* ctx;
	read_fn fn;
} reader_t;

// TODO: error union type for error handling

static inline usize write_all(writer_t writer, opaque data) {
	return writer.fn(writer.ctx, data);
}

#define write(__T, __writer, __value) \
	(usize)({ \
		__T value = __value; \
		opaque payload = as(opaque, arrayify(value)); \
		write_all(__writer, payload); \
	})

static inline usize read_all(reader_t reader, opaque_mut buffer) {
	return reader.fn(reader.ctx, buffer);
}

#define read(__T, __reader) \
	(__T)({ \
		__T value = {0}; \
		opaque_mut payload = as(opaque_mut, arrayify(value)); \
		read_all(__reader, payload); \
		value; \
	})

#endif // WEED_STREAM_H
