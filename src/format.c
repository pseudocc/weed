/**
 * vim: ts=4:noet
**/

#include <weed/format.h>
#include <weed/stdarg.h>

char digits2[100][2] = {
	"00", "01", "02", "03", "04", "05", "06", "07", "08",
	"09", "10", "11", "12", "13", "14", "15", "16", "17",
	"18", "19", "20", "21", "22", "23", "24", "25", "26",
	"27", "28", "29", "30", "31", "32", "33", "34", "35",
	"36", "37", "38", "39", "40", "41", "42", "43", "44",
	"45", "46", "47", "48", "49", "50", "51", "52", "53",
	"54", "55", "56", "57", "58", "59", "60", "61", "62",
	"63", "64", "65", "66", "67", "68", "69", "70", "71",
	"72", "73", "74", "75", "76", "77", "78", "79", "80",
	"81", "82", "83", "84", "85", "86", "87", "88", "89",
	"90", "91", "92", "93", "94", "95", "96", "97", "98",
	"99"
};

#define FORMAT_INTEGER(type) \
	usize format_##type(writer_t writer, type value, format_integer_t options) { \
		if (options.base < 2 || options.base > 36) \
			return (usize)-1; \
		let bits = bitsizeof(type); \
		type rem; \
		bool loop; \
		let abs_value = static_if(is_signed(type), \
			value < 0 ? -value : value, \
			value \
		); \
		var v = abs_value; \
		char buffer[bits]; \
		register char* c = &buffer[bits]; \
		char* d; \
		goto_if(options.base != 10, base_other); \
	base_10: \
		if ((loop = v >= 100)) { \
			rem = v % 100; \
			v /= 100; \
			goto base_10_write2; \
		} else if (v < 10) { \
			d = digits2[v] + 1; \
			goto base_10_write1; \
		} \
	base_10_write2: \
		d = digits2[rem]; \
		*(--c) = d[1]; \
	base_10_write1: \
		*(--c) = d[0]; \
		goto_if(loop, base_10); \
		goto sign; \
	base_other: \
		rem = v % options.base; \
		v /= options.base; \
		*(--c) = rem < 10 \
			? '0' + rem \
			: (options.uppercase ? 'A' : 'a') + rem - 10; \
		goto_if(v, base_other); \
	sign: \
		if (is_signed(type) && value < 0) \
			*(--c) = '-'; \
		return write_all(writer, (opaque){ \
			.ptr = c, \
			.len = &buffer[bits] - c, \
		}); \
	}

FORMAT_INTEGER(u64)
FORMAT_INTEGER(u32)
FORMAT_INTEGER(u16)
FORMAT_INTEGER(u8)

FORMAT_INTEGER(i64)
FORMAT_INTEGER(i32)
FORMAT_INTEGER(i16)
FORMAT_INTEGER(i8)

usize format_bool(writer_t writer, bool value) {
	string text = value ? string_true : string_false;
	return write_all(writer, to(opaque, text));
}

const string string_true = as(string, "true");
const string string_false = as(string, "false");

#ifdef WEED_DEV

#include <weed/stdio.h>
#include <weed/linux/syscall.h>

#define TEST_INT(__T, __value, __base) \
	do {\
		write_all(stdout, as(opaque, #__T" " #__value"(" #__base"): ")); \
		format_##__T(stdout, __value, (format_integer_t){ __base, false }); \
		write(char, stdout, '\n'); \
	} while (0)

#define TEST_BOOL(__value) \
	do {\
		write_all(stdout, as(opaque, "bool(" #__value "): ")); \
		format_bool(stdout, __value); \
		write(char, stdout, '\n'); \
	} while (0)

void weed() {
	TEST_INT(u8, 0x7f, 10);
	TEST_INT(i32, -123456789, 10);
	TEST_INT(u32, 0xdeadbeef, 16);

	TEST_BOOL(true);
	TEST_BOOL(false);
	syscall_exit(0);
}

#endif // WEED_DEV
