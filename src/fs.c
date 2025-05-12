/**
 * vim: ts=4:noet
**/

#include <weed/fs.h>

#ifdef __linux__
#include <weed/linux/syscall.h>

file_t file_open(string path, file_open_t options) {
	char pathbuf[PATH_MAX];
	weeds(char) path0 = weeds_stack(char, PATH_MAX);
	memcpy(path0, path);
	return (i32)syscall_open(path0.ptr, options.flags, options.mode);
}

usize file_close(file_t file) {
	return syscall_close((usize)file);
}

usize __file_write(void* ctx, opaque data) {
	const file_t file = *(file_t*)ctx;
	return syscall_write((usize)file, data);
}

usize __file_read(void* ctx, opaque_mut buffer) {
	const file_t file = *(file_t*)ctx;
	return syscall_read((usize)file, buffer);
}

#endif

writer_t file_writer(const file_t* file) {
	return (writer_t){
		.ctx = unconst(file),
		.fn = __file_write,
	};
}

reader_t file_reader(const file_t* file) {
	return (reader_t){
		.ctx = unconst(file),
		.fn = __file_read,
	};
}

#ifdef WEED_DEV

#include <weed/stdio.h>

void weed() {
	write_all(stdout, opaque_string("bye!"));
	write(char, stdout, '\n');
	syscall_exit(0);
}

#endif
