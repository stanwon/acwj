#include "0defs.h"
#include "1decl.h"
#include "2data.h"

static int freereg[4];
static char *reglist[4] = {"r4", "r5", "r6", "r7"};
static char *cmplist[] = {"sete", "setne", "setl", "setg", "setle", "setge"};
static char *invcmplist[] = {"jne", "je", "jge", "jle", "jg", "jl"};

void freeall_registers() {
  freereg[0] = freereg[1] = freereg[2] = freereg[3] = 1;
}

static int alloc_register() {
  for (int loop = 0; loop < 4; loop++) {
    if (freereg[loop]) {
      freereg[loop] = 0;
      return loop;
    }
  }

  fatal("Out of registers");
}

static void free_register(int reg) {
  if (0 != freereg[reg]) {
    fatald("Error trying to free register", reg);
  }
  freereg[reg] = 1;
}

#define MAXINTS 1024
int Intlist[MAXINTS];
static int Intslot = 0;

static void set_int_offset(int val) {
  int offset = -1;

  // See if it is already there
  for (int i = 0; i < Intslot; i++) {
    if (Intlist[i] == val) {
      offset = 4 * i;
      break;
    }
  }

  // Not in the list, so add it
  if (offset == -1) {
    offset = 4 * Intslot;
    if (Intslot == MAXINTS)
      fatal("Out of int slots in set_int_offset()");
    Intlist[Intslot++] = val;
  }
  // Load r3 with this offset
  fprintf(Outfile, "\tldr\tr3, .L3+%d\n", offset);
}

static void set_var_offset(int id) {
  int offset = 0;

  for (int i = 0; i < id; i++) {
    if (Gsym[i].stype == S_VARIABLE)
      offset += 4;
  }
  fprintf(Outfile, "\tldr\tr3, .L2+%d\n", offset);
}

void cgpreamble() {
  freeall_registers();
  fputs("\t.text\n", Outfile);
}

void cgfuncpreamble(int id) {
  char *name = Gsym[id].name;
  fprintf(Outfile,
          "\t.text\n"
          "\t.globl\t%s\n"
          "\t.type\t%s, \%%function\n"
          "%s:\n"
          "\tpush\t{fp, lr}\n"
          "\tadd\tfp, sp, #4\n"
          "\tsub\tsp, sp, #8\n"
          "\tstr\tr0, [fp, #-8]\n",
          name, name, name);
}


void cgpostamble() {
  fprintf(Outfile, ".L2:\n");
  for (int i = 0; i < Globs; i++) {
    if (Gsym[i].stype == S_VARIABLE)
      fprintf(Outfile, "\t.word %s\n", Gsym[i].name);
  }

  fprintf(Outfile, ".L3:\n");
  for (int i = 0; i < Intslot; i++) {
    fprintf(Outfile, "\t.word %d\n", Intlist[i]);
  }
}

void cgfuncpostamble(int id) {
  cglabel(Gsym[id].endlabel);
  fputs("\tsub\tsp, fp, #4\n"
        "\tpop\t{fp, pc}\n"
        "\t.align\t2\n",
        Outfile);
}

int cgloadint(int value) {
  int r = alloc_register();

  if (value <= 1000)
    fprintf(Outfile, "\tmov\t%s, #%d\n", reglist[r], value);
  else {
    set_int_offset(value);
    fprintf(Outfile, "\tldr\t%s, [r3]\n", reglist[r]);
  }
  return (r);
}

int cgloadglob(int id) {
  // Get a new register
  int r = alloc_register();

  // Get the offset to the variable
  set_var_offset(id);
  fprintf(Outfile, "\tldr\t%s, [r3]\n", reglist[r]);
  return (r);
}

int cgadd(int r1, int r2) {
  fprintf(Outfile, "\tadd\t%s, %s, %s\n", reglist[r2], reglist[r1],
          reglist[r2]);
  free_register(r1);
  return (r2);
}

