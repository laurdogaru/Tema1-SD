all: 2final.c

build: 2final.c
	gcc -g -Wall -std=c99 2final.c -o editor

clean:
	rm editor
