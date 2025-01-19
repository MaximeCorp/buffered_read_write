CC=gcc
CFALGS=-std=c99 -pedantic -Werror -Wall -Wextra -Wvla

SRC=$(wildcard src/*.c)
OBJ=$(patsubst %.c, %.o, $(SRC))

library: $(OBJ)
	ar rcs -o libstream.a $^

%.o: %.c{
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -f $(OBJ) libsream.a

.PHONY: library clean
