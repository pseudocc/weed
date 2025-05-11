/**
 * vim: ts=4:noet
**/

#ifndef WEED_LINUX_SYSCALL_H
#define WEED_LINUX_SYSCALL_H

#ifndef __linux__
#pragma error "only for linux"
#endif

#include <weed/builtins.h>
#include <sys/syscall.h>

usize syscall_read(usize fd, opaque_mut slice);
usize syscall_write(usize fd, opaque slice);
usize syscall_open(string0 path, usize flags, usize mode);
usize syscall_close(usize fd);
void syscall_exit(i32 status);

#endif // WEED_LINUX_SYSCALL_H
