/**
 * vim: ts=4:noet
**/

#include <weed/linux/syscall.h>

static inline usize syscall0(usize nr) {
	usize rt;
	asm volatile ("syscall"
		: "=rax" (rt)
		: "rax" (nr)
		: "rcx", "r11", "memory"
	);
	return rt;
}

static inline usize syscall1(usize nr, usize a0) {
	usize rt;
	asm volatile ("syscall"
		: "=rax" (rt)
		: "rax" (nr),
		  "rdi" (a0)
		: "rcx", "r11", "memory"
	);
	return rt;
}

static inline usize syscall2(usize nr, usize a0, usize a1) {
	usize rt;
	asm volatile ("syscall"
		: "=rax" (rt)
		: "rax" (nr),
		  "rdi" (a0),
		  "rsi" (a1)
		: "rcx", "r11", "memory"
	);
	return rt;
}

static inline usize syscall3(usize nr, usize a0, usize a1, usize a2) {
	usize rt;
	asm volatile ("syscall"
		: "=rax" (rt)
		: "rax" (nr),
		  "rdi" (a0),
		  "rsi" (a1),
		  "rdx" (a2)
		: "rcx", "r11", "memory"
	);
	return rt;
}

static inline usize syscall4(usize nr, usize a0, usize a1, usize a2, usize a3) {
	usize rt;
	register long r10 asm("r10") = a3;
	asm volatile ("syscall"
		: "=rax" (rt)
		: "rax" (nr),
		  "rdi" (a0),
		  "rsi" (a1),
		  "rdx" (a2),
		  "r" (r10)
		: "rcx", "r11", "memory"
	);
	return rt;
}

static inline usize syscall5(usize nr, usize a0, usize a1, usize a2, usize a3, usize a4) {
	usize rt;
	register long r10 asm("r10") = a3;
	register long r8 asm("r8") = a4;
	asm volatile ("syscall"
		: "=rax" (rt)
		: "rax" (nr),
		  "rdi" (a0),
		  "rsi" (a1),
		  "rdx" (a2),
		  "r" (r10),
		  "r" (r8)
		: "rcx", "r11", "memory"
	);
	return rt;
}

static inline usize syscall6(usize nr, usize a0, usize a1, usize a2, usize a3, usize a4, usize a5) {
	usize rt;
	register long r10 asm("r10") = a3;
	register long r8 asm("r8") = a4;
	register long r9 asm("r9") = a5;
	asm volatile ("syscall"
		: "=rax" (rt)
		: "rax" (nr),
		  "rdi" (a0),
		  "rsi" (a1),
		  "rdx" (a2),
		  "r" (r10),
		  "r" (r8),
		  "r" (r9)
		: "rcx", "r11", "memory"
	);
	return rt;
}

usize syscall_read(usize fd, opaque_mut slice) {
	return syscall3(SYS_read, fd, (usize)slice.ptr, slice.len);
}

usize syscall_write(usize fd, opaque slice) {
	return syscall3(SYS_write, fd, (usize)slice.ptr, slice.len);
}

usize syscall_open(string0 path, usize flags, usize mode) {
	return syscall3(SYS_open, (usize)path, flags, mode);
}

usize syscall_close(usize fd) {
	return syscall1(SYS_close, fd);
}

void syscall_exit(i32 status) {
	syscall1(SYS_exit, status);
}

#ifdef WEED_DEV

#define opaque_string(__str) \
	(opaque)weeds_ptr(char, __str, 0)

void weed() {
	i32 status = 0;
	usize fd;

	syscall_write(1, opaque_string("hello weed\n"));
	fd = syscall_open("/dev/zero", 0, 0);

	if (fd == (usize)-1) {
		syscall_write(2, opaque_string("error opening /dev/zero\n"));
		status = 1;
		goto early_exit;
	}

	weeds(char) stream = weeds_stack(char, 128);
	syscall_write(1, (opaque)weeds_ptr(char, "opened /dev/zero\n", 0));

	if (syscall_read(fd, (opaque_mut)weeds_shallow(stream)) == (usize)-1) {
		syscall_write(2, (opaque)weeds_ptr(char, "error reading /dev/zero\n", 0));
		syscall_exit(1);
	}

	syscall_write(1, (opaque)weeds_ptr(char, "read /dev/zero\n", 0));
	for (usize i = 0; i < stream.len; i++) {
		if (stream.ptr[i] != 0) {
			syscall_write(2, (opaque)weeds_ptr(char, "error\n", 0));
			status = 1;
			goto exit;
		}
	}
	syscall_write(1, (opaque)weeds_ptr(char, "read /dev/zero ok, everything is zero\n", 0));

exit:
	if (syscall_close(fd) == (usize)-1) {
		syscall_write(2, (opaque)weeds_ptr(char, "error closing /dev/zero\n", 0));
		status = 1;
	} else {
		syscall_write(1, (opaque)weeds_ptr(char, "closed /dev/zero\n", 0));
	}
early_exit:
	syscall_exit(status);
}

#endif
