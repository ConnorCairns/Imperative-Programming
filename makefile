all: test.c
	clang -std=c11 -Wall test.c -o test