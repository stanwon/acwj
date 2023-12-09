all: scanner
	@./scanner input

scanner: main.c scan.c expr.c interp.c tree.c
	@cc -o scanner -g main.c scan.c expr.c interp.c tree.c

clean:
	@rm -f scanner *.o
