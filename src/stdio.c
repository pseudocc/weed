/**
 * vim: ts=4:noet
**/

#include <weed/stdio.h>

#ifdef __linux__
#include <weed/linux/syscall.h>

file_t file_open(string path, file_open_t options) {
	char pathbuf[PATH_MAX];
	weeds(char) path0 = weeds_stack(char, PATH_MAX);
	memcpy(path0, path);
	return (file_t)syscall_open(path0.ptr, options.flags, options.mode);
}

usize file_close(file_t file) {
	return syscall_close((usize)file);
}

static usize __file_write(void* ctx, opaque data) {
	const file_t file = *(file_t*)ctx;
	return syscall_write((usize)file, data);
}

static usize __file_read(void* ctx, opaque_mut buffer) {
	const file_t file = *(file_t*)ctx;
	return syscall_read((usize)file, buffer);
}

const file_t file_stdin = 0;
const file_t file_stdout = 1;
const file_t file_stderr = 2;

const reader_t stdin = {
	.ctx = (void*)&file_stdin,
	.fn = __file_read
};

const writer_t stdout = {
	.ctx = (void*)&file_stdout,
	.fn = __file_write,
};

const writer_t stderr = {
	.ctx = (void*)&file_stderr,
	.fn = __file_write,
};

#endif

usize write_all(writer_t writer, opaque data) {
	return writer.fn(writer.ctx, data);
}

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

void weed() {
	write_all(stdout, as(opaque, "hi!\n"));
	write(char, stdout, '\n');
	syscall_exit(0);
}

#endif
