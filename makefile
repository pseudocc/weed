CC = gcc
CFLAGS = -nostdlib \
		 -DUNHINGED \
		 -std=gnu11 \
		 -fno-stack-protector

CFLAGS_TEST = $(CFLAGS) \
			  -DWEED_DEV \
			  -Wl,-eweed \
			  -g

SRC = $(wildcard src/*.c) \
	  $(wildcard src/linux/*.c) \
	  #$(wildcard src/format/*.c)

OBJ = $(SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test/linux_x86_64: src/linux/x86_64.c src/builtins.o
	$(CC) $(CFLAGS_TEST) $^ -o $@

test/stdio: src/stdio.c src/linux/x86_64.o src/builtins.o
	$(CC) $(CFLAGS_TEST) $^ -o $@

test/format: src/format.c src/stdio.o src/linux/x86_64.o src/builtins.o
	$(CC) $(CFLAGS_TEST) $^ -o $@

test/playground: playground.c $(OBJ)
	$(CC) -g $(CFLAGS_TEST) $^ -o $@

playground: test/playground
	./$^

clean:
	rm -f $(OBJ) test/*
