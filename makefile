CC = gcc
CFLAGS = -nostdlib \
		 -DUNHINGED \
		 -std=gnu11 \
		 -fno-stack-protector

CFLAGS_TEST = $(CFLAGS) \
			  -DWEED_DEV \
			  -Wl,-eweed

SRC = $(wildcard src/*.c) \
	  $(wildcard src/linux/*.c)

OBJ = $(SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test/linux_x86_64: src/linux/x86_64.c src/builtins.o
	$(CC) $(CFLAGS_TEST) $^ -o $@

test/stdio: src/stdio.c src/linux/x86_64.o src/builtins.o
	$(CC) $(CFLAGS_TEST) $^ -o $@

clean:
	rm -f $(OBJ) test/*
