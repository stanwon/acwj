SRC = *.c

all: scanner
	@./$< input

scanner: $(SRC)
	@cc -o $@ -g $^

out: tar
	@./$@

tar: out.s
	@cc -o out $<

clean:
	@rm -f scanner *.o *.s out
