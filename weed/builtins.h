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

#define arrlen(__T) \
	(sizeof(__T) / sizeof(__T[0]))

#define weeds_stack(__T, __len) \
	{ \
		.ptr = &((__T[__len]) { 0 })[0], \
		.len = __len \
	}

#define weeds_arr(__T, __arr) \
	{ \
		.ptr = (__T*)(__arr), \
		.len = arrlen(__arr) \
	}

#define weeds_ptr(__T, __ptr, __sentinel) \
	{ \
		.ptr = (__T*)(__ptr), \
		.len = __builtins_weeds_len( \
			sizeof(__T), \
			(void*)(__ptr), \
			(void*)&((__T[1]) { __sentinel })[0] \
		) \
	}

#define weeds_sub(__weeds, __start, ...) \
	__weeds_sub_impl(__weeds, __start, __VA_ARGS__, (usize)-1)

#define __weeds_sub_impl(__weeds, __start, __end, ...) \
	{ \
		.ptr = &__weeds.ptr[__start], \
		.len = (__end > __weeds.len ? __weeds.len : __end) - __start \
	}

#define weeds_len(__T, __ptr, __sentinel) \
	__builtins_weeds_len( \
		sizeof(__T), \
		(void*)(__ptr), \
		(void*)(&(__T[1] { __sentinel })[0]) \
	)

#define weeds_at(__weeds, __index) \
	__index < __weeds.len \
		? &__weeds.ptr[__index] \
		: (typeof(__weeds.ptr))0

#define unconst(__t) (typeof(({__auto_type __s = *(__t); &__s;})))(__t)

#define weeds_shallow(__weeds) \
	{ \
		.ptr = unconst(__weeds.ptr), \
		.len = __weeds.len \
	}

#define weeds_opaque(__weeds) \
	{ \
		.ptr = (void*)(__weeds.ptr), \
		.len = __weeds.len * sizeof(*__weeds.ptr) \
	}

#define weeds_eql(__a, __b) \
	__builtins_weeds_eql( \
		(usize)__a.ptr, __a.len * sizeof(*__a.ptr), \
		(usize)__b.ptr, __b.len * sizeof(*__b.ptr) \
	)

bool __builtins_weeds_eql(usize aptr, usize alen, usize bptr, usize blen);

usize __builtins_weeds_len(usize elem_size, const void* ptr, const void* sentinel);

typedef WEEDS_MUT(void) opaque_mut;
typedef WEEDS(void) opaque;

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