int cgsub(int r1, int r2) {
  fprintf(Outfile, "\tsub\t%s, %s, %s\n", reglist[r1], reglist[r1],
          reglist[r2]);
  free_register(r2);
  return (r1);
}

int cgmul(int r1, int r2) {
  fprintf(Outfile, "\tmul\t%s, %s, %s\n", reglist[r2], reglist[r1],
          reglist[r2]);
  free_register(r1);
  return (r2);
}

int cgdiv(int r1, int r2) {
  fprintf(Outfile, "\tmov\tr0, %s\n", reglist[r1]);
  fprintf(Outfile, "\tmov\tr1, %s\n", reglist[r2]);
  fprintf(Outfile, "\tbl\t__aeabi_idiv\n");
  fprintf(Outfile, "\tmov\t%s, r0\n", reglist[r1]);
  free_register(r2);
  return (r1);
}

void cgprintint(int r) {
  fprintf(Outfile, "\tmov\tr0, %s\n", reglist[r]);
  fprintf(Outfile, "\tbl\tprintint\n");
  fprintf(Outfile, "\tnop\n");
  free_register(r);
}

int cgstorglob(int r, int id) {
  set_var_offset(id);

  switch (Gsym[id].type) {
  case P_CHAR:
    fprintf(Outfile, "\tstrb\t%s, [r3]\n", reglist[r]);
    break;
  case P_INT:
  case P_LONG:
    fprintf(Outfile, "\tstr\t%s, [r3]\n", reglist[r]);
    break;
  default:
    fatald("Bad type in cgloadglob:", Gsym[id].type);
  }

  return (r);
}

void cgglobsym(int id) {
  int typesize;

  typesize = cgprimsize(Gsym[id].type);

  fprintf(Outfile, "\t.comm\t%s,%d,%d\n", Gsym[id].name, typesize, typesize);
}

int cgcompare_and_set(int ASTop, int r1, int r2) {
  if (ASTop < A_EQ || ASTop > A_GE)
    fatal("Bad ASTop in cgcompare_and_set()");

  fprintf(Outfile, "\tcmp\t%s, %s\n", reglist[r1], reglist[r2]);
  fprintf(Outfile, "\t%s\t%s, #1\n", cmplist[ASTop - A_EQ], reglist[r2]);
  fprintf(Outfile, "\t%s\t%s, #0\n", invcmplist[ASTop - A_EQ], reglist[r2]);
  fprintf(Outfile, "\tuxtb\t%s, %s\n", reglist[r2], reglist[r2]);
  free_register(r1);
  return (r2);
}

static char *brlist[] = {"bne", "beq", "bge", "ble", "bgt", "blt"};

int cgcompare_and_jump(int ASTop, int r1, int r2, int label) {
  if (ASTop < A_EQ || ASTop > A_GE)
    fatal("Bad ASTop in cgcompare_and_set()");

  fprintf(Outfile, "\tcmp\t%s, %s\n", reglist[r1], reglist[r2]);
  fprintf(Outfile, "\t%s\tL%d\n", brlist[ASTop - A_EQ], label);
  freeall_registers();
  return (NOREG);
}

void cglabel(int l) { fprintf(Outfile, "L%d:\n", l); }

void cgjump(int l) { fprintf(Outfile, "\tb\tL%d\n", l); }

int cgwiden(int r, int oldtype, int newtype) { return (r); }

static int psize[] = {0, 0, 1, 4, 8};
int cgprimsize(int type) {
  if (P_NONE > type || P_LONG < type) {
    fatal("Bad type in cgprimsize()");
  }
  return psize[type];
}

int cgcall(int r, int id) {
  fprintf(Outfile, "\tmov\tr0, %s\n", reglist[r]);
  fprintf(Outfile, "\tbl\t%s\n", Gsym[id].name);
  fprintf(Outfile, "\tmov\t%s, r0\n", reglist[r]);
  return (r);
}

int cgreturn(int r, int id) {
  fprintf(Outfile, "\tmov\tr0, %s\n", reglist[r]);
  cgjump(Gsym[id].endlabel);
}
