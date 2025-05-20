/**
 * vim: ts=4:noet
**/

#include <weed/builtins.h>

static bool __weed_weeds_eql_checked(usize len, const u8* aptr, const u8* bptr) {
	bool eql = true;
	usize i = 0;
	register const u8* a = aptr;
	register const u8* b = bptr;
loop:
	if (i++ >= len)
		goto end;
	if ((eql = *a++ == *b++))
		goto loop;
	goto end;
end:
	return eql;
}

bool __weed_weeds_eql(usize aptr, usize alen, usize bptr, usize blen) {
	return alen == blen && __weed_weeds_eql_checked(alen, (const u8*)aptr, (const u8*)bptr);
}

usize __weed_weeds_len(usize elem_size, const void* ptr, const void* sentinel) {
	usize i = 0;
loop:
	if (__weed_weeds_eql_checked(elem_size, ptr, sentinel))
		goto end;
	i++;
	ptr += elem_size;
	goto loop;
end:
	return i;
}

void __weed_memcpy(opaque_mut dest, opaque src) {
	usize i = 0;
	register u8* restrict d = dest.ptr;
	register const u8* restrict s = src.ptr;
loop:
	if (i >= src.len)
		goto end;
	i++;
	*d++ = *s++;
	goto loop;
end:
	return;
}
