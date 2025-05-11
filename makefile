CC = gcc
CFLAGS = -nostdlib \
		 -DUNHINGED \
		 -std=gnu11
CFLAGS_TEST = -DWEED_DEV \
			  -Wl,-eweed

SRC = src/builtins.c \
	  src/linux/x86_64.c

OBJ = $(SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test/linux_x86_64: src/linux/x86_64.c src/builtins.o
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -fno-stack-protector $^ -o $@

clean:
	rm -f $(OBJ) test/*
