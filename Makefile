# SRC=*.c test.c
SRC=${wildcard *.c}
# SRC_TMP=$(SRC)
SRC_X86 = $(filter-out cg_arm.c, $(SRC))
SRC_ARM = $(filter-out cg.c, $(SRC))

all: out
	@./out

out: out.s
	@cc -o out $< lib/printint.c

out.s: comp input
	@./$< input

comp: $(SRC_ARM)
# comp: $(SRC_X86)
	@cc -o $@ -g $^

clean:
	@rm -f comp *.o *.s out
