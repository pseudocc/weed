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

#define array_len(__value) \
	(sizeof(__value) / sizeof(__value[0]))

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
		.len = __builtins_weeds_len( \
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
	__builtins_weeds_len(sizeof(__T), __ptr, __sentinel)

#define weeds_at(__weeds, __index) \
	__index < __weeds.len \
		? &__weeds.ptr[__index] \
		: (typeof(__weeds.ptr))0

#define let __auto_type

#define unconst(__t) \
	(typeof(({let __s = *(__t); &__s;})))(__t)

#define as(__T, ...) \
	__as_impl(__T, __VA_ARGS__, 1, 0)

#define __as_impl(__T, __value, __sentinel, __has_sentinel, ...) \
	_Generic(*(__T*)0, \
		opaque: (opaque){ \
			.ptr = &(__value), \
			.len = sizeof(__value) - is_string(__value) \
		}, \
		opaque_mut: (opaque_mut){ \
			.ptr = &(__value), \
			.len = sizeof(__value) - is_string(__value) \
		}, \
		default: (__T){ \
			.ptr = (void*)(usize)__value, \
			.len = static_if(__has_sentinel, \
				weeds_len(weeds_type(__T), __value, arrayify(__sentinel)), \
				array_len(__value) - is_string(__value) \
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
	__builtins_weeds_eql( \
		(usize)__a.ptr, __a.len * sizeof(*__a.ptr), \
		(usize)__b.ptr, __b.len * sizeof(*__b.ptr) \
	)

bool __builtins_weeds_eql(usize aptr, usize alen, usize bptr, usize blen);

usize __builtins_weeds_len(usize elem_size, const void* ptr, const void* sentinel);

typedef WEEDS_MUT(void) opaque_mut;
typedef WEEDS(void) opaque;

usize __builtins_memcpy(opaque_mut dest, opaque src);
#define memcpy(__dest, __src) \
	__builtins_memcpy(to(opaque_mut, __dest), to(opaque, __src))

typedef WEEDS_MUT(char) string_mut;
typedef WEEDS(char) string;
typedef const char* string0;

#define primitive_eql_fn(_T) \
	(_T a, _T b) { \
		return a == b; \
	}
#define typedef_eql_fn(_T, _tyname) \
	typedef bool (*_tyname)(_T, _T)

static inline bool bool_eql primitive_eql_fn(bool);
static inline bool usize_eql primitive_eql_fn(usize);
static inline bool u8_eql primitive_eql_fn(u8);
static inline bool u16_eql primitive_eql_fn(u16);
static inline bool u32_eql primitive_eql_fn(u32);
static inline bool u64_eql primitive_eql_fn(u64);
static inline bool isize_eql primitive_eql_fn(isize);
static inline bool i8_eql primitive_eql_fn(i8);
static inline bool i16_eql primitive_eql_fn(i16);
static inline bool i32_eql primitive_eql_fn(i32);
static inline bool i64_eql primitive_eql_fn(i64);
static inline bool f32_eql primitive_eql_fn(f32);
static inline bool f64_eql primitive_eql_fn(f64);
static inline bool char_eql primitive_eql_fn(char);

#endif // WEED_BUILTINS_H
