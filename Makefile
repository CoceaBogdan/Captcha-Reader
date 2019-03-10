CC=gcc
CFLAGS=-Wall -Wextra 

build: ex.c bmp_header.h get_numbers.h
	$(CC) $(CFLAGS) ex.c bmp_header.h get_numbers.h

run: 
	./a.out

clean:
	rm -rf a.out $(EXE)


