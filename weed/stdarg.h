/**
 * vim: ts=4:noet
**/

#ifndef WEED_STDARG_H
#define WEED_STDARG_H

typedef __builtin_va_list va_list;
#define va_start __builtin_va_start
#define va_end __builtin_va_end
#define va_arg __builtin_va_arg

#ifdef __x86_64__
#define va_ptr(__ap, __type) \
	({ \
		__type* __ptr = __ap->reg_save_area + __ap->gp_offset; \
		__builtin_va_arg(__ap, __type); \
		__ptr; \
	})
#endif

#endif // WEED_STDARG_H
