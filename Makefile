SRC=*.c

all: out
	@./out

out: out.s
	@cc -o out $< lib/printint.c

out.s: comp input
	@./$< input

comp: $(SRC)
	@cc -o $@ -g $^

clean:
	@rm -f comp *.o *.s out
