/**
 * vim: ts=4:noet
**/

#ifndef WEED_FORMAT_H
#define WEED_FORMAT_H

#include <weed/stream.h>

typedef void (*format_fn)(const void* ctx, string fmt);

typedef struct {
	const void* ctx;
	format_fn fn;
} format_t;

WEEDS(format_t);

usize format(writer_t writer, string0 fmt, ...);

typedef struct {
	u8 base;
	bool uppercase;
} format_integer_t;

usize format_u8(writer_t writer, u8 value, format_integer_t options);
usize format_u16(writer_t writer, u16 value, format_integer_t options);
usize format_u32(writer_t writer, u32 value, format_integer_t options);
usize format_u64(writer_t writer, u64 value, format_integer_t options);
usize format_i8(writer_t writer, i8 value, format_integer_t options);
usize format_i16(writer_t writer, i16 value, format_integer_t options);
usize format_i32(writer_t writer, i32 value, format_integer_t options);
usize format_i64(writer_t writer, i64 value, format_integer_t options);

typedef enum {
	decimal,
	scientific,
} format_float_mode_t;

typedef struct {
	u8 precision;
	format_float_mode_t mode;
	// scientific only flags
	bool uppercase;
	bool show_plus;
} format_float_t;

usize format_f32(writer_t writer, f32 value, format_float_t options);
usize format_f64(writer_t writer, f64 value, format_float_t options);

extern const string string_true;
extern const string string_false;
usize format_bool(writer_t writer, bool value);

#endif // WEED_FORMAT_H
