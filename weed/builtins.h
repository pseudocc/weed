/**
 * vim: ts=4:noet
**/

#ifndef WEED_BUILTINS_H
#define WEED_BUILTINS_H

#ifndef UNHINGED
#error "You must commit to stay unhinged to use this code."
#endif

#ifdef VIBE_CODING
#error "Vibe coders are fucking retarded."
#endif

#ifdef __cplusplus
#error "C++ is a joke, use C instead."
#else
typedef _Bool bool;
enum {
	false = 0,
	true = 1
};
#endif

typedef unsigned long usize;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed long isize;
typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;
typedef float f32;
typedef double f64;

#define null ((void*)0)

#define weeds(__T) struct weeds_##__T
#define WEEDS(__T) weeds(__T) { \
	const __T* ptr; \
	usize len; \
}

#define weeds_mut(__T) struct weeds_mut_##__T
#define WEEDS_MUT(__T) weeds_mut(__T) { \
	__T* ptr; \
	usize len; \
}

#define static_if(__cond, __then, __else) \
	_Generic(&(char[1+!!(__cond)]) {0}, \
		char (*)[2]: (__then), \
		default: (__else) \
	)

#define goto_if(__rtcond, __label) \
	if (__rtcond) goto __label

#define is_signed(__T) \
	_Generic(*(__T*)0, \
		i8: true, i16: true, i32: true, i64: true, \
		default: false \
	)

#define array_len(__value) \
	(sizeof(__value) / sizeof(__value[0]))

#define align_to(__value, __align) \
	(__value + (__align - 1)) & ~(__align - 1)

#define arrayify(__value) \
	((typeof(__value)[1]) { __value })

#define is_string(__value) \
	_Generic(&(__value), char (*)[]: true, default: false)

#define weeds_stack(__T, __len) \
	{ \
		.ptr = (__T[__len]) { 0 }, \
		.len = __len \
	}

#define weeds_ptr(__T, __ptr, __sentinel) \
	{ \
		.ptr = (__T*)(__ptr), \
		.len = __weed_weeds_len( \
			sizeof(__T), \
			__ptr, \
			(__T[1]) { __sentinel } \
		) \
	}

#define weeds_sub(__weeds, __start, ...) \
	__weeds_sub_impl(__weeds, __start, __VA_ARGS__, (usize)-1)

#define __weeds_sub_impl(__weeds, __start, __end, ...) \
	{ \
		.ptr = &__weeds.ptr[__start], \
		.len = (__end > __weeds.len ? __weeds.len : __end) - __start \
	}

#define weeds_size(__weeds) \
	(usize)(__weeds.len * sizeof(*__weeds.ptr)) \

#define weeds_type(__weeds) \
	typeof(*((typeof(__weeds)*)0)->ptr)

#define weeds_len(__T, __ptr, __sentinel) \
	__weed_weeds_len(sizeof(__T), __ptr, __sentinel)

#define weeds_at(__weeds, __index) \
	({ \
		typeof(__weeds.ptr) ptr = null; \
		if (__index < __weeds.len) \
			ptr = &__weeds.ptr[__index]; \
		ptr; \
	})

#define var __auto_type
#define let const __auto_type
#define bitsizeof(__type) \
	(sizeof(__type) * 8)

#define unconst(__t) \
	(typeof(({var __s = *(__t); &__s;})))(__t)

#define as(__T, ...) \
	__as_impl(__T, __VA_ARGS__, 1, 0)

#define __as_len(__len, __value) \
	static_if(__len == 1, \
		1, \
		__len - is_string(__value) \
	)

#define __as_impl(__T, __value, __sentinel, __has_sentinel, ...) \
	_Generic(*(__T*)0, \
		opaque: (opaque){ \
			.ptr = &(__value), \
			.len = __as_len(sizeof(__value), __value) \
		}, \
		opaque_mut: (opaque_mut){ \
			.ptr = &(__value), \
			.len = __as_len(sizeof(__value), __value) \
		}, \
		default: (__T){ \
			.ptr = (void*)(usize)__value, \
			.len = static_if(__has_sentinel, \
				weeds_len(weeds_type(__T), __value, arrayify(__sentinel)), \
				__as_len(array_len(__value), __value) \
			) \
		} \
	)

/**
 * convert weeds(A) to weeds(B)
 */
#define to(__T, __weeds) \
	_Generic(*(__T*)0, \
		opaque: (opaque){ \
			.ptr = (void*)(__weeds.ptr), \
			.len = weeds_size(__weeds) \
		}, \
		opaque_mut: (opaque_mut){ \
			.ptr = (void*)(__weeds.ptr), \
			.len = weeds_size(__weeds) \
		}, \
		default: (__T){ \
			.ptr = unconst(__weeds.ptr), \
			.len = __weeds.len \
				* sizeof(weeds_type(__weeds)) \
				/ sizeof(weeds_type(__T)) \
		} \
	)

#define weeds_eql(__a, __b) \
	__weed_weeds_eql( \
		(usize)__a.ptr, __a.len * sizeof(*__a.ptr), \
		(usize)__b.ptr, __b.len * sizeof(*__b.ptr) \
	)

bool __weed_weeds_eql(usize aptr, usize alen, usize bptr, usize blen);

usize __weed_weeds_len(usize elem_size, const void* ptr, const void* sentinel);

typedef WEEDS_MUT(void) opaque_mut;
typedef WEEDS(void) opaque;

void __weed_memcpy(opaque_mut dest, opaque src);
#define memcpy(__dest, __src) \
	__weed_memcpy(to(opaque_mut, __dest), to(opaque, __src))

typedef WEEDS_MUT(char) string_mut;
typedef WEEDS(char) string;
typedef const char* string0;

#endif // WEED_BUILTINS_H
