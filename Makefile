main.out: main.c
	cc -Wall main.c -o main.out

run: main.out
	./main.out
