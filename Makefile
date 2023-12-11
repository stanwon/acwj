SRC = main.c \
			scan.c \
			expr.c \
			interp.c \
			tree.c

all: scanner
	@./scanner input


scanner: $(SRC)
	@cc -o $@ -g $^

clean:
	@rm -f scanner *.o
